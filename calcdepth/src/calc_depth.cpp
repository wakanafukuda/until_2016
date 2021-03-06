/*
0. パッケージの作成
$ catkin_create_pkg calcDepth cv_bridge image_transport roscpp sensor_msgs std_msgs

1. Kinectから画像を取得
$ roslaunch openni_launch openni.launch

2. image_view かrvizで画像を見る
*/
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

//赤枠の大きさ(測定する範囲)
//実際の画素数は、(WIDTH*2)*(HEIGHT*2)
#define WIDTH   50
#define HEIGHT  25


class depth_estimater{
public:
	depth_estimater();//コンストラクタ
	~depth_estimater();
	void rgbImageCallback(const sensor_msgs::ImageConstPtr& msg);//RGB画像を購読した時に呼ばれる関数
	void depthImageCallback(const sensor_msgs::ImageConstPtr& msg);//Depth画像を購読した時に呼ばれる関数

private:
	ros::NodeHandle nh;
	ros::Subscriber sub_rgb, sub_depth;
};

depth_estimater::depth_estimater(){
	sub_rgb = nh.subscribe<sensor_msgs::Image>("/camera/rgb/image_color", 1, &depth_estimater::rgbImageCallback, this);
	sub_depth = nh.subscribe<sensor_msgs::Image>("/camera/depth/image", 1, &depth_estimater::depthImageCallback, this);
}

depth_estimater::~depth_estimater(){
}
 
void depth_estimater::rgbImageCallback(const sensor_msgs::ImageConstPtr& msg){

	int i, j;
	int x1, x2, y1, y2;
	int width = WIDTH;
	int height = HEIGHT;
	cv_bridge::CvImagePtr cv_ptr;//画像を扱うためのもの

/*
	画像を購読し、BGR8形式(1チャンネル(青、緑、赤のどれか1つ)につき8ビット(0〜255階調))
	のカラー画像に変換し、cv_ptrに代入している。ここで注意すべき点は、チャンネルの順番が
	BGRすなわち青、緑、赤の順になっている点。画像を購読できない場合は、エラーを出力。
*/

	try{
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}catch (cv_bridge::Exception& ex){
		ROS_ERROR("error");
		exit(-1);
	}

	cv::Mat &mat = cv_ptr->image;
	//赤枠の大きさを設定
	x1 = int(mat.cols / 2) - width;
	x2 = int(mat.cols / 2) + width;
	y1 = int(mat.rows / 2) - height;
	y2 = int(mat.rows / 2) + height;

/*	赤枠内の画素にアクセスし、青と緑のチャンネルに0を代入している。
	チャンネルの順番は青、緑、赤。画素にアクセスしているプログラムは3行あるが
	、先ほどのチャンネルの順番となっているので注意。画素にアクセスする方法は
	いくつかある。
*/
	for(i = y1; i < y2; i++){
		for(j = x1; j < x2; j++){
			// 0 : blue, 1 : green, 2 : red.
			mat.data[i * mat.step + j * mat.elemSize() + 0] = 0;
			mat.data[i * mat.step + j * mat.elemSize() + 1] = 0;
			//mat.data[i * mat.step + j * mat.elemSize() + 2] = 0;
		}
	}

	//rectangle関数で枠の外側を赤色で囲っている。
	cv::rectangle(cv_ptr->image, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(0, 0, 255), 3, 4);
	cv::imshow("RGB image", cv_ptr->image);//imshow関数により画像を表示
/*	waitKey関数はキーが押されるまで待機する関数。今回はキー入力を必要としないが、
	画像を表示する場合には必要な関数なので記述する。
*/
	cv::waitKey(10);
}

void depth_estimater::depthImageCallback(const sensor_msgs::ImageConstPtr& msg){

	int x1, x2, y1, y2;
	int i, j, k;
	int width = WIDTH;
	int height = HEIGHT;
	double sum = 0.0;
	double ave;
	cv_bridge::CvImagePtr cv_ptr;

	//TYPE_32FC1は、1チャンネル浮動小数点数(1つの画素に32ビット)を表わす。
	
	try{
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_32FC1);
	}catch (cv_bridge::Exception& ex){
		ROS_ERROR("error");
		exit(-1);
	}
	//CV_8UC1は、先ほど出てきたBGR8と同じ。
	cv::Mat depth(cv_ptr->image.rows, cv_ptr->image.cols, CV_32FC1);
	cv::Mat img(cv_ptr->image.rows, cv_ptr->image.cols, CV_8UC1);

	x1 = int(depth.cols / 2) - width;
	x2 = int(depth.cols / 2) + width;
	y1 = int(depth.rows / 2) - height;
	y2 = int(depth.rows / 2) + height;

/*	枠内の距離値の合計を計算している。その際、もし距離が0[m]以下であれば、除外。
	CV_32FC1型では、dataメソッドを使用して画素にアクセスすることができないため、
	ptrメソッドを使用している。また、距離画像を可視化するために、距離
	値を取得できる最大距離で割り、255を掛けている。255は8ビットで表現できる最大の
	数。最後に、枠内の画素数で割り、計算した平均距離を次の行で出力している。
*/
	for(i = 0; i < cv_ptr->image.rows;i++){
		float* Dimage = cv_ptr->image.ptr<float>(i);
		float* Iimage = depth.ptr<float>(i);
		char* Ivimage = img.ptr<char>(i);
		for(j = 0 ; j < cv_ptr->image.cols; j++){
			if(Dimage[j] > 0.0){
				Iimage[j] = Dimage[j];
				Ivimage[j] = (char)(255*(Dimage[j]/5.5));
			}else{
			}

			if(i > y1 && i < y2){
				if(j > x1 && j < x2){
					if(Dimage[j] > 0.0){
						sum += Dimage[j];
					}
				}
			}
		}
	}

	ave = sum / ((width * 2) * (height * 2));
	ROS_INFO("depth : %f [m]", ave);

	cv::imshow("DEPTH image", img);
	cv::waitKey(10);
}

int main(int argc, char **argv){
	ros::init(argc, argv, "depth_estimater");

	depth_estimater depth_estimater;

	ros::spin();
	return 0;
}
