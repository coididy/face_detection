#include "SeetaFaceThread.h"
SeetaFaceThread::SeetaFaceThread(QObject *parent) : QObject(parent)
{
	faceDetection = new FaceDetection("./model/seeta_fd_frontal_v1.0.bin");//��ģ���ļ���·��ʵ����seeta::FaceDetection�Ķ���
	faceDetection->SetMinFaceSize(40);//����Ҫ������������С�ߴ�(Ĭ��ֵ:20����������),Ҳ���������face_detector.SetMaxFaceSize(size);
	faceDetection->SetScoreThresh(2.f);//���ü�⵽�������ĵ÷���ֵ(Ĭ��ֵ:2.0)
	faceDetection->SetImagePyramidScaleFactor(0.8f);//����ͼ���������������(0 <����< 1��Ĭ��ֵ:0.8)
	faceDetection->SetWindowStep(4, 4);//���û������ڵĲ���(Ĭ��:4),face_detector.SetWindowStep(step_x, step_y);

	//��ʼ����������ģ��
	faceAlignment = new FaceAlignment("./model/seeta_fa_v1.1.bin");
	//��ʼ������ʶ��ģ��
	faceIdentification = new FaceIdentification("./model/seeta_fr_v1.0.bin");

	stopFlag = false;
	stoped = false;
	srcFeat = nullptr;
	testFeat = nullptr;
}
SeetaFaceThread::~SeetaFaceThread()
{
	delete faceDetection;
	delete faceAlignment;
	delete faceIdentification;
	delete srcFeat;
	srcFeat = nullptr;
	delete testFeat;
	testFeat = nullptr;
}

//��ȡԭʼ���ڱȶԵ�ͼƬ
void SeetaFaceThread::getSrcImage(QImage image)
{
	srcMat = QImage2cvMat(image);
	
	//�ӵ�ԭʼͼƬ��ʼ��ȡ����
	srcFeat = newFeatureBuffer();

	bool haveFace = false;
	haveFace = getFeature(srcMat, srcFeat);

	if (haveFace == false)//���ԭͼδ��⵽����
	{
		emit sendError(2);
		qDebug() <<QString::fromLocal8Bit("δ��⵽����")  << endl;
		deleteData();//�������
		return;
	}
	//��ʼ�����ȶ�
	qDebug() << QString::fromLocal8Bit("��ʼ�����ȶ�") << endl;
	seetaFace();
	//startFlag = true;
}
//���ղ���ͼƬ
void SeetaFaceThread::getTestImage(QImage image)
{
	testMat = QImage2cvMat(image);
	//�յ�����ͼƬ��ʼ���������ȶ�
	qDebug() << QString::fromLocal8Bit("�յ�testͼƬ") << endl;
}

//��float *���͵����ݷ����ڴ�
float *SeetaFaceThread::newFeatureBuffer()
{
	return new float[this->faceIdentification->feature_size()];
	//return new float[2048];
}
//������ȡ������ֵ��feat
bool SeetaFaceThread::getFeature(const cv::Mat &mat, float *feat)
{
	/*
		1������ж��������������һ�����������������뻺����feat������true
		2�����û���������򷵻�false
	*/
	//ͼƬ�Ҷȴ���
	cv::Mat src_img_gray;
	qDebug() << "mat.channels:" << mat.channels() << endl;
	cv::cvtColor(mat, src_img_gray, CV_BGR2GRAY);
	if (src_img_gray.empty())
	{
		return false;
	}
	qDebug() << "1" << endl;
	//��ȡ�Ҷ�����
	seeta::ImageData src_img_gray_data(src_img_gray.cols, src_img_gray.rows, src_img_gray.channels());
	src_img_gray_data.data = src_img_gray.data;

	//��ȡͼƬ��ɫ
	cv::Mat src_img_color = mat;
	qDebug() << "2" << endl;
	seeta::ImageData src_img_color_data(src_img_color.cols, src_img_color.rows, src_img_color.channels());
	src_img_color_data.data = src_img_color.data;
	qDebug() << "3" << endl;
	std::vector<seeta::FaceInfo>faces = this->faceDetection->Detect(src_img_gray_data);
	int32_t face_num = static_cast<int32_t>(faces.size());//������
	if (face_num == 0) return false;
	qDebug() << "4" << endl;
	this->faceAlignment->PointDetectLandmarks(src_img_gray_data, faces[0], points);//ֻ����һ����
	qDebug() << "5" << endl;
	this->faceIdentification->ExtractFeatureWithCrop(src_img_color_data, points,feat);
	qDebug() << "6" << endl;
	return true;
}
//�����Ƚ�
float SeetaFaceThread::featureCompare(float *feat1, float *feat2)
{
	return this->faceIdentification->CalcSimilarity(feat1, feat2);
}
// QImageתCV::Mat
cv::Mat SeetaFaceThread::QImage2cvMat(const QImage &inImage, bool inCloneImageData)
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
//�����������
void SeetaFaceThread::deleteData()
{
	qDebug() << QString::fromLocal8Bit("�ͷ����߳�������") << endl;

	//delete�����ͷſ�ָ�룬���ǲ����ͷ�Ұָ�룬���Է���ָ������ʼ������������
	//һ��ʼ���ǳ�ʼ��srcFeat��testFeat,����δ֪bug��������ã��ش˼�¼
	delete srcFeat;
	srcFeat = nullptr;
	delete testFeat;
	testFeat = nullptr;
	srcMat.release();
	testMat.release();
}
//�����ȶ�
void SeetaFaceThread::seetaFace()
{
	while (!stopFlag)
	{
		emit sendGetTestImage();
		qDebug() << QString::fromLocal8Bit("��ʼѭ��") << endl;
		//capture >> testMat;
		if (testMat.empty())
		{
			qDebug() << QString::fromLocal8Bit("ûͼƬ") << endl;
			continue;
		}

		testFeat = newFeatureBuffer();

		//������ȡ
		bool haveFace = getFeature(testMat, testFeat);
		
		if (haveFace == false)
		{
			qDebug() << QString::fromLocal8Bit("û��") << endl;
			continue;
		}
		//�����ȶ�
		result=featureCompare(srcFeat, testFeat);

		qDebug() << QString::fromLocal8Bit("�ȶԽ����") << result << endl;

		if (result >= 0.7)//�ȶԽ�����ڵ���70%���ȶԽ���
		{
			//stopFlag = true;
			for (int i = 0; i < 5; ++i)
			{
				cv::circle(testMat, cv::Point(points[i].x, points[i].y), 2, CV_RGB(0, 255, 0));
			}
			emit sendResult(result,testMat);
			break;
		}
		delete testFeat;
		testFeat = nullptr;
		testMat.release();
	}
	qDebug() << QString::fromLocal8Bit("�����ȶԽ���") << endl;
	capture.release();
	deleteData();
	if (stoped)//����Ǳ�Ҫ��ֹͣ��
	{
		emit sendError(1);
	}
}
//CV::MatתQImage
QImage SeetaFaceThread::cvMat2QImage(const cv::Mat &inMat)
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

//�յ�ֹͣ�ź�
void SeetaFaceThread::getStop()
{
	stopFlag = true;
	stoped = true;
}