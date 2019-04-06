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
	//托盘中的图标被点击
	void systemTrayIconClicked(QSystemTrayIcon::ActivationReason reason);
	//视频播放按钮被点击
	void videoBtnClicked();
	//显示视频
	void videoLabelShow();
	//摄像头按钮被点击
	void cameraBtnClicked();
	//拍照按钮被点击
	void photographBtnClicked();
	//边缘检测按钮被点击
	void edgeDetectionBtnClicked();
	//视频录制开始按钮被点击
	void videoRecordBtnClicked();
	//文件按钮被点击
	void openFileBtnClicked();
	//人脸比对按钮被点击
	void faceConBtnClicked();
	//获取子线程错误
	void getThreadError(int flag);
	//获取子线程传出的摄像头图片
	//void getCameraMat(cv::Mat mat);
private:
	QPushButton *minBtn;//最小化按钮
	QPushButton *closeBtn;//关闭按钮
	QPushButton *videoBtn;//视频播放按钮
	QPushButton *cameraBtn;//摄像头按钮
	QPushButton *openFileBtn;//打开文件按钮
	QPushButton *faceConBtn;//人脸比对按钮
	QSystemTrayIcon *systemTrayIcon;//系统托盘
	QLabel		*showtimeLabel;//显示时间
	QLabel		*videoLabel;//显示视频
	QLabel		*videoLabelSon;//视频窗口子窗口
	QPushButton	*photographBtn;//拍照按钮
	QPushButton	*edgeDetectionBtn;//边缘检测按钮
	QPushButton *videoRecordBtn;//视频录制按钮
	QLabel		*picLabel;//显示图片
	QLabel		*picLabelSon;//图片窗口子窗口
	QImage		picImage;//图片
	QLabel		*showResultLabel;//显示比对结果

	//显示视频的相关数据
	cv::Mat		frame;//OpenCV视频图片数据
	cv::VideoCapture	capture;//OpenCV视频数据
	QImage		image;//Qt视频图片数据
	double		rate;//视频帧数
	QTimer		*timer;//定时器，根据每帧的时间定时，不断切换图片，形成视频
	cv::VideoWriter	writer;//用图片制作视频显示记录

	bool mMoveing;//判断鼠标是否被按下
	bool picLabelIsOpen;//已经打开了一张图片
	bool isVideoing;//正在录像
	QPoint mMovePosition;//鼠标按下去的坐标

	SeetaFaceThread *seetaFaceThread;//人脸识别线程
	bool startSeetaFace;//正在人脸比对标志
	QThread *thread;

	//创建最大最小化按钮
	void createMinMaxBtn();
	//创建工具栏四个按钮
	void createToolBtn();
	//创建状态栏显示时间
	void createStateLabel();
	//创建主界面摄像头与图片的显示
	void createVideoImage();
	//Mat转QImage
	QImage cvMat2QImage(const cv::Mat &inmat);
	//QImage转Mat
	cv::Mat QImage2cvMat(const QImage &inImage, bool inCloneImageData = true);
protected:
	//重载基类的鼠标按下事件处理函数
	virtual void mousePressEvent(QMouseEvent *event);
	//重载基类的鼠标释放事件处理函数
	virtual void mouseReleaseEvent(QMouseEvent *event);
	//重载基类的鼠标移动事件处理函数
	virtual void mouseMoveEvent(QMouseEvent *event);
signals:
	//传递原始图片
	void sendSrcImage(QImage image);
	//发送测试图片
	void sendTestImage(QImage image);
	//发送停止信号
	void sendStop();
};

