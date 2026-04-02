#include <opencv2/opencv.hpp>
#include "MjpegStreamer.h"

using namespace cv;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) return -1;

    // 1. 初始化推流器，指定端口为 8080
    MjpegStreamer streamer(8080);
    Mat frame, processed_frame;

    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        // ============================
        // 你的 OpenCV 算法处理逻辑
        // ============================
        cvtColor(frame, processed_frame, COLOR_BGR2GRAY);

        // 2. 调用函数：把你想看的图像传进去（原图或处理后的图都可以）
        streamer.send(processed_frame); 
        
        // 注意：如果你在无头开发板上运行，不要使用 cv::imshow 和 cv::waitKey
    }

    return 0;
}