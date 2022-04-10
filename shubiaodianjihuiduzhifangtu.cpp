#include <opencv2/opencv.hpp>
#include  <String> 
using namespace std;
using namespace cv;


#define WINDOW_NAME1 "【窗口1】"       
#define WINDOW_NAME2 "【窗口2】"   



void on_MouseHandle(int event, int x, int y, int flags, void* param);
void DrawRectangle(cv::Mat& img, cv::Rect box);
void qiuhuiduzhifangtu(Mat& image, Rect box,int i);

Rect g_rectangle;
Point point;
bool g_bDrawingBox = false;
int i=0;




int main()
{
	system("color 9F");

	Mat srcImage1,srcImage2;
	//Mat tempImage;
	Mat gray1,gray2;

	srcImage1 = imread("./1.jpg");
	srcImage2 = imread("./2.jpg");

	cvtColor(srcImage1, gray1, COLOR_BGR2GRAY);
	cvtColor(srcImage2, gray2, COLOR_BGR2GRAY);
	namedWindow(WINDOW_NAME1);
	namedWindow(WINDOW_NAME2);
	setMouseCallback(WINDOW_NAME1 ,on_MouseHandle, (void*)&gray1);
	setMouseCallback(WINDOW_NAME2, on_MouseHandle, (void*)&gray2);
	while (1)
	{
		
		imshow(WINDOW_NAME1, gray1);
		imshow(WINDOW_NAME2, gray2);
		waitKey(1);
	}
	
	return 0;
}




void on_MouseHandle(int event, int x, int y, int flags, void* param)
{

	Mat& image = *(Mat*)param;
	switch (event)
	{

	
	case EVENT_LBUTTONDOWN:
	{
		g_bDrawingBox = true;
		g_rectangle = Rect(x-4.5, y-4.5, 9, 9);
		point.x = x;
		point.y = y;
	}
	break;

	case EVENT_LBUTTONUP:
	{
		
		qiuhuiduzhifangtu(image, g_rectangle,i);
		DrawRectangle(image, g_rectangle);
		putText(image, format("%d", i),point,1,2,(255,0,255),2);
		i++;
	}
	break;

	}
}




void DrawRectangle(cv::Mat& img, cv::Rect box)
{
	cv::rectangle(img, box.tl(), box.br(), cv::Scalar(0,255,255));
}
void qiuhuiduzhifangtu(Mat& image, Rect box,int i)
{
	Rect roi = box;
	Mat srcImage11;
	srcImage11 = image(roi);
	

	MatND dstHist;      
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float* ranges[] = { hranges };   
	int size = 256;
	int channels = 0;
	calcHist(&srcImage11, 1, &channels, Mat(), dstHist, dims, &size, ranges);    
	int scale = 1;
	Mat dstImage(size * scale, size, CV_8U, Scalar(0));
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0); 
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);          
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		rectangle(dstImage, Point(i * scale, size - 1), Point((i + 1) * scale - 1, size - realValue), Scalar(255));
	}
	imshow(format("一维直方图 %d",i), dstImage);

}