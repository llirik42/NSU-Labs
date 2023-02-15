#include <ctime>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using Image = Mat;
using FramesArray = std::vector<Image>;

// List of all colormaps: https://docs.opencv.org/4.x/d3/d50/group__imgproc__colormap.html
const std::vector<int> COLORMAPS_INDEXES({0, 1, 3, 5, 6, 7, 10, 11, 13, 14, 15, 16, 17, 21});

inline const unsigned int CAPTURE_INDEX = 0;

inline const unsigned int PEPE_PERIOD_MS = 100;
inline const unsigned int BACKGROUND_PERIOD_MS = 200;
inline const unsigned int BLUR_SIZE = 10;

inline const unsigned int GIF_FRAMES_COUNT = 22;
inline const std::string GIF_FRAMES_PATH = "../frames/";
inline const std::string GIF_FRAMES_EXT = ".png";

inline const int ESC_CODE = 27;

inline const unsigned int X_MIRRORING_CODE = 1;

void input_gif_frames(FramesArray& gif_frames){
    for (unsigned int i = 0; i < GIF_FRAMES_COUNT; ++i){
        std::string current_frame_path = GIF_FRAMES_PATH;

        current_frame_path.append(std::to_string(i));
        current_frame_path.append(GIF_FRAMES_EXT);

        gif_frames.push_back(imread(current_frame_path));
    }
}

void merge_gif_frame(const Mat& gif_frame, Mat& main_frame){
    for(int i = 0; i < gif_frame.rows; ++i) {
        const auto gif_frame_row = gif_frame.ptr<cv::Vec3b>(i);
        auto main_frame_row = main_frame.ptr<cv::Vec3b>(i);

        // cols = columns
        for(int j = 0; j < gif_frame.cols; ++j) {
            int cur_blue = gif_frame_row[j][0];
            int cur_green = gif_frame_row[j][1];
            int cur_red = gif_frame_row[j][2];

            // some empirical numbers
            bool f1 = cur_red > 150 && cur_green > 200 && cur_blue > 150;
            bool f2 = cur_red < 50 && cur_green < 50 && cur_blue < 50;

            if (!f1 && !f2){
                main_frame_row[j] = gif_frame_row[j];
            }
        }
    }
}

int main(){
    VideoCapture capture(CAPTURE_INDEX);

    FramesArray gif_frames;

    input_gif_frames(gif_frames);

    unsigned int frames_processed_counter = 0;

    double total_time = 0;
    double input_time = 0;
    double output_time = 0;
    double processing_time = 0;

    unsigned int current_frame_index = 0;
    unsigned int current_colormap_index = 0;

    time_t start_time = clock();
    while (true){
        time_t start_iteration_time = clock();

        Image webcam_image;

        time_t start_input_time = clock();
        capture.read(webcam_image);
        time_t full_input_time = clock();
        input_time += difftime(full_input_time, start_input_time);

        Image current_gif_frame = gif_frames[current_frame_index];

        time_t start_processing_time = clock();
        flip(webcam_image,webcam_image,X_MIRRORING_CODE);
        applyColorMap(webcam_image, webcam_image, COLORMAPS_INDEXES[current_colormap_index]);
        merge_gif_frame(current_gif_frame, webcam_image);
        blur( webcam_image, webcam_image, Size(BLUR_SIZE, BLUR_SIZE));
        time_t full_processing_time = clock();
        processing_time += difftime(full_processing_time, start_processing_time);

        time_t start_output_time = clock();
        imshow("Result", webcam_image);
        time_t full_output_time = clock();
        output_time += difftime(full_output_time, start_output_time);

        int c = waitKey(1);
        if (c == ESC_CODE){
            break;
        }

        time_t current_time = clock();
        const auto delta_time = difftime(current_time, start_time);
        current_frame_index = int(delta_time / PEPE_PERIOD_MS) % GIF_FRAMES_COUNT;
        current_colormap_index = int(delta_time / BACKGROUND_PERIOD_MS) % COLORMAPS_INDEXES.size();

        time_t full_iteration_time = clock();

        frames_processed_counter++;

        total_time += difftime(full_iteration_time, start_iteration_time);
    }

    std::cout << std::fixed << std::setprecision(0);

    std::cout << "FPS=" << (1000 * frames_processed_counter / total_time) << "\n",
    std::cout << "Input=" << (input_time / total_time * 100) << "%\n";
    std::cout << "Processing=" << (processing_time / total_time * 100) << "%\n";
    std::cout << "Output=" << (output_time / total_time * 100) << "%\n\n";

    return 0;
}
