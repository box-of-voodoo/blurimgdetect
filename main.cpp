#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <Windows.h>
#include <filesystem>
#include <vector>

using std::cout;
using std::endl;

int main(/*int argc, char** argv*/)
{
    cv::Mat image, eges;
    cv::Mat m, sd;
    cv::Mat out;// (image.rows, image.cols * image.channels(), CV_64F);
    cv::Mat splitchannels[3];
    
    std::string path = "d:/1_Kuba/fotky";
    double suma = 0;
    double maxVal = DBL_MIN;
    double minVal = DBL_MAX;
    std::vector<double> stds;
    cout << "Processing images..." << endl;

        for (const auto& file : std::filesystem::directory_iterator(path))
        {

            image = cv::imread(file.path().string(), cv::IMREAD_COLOR); // Read the file

            if (image.empty()) // Check for invalid input
            {
                std::cout << "Could not open or find the image" << std::endl;
                return -1;
            }

            cv::Laplacian(image, eges, CV_64F);
            cv::split(eges, splitchannels);
            cv::hconcat(splitchannels, 3, out);


            cv::meanStdDev(out, m, sd);

            //bool remove(const std::filesystem::path& p);
            stds.push_back(pow(*(double*)sd.data, 2.));
            minVal = min(minVal, stds.back());
            maxVal = max(maxVal, stds.back());
            suma += stds.back();
        }
        double limit = 0;
        cout << "Number of photos: " << stds.size() << endl;
        cout << "Max value: " << maxVal << "\tMin value: " << minVal << "\tAverage value: " << suma / stds.size() << endl;
        cout << "\nRemove under: ";
        std::cin >> limit;
        cout << "\nRemoving photos..." << endl;

        int count = 0;
        for (const auto& file : std::filesystem::directory_iterator(path))
        {
            if (stds[count++] < limit) 
                std::filesystem::remove(file.path());
        }
            
        
        cv::putText(image, cv::String(std::to_string(pow(*(double*)sd.data, 2.))), cv::Point(100, 100), 0, 4, cv::Scalar(0, 0, 255), 5);
        cv::namedWindow("Display window", cv::WINDOW_NORMAL); // Create a window for display.
        cv::imshow("Display window", image); // Show our image inside it.

        cv::waitKey(0); // Wait for a keystroke in the window
    
    return 0;
}