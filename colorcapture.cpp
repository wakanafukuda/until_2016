#include "ros/ros.h"
#include <cv_bridge/cv_bridge.h>
#include <sensors_msgs/Image.h>
#include <sensors_msgs/image_transport.h>

#include <opencv2/opencv.hpp>
#include <iostream>
#include "Labeling.h"

using namespace std;

image_transport::Subscriber subThermalImage("thermal_image_view", 1, receiverThermalImageView);

int main(int argc, char* argv[]){
  ros::init(argc, argv, "colorcapture_node");
  ros::NodeHandle nh;

}

int main(int argc, char *argv[]){
    //�J�����̐ݒ�
    VideoCapture cap(1);
    if(!cap.isOpened()){
        cout << "Camera not Found!" << endl;
        return -1;
    }

    //�摜������Mat�̐錾
    Mat org;    //�J�����摜
    Mat hsv;    //HSV�ϊ���̉摜
    
    //���x�����O�֘A�̕ϐ�
    LabelingBS labeling;
    RegionInfoBS *ri;
    
    //�摜�T�C�Y�𒲂ׂ邽�߂Ɉ�x�摜���擾����
    cap >> org;
    
    //�摜�T�C�Y�̎w��
    const int height = org.rows;
    const int width = org.cols;
    
    Mat result(height, width, CV_8UC1);    //������؂�o���������摜
    short *dst = new short[width * height];//���x�����O�o�͐�

    while(waitKey(1)%256 != 'q'){
        cap >> org;
        //�J�����摜�̎擾
        //���ʔz��̃[���N���A
        result = Mat::zeros(height, width, CV_8UC1);
        //HSV�摜�ւ̕ϊ�
        //���̕��@�ł�H��0-180�͈̔͂ŕ\�����
        cvtColor(org, hsv, CV_BGR2HSV);
        
        for(int y=0; y<height;y++){
            for(int x=0; x<width; x++){
                int index = hsv.step*y+(x*3);
                if(hsv.data[index] >=95 && hsv.data[index] <=105 &&  //H�͈͎̔w��
                        hsv.data[index+1] >= 70 &&                   //S�͈͎̔w��
                           hsv.data[index+2] >= 70 ) {               //V�͈͎̔w��
                    result.data[result.step*y+x] = 255;              //�Y���F�͈͂𔒂�����
                }
            }
        }
        //���x�����O�̎��s
        labeling.Exec((uchar *)result.data, dst, width, height, true, 30);
        //�ő�̗̈���l�p�ň͂�
        ri = labeling.GetResultRegionInfo( 0 );
        int ltop_x,ltop_y,rbottom_x,rbottom_y;
        ri->GetMin(ltop_x,ltop_y);
        ri->GetMax(rbottom_x, rbottom_y);
        rectangle(org, Point(ltop_x, ltop_y), Point(rbottom_x, rbottom_y), Scalar(255, 255, 255));
        //�摜�̕\��
        imshow("Original", org);
        imshow("Extract", result);
    }
}
