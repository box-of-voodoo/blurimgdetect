#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
#include <set>
#include <filesystem>
#include <map>
#include "filterPhotos.h"

//#define TIME_MEASURE
#ifdef TIME_MEASURE
#include <chrono>
#endif
using std::cout;
using std::endl;

int main(int argc, char** argv)// help,-h,-help; -r; -m;
{
    int command = 0; //0 default mode;1 remove;2 move
    if ((argc < 2) || strcmp(argv[1], "help") == 0 || strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        cout << "blurimgdetect.exe [-r||-m path_to_move] path_of_folder" << endl;
        cout << "-r\tremove images under chosen value of variance\n"
            << "-m\tmove images under chosen value of variance to path_to_move\n"
            << "when only path_of_folder with images, chosen images are moved to new folder with path - path_of_folder/removed" << endl;
        return 0;
    }
    if (strcmp(argv[1], "-r") == 0 && std::filesystem::exists(argv[2]))
        command = 1;
    else if (strcmp(argv[1], "-m") == 0 && std::filesystem::exists(argv[2]) && std::filesystem::exists(argv[3]))
        command = 2;
    else if (std::filesystem::exists(argv[1]))
        command = 0;
    else
    {
        cout << "Wrong parametrs or path doesn't exist\n"
            << "To show help use no parametr or parametr \"-h\"" << endl;
        return 0;
    }

    std::filesystem::path imgFolder(argv[command + 1]);
    //cv::Mat image, eges;
    //cv::Mat m, sd;
    //cv::Mat out;
    //double suma = 0;
    //double maxVal = DBL_MIN;
    //double minVal = DBL_MAX;
    double limit = 0;
    int countImg = 0;
    filterPhotos test(argv[command + 1]);
//
//
//#ifdef TIME_MEASURE
//    auto start = std::chrono::high_resolution_clock::now();
//#endif
//    cout << "Processing images";
//    test.FindStdOfEdges();
//    std::map<std::filesystem::path, double> imagesStd(test.getPhotosStd());
//
//    //for (const auto& file : std::filesystem::directory_iterator(imgFolder))
//    //{
//    //    image = cv::imread(file.path().string(), cv::IMREAD_GRAYSCALE); // Read the file in gray scale
//    //    if (!image.empty()) // If image read or if is image
//    //    {
//    //        cout << ".";
//    //        cv::Laplacian(image, out, CV_64F); //find egdes in image
//    //        cv::meanStdDev(out, m, sd); //compute standart deviation of edge
//    //        double temp = (pow(*(double*)sd.data, 2.)); //variance
//    //        imagesStd[file.path()] = temp; 
//    //        minVal = MIN(minVal, temp);
//    //        maxVal = MAX(maxVal, temp);
//    //        suma += temp;
//    //    }
//    //}
//    cout << endl;
//#ifdef TIME_MEASURE
//    auto end = std::chrono::high_resolution_clock::now();
//    auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
//    cout << "Time calculating: " << time.count() << " ms" << endl;
//#endif
//    cout << "Number of photos: " << imagesStd.size() << endl;
//    cout << "Max value: " << test.maxVal << "\tMin value: " << test.minVal << "\tAverage value: " << test.average << endl;
//    
    test.similarHist();
    //if (command != 1) 
    //{
    //    cout << "Move under: ";
    //    std::cin >> limit;
    //    cout << "Moving photos" << endl;
    //}
    //else
    //{
    //    cout << "Remove under: ";
    //    std::cin >> limit;
    //    cout << "Removing photos" << endl;
    //}

    //std::filesystem::path newFolder;
    //if (command == 0)
    //{
    //    newFolder = imgFolder/ "blurred";
    //    std::filesystem::create_directory(newFolder);
    //}
    //else if (command == 2)
    //    newFolder = argv[2];

    //if (commandc == 1)
    //{
    //    for (const auto& [key, val] : imagesStd)
    //    {
    //        if (val < limit)
    //        {
    //            cout << ".";
    //            countImg += std::filesystem::remove(key);
    //        }
    //    }
    //    cout << "\nPhotos removed: " << countImg << endl;
    //}
    //else
    //{
    //    for (const auto& [key, val] : imagesStd)
    //    {
    //        if (val < limit)
    //        {
    //            cout << ".";
    //            auto newPath = newFolder/ key.filename();
    //            std::filesystem::rename(key, newPath);
    //            ++countImg;
    //        }
    //    }
    //    cout << "\nPhotos moved: " << countImg << endl;
    //}
    return 0;
}
