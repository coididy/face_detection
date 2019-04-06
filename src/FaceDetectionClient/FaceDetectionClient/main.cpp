#include "FaceDetectionClient.h"
#include <QtWidgets/QApplication>
#include "FaceLogin.h"
#include "SeetaFaceThread.h"
#include <QDebug>
#include <QMetaType>
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>  
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FaceDetectionClient w;
	//注册信号与槽自定义数据类型
	qRegisterMetaType<cv::Mat>("cv::Mat");
	//登陆界面
	/*FaceLogin login;
	if (login.exec() == QDialog::Accepted)
	{
		w.show();
		return a.exec();
	}
	else
	{
		return 0;
	}*/
	w.show();
	return a.exec();
	//人脸识别测试
	//SeetaFaceThread seetaFaceThread(nullptr);
	//seetaFaceThread.srcMat = cv::imread("C:\\Users\\lenovo\\Desktop\\6.jpg");
	//seetaFaceThread.testMat = cv::imread("C:\\Users\\lenovo\\Desktop\\7.jpg");
	//seetaFaceThread.srcFeat = seetaFaceThread.newFeatureBuffer();
	//seetaFaceThread.testFeat = seetaFaceThread.newFeatureBuffer();
	//bool feat1 = false;
	//feat1=seetaFaceThread.getFeature(seetaFaceThread.srcMat, seetaFaceThread.srcFeat);
	//qDebug() << feat1 << endl;
	//bool feat2 = false;
	//feat2=seetaFaceThread.getFeature(seetaFaceThread.testMat, seetaFaceThread.testFeat);
	//qDebug() << feat2 << endl;
	//float result = -1;
	//result=seetaFaceThread.featureCompare(seetaFaceThread.srcFeat, seetaFaceThread.testFeat);
	//qDebug() << result << endl;
	//return 0;

}
