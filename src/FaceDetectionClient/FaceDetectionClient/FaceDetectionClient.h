#pragma once

#include <QtWidgets/QWidget>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QDebug>
#include <QFont>
#include <QThread>
#include <QDateTime>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QFileDialog>
#include <opencv2/opencv.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include "SeetaFaceThread.h"
class FaceDetectionClient : public QWidget
{
	Q_OBJECT

public:
	FaceDetectionClient(QWidget *parent = Q_NULLPTR);
	~FaceDetectionClient();
private slots:
	//�����е�ͼ�걻���
	void systemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
	//��Ƶ���Ű�ť�����
	void videoBtnClicked();
	//��ʾ��Ƶ
	void videoLabelShow();
	//����ͷ��ť�����
	void cameraBtnClicked();
	//���հ�ť�����
	void photographBtnClicked();
	//��Ե��ⰴť�����
	void edgeDetectionBtnClicked();
	//��Ƶ¼�ƿ�ʼ��ť�����
	void videoRecordBtnClicked();
	//�ļ���ť�����
	void openFileBtnClicked();
	//�����ȶ԰�ť�����
	void faceConBtnClicked();
	//��ȡ���̴߳���
	void getThreadError(int flag);
	//��ȡ���̴߳���������ͷͼƬ
	//void getCameraMat(cv::Mat mat);
private:
	QPushButton *minBtn;//��С����ť
	QPushButton *closeBtn;//�رհ�ť
	QPushButton *videoBtn;//��Ƶ���Ű�ť
	QPushButton *cameraBtn;//����ͷ��ť
	QPushButton *openFileBtn;//���ļ���ť
	QPushButton *faceConBtn;//�����ȶ԰�ť
	QSystemTrayIcon *systemTrayIcon;//ϵͳ����
	QLabel		*showtimeLabel;//��ʾʱ��
	QLabel		*videoLabel;//��ʾ��Ƶ
	QLabel		*videoLabelSon;//��Ƶ�����Ӵ���
	QPushButton	*photographBtn;//���հ�ť
	QPushButton	*edgeDetectionBtn;//��Ե��ⰴť
	QPushButton *videoRecordBtn;//��Ƶ¼�ư�ť
	QLabel		*picLabel;//��ʾͼƬ
	QLabel		*picLabelSon;//ͼƬ�����Ӵ���
	QImage		picImage;//ͼƬ
	QLabel		*showResultLabel;//��ʾ�ȶԽ��

	//��ʾ��Ƶ���������
	cv::Mat		frame;//OpenCV��ƵͼƬ����
	cv::VideoCapture	capture;//OpenCV��Ƶ����
	QImage		image;//Qt��ƵͼƬ����
	double		rate;//��Ƶ֡��
	QTimer		*timer;//��ʱ��������ÿ֡��ʱ�䶨ʱ�������л�ͼƬ���γ���Ƶ
	cv::VideoWriter	writer;//��ͼƬ������Ƶ��ʾ��¼

	bool mMoveing;//�ж�����Ƿ񱻰���
	bool picLabelIsOpen;//�Ѿ�����һ��ͼƬ
	bool isVideoing;//����¼��
	QPoint mMovePosition;//��갴��ȥ������

	SeetaFaceThread *seetaFaceThread;//����ʶ���߳�
	bool startSeetaFace;//���������ȶԱ�־
	QThread *thread;

	//���������С����ť
	void createMinMaxBtn();
	//�����������ĸ���ť
	void createToolBtn();
	//����״̬����ʾʱ��
	void createStateLabel();
	//��������������ͷ��ͼƬ����ʾ
	void createVideoImage();
	//MatתQImage
	QImage cvMat2QImage(const cv::Mat &inmat);
	//QImageתMat
	cv::Mat QImage2cvMat(const QImage &inImage, bool inCloneImageData = true);
protected:
	//���ػ������갴���¼�������
	virtual void mousePressEvent(QMouseEvent *event);
	//���ػ��������ͷ��¼�������
	virtual void mouseReleaseEvent(QMouseEvent *event);
	//���ػ��������ƶ��¼�������
	virtual void mouseMoveEvent(QMouseEvent *event);
signals:
	//����ԭʼͼƬ
	void sendSrcImage(QImage image);
	//���Ͳ���ͼƬ
	void sendTestImage(QImage image);
	//����ֹͣ�ź�
	void sendStop();
};

