#include "SeetaFaceThread.h"
SeetaFaceThread::SeetaFaceThread(QObject *parent) : QObject(parent)
{
	faceDetection = new FaceDetection("./model/seeta_fd_frontal_v1.0.bin");//用模型文件的路径实例化seeta::FaceDetection的对象
	faceDetection->SetMinFaceSize(40);//设置要检测的人脸的最小尺寸(默认值:20，不受限制),也能设置最大：face_detector.SetMaxFaceSize(size);
	faceDetection->SetScoreThresh(2.f);//设置检测到的人脸的得分阈值(默认值:2.0)
	faceDetection->SetImagePyramidScaleFactor(0.8f);//设置图像金字塔比例因子(0 <因子< 1，默认值:0.8)
	faceDetection->SetWindowStep(4, 4);//设置滑动窗口的步长(默认:4),face_detector.SetWindowStep(step_x, step_y);

	//初始化人脸对齐模型
	faceAlignment = new FaceAlignment("./model/seeta_fa_v1.1.bin");
	//初始化人脸识别模型
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

//获取原始用于比对的图片
void SeetaFaceThread::getSrcImage(QImage image)
{
	srcMat = QImage2cvMat(image);
	
	//接到原始图片后开始提取特征
	srcFeat = newFeatureBuffer();

	bool haveFace = false;
	haveFace = getFeature(srcMat, srcFeat);

	if (haveFace == false)//如果原图未检测到人脸
	{
		emit sendError(2);
		qDebug() <<QString::fromLocal8Bit("未检测到人脸")  << endl;
		deleteData();//清空数据
		return;
	}
	//开始人脸比对
	qDebug() << QString::fromLocal8Bit("开始人脸比对") << endl;
	seetaFace();
	//startFlag = true;
}
//接收测试图片
void SeetaFaceThread::getTestImage(QImage image)
{
	testMat = QImage2cvMat(image);
	//收到测试图片后开始进行人脸比对
	qDebug() << QString::fromLocal8Bit("收到test图片") << endl;
}

//给float *类型的数据分配内存
float *SeetaFaceThread::newFeatureBuffer()
{
	return new float[this->faceIdentification->feature_size()];
	//return new float[2048];
}
//特征抽取，并赋值给feat
bool SeetaFaceThread::getFeature(const cv::Mat &mat, float *feat)
{
	/*
		1、如果有多张人脸，输出第一张人脸，把特征放入缓冲区feat，返回true
		2、如果没有人脸，则返回false
	*/
	//图片灰度处理
	cv::Mat src_img_gray;
	qDebug() << "mat.channels:" << mat.channels() << endl;
	cv::cvtColor(mat, src_img_gray, CV_BGR2GRAY);
	if (src_img_gray.empty())
	{
		return false;
	}
	qDebug() << "1" << endl;
	//读取灰度数据
	seeta::ImageData src_img_gray_data(src_img_gray.cols, src_img_gray.rows, src_img_gray.channels());
	src_img_gray_data.data = src_img_gray.data;

	//读取图片颜色
	cv::Mat src_img_color = mat;
	qDebug() << "2" << endl;
	seeta::ImageData src_img_color_data(src_img_color.cols, src_img_color.rows, src_img_color.channels());
	src_img_color_data.data = src_img_color.data;
	qDebug() << "3" << endl;
	std::vector<seeta::FaceInfo>faces = this->faceDetection->Detect(src_img_gray_data);
	int32_t face_num = static_cast<int32_t>(faces.size());//人脸数
	if (face_num == 0) return false;
	qDebug() << "4" << endl;
	this->faceAlignment->PointDetectLandmarks(src_img_gray_data, faces[0], points);//只检测第一张脸
	qDebug() << "5" << endl;
	this->faceIdentification->ExtractFeatureWithCrop(src_img_color_data, points,feat);
	qDebug() << "6" << endl;
	return true;
}
//特征比较
float SeetaFaceThread::featureCompare(float *feat1, float *feat2)
{
	return this->faceIdentification->CalcSimilarity(feat1, feat2);
}
// QImage转CV::Mat
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
//进行数据清空
void SeetaFaceThread::deleteData()
{
	qDebug() << QString::fromLocal8Bit("释放子线程类数据") << endl;

	//delete可以释放空指针，但是不能释放野指针，所以凡是指针必须初始化！！！！！
	//一开始忘记初始化srcFeat与testFeat,出现未知bug，调试许久，特此记录
	delete srcFeat;
	srcFeat = nullptr;
	delete testFeat;
	testFeat = nullptr;
	srcMat.release();
	testMat.release();
}
//人脸比对
void SeetaFaceThread::seetaFace()
{
	while (!stopFlag)
	{
		emit sendGetTestImage();
		qDebug() << QString::fromLocal8Bit("开始循环") << endl;
		//capture >> testMat;
		if (testMat.empty())
		{
			qDebug() << QString::fromLocal8Bit("没图片") << endl;
			continue;
		}

		testFeat = newFeatureBuffer();

		//特征提取
		bool haveFace = getFeature(testMat, testFeat);
		
		if (haveFace == false)
		{
			qDebug() << QString::fromLocal8Bit("没脸") << endl;
			continue;
		}
		//特征比对
		result=featureCompare(srcFeat, testFeat);

		qDebug() << QString::fromLocal8Bit("比对结果：") << result << endl;

		if (result >= 0.7)//比对结果大于等于70%，比对结束
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
	qDebug() << QString::fromLocal8Bit("人脸比对结束") << endl;
	capture.release();
	deleteData();
	if (stoped)//如果是被要求停止的
	{
		emit sendError(1);
	}
}
//CV::Mat转QImage
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

//收到停止信号
void SeetaFaceThread::getStop()
{
	stopFlag = true;
	stoped = true;
}