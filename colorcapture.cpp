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
    //カメラの設定
    VideoCapture cap(1);
    if(!cap.isOpened()){
        cout << "Camera not Found!" << endl;
        return -1;
    }

    //画像を入れるMatの宣言
    Mat org;    //カメラ画像
    Mat hsv;    //HSV変換後の画像
    
    //ラベリング関連の変数
    LabelingBS labeling;
    RegionInfoBS *ri;
    
    //画像サイズを調べるために一度画像を取得する
    cap >> org;
    
    //画像サイズの指定
    const int height = org.rows;
    const int width = org.cols;
    
    Mat result(height, width, CV_8UC1);    //青部分を切り出した白黒画像
    short *dst = new short[width * height];//ラベリング出力先

    while(waitKey(1)%256 != 'q'){
        cap >> org;
        //カメラ画像の取得
        //結果配列のゼロクリア
        result = Mat::zeros(height, width, CV_8UC1);
        //HSV画像への変換
        //この方法ではHが0-180の範囲で表される
        cvtColor(org, hsv, CV_BGR2HSV);
        
        for(int y=0; y<height;y++){
            for(int x=0; x<width; x++){
                int index = hsv.step*y+(x*3);
                if(hsv.data[index] >=95 && hsv.data[index] <=105 &&  //Hの範囲指定
                        hsv.data[index+1] >= 70 &&                   //Sの範囲指定
                           hsv.data[index+2] >= 70 ) {               //Vの範囲指定
                    result.data[result.step*y+x] = 255;              //該当色範囲を白くする
                }
            }
        }
        //ラベリングの実行
        labeling.Exec((uchar *)result.data, dst, width, height, true, 30);
        //最大の領域を四角で囲む
        ri = labeling.GetResultRegionInfo( 0 );
        int ltop_x,ltop_y,rbottom_x,rbottom_y;
        ri->GetMin(ltop_x,ltop_y);
        ri->GetMax(rbottom_x, rbottom_y);
        rectangle(org, Point(ltop_x, ltop_y), Point(rbottom_x, rbottom_y), Scalar(255, 255, 255));
        //画像の表示
        imshow("Original", org);
        imshow("Extract", result);
    }
}
