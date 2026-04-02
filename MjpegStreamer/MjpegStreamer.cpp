#include "MjpegStreamer.h"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

MjpegStreamer::MjpegStreamer(int port) : port_(port), server_fd_(-1), client_fd_(-1) {
    initServer();
}

MjpegStreamer::~MjpegStreamer() {
    if (client_fd_ >= 0) close(client_fd_);
    if (server_fd_ >= 0) close(server_fd_);
}

void MjpegStreamer::initServer() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        std::cerr << "[Streamer] 创建 Socket 失败！" << std::endl;
        return;
    }

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_fd_, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cerr << "[Streamer] 端口 " << port_ << " 绑定失败！" << std::endl;
        return;
    }

    listen(server_fd_, 3);

    // 将 Server Socket 设置为非阻塞模式，防止 accept() 卡死主循环
    int flags = fcntl(server_fd_, F_GETFL, 0);
    fcntl(server_fd_, F_SETFL, flags | O_NONBLOCK);

    std::cout << "[Streamer] 视频流服务已启动在端口: " << port_ << std::endl;
}

void MjpegStreamer::send(const cv::Mat& frame) {
    if (server_fd_ < 0 || frame.empty()) return;

    // 如果当前没有客户端连接，尝试接收新连接（非阻塞）
    if (client_fd_ < 0) {
        client_fd_ = accept(server_fd_, nullptr, nullptr);
        if (client_fd_ >= 0) {
            std::cout << "[Streamer] 笔记本已连接！" << std::endl;
            std::string header = "HTTP/1.1 200 OK\r\n"
                                 "Content-Type: multipart/x-mixed-replace; boundary=frame\r\n\r\n";
            ::send(client_fd_, header.c_str(), header.size(), MSG_NOSIGNAL);
        }
    }

    // 如果有客户端连接，则进行 JPEG 压缩并发送
    if (client_fd_ >= 0) {
        std::vector<uchar> buffer;
        // 可选：添加 {cv::IMWRITE_JPEG_QUALITY, 80} 来降低画质减少网络延迟
        cv::imencode(".jpg", frame, buffer);

        std::string chunk_header = "--frame\r\n"
                                   "Content-Type: image/jpeg\r\n"
                                   "Content-Length: " + std::to_string(buffer.size()) + "\r\n\r\n";

        // 发送数据，如果任意一个 send 失败，说明客户端已断开
        if (::send(client_fd_, chunk_header.c_str(), chunk_header.size(), MSG_NOSIGNAL) < 0 ||
            ::send(client_fd_, buffer.data(), buffer.size(), MSG_NOSIGNAL) < 0 ||
            ::send(client_fd_, "\r\n", 2, MSG_NOSIGNAL) < 0) {
            
            std::cout << "[Streamer] 笔记本已断开连接。" << std::endl;
            close(client_fd_);
            client_fd_ = -1; // 重置状态，等待下一次连接
        }
    }
}