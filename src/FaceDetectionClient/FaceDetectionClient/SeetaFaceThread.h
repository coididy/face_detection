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
	//����ԭʼ���ڱȶԵ�ͼƬ
	void getSrcImage(QImage image);
	//���ղ���ͼƬ
	void getTestImage(QImage image);
	//ֹͣ�ź�
	void getStop();
private:

 	volatile bool stopFlag;//ֹͣ�Աȱ�־
	bool stoped;//�����߳�Ҫ��ֹͣ

	cv::Mat srcMat;//ԭͼƬ�����ڱȶԵ�ͼƬ
	cv::Mat testMat;//����ͼƬ
	cv::VideoCapture	capture;//OpenCV�����

	FaceDetection *faceDetection;
	FaceAlignment *faceAlignment;
	FaceIdentification *faceIdentification;

	float *srcFeat;//ԭʼͼƬ����   ����ָ���ԭ��float����
	float *testFeat;//����ͼƬ����
	seeta::FacialLandmark points[5];//�������
	float result;//�ȶԽ��

	//��float *���͵����ݷ����ڴ�
	float *newFeatureBuffer();
	//������ȡ������ֵ��feat
	bool getFeature(const cv::Mat &mat,float *feat);
	//�����Ƚ�
	float featureCompare(float *feat1, float *feat2);
	//�����ȶ�
	void seetaFace();
	//QImageתMat
	cv::Mat QImage2cvMat(const QImage &inImage, bool inCloneImageData = true);
	//MatתQImage
	QImage cvMat2QImage(const cv::Mat &inMat);
signals:
	//�����̷߳��ʹ����ź�
	void sendError(int flag);
	//�����̷߳��ͽ��
	void sendResult(float result,cv::Mat);
	//�����߳���������
	void sendGetTestImage();
private slots:
	//�����������
	void deleteData();
};

