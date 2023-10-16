#include "main.h"

int main(int argc, char** argv){
    if(argc < 2){
        printf("Enter a file! \n");
        return -1;
    }

    cv::VideoCapture video(argv[1]);
    if (!video.isOpened()){
        printf("File failed to open! \n");
        return -1;
    }

    float fps = video.get(cv::CAP_PROP_FPS);
    // Time between frames in milliseconds,
    // minus the rough amount time to launch a thread
    int timeBetweenFrames = floor(940/fps);
    cv::Mat frame;
    video.read(frame);
    NCursesManager cursesManager(frame.cols, frame.rows);
    cursesManager.initialize();

    // Play video while there's frames
    while(video.read(frame))
    {
        int input = getch();

        if (input == KEY_ESCAPE || input == 'q' || input == 'Q') break;
        // Pause video
        else if(input == ' '){
            input = '_';
            while(input != ' '){
                input = getch();
                if (input == KEY_ESCAPE){
                    endwin();
                    return 0;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(5));
            }

        }

        std::thread generation(&NCursesManager::generateFrame, cursesManager, frame);

        std::this_thread::sleep_for(std::chrono::milliseconds(timeBetweenFrames));

        generation.join();
    }

    clear();
    endwin();
    return 0;
}