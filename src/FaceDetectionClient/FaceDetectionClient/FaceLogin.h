#pragma once

#include <QDialog>
#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QPoint>
#include <QApplication>
#include <QDateTime>
#include <QTimer>
#include <QLineEdit>
#include <QAction>
#include <QPainter>
#include <QRegExpValidator>
#include <QDebug>
#include <QCheckBox>
#include <QFile>
#include <QVector>
#include <QDataStream>
#include "AccountPassStruct.h"
class FaceLogin : public QDialog
{
	Q_OBJECT

public:
	FaceLogin(QWidget *parent = Q_NULLPTR);
	~FaceLogin();
private slots:
	//��½��ť�����
	void loginBtnClicked();
private:
	QPushButton *loginBtn;//��¼��ť
	QPushButton *minBtn;//��С����ť
	QPushButton *closeBtn;//�رհ�ť
	QPushButton *registBtn;//ע�ᰴť
	QLabel		*showtimeLabel;//��ʾʱ��
	QLineEdit	*acountEdit;//�˺�
	QLineEdit	*passwordEdit;//����
	QCheckBox	*checkBox;//�Ƿ��ס����

	bool mMoveing;//�ж�����Ƿ񱻰���
	QPoint mMovePosition;//��갴��ȥ������
	QVector<AccountAndPass>accountAndPassVector;//�˺ź�������б�

	//��ȡ���˻����±꣬���˻��������򷵻�-1
	int getAccountIndex(const QString &account);
	//��ȡ���˻����ļ��е�λ�ã����ж������Ƿ�һ�£���һ���򷵻�-1
	qint64 getAccountPos(const QString &password, const int &index);
	//��ʽ���˺��������ַ���,��ϳ�24λ��
	QString formatAccountPassword(int index);
protected:
	//�¼�����������������򱻵���¼�
	bool eventFilter(QObject *obj, QEvent *e);
	//���ػ���Ļ����¼�
	void paintEvent(QPaintEvent *event);
	//���ػ������갴���¼�������
	virtual void mousePressEvent(QMouseEvent *event);
	//���ػ��������ͷ��¼�������
	virtual void mouseReleaseEvent(QMouseEvent *event);
	//���ػ��������ƶ��¼�������
	virtual void mouseMoveEvent(QMouseEvent *event);
};
