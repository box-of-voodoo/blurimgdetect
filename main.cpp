#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <filesystem>
#include <vector>
#include <map>

//#define TIME_MEASURE
#ifdef TIME_MEASURE
#include <chrono>
#endif
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cout << "Not sufficient number of input arguments, program will end" << endl;
        return 0;
    }

    cv::Mat image, eges;
    cv::Mat m, sd;
    cv::Mat out;// (image.rows, image.cols * image.channels(), CV_64F);
    cv::Mat splitchannels[3];
    
    std::string path = "d:/1_Kuba/fotky/x";
    double suma = 0;
    double maxVal = DBL_MIN;
    double minVal = DBL_MAX;
    double limit = 0;
    int count = 0;
    std::vector<double> stds;
    std::map<std::filesystem::path, double> imagesStd;
    cout << "Processing images..." << endl;

#ifdef TIME_MEASURE
    auto start = std::chrono::high_resolution_clock::now();
#endif
    for (const auto& file : std::filesystem::directory_iterator(argv[1]))
    {

        image = cv::imread(file.path().string(), cv::IMREAD_GRAYSCALE); // Read the file

        if (!image.empty()) // Check for invalid input
        {
            cv::Laplacian(image, out, CV_64F);
            cv::meanStdDev(out, m, sd);
            double temp = (pow(*(double*)sd.data, 2.));
            imagesStd[file.path()] = temp;
            minVal = MIN(minVal, temp);
            maxVal = MAX(maxVal, temp);
            suma += temp;
        }


    }
#ifdef TIME_MEASURE
    auto end = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    cout << "Time calculating: " << time.count() << " ms" << endl;
#endif

    cout << "Number of photos: " << imagesStd.size() << endl;
    cout << "Max value: " << maxVal << "\tMin value: " << minVal << "\tAverage value: " << suma / imagesStd.size() << endl;
    cout << "Remove under: ";
    std::cin >> limit;
    cout << "Removing photos..." << endl;

    for (const auto& [key,val] : imagesStd)
    {
        if (val < limit) 
            count += std::filesystem::remove(key);
    }

    cout << "Photos removed: " << count << endl;
       
    return 0;
}