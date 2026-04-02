#ifndef MJPEG_STREAMER_H
#define MJPEG_STREAMER_H

#include <opencv2/opencv.hpp>

class MjpegStreamer {
public:
    // 构造函数：传入指定的端口号，默认 8080
    MjpegStreamer(int port = 8080);
    
    // 析构函数：释放 Socket 资源
    ~MjpegStreamer();

    // 核心调用函数：传入你要查看的 Mat 图像
    void send(const cv::Mat& frame);

private:
    int port_;
    int server_fd_;
    int client_fd_;

    void initServer();
};

#endif // MJPEG_STREAMER_H