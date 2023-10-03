#include "ncursesManager.h"

void NCursesManager::initialize(){    
    initscr();
    noecho();
    start_color();
    init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK);
    init_pair(COLOR_RED, COLOR_RED, COLOR_RED);
    init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_GREEN);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLUE);
    init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_MAGENTA);
    init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_CYAN);
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_WHITE);
    refresh();
    nodelay(stdscr, TRUE);

    prevFrame.resize(videoHeight);
    for(int i = 0; i < videoHeight; i++){
        prevFrame[i].resize(videoWidth);
    }
    rgbDivisor = 1.0/255.0;
}

void NCursesManager::generateFrame(cv::Mat frame){
    float screenWidth, screenHeight;
    // Frame width takes into account that,
    // characters are larger height than width
    float frameWidth = videoWidth * 2.3;
    float frameHeight = videoHeight;
    getmaxyx(stdscr, screenHeight, screenWidth);

    // Resize frame to fit within screen
    if(frameWidth > screenWidth){
        frameHeight = (screenWidth/frameWidth) * frameHeight;
        frameWidth = screenWidth;
    }
    if(frameHeight > screenHeight){
        frameWidth = (screenHeight/frameHeight) * frameWidth;
        frameHeight = screenHeight;
    }

    cv::resize(frame, frame, cv::Size(int(frameWidth), int(frameHeight)));
    float offsetX = round(screenWidth * 0.5 - frameWidth * 0.5);
    float offsetY = round(screenHeight * 0.5 - frameHeight * 0.5);

    int color = 0;

    // Check if frame is different from previous frame
    // If true repaint the whole screen
    if(prevWidth != frameWidth || prevHeight != frameHeight){
        clearScreen(screenWidth, screenHeight);
        for(int x = 0; x < frame.cols; x++){
            for(int y = 0; y < frame.rows; y++){
                int newColor = getColor(y, x, frame);
                int repeated = 1;
                if(color != newColor){
                    attroff(COLOR_PAIR(color));
                    color = newColor;
                    attron(COLOR_PAIR(color));
                }
                mvaddch(y + offsetY, x + offsetX, *" ");
                prevFrame.at(y).at(x) = color;
            }
        }
    }
    // Otherwise only parts of it
    else{
        for(int x = 0; x < frame.cols; x++){
            for(int y = 0; y < frame.rows; y++){
                int newColor = getColor(y, x, frame);
                // Check whether we need to place a new pixel
                if(newColor != prevFrame.at(y).at(x)){
                    if(color != newColor){
                        attroff(COLOR_PAIR(color));
                        color = newColor;
                        attron(COLOR_PAIR(color));
                    }
                    mvaddch(y + offsetY, x + offsetX, *" ");
                    prevFrame.at(y).at(x) = color;
                }
            }
        }
    }

    prevHeight = frameHeight;
    prevWidth = frameWidth;

    attroff(COLOR_PAIR(color));
    refresh();
}


// Get colors at y, x position
int NCursesManager::getColor(int y, int x, cv::Mat& frame){
    float r = frame.at<cv::Vec3b>(y, x)[2];
    float g = frame.at<cv::Vec3b>(y, x)[1];
    float b = frame.at<cv::Vec3b>(y, x)[0];
    int newColor = int(round(r * rgbDivisor) + round(g * rgbDivisor) * 2 + round(b * rgbDivisor) * 4);
    return newColor;
}

void NCursesManager::clearScreen(int screenWidth, int screenHeight){
    attron(COLOR_PAIR(COLOR_BLACK));
    for(int x = 0; x < screenWidth; x++){
        for(int y = 0; y < screenHeight; y++){
            mvaddch(y, x, *" ");
        }
    }
}

