#pragma once

#include <filesystem>
#include <map>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class filterPhotos
{
	std::filesystem::path imgFolder;
	std::map<std::filesystem::path, double> imagesStd;
	cv::Mat image, eges;
	cv::Mat m, sd;
	cv::Mat out;
	


    

public:
    double average = 0;
    double maxVal = DBL_MIN;
    double minVal = DBL_MAX;

    filterPhotos(char *folderPath) 
    {
        imgFolder = std::filesystem::path(folderPath);
    }

    std::map<std::filesystem::path, double> getPhotosStd() const
    {
        return imagesStd;
    }

    void FindStdOfEdges()
    {
        for (const auto& file : std::filesystem::directory_iterator(imgFolder))
        {
            image = cv::imread(file.path().string(), cv::IMREAD_GRAYSCALE); // Read the file in gray scale
            if (!image.empty()) // If image read or if is image
            {
                cv::Laplacian(image, out, CV_64F); //find egdes in image
                cv::meanStdDev(out, m, sd); //compute standart deviation of edge
                double temp = (pow(*(double*)sd.data, 2.)); //variance
                imagesStd[file.path()] = temp;
                minVal = MIN(minVal, temp);
                maxVal = MAX(maxVal, temp);
                average += temp;
            }
        }
        average /= imagesStd.size();
    }
};

