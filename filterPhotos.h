#pragma once

#include <iostream>
#include <filesystem>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class filterPhotos
{
	std::filesystem::path imgFolder;
	std::map<std::filesystem::path, double> imagesStd;
    std::vector<std::filesystem::path> images;
	cv::Mat imageA, imageB;
	cv::Mat m, sd;
    std::set<std::string> extensions = {".jpg",".jpeg",".png",".jpe",".bmp"};
	

public:
    double average = 0;
    double maxVal = DBL_MIN;
    double minVal = DBL_MAX;

    filterPhotos(char *folderPath) 
    {
        imgFolder = std::filesystem::path(folderPath);
        for (const auto& file : std::filesystem::directory_iterator(imgFolder))
        {
            if (extensions.count(file.path().extension().string()))
                images.push_back(file.path());
        }

    }

    std::map<std::filesystem::path, double> getPhotosStd() const
    {
        return imagesStd;
    }

    void FindStdOfEdges()
    {
        for (const auto& file : images)
        {
            imageA = cv::imread(file.string(), cv::IMREAD_GRAYSCALE); // Read the file in gray scale
            if (!imageA.empty()) // If image read or if is image
            {
                cv::Laplacian(imageA, imageB, CV_64F); //find egdes in image
                cv::meanStdDev(imageB, m, sd); //compute standart deviation of edge
                double temp = (pow(*(double*)sd.data, 2.)); //variance
                imagesStd[file] = temp;
                minVal = MIN(minVal, temp);
                maxVal = MAX(maxVal, temp);
                average += temp;
            }
        }
        average /= imagesStd.size();
    }

    void similarHist()
    {
        double compVal = 0;
        cv::Mat tempHSV;
        cv::Mat histA,histB;
        int nimages = 1; // Only 1 image, that is the Mat scene.
        int channels[] = { 0 }; // Index for hue channel
        int dims = 1; // Only 1 channel, the hue channel
        int histSize[] = { 30}; // 9 bins, 1 each for Red, RY, Yellow, YG etc.
        float hranges[] = { 0, 256 }; // hue varies from 0 to 179, see cvtColor
        const float* ranges[] = { hranges };

        for (const auto& file :images)
        {   
            imageA = cv::imread(file.string(), cv::IMREAD_COLOR); // Read the file in gray scale
            if (!imageA.empty()) // If image read or if is image
            {
                cv::cvtColor(imageA, tempHSV, cv::COLOR_BGR2HSV_FULL);

                cv::calcHist(&tempHSV, nimages, channels, cv::Mat(), histA, dims, histSize, ranges,true,false);
                for (const  auto& file2 : images)
                {
                    imageB = cv::imread(file2.string(), cv::IMREAD_COLOR);
                    
                    
                    cv::cvtColor(imageB, tempHSV, cv::COLOR_BGR2HSV_FULL);
                    cv::calcHist(&tempHSV, 1, channels, cv::Mat(), histB, dims, histSize, ranges, true, false);

                    compVal = cv::compareHist(histA, histB, cv::HISTCMP_CORREL);
                    std::cout << file.filename() << "\t" << file2.filename() << "\t" << compVal << std::endl;
                    if (compVal < 0.5)
                    {
                        cv::putText(imageB, std::to_string(compVal), cv::Point((imageB.cols / 2) - 100, (imageB.rows / 2) - 100), cv::FONT_HERSHEY_SIMPLEX, 15, cv::Scalar(255, 0, 255), 4);
                        cv::namedWindow("window", cv::WINDOW_KEEPRATIO);
                        cv::imshow("window", imageB);
                        cv::waitKey(0);
                    }
                }

            }
        }
        average /= imagesStd.size();
    }
};

