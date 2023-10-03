#include <opencv2/opencv.hpp>
#include <curses.h>

class NCursesManager{
    public:
        NCursesManager(float videoWidth, float videoHeight){
            this->videoWidth = videoWidth;
            this->videoHeight = videoHeight;
        }
        void initialize();
        void generateFrame(cv::Mat);
    private:
        int getColor(int, int, cv::Mat&);
        void clearScreen(int, int);
        std::vector<std::vector<int>> prevFrame;
        float rgbDivisor;
        float videoWidth, videoHeight;
        int prevWidth, prevHeight;
};