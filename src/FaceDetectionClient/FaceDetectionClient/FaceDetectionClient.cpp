#include "FaceDetectionClient.h"

FaceDetectionClient::FaceDetectionClient(QWidget *parent)
	: QWidget(parent)
{
	this->setWindowIcon(QIcon(":/Resources/icon/main.ico"));
	this->setWindowTitle(QString::fromLocal8Bit("����ʶ��ϵͳ"));

	QPixmap pix(":/Resources/images/mainWindow.png");
	this->setMaximumSize(QSize(pix.width(), pix.height()));
	this->setMinimumSize(QSize(pix.width(), pix.height()));
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setAutoFillBackground(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(pix));
	this->setPalette(palette);

	//���������С����ť
	createMinMaxBtn();
	//�����������ĸ���ť
	createToolBtn();
	//����״̬����ʾʱ��
	createStateLabel();
	//��������������ͷ��ͼƬ����ʾ
	createVideoImage();

	thread = new QThread(this);
	startSeetaFace = false;
	isVideoing = false;
	picLabelIsOpen = false;//�Ѿ�����һ��ͼƬ
	seetaFaceThread = nullptr;
}
//������С���رհ�ť
void FaceDetectionClient::createMinMaxBtn()
{

	//��С����ť
	minBtn = new QPushButton(this);
	minBtn->move(this->width() - 80, 0);
	minBtn->setFixedSize(40, 40);
	minBtn->setFlat(true);//����͸��������ʾ�߿�
	minBtn->setToolTip(QString::fromLocal8Bit("��С��"));
	minBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/minimize.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/minimize_hover.png);}");
	connect(minBtn, &QPushButton::clicked, this, [=]()
	{
		//����������
		this->hide();
		//��ʼ��ϵͳ����
		systemTrayIcon = new QSystemTrayIcon(this);
		//�½������������е���ʾͼ��
		QIcon icon(":/Resources/icon/main.ico");
		//��ͼ�����ϵͳ���̶�����
		systemTrayIcon->setIcon(icon);
		//��ʾ��ʾ��Ϣ
		systemTrayIcon->setToolTip(QString::fromLocal8Bit("����ʶ��ϵͳ"));
		connect(systemTrayIcon, &QSystemTrayIcon::activated, this, &FaceDetectionClient::systemTrayIconClicked);
		
		//��ϵͳ������ʾ�����̶���
		systemTrayIcon->show();
	});

	//�رհ�ť
	closeBtn = new QPushButton(this);
	closeBtn->move(this->width() - 40, 0);
	closeBtn->setFixedSize(40, 40);
	closeBtn->setFlat(true);//����͸��������ʾ�߿�
	closeBtn->setToolTip(QString::fromLocal8Bit("�ر�"));
	closeBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/close.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/close_hover.png);}");
	connect(closeBtn, &QPushButton::clicked, this, [=]()
	{
		if (QMessageBox::Yes == QMessageBox::information(this, QString::fromLocal8Bit("��ʾ!"), QString::fromLocal8Bit("�Ƿ��˳���ϵͳ��"), QMessageBox::Yes|QMessageBox::No))
		{
			this->close();
		}
		else return;
	});

}
//�����������ĸ���ť
void FaceDetectionClient::createToolBtn()
{
	//��Ƶ���Ű�ť
	videoBtn = new QPushButton(this);
	videoBtn->move(125, 50);
	videoBtn->setFixedSize(150, 150);
	videoBtn->setCursor(Qt::PointingHandCursor);//�����������
	videoBtn->setFlat(true);//����͸��������ʾ�߿�
	videoBtn->setToolTip(QString::fromLocal8Bit("���������Ƶ"));
	videoBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/playBtn.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/playBtn_pressed.png);}");
	connect(videoBtn, &QPushButton::clicked, this, &FaceDetectionClient::videoBtnClicked);
	//����ͷ��ť
	cameraBtn = new QPushButton(this);
	cameraBtn->move(375, 50);
	cameraBtn->setFixedSize(150, 150);
	cameraBtn->setCursor(Qt::PointingHandCursor);//�����������
	cameraBtn->setFlat(true);//����͸��������ʾ�߿�
	cameraBtn->setToolTip(QString::fromLocal8Bit("���������ͷ"));
	cameraBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/videoBtn.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/videoBtn_hover.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/videoBtn_pressed.png);}");
	connect(cameraBtn, &QPushButton::clicked, this, &FaceDetectionClient::cameraBtnClicked);
	//���ļ���ť
	openFileBtn = new QPushButton(this);
	openFileBtn->move(625, 50);
	openFileBtn->setFixedSize(150, 150);
	openFileBtn->setCursor(Qt::PointingHandCursor);//�����������
	openFileBtn->setFlat(true);//����͸��������ʾ�߿�
	openFileBtn->setToolTip(QString::fromLocal8Bit("������ļ�"));
	openFileBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/openFileBtn.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/openFileBtn_pressed.png);}");
	connect(openFileBtn, &QPushButton::clicked, this, &FaceDetectionClient::openFileBtnClicked);
	//�����ȶ԰�ť
	faceConBtn = new QPushButton(this);
	faceConBtn->move(875, 50);
	faceConBtn->setFixedSize(150, 150);
	faceConBtn->setCursor(Qt::PointingHandCursor);//�����������
	faceConBtn->setFlat(true);//����͸��������ʾ�߿�
	faceConBtn->setToolTip(QString::fromLocal8Bit("������������ȶ�"));
	faceConBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/faceConBtn.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/faceConBtn_pressed.png);}");
	connect(faceConBtn, &QPushButton::clicked, this, &FaceDetectionClient::faceConBtnClicked);

	//���հ�ť
	photographBtn = new QPushButton(this);
	photographBtn->move(70, 600);//�����ͼƬ��550����
	photographBtn->setFixedSize(65, 65);
	photographBtn->setToolTip(QString::fromLocal8Bit("�������"));
	photographBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/normal_photograph_button.png);}"
		"QPushButton:hover{ border-image:url(:/Resources/images/hover_photograph_button.png); }"
		"QPushButton:pressed{border-image:url(:/Resources/images/pressed_photograph_button.png);}");
	connect(photographBtn, &QPushButton::clicked, this, &FaceDetectionClient::photographBtnClicked);
	photographBtn->hide();//����

	//��Ե��ⰴť
	edgeDetectionBtn = new QPushButton(this);
	edgeDetectionBtn->move(15, 560 + photographBtn->height() + 10);
	edgeDetectionBtn->setText(QString::fromLocal8Bit("��Ե���"));
	edgeDetectionBtn->hide();//ȡ���ù���
	connect(edgeDetectionBtn, &QPushButton::clicked, this, &FaceDetectionClient::edgeDetectionBtnClicked);

	//��Ƶ¼�ƿ�ʼ��ť
	videoRecordBtn = new QPushButton(this);
	videoRecordBtn->move(210, 600);
	videoRecordBtn->setFixedSize(65, 65);
	videoRecordBtn->setToolTip(QString::fromLocal8Bit("��ʼ¼��"));
	videoRecordBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/normal_play_button.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/hover_play_button.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/pressed_play_button.png);}");
	videoRecordBtn->hide();//����
	connect(videoRecordBtn, &QPushButton::clicked, this, &FaceDetectionClient::videoRecordBtnClicked);
	
	//��ʾ�����ȶԽ��
	showResultLabel = new QLabel(this);
	showResultLabel->move(755, 580);
	showResultLabel->setFixedWidth(1070-755);
	showResultLabel->setFixedHeight(630 - 580);
	showResultLabel->setStyleSheet("QLabel{color:white;font-size:25px;font-weight:bold;font-family:'����';}");
	showResultLabel->hide();//����
}
//����״̬����ʾʱ��
void FaceDetectionClient::createStateLabel()
{
	//��ʾʱ��
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
	timer->start(1000);//��ʱ����1s��һ��
}
//��������������ͷ��ͼƬ����ʾ
void FaceDetectionClient::createVideoImage()
{
	//��ʾ��Ƶ
	videoLabel = new QLabel(this);
	videoLabel->move(15, 200);
	videoLabel->setFixedSize(550, 350);
	videoLabel->setStyleSheet("QLabel{border-image:url(:/Resources/images/videoLabel.png);}");
							//"QLabel:pressed{border-image:url(:/Resources/images/videoLabel_pressed.png);}");  /*QLabelû�е������*/
	//�Ӵ���
	videoLabelSon = new QLabel(this);
	videoLabelSon->move(44, 230);
	videoLabelSon->setFixedSize(473, 280);
	videoLabelSon->hide();//��ʼʱ����
	
						  //��ʾͼƬ
	picLabel = new QLabel(this);
	picLabel->move(585, 200);
	picLabel->setFixedSize(550, 350);
	picLabel->setStyleSheet("QLabel{border-image:url(:/Resources/images/picLabel.png);}");
	//�Ӵ���
	picLabelSon = new QLabel(this);
	picLabelSon->move(620, 230);
	picLabelSon->setFixedSize(473, 280);
	picLabelSon->hide();//��ʼʱ����
}
//���ػ������갴���¼�������
void FaceDetectionClient::mousePressEvent(QMouseEvent * event)
{
	mMoveing = true;
	/*
	 * �����ƶ�����
	 * globalPose(), ���������Ͻ�Ϊԭ�㣬��������
	 * pos(), �������Ͻǣ������߿򣩵����ֻ꣬Ҫ����ڴ����ڣ��Ҵ��岻�������겻��
	 */
	mMovePosition = event->globalPos() - pos();
	return QWidget::mousePressEvent(event);
}
//���ػ��������ͷ��¼�������
void FaceDetectionClient::mouseReleaseEvent(QMouseEvent * event)
{
	mMoveing = false;
	return QWidget::mouseReleaseEvent(event);
}
//���ػ��������ƶ��¼�������
void FaceDetectionClient::mouseMoveEvent(QMouseEvent * event)
{
	/*
		 *  event->globalPos() - move_point
		 * =event->globalPos() - (event->globalPos0(ԭ) - pos0())
		 * =pos() + (event->globalPos(��) - event->globalPos0(ԭ))
		 */
	if (mMoveing && (event->buttons() && Qt::LeftButton)
		&& (event->globalPos() - mMovePosition).manhattanLength() > QApplication::startDragDistance())
	{
		this->move(event->globalPos() - mMovePosition);
		mMovePosition = event->globalPos() - pos();
	}
	return QWidget::mouseMoveEvent(event);
}
//�����е�ͼ�걻�����
void FaceDetectionClient::systemTrayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
	switch (reason)
	{
	case QSystemTrayIcon::Unknown:
		break;
	case QSystemTrayIcon::Context://����Ҽ����
		break;
	case QSystemTrayIcon::DoubleClick://���˫��
		break;
	case QSystemTrayIcon::Trigger:
		this->show();//��ʾ������
		systemTrayIcon->hide();//��������
		break;
	case QSystemTrayIcon::MiddleClick://����м�
		break;
	default:
		break;
	}
}
//��Ƶ���Ű�ť�����
void FaceDetectionClient::videoBtnClicked()
{
	if (startSeetaFace)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("���ڽ��������ȶԣ����Ժ���в�����"));
		return;
	}
	//����
	showResultLabel->hide();

	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("����Ƶ�ļ�"), ".", "Video File(*.mp4 *.avi *.flv *.mkv)");
	if (filename.isEmpty())//���ļ��Ի����û�н����κβ���Ӧ��ֱ�ӷ��أ�����������Ƶ
	{
		return;
	}
	if (capture.isOpened())//����Ѿ�����һ����Ƶ����ô�����������Ƶ
	{
		capture.release();//���
		timer->stop();
	}
	//open�����������Ϊ�գ�������0���棩��Ĭ�ϴ�����ͷ�������·����Ĭ�ϴ���ƵԴ��
	capture.open(filename.toLocal8Bit().data());
	if (capture.isOpened())//�����Ƶ�ɹ���
	{
		rate = capture.get(CV_CAP_PROP_FPS);//��ȡ��Ƶÿ��֡��
		capture >> frame;//��Mat��ȡһ֡ͼƬ
		if (!frame.empty())//Mat��ֵ������ͼƬ
		{
			videoLabelSon->show();//��ʾ�Ӵ���
			timer = new QTimer(this);
			image = cvMat2QImage(frame);
			//ָ�����δ�С������ԭͼƬ��߱Ȳ�������������þ���
			image = image.scaled(473, 280, Qt::KeepAspectRatio);
			//ͼƬ������ʾ
			videoLabelSon->setAlignment(Qt::AlignCenter);
			videoLabelSon->setPixmap(QPixmap::fromImage(image));//��ʾ֡ͼƬ
			timer->setInterval(1000 / rate);//��ȡ֡ʱ
			connect(timer, &QTimer::timeout, this, &FaceDetectionClient::videoLabelShow);
			timer->start();
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("��Ƶ��ʧ�ܣ�"));
		videoLabelSon->hide();
	}
}
//������Ƶ֡ͼƬ
void FaceDetectionClient::videoLabelShow()
{
	capture >> frame;
	if (!frame.empty())
	{
		image = cvMat2QImage(frame);
		image = image.scaled(473, 280, Qt::KeepAspectRatio);
		videoLabelSon->setAlignment(Qt::AlignCenter);
		videoLabelSon->setPixmap(QPixmap::fromImage(image));//��ʾ֡ͼƬ
		if (writer.isOpened())//�������¼����Ƶ�������д��
		{
			writer << frame;
		}
	}
	else videoLabelSon->hide();//��Ƶ�������Ӵ�������
}
//����ͷ��ť�����
void FaceDetectionClient::cameraBtnClicked()
{
	if (startSeetaFace)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("���ڽ��������ȶԣ����Ժ���в�����"));
		return;
	}
	photographBtn->show();
	videoRecordBtn->show();

	//����
	showResultLabel->hide();

	if (writer.isOpened())//�������¼����Ƶ����ֱ�ӷ���
	{
		return;
	}
	if (capture.isOpened())//����Ѿ�����һ����Ƶ����ô�����������Ƶ
	{
		capture.release();//���
	}
	capture.open(0);//open����Ϊ0��������ͷ
	if (capture.isOpened())//�����Ƶ�ɹ���
	{
		rate = 24;//����֡��
		capture >> frame;//��Mat��ȡһ֡ͼƬ
		if (!frame.empty())//Mat��ֵ������ͼƬ
		{
			videoLabelSon->show();//��ʾ�Ӵ���
			timer = new QTimer(this);
			image = cvMat2QImage(frame);
			//ָ�����δ�С������ԭͼƬ��߱Ȳ�������������þ���
			image = image.scaled(473, 280, Qt::KeepAspectRatio);
			//ͼƬ������ʾ
			videoLabelSon->setAlignment(Qt::AlignCenter);
			videoLabelSon->setPixmap(QPixmap::fromImage(image));//��ʾ֡ͼƬ
			timer->setInterval(1000 / rate);//��ȡ֡ʱ
			connect(timer, &QTimer::timeout, this, &FaceDetectionClient::videoLabelShow);
			timer->start();
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("����ͷ��ʧ�ܣ�"));
		videoLabelSon->hide();
	}
}
//���հ�ť�����
void FaceDetectionClient::photographBtnClicked()
{
	capture >> frame;//��Mat��ȡһ֡��Ƭ
	if (!frame.empty())//Mat��ֵ������ͼƬ
	{
		image = cvMat2QImage(frame);
		//ָ�����δ�С������ԭͼƬ��߱Ȳ�������������þ���
		image = image.scaled(473, 280, Qt::KeepAspectRatio);
		//ͼƬ������ʾ
		picLabelSon->show();//��ʾ
		picLabelSon->setAlignment(Qt::AlignCenter);
		picLabelSon->setPixmap(QPixmap::fromImage(image));//��ʾ֡ͼƬ
		QDateTime time = QDateTime::currentDateTime();
		QString current_date = time.toString("hh:mm:ss");
		QStringList list = current_date.split(':');
		QString filename = QString("./photograph/") + list[0] + list[1] + list[2] + ".png";
		image.save(filename);
	}
}
//��Ե��ⰴť�����
void FaceDetectionClient::edgeDetectionBtnClicked()
{
	cv::Mat cannyMat;
	//����ͼ�����ͼ����ֵ1�������������ص㱻��Ϊ���Ǳ�Ե������ֵ2�������������ص�ᱻ��Ϊ�Ǳ�Ե����sobel���Ӵ�С
	Canny(frame, cannyMat, 50, 150, 3);
	cv::namedWindow("Canny",0);//0��ʾ���ջ�ȡʱ��ͼƬ��С������ʾ����  ������  ͼƬ��ԭʼ��С������ʾ
	cv::imshow("Canny", cannyMat);
}
//��Ƶ¼�ƿ�ʼ��ť�����
void FaceDetectionClient::videoRecordBtnClicked()
{
	if (isVideoing)//����¼��
	{
		isVideoing = false;
		writer.release();
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("��Ƶ¼�ƽ���"));
		videoRecordBtn->setToolTip(QString::fromLocal8Bit("��ʼ¼��"));
		videoRecordBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/normal_play_button.png);}"
			"QPushButton:hover{border-image:url(:/Resources/images/hover_play_button.png);}");
	}
	else
	{
		isVideoing = true;
		videoRecordBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/pressed_play_button.png);}"
			"QPushButton:hover{border-image:url(:/Resources/images/pressed_hover_play_button.png);}");
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("��Ƶ¼�ƿ�ʼ"));
		videoRecordBtn->setToolTip(QString::fromLocal8Bit("ֹͣ¼��"));
		QDateTime time = QDateTime::currentDateTime();
		QString current_date = time.toString("hh:mm:ss");
		QStringList list = current_date.split(':');
		QString filename = QString("./videoRecord/") + list[0] + list[1] + list[2] + ".avi";
		//�ļ�����������ʽ��ʹ�� CV_FOURCC()�ꡢ֡�ʡ��ֱ��ʡ��Ƿ�����ɫ
		writer.open(filename.toStdString(), cv::VideoWriter::fourcc('P', 'I', 'M', '1'), 24, cv::Size(640, 480), true);
		int t = 100;
	}
}
//�ļ���ť�����
void FaceDetectionClient::openFileBtnClicked()
{
	//����֧��PNGͼƬ
	QString filename = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("��ͼƬ�ļ�"), ".", "*.jpg *.bmp *.gif");
	if (filename.isEmpty()) return;

	if (startSeetaFace)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("���ڽ��������ȶԣ����Ժ���в�����"));
		return;
	}
	//����
	showResultLabel->hide();

	picLabelSon->show();
	QImage image(filename);
	image = image.scaled(473, 280, Qt::KeepAspectRatio);
	picLabelSon->setAlignment(Qt::AlignCenter);
	picLabelSon->setPixmap(QPixmap::fromImage(image));//��ʾͼƬ
	picImage = image;
	picLabelIsOpen = true;
}
//�����ȶ԰�ť�����
void FaceDetectionClient::faceConBtnClicked()
{
	if (!picLabelIsOpen)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("���ȴ�һ�űȶ�ԭͼ��"));
		return;
	}

	if (startSeetaFace)//���ڽ��������ȶ�
	{
		if (QMessageBox::Yes == QMessageBox::information(this, QString::fromLocal8Bit("��ʾ��"), QString::fromLocal8Bit("�Ƿ��˳������ȶԣ�"), QMessageBox::Yes | QMessageBox::No))
		{
			emit sendStop();//����ֹͣ�ź�
			return;
		}
		else
		{
			return;
		}
	}
	faceConBtn->setToolTip(QString::fromLocal8Bit("����ر������ȶ�"));

	cameraBtnClicked();//������ͷ
	startSeetaFace = true;//��־λ��true

	showResultLabel->show();//��ʾ
	showResultLabel->setText(QString::fromLocal8Bit("���ڽ��������ȶԡ���"));

	//�������̣߳������߳��д���һ����ʱ��
	seetaFaceThread = new SeetaFaceThread;
	seetaFaceThread->moveToThread(thread);
	connect(thread, &QThread::finished, seetaFaceThread, &SeetaFaceThread::deleteLater);
	//�������߳�
	thread->start();

	//���߳������̴߳���ԭʼ�ȶ�ͼƬ
	connect(this, &FaceDetectionClient::sendSrcImage, seetaFaceThread, &SeetaFaceThread::getSrcImage);
	//�����߳��н��մ�����
	connect(seetaFaceThread, &SeetaFaceThread::sendError, this, &FaceDetectionClient::getThreadError);
	//�յ����̵߳Ľ��
	connect(seetaFaceThread, &SeetaFaceThread::sendResult, this, [=](float result, cv::Mat mat)
	{
		showResultLabel->setText(QString::fromLocal8Bit("���ƶȣ�")+QString::number(result));
		showResultLabel->show();

		cv::namedWindow("Face_feature_points", 1);
		cv::imshow("Face_feature_points", mat);

		//�ر�����ͷ
		capture.release();
		//�ͷ��Զ�����
		//seetaFaceThread->deleteLater();
		thread->quit();
		thread->wait();
		faceConBtn->setToolTip(QString::fromLocal8Bit("������������ȶ�"));
		startSeetaFace = false;
	});
	//���߳������߳���������
	connect(seetaFaceThread, &SeetaFaceThread::sendGetTestImage, this, [=]()
	{
		capture >> frame;
		image = cvMat2QImage(frame);
		emit sendTestImage(image);
	}, Qt::DirectConnection);
	//https://blog.csdn.net/kucoffee12/article/details/78523106
	connect(this, &FaceDetectionClient::sendTestImage, seetaFaceThread, &SeetaFaceThread::getTestImage, Qt::DirectConnection);
	//���߳��յ�ֹͣ�ź�
	connect(this, &FaceDetectionClient::sendStop, seetaFaceThread, &SeetaFaceThread::getStop,Qt::DirectConnection);

	//��ԭʼ�ȶ�ͼƬ�������̣߳����߳̿�ʼ���������ȶ�
	emit sendSrcImage(picImage);
}
//��ȡ���̴߳���
void FaceDetectionClient::getThreadError(int flag)
{
	qDebug() <<QString::fromLocal8Bit("���߳��յ�NOFACE")<<"flag:"<<flag<< endl;
	switch (flag)
	{
	case 1://δ������ƥ�������
		showResultLabel->setText(QString::fromLocal8Bit("δ������ƥ���������"));
		break;
	case 2://ԭʼ�ȶ�ͼƬ��������
		showResultLabel->setText(QString::fromLocal8Bit("�ȶ�ͼ��δ��⵽������"));
		break;
	case 3://ϵͳ����
		showResultLabel->setText(QString::fromLocal8Bit("ϵͳ����"));
		break;
	default:
		break;
	}
	//�ر�����ͷ
	capture.release();
	//�ͷ��Զ�����
	//seetaFaceThread->deleteLater();
	thread->quit();
	thread->wait();
	startSeetaFace = false;
	faceConBtn->setToolTip(QString::fromLocal8Bit("������������ȶ�"));
}
//QImageתCV::Mat
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
//CV::MatתQImage
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