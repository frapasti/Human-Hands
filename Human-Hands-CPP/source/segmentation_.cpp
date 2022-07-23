#include "opencv2/highgui.hpp"
#include "../include/functions.h"
#include <iostream>

using namespace std;
using namespace cv;

int main( int argc, char** argv ){
    string folder_test = "/home/local/furcann17861/Downloads/project/Test_Hands/Images/*.jpg";
    vector<string> filenames_test;
    glob(folder_test, filenames_test);
    string folder_bboxes = "/home/local/furcann17861/Downloads/project/Test_Hands/Bboxes/*.txt";
    vector<string> filenames_bboxes;
    glob(folder_bboxes, filenames_bboxes);
    string folder_got = "/home/local/furcann17861/Downloads/project/Test_Hands/mask/*.png";
    vector<string> filenames_got;
    glob(folder_got, filenames_got);
    
    for (size_t j = 0; j < filenames_test.size(); ++j){
        Mat img = imread(filenames_test[j]);
        Mat img_got = img.clone();
        Mat mask_got = imread(filenames_got[j]);
        string filePath = filenames_bboxes[j];
    
        vector<int> numbers;//all the bounding boxes
        Mat mask = Mat::zeros(img.rows,img.cols, CV_8UC1);//all black
        Mat dst;
    
        readNumbers(numbers, filePath);

        for(int i = 0; i < numbers.size(); i = i + 4){

            Mat mask1 = Mat::zeros(img.rows,img.cols, CV_8UC1);//all black
            Rect r = Rect(numbers.at(i),numbers.at(i+1), numbers.at(i+2), numbers.at(i+3));

            improveBySkin(r, img);
            segmentation_rect(img, r, mask1);
            improveMask(r, mask1);
            segmentation_mask(img, mask1);
            
            vector<double> avg_intensity;
            calculate_avg(avg_intensity, img, r, mask1);
            double global_avg = avg_intensity.at(0)+avg_intensity.at(1)+avg_intensity.at(2);
            improveMask_avg(img, r, avg_intensity, mask1);
            
            mask1 = (mask1==1)+(mask1==3);
            mask = mask + mask1;
            //img.copyTo(dst, mask);
            /*
            dilate(mask, mask, cv::Mat());
            erode(mask, mask, cv::Mat());*/
            
            //color randomically the masks not complete
            vector<Mat> masks;
            masks.push_back(mask);
            
            for(size_t l = 0; l < masks.size(); l++){
                Scalar random = Scalar(rand()%255,rand()%255,rand()%255);
                img.setTo(random, mask);
                img.setTo(random, mask);
                imshow("dst", img);
                waitKey(1);
        
            }
        }
    
        /*Mat dst_got;
        img_got.copyTo(dst_got, mask_got);


        imshow("mask", mask);
        waitKey(0);*/
        cout<<"Pixel accuracy:"<< pixelAccuracy(mask, mask_got)<<endl;
    }
    return 0;
}
