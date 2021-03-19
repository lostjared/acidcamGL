#include<acidcam/ac.h>

extern "C" void filter(cv::Mat &frame) {
	for(int z = 0; z < frame.rows; ++z) {
		for(int i = 0; i < frame.cols; ++i) {
			cv::Vec3b &pixel = frame.at<cv::Vec3b>(z, i);
			pixel = cv::Vec3b(rand()%255, rand()%255, rand()%255);
		}
	}
}
