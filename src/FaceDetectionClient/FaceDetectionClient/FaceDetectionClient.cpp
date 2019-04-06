#include "FaceDetectionClient.h"

FaceDetectionClient::FaceDetectionClient(QWidget *parent)
	: QWidget(parent)
{
	this->setWindowIcon(QIcon(":/Resources/icon/main.ico"));
	this->setWindowTitle(QString::fromLocal8Bit("人脸识别系统"));

	QPixmap pix(":/Resources/images/mainWindow.png");
	this->setMaximumSize(QSize(pix.width(), pix.height()));
	this->setMinimumSize(QSize(pix.width(), pix.height()));
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setAutoFillBackground(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(pix));
	this->setPalette(palette);

	//创建最大最小化按钮
	createMinMaxBtn();
	//创建工具栏四个按钮
	createToolBtn();
	//创建状态栏显示时间
	createStateLabel();
	//创建主界面摄像头与图片的显示
	createVideoImage();

	thread = new QThread(this);
	startSeetaFace = false;
	isVideoing = false;
	picLabelIsOpen = false;//已经打开了一张图片
	seetaFaceThread = nullptr;
}
//创建最小化关闭按钮
void FaceDetectionClient::createMinMaxBtn()
{

	//最小化按钮
	minBtn = new QPushButton(this);
	minBtn->move(this->width() - 80, 0);
	minBtn->setFixedSize(40, 40);
	minBtn->setFlat(true);//窗口透明并不显示边框
	minBtn->setToolTip(QString::fromLocal8Bit("最小化"));
	minBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/minimize.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/minimize_hover.png);}");
	connect(minBtn, &QPushButton::clicked, this, [=]()
	{
		//隐藏主窗体
		this->hide();
		//初始化系统托盘
		systemTrayIcon = new QSystemTrayIcon(this);
		//新建程序在托盘中的显示图标
		QIcon icon(":/Resources/icon/main.ico");
		//将图标放入系统托盘对象中
		systemTrayIcon->setIcon(icon);
		//显示提示信息
		systemTrayIcon->setToolTip(QString::fromLocal8Bit("人脸识别系统"));
		connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &FaceDetectionClient::systemTrayIconClicked);
		
		//在系统托盘显示该托盘对象
		systemTrayIcon->show();
	});

	//关闭按钮
	closeBtn = new QPushButton(this);
	closeBtn->move(this->width() - 40, 0);
	closeBtn->setFixedSize(40, 40);
	closeBtn->setFlat(true);//窗口透明并不显示边框
	closeBtn->setToolTip(QString::fromLocal8Bit("关闭"));
	closeBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/close.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/close_hover.png);}");
	connect(closeBtn, &QPushButton::clicked, this, [=]()
	{
		if (QMessageBox::Yes == QMessageBox::information(this, QString::fromLocal8Bit("提示!"), QString::fromLocal8Bit("是否退出本系统？"), QMessageBox::Yes|QMessageBox::No))
		{
			this->close();
		}
		else return;
	});

}
//创建工具栏四个按钮
void FaceDetectionClient::createToolBtn()
{
	//视频播放按钮
	videoBtn = new QPushButton(this);
	videoBtn->move(125, 50);
	videoBtn->setFixedSize(150, 150);
	videoBtn->setCursor(Qt::PointingHandCursor);//设置手形鼠标
	videoBtn->setFlat(true);//窗口透明并不显示边框
	videoBtn->setToolTip(QString::fromLocal8Bit("点击播放视频"));
	videoBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/playBtn.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/playBtn_pressed.png);}");
	connect(videoBtn, &QPushButton::clicked, this, &FaceDetectionClient::videoBtnClicked);
	//摄像头按钮
	cameraBtn = new QPushButton(this);
	cameraBtn->move(375, 50);
	cameraBtn->setFixedSize(150, 150);
	cameraBtn->setCursor(Qt::PointingHandCursor);//设置手形鼠标
	cameraBtn->setFlat(true);//窗口透明并不显示边框
	cameraBtn->setToolTip(QString::fromLocal8Bit("点击打开摄像头"));
	cameraBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/videoBtn.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/videoBtn_hover.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/videoBtn_pressed.png);}");
	connect(cameraBtn, &QPushButton::clicked, this, &FaceDetectionClient::cameraBtnClicked);
	//打开文件按钮
	openFileBtn = new QPushButton(this);
	openFileBtn->move(625, 50);
	openFileBtn->setFixedSize(150, 150);
	openFileBtn->setCursor(Qt::PointingHandCursor);//设置手形鼠标
	openFileBtn->setFlat(true);//窗口透明并不显示边框
	openFileBtn->setToolTip(QString::fromLocal8Bit("点击打开文件"));
	openFileBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/openFileBtn.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/openFileBtn_pressed.png);}");
	connect(openFileBtn, &QPushButton::clicked, this, &FaceDetectionClient::openFileBtnClicked);
	//人脸比对按钮
	faceConBtn = new QPushButton(this);
	faceConBtn->move(875, 50);
	faceConBtn->setFixedSize(150, 150);
	faceConBtn->setCursor(Qt::PointingHandCursor);//设置手形鼠标
	faceConBtn->setFlat(true);//窗口透明并不显示边框
	faceConBtn->setToolTip(QString::fromLocal8Bit("点击进行人脸比对"));
	faceConBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/faceConBtn.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/faceConBtn_pressed.png);}");
	connect(faceConBtn, &QPushButton::clicked, this, &FaceDetectionClient::faceConBtnClicked);

	//拍照按钮
	photographBtn = new QPushButton(this);
	photographBtn->move(70, 600);//上面的图片从550结束
	photographBtn->setFixedSize(65, 65);
	photographBtn->setToolTip(QString::fromLocal8Bit("点击拍照"));
	photographBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/normal_photograph_button.png);}"
		"QPushButton:hover{ border-image:url(:/Resources/images/hover_photograph_button.png); }"
		"QPushButton:pressed{border-image:url(:/Resources/images/pressed_photograph_button.png);}");
	connect(photographBtn, &QPushButton::clicked, this, &FaceDetectionClient::photographBtnClicked);
	photographBtn->hide();//隐藏

	//边缘检测按钮
	edgeDetectionBtn = new QPushButton(this);
	edgeDetectionBtn->move(15, 560 + photographBtn->height() + 10);
	edgeDetectionBtn->setText(QString::fromLocal8Bit("边缘检测"));
	edgeDetectionBtn->hide();//取消该功能
	connect(edgeDetectionBtn, &QPushButton::clicked, this, &FaceDetectionClient::edgeDetectionBtnClicked);

	//视频录制开始按钮
	videoRecordBtn = new QPushButton(this);
	videoRecordBtn->move(210, 600);
	videoRecordBtn->setFixedSize(65, 65);
	videoRecordBtn->setToolTip(QString::fromLocal8Bit("开始录制"));
	videoRecordBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/normal_play_button.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/hover_play_button.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/pressed_play_button.png);}");
	videoRecordBtn->hide();//隐藏
	connect(videoRecordBtn, &QPushButton::clicked, this, &FaceDetectionClient::videoRecordBtnClicked);
	
	//显示人脸比对结果
	showResultLabel = new QLabel(this);
	showResultLabel->move(755, 580);
	showResultLabel->setFixedWidth(1070-755);
	showResultLabel->setFixedHeight(630 - 580);
	showResultLabel->setStyleSheet("QLabel{color:white;font-size:25px;font-weight:bold;font-family:'楷体';}");
	showResultLabel->hide();//隐藏
}
//创建状态栏显示时间
void FaceDetectionClient::createStateLabel()
{
	//显示时间
	showtimeLabel = new QLabel(this);
	showtimeLabel->move(0, this->height() - 30);
	showtimeLabel->setFixedWidth(275);
	showtimeLabel->setFixedHeight(36);
	showtimeLabel->setStyleSheet("QLabel{color:white;font-size:20px}");
	QDateTime time = QDateTime::currentDateTime();
	QString current_date = time.toString("yyyy-MM-dd hh:mm:ss dddd");
	showtimeLabel->setText(current_date);

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [=]()
	{
		QDateTime time = QDateTime::currentDateTime();
		QString current_date = time.toString("yyyy-MM-dd hh:mm:ss dddd");
		showtimeLabel->setText(current_date);
	});
	timer->start(1000);//定时器，1s动一次
}
//创建主界面摄像头与图片的显示
void FaceDetectionClient::createVideoImage()
{
	//显示视频
	videoLabel = new QLabel(this);
	videoLabel->move(15, 200);
	videoLabel->setFixedSize(550, 350);
	videoLabel->setStyleSheet("QLabel{border-image:url(:/Resources/images/videoLabel.png);}");
							//"QLabel:pressed{border-image:url(:/Resources/images/videoLabel_pressed.png);}");  /*QLabel没有点击属性*/
	//子窗口
	videoLabelSon = new QLabel(this);
	videoLabelSon->move(44, 230);
	videoLabelSon->setFixedSize(473, 280);
	videoLabelSon->hide();//初始时隐藏
	
						  //显示图片
	picLabel = new QLabel(this);
	picLabel->move(585, 200);
	picLabel->setFixedSize(550, 350);
	picLabel->setStyleSheet("QLabel{border-image:url(:/Resources/images/picLabel.png);}");
	//子窗口
	picLabelSon = new QLabel(this);
	picLabelSon->move(620, 230);
	picLabelSon->setFixedSize(473, 280);
	picLabelSon->hide();//初始时隐藏
}
//重载基类的鼠标按下事件处理函数
void FaceDetectionClient::mousePressEvent(QMouseEvent * event)
{
	mMoveing = true;
	/*
	 * 窗口移动距离
	 * globalPose(), 以桌面左上角为原点，绝对坐标
	 * pos(), 窗体左上角（不含边框）的坐标，只要鼠标在窗体内，且窗体不动，坐标不变
	 */
	mMovePosition = event->globalPos() - pos();
	return QWidget::mousePressEvent(event);
}
//重载基类的鼠标释放事件处理函数
void FaceDetectionClient::mouseReleaseEvent(QMouseEvent * event)
{
	mMoveing = false;
	return QWidget::mouseReleaseEvent(event);
}
//重载基类的鼠标移动事件处理函数
void FaceDetectionClient::mouseMoveEvent(QMouseEvent * event)
{
	/*
		 *  event->globalPos() - move_point
		 * =event->globalPos() - (event->globalPos0(原) - pos0())
		 * =pos() + (event->globalPos(现) - event->globalPos0(原))
		 */
	if (mMoveing && (event->buttons() && Qt::LeftButton)
		&& (event->globalPos() - mMovePosition).manhattanLength() > QApplication::startDragDistance())
	{
		this->move(event->globalPos() - mMovePosition);
		mMovePosition = event->globalPos() - pos();
	}
	return QWidget::mouseMoveEvent(event);
}
//托盘中的图标被点击后
void FaceDetectionClient::systemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
	case QSystemTrayIcon::Context://鼠标右键点击
		break;
	case QSystemTrayIcon::DoubleClick://鼠标双击
		break;
	case QSystemTrayIcon::Trigger:
		this->show();//显示主窗口
		systemTrayIcon->hide();//托盘隐藏
		break;
	case QSystemTrayIcon::MiddleClick://鼠标中键
		break;
	default:
		break;
	}
}
//视频播放按钮被点击
void FaceDetectionClient::videoBtnClicked()
{
	if (startSeetaFace)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("正在进行人脸比对，请稍后进行操作！"));
		return;
	}
	//隐藏
	showResultLabel->hide();

	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开视频文件"), ".", "Video File(*.mp4 *.avi *.flv *.mkv)");
	if (filename.isEmpty())//打开文件对话框后，没有进行任何操作应该直接返回，继续播放视频
	{
		return;
	}
	if (capture.isOpened())//如果已经打开了一个视频，那么先清除现有视频
	{
		capture.release();//清除
		timer->stop();
	}
	//open函数参数如果为空（可以用0代替）则默认打开摄像头，如果是路径则默认打开视频源。
	capture.open(filename.toLocal8Bit().data());
	if (capture.isOpened())//如果视频成功打开
	{
		rate = capture.get(CV_CAP_PROP_FPS);//获取视频每秒帧数
		capture >> frame;//给Mat读取一帧图片
		if (!frame.empty())//Mat有值，即有图片
		{
			videoLabelSon->show();//显示子窗口
			timer = new QTimer(this);
			image = cvMat2QImage(frame);
			//指定矩形大小，保持原图片宽高比不变进行缩放至该矩形
			image = image.scaled(473, 280, Qt::KeepAspectRatio);
			//图片居中显示
			videoLabelSon->setAlignment(Qt::AlignCenter);
			videoLabelSon->setPixmap(QPixmap::fromImage(image));//显示帧图片
			timer->setInterval(1000 / rate);//获取帧时
			connect(timer, &QTimer::timeout, this, &FaceDetectionClient::videoLabelShow);
			timer->start();
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("视频打开失败！"));
		videoLabelSon->hide();
	}
}
//更新视频帧图片
void FaceDetectionClient::videoLabelShow()
{
	capture >> frame;
	if (!frame.empty())
	{
		image = cvMat2QImage(frame);
		image = image.scaled(473, 280, Qt::KeepAspectRatio);
		videoLabelSon->setAlignment(Qt::AlignCenter);
		videoLabelSon->setPixmap(QPixmap::fromImage(image));//显示帧图片
		if (writer.isOpened())//如果正在录制视频，则进行写入
		{
			writer << frame;
		}
	}
	else videoLabelSon->hide();//视频结束，子窗口隐藏
}
//摄像头按钮被点击
void FaceDetectionClient::cameraBtnClicked()
{
	if (startSeetaFace)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("正在进行人脸比对，请稍后进行操作！"));
		return;
	}
	photographBtn->show();
	videoRecordBtn->show();

	//隐藏
	showResultLabel->hide();

	if (writer.isOpened())//如果正在录制视频，则直接返回
	{
		return;
	}
	if (capture.isOpened())//如果已经打开了一个视频，那么先清除现有视频
	{
		capture.release();//清除
	}
	capture.open(0);//open参数为0，打开摄像头
	if (capture.isOpened())//如果视频成功打开
	{
		rate = 24;//设置帧数
		capture >> frame;//给Mat读取一帧图片
		if (!frame.empty())//Mat有值，即有图片
		{
			videoLabelSon->show();//显示子窗口
			timer = new QTimer(this);
			image = cvMat2QImage(frame);
			//指定矩形大小，保持原图片宽高比不变进行缩放至该矩形
			image = image.scaled(473, 280, Qt::KeepAspectRatio);
			//图片居中显示
			videoLabelSon->setAlignment(Qt::AlignCenter);
			videoLabelSon->setPixmap(QPixmap::fromImage(image));//显示帧图片
			timer->setInterval(1000 / rate);//获取帧时
			connect(timer, &QTimer::timeout, this, &FaceDetectionClient::videoLabelShow);
			timer->start();
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("摄像头打开失败！"));
		videoLabelSon->hide();
	}
}
//拍照按钮被点击
void FaceDetectionClient::photographBtnClicked()
{
	capture >> frame;//给Mat读取一帧照片
	if (!frame.empty())//Mat有值，即有图片
	{
		image = cvMat2QImage(frame);
		//指定矩形大小，保持原图片宽高比不变进行缩放至该矩形
		image = image.scaled(473, 280, Qt::KeepAspectRatio);
		//图片居中显示
		picLabelSon->show();//显示
		picLabelSon->setAlignment(Qt::AlignCenter);
		picLabelSon->setPixmap(QPixmap::fromImage(image));//显示帧图片
		QDateTime time = QDateTime::currentDateTime();
		QString current_date = time.toString("hh:mm:ss");
		QStringList list = current_date.split(':');
		QString filename = QString("./photograph/") + list[0] + list[1] + list[2] + ".png";
		image.save(filename);
	}
}
//边缘检测按钮被点击
void FaceDetectionClient::edgeDetectionBtnClicked()
{
	cv::Mat cannyMat;
	//输入图像，输出图像，阙值1（低于它的像素点被认为不是边缘），阙值2（高于它的像素点会被认为是边缘），sobel算子大小
	Canny(frame, cannyMat, 50, 150, 3);
	cv::namedWindow("Canny",0);//0表示按照获取时的图片大小进行显示，而  不按照  图片的原始大小进行显示
	cv::imshow("Canny", cannyMat);
}
//视频录制开始按钮被点击
void FaceDetectionClient::videoRecordBtnClicked()
{
	if (isVideoing)//正在录像
	{
		isVideoing = false;
		writer.release();
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("视频录制结束"));
		videoRecordBtn->setToolTip(QString::fromLocal8Bit("开始录制"));
		videoRecordBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/normal_play_button.png);}"
			"QPushButton:hover{border-image:url(:/Resources/images/hover_play_button.png);}");
	}
	else
	{
		isVideoing = true;
		videoRecordBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/pressed_play_button.png);}"
			"QPushButton:hover{border-image:url(:/Resources/images/pressed_hover_play_button.png);}");
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("视频录制开始"));
		videoRecordBtn->setToolTip(QString::fromLocal8Bit("停止录制"));
		QDateTime time = QDateTime::currentDateTime();
		QString current_date = time.toString("hh:mm:ss");
		QStringList list = current_date.split(':');
		QString filename = QString("./videoRecord/") + list[0] + list[1] + list[2] + ".avi";
		//文件名、编码形式，使用 CV_FOURCC()宏、帧率、分辨率、是否有颜色
		writer.open(filename.toStdString(), cv::VideoWriter::fourcc('P', 'I', 'M', '1'), 24, cv::Size(640, 480), true);
		int t = 100;
	}
}
//文件按钮被点击
void FaceDetectionClient::openFileBtnClicked()
{
	//不能支持PNG图片
	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开图片文件"), ".", "*.jpg *.bmp *.gif");
	if (filename.isEmpty()) return;

	if (startSeetaFace)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("正在进行人脸比对，请稍后进行操作！"));
		return;
	}
	//隐藏
	showResultLabel->hide();

	picLabelSon->show();
	QImage image(filename);
	image = image.scaled(473, 280, Qt::KeepAspectRatio);
	picLabelSon->setAlignment(Qt::AlignCenter);
	picLabelSon->setPixmap(QPixmap::fromImage(image));//显示图片
	picImage = image;
	picLabelIsOpen = true;
}
//人脸比对按钮被点击
void FaceDetectionClient::faceConBtnClicked()
{
	if (!picLabelIsOpen)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("请先打开一张比对原图！"));
		return;
	}

	if (startSeetaFace)//正在进行人脸比对
	{
		if (QMessageBox::Yes == QMessageBox::information(this, QString::fromLocal8Bit("提示！"), QString::fromLocal8Bit("是否退出人脸比对？"), QMessageBox::Yes | QMessageBox::No))
		{
			emit sendStop();//发送停止信号
			return;
		}
		else
		{
			return;
		}
	}
	faceConBtn->setToolTip(QString::fromLocal8Bit("点击关闭人脸比对"));

	cameraBtnClicked();//打开摄像头
	startSeetaFace = true;//标志位置true

	showResultLabel->show();//显示
	showResultLabel->setText(QString::fromLocal8Bit("正在进行人脸比对……"));

	//创建子线程，在子线程中创建一个定时器
	seetaFaceThread = new SeetaFaceThread;
	seetaFaceThread->moveToThread(thread);
	connect(thread, &QThread::finished, seetaFaceThread, &SeetaFaceThread::deleteLater);
	//启动子线程
	thread->start();

	//主线程向子线程传递原始比对图片
	connect(this, &FaceDetectionClient::sendSrcImage, seetaFaceThread, &SeetaFaceThread::getSrcImage);
	//从子线程中接收错误反馈
	connect(seetaFaceThread, &SeetaFaceThread::sendError, this, &FaceDetectionClient::getThreadError);
	//收到子线程的结果
	connect(seetaFaceThread, &SeetaFaceThread::sendResult, this, [=](float result, cv::Mat mat)
	{
		showResultLabel->setText(QString::fromLocal8Bit("相似度：")+QString::number(result));
		showResultLabel->show();

		cv::namedWindow("Face_feature_points", 1);
		cv::imshow("Face_feature_points", mat);

		//关闭摄像头
		capture.release();
		//释放自定义类
		//seetaFaceThread->deleteLater();
		thread->quit();
		thread->wait();
		faceConBtn->setToolTip(QString::fromLocal8Bit("点击进行人脸比对"));
		startSeetaFace = false;
	});
	//子线程向主线程请求数据
	connect(seetaFaceThread, &SeetaFaceThread::sendGetTestImage, this, [=]()
	{
		capture >> frame;
		image = cvMat2QImage(frame);
		emit sendTestImage(image);
	}, Qt::DirectConnection);
	//https://blog.csdn.net/kucoffee12/article/details/78523106
	connect(this, &FaceDetectionClient::sendTestImage, seetaFaceThread, &SeetaFaceThread::getTestImage, Qt::DirectConnection);
	//子线程收到停止信号
	connect(this, &FaceDetectionClient::sendStop, seetaFaceThread, &SeetaFaceThread::getStop,Qt::DirectConnection);

	//将原始比对图片传入子线程，子线程开始进行人脸比对
	emit sendSrcImage(picImage);
}
//获取子线程错误
void FaceDetectionClient::getThreadError(int flag)
{
	qDebug() <<QString::fromLocal8Bit("主线程收到NOFACE")<<"flag:"<<flag<< endl;
	switch (flag)
	{
	case 1://未发现相匹配的人脸
		showResultLabel->setText(QString::fromLocal8Bit("未捕获到相匹配的人脸！"));
		break;
	case 2://原始比对图片中无人脸
		showResultLabel->setText(QString::fromLocal8Bit("比对图中未检测到人脸！"));
		break;
	case 3://系统错误
		showResultLabel->setText(QString::fromLocal8Bit("系统错误！"));
		break;
	default:
		break;
	}
	//关闭摄像头
	capture.release();
	//释放自定义类
	//seetaFaceThread->deleteLater();
	thread->quit();
	thread->wait();
	startSeetaFace = false;
	faceConBtn->setToolTip(QString::fromLocal8Bit("点击进行人脸比对"));
}
//QImage转CV::Mat
cv::Mat FaceDetectionClient::QImage2cvMat(const QImage &inImage, bool inCloneImageData)
{
	switch (inImage.format())
	{
		// 8-bit, 4 channel
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
	{
		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		return (inCloneImageData ? mat.clone() : mat);
	}
	// 8-bit, 3 channel
	case QImage::Format_RGB32:
	{
		if (!inCloneImageData)
		{
			qWarning() << "QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
		}

		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC4,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		cv::Mat  matNoAlpha;

		cv::cvtColor(mat, matNoAlpha, cv::COLOR_BGRA2BGR);   // drop the all-white alpha channel

		return matNoAlpha;
	}
	// 8-bit, 3 channel
	case QImage::Format_RGB888:
	{
		if (!inCloneImageData)
		{
			qWarning() << "QImageToCvMat() - Conversion requires cloning so we don't modify the original QImage data";
		}

		QImage   swapped = inImage.rgbSwapped();

		return cv::Mat(swapped.height(), swapped.width(),
			CV_8UC3,
			const_cast<uchar*>(swapped.bits()),
			static_cast<size_t>(swapped.bytesPerLine())
		).clone();
	}

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
	{
		cv::Mat  mat(inImage.height(), inImage.width(),
			CV_8UC1,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
		);

		return (inCloneImageData ? mat.clone() : mat);
	}
	default:
		qWarning() << "QImage2CvMat() - QImage format not handled in switch:" << inImage.format();
		break;
	}
	return cv::Mat();
}
//CV::Mat转QImage
QImage FaceDetectionClient::cvMat2QImage(const cv::Mat &inMat)
{
	switch (inMat.type())
	{
		// 8-bit, 4 channel
	case CV_8UC4:
	{
		QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_ARGB32);
		return image;
	}
	// 8-bit, 3 channel
	case CV_8UC3:
	{
		QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	// 8-bit, 1 channel
	case CV_8UC1:
	{
#if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
		QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Grayscale8);
#else
		static QVector<QRgb>  sColorTable;

		// only create our color table the first time
		if (sColorTable.isEmpty())
		{
			sColorTable.resize(256);

			for (int i = 0; i < 256; ++i)
			{
				sColorTable[i] = qRgb(i, i, i);
			}
		}

		QImage image(inMat.data,
			inMat.cols, inMat.rows,
			static_cast<int>(inMat.step),
			QImage::Format_Indexed8);

		image.setColorTable(sColorTable);
#endif
		return image;
	}
	default:
		qWarning() << "cvMat2QImage() - cv::Mat image type not handled in switch:" << inMat.type();
		break;
	}
	return QImage();
}
FaceDetectionClient::~FaceDetectionClient()
{
	thread->quit();
	thread->wait();
}