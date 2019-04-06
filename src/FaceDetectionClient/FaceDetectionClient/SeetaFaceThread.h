#pragma once
#include <qobject.h>
#include <QImage>
#include <QDebug>
#include <QTimer>
#include <QThread>
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "SeetaFace\FaceAlignment\face_alignment.h"
#include "SeetaFace\FaceDetection\face_detection.h"
#include "SeetaFace\FaceIdentification\face_identification.h"
using namespace seeta;
class SeetaFaceThread :
	public QObject
{
	Q_OBJECT
public:
	explicit SeetaFaceThread(QObject *parent = nullptr);
	~SeetaFaceThread();


public slots :
	//接收原始用于比对的图片
	void getSrcImage(QImage image);
	//接收测试图片
	void getTestImage(QImage image);
	//停止信号
	void getStop();
private:

 	volatile bool stopFlag;//停止对比标志
	bool stoped;//被主线程要求停止

	cv::Mat srcMat;//原图片，用于比对的图片
	cv::Mat testMat;//测试图片
	cv::VideoCapture	capture;//OpenCV摄像机

	FaceDetection *faceDetection;
	FaceAlignment *faceAlignment;
	FaceIdentification *faceIdentification;

	float *srcFeat;//原始图片特征   设置指针的原因：float数组
	float *testFeat;//测试图片特征
	seeta::FacialLandmark points[5];//五个特征
	float result;//比对结果

	//给float *类型的数据分配内存
	float *newFeatureBuffer();
	//特征抽取，并赋值给feat
	bool getFeature(const cv::Mat &mat,float *feat);
	//特征比较
	float featureCompare(float *feat1, float *feat2);
	//人脸比对
	void seetaFace();
	//QImage转Mat
	cv::Mat QImage2cvMat(const QImage &inImage, bool inCloneImageData = true);
	//Mat转QImage
	QImage cvMat2QImage(const cv::Mat &inMat);
signals:
	//向主线程发送错误信号
	void sendError(int flag);
	//向主线程发送结果
	void sendResult(float result,cv::Mat);
	//向主线程请求数据
	void sendGetTestImage();
private slots:
	//进行数据清空
	void deleteData();
};

