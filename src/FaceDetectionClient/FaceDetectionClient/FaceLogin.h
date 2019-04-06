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
	//登陆按钮被点击
	void loginBtnClicked();
private:
	QPushButton *loginBtn;//登录按钮
	QPushButton *minBtn;//最小化按钮
	QPushButton *closeBtn;//关闭按钮
	QPushButton *registBtn;//注册按钮
	QLabel		*showtimeLabel;//显示时间
	QLineEdit	*acountEdit;//账号
	QLineEdit	*passwordEdit;//密码
	QCheckBox	*checkBox;//是否记住密码

	bool mMoveing;//判断鼠标是否被按下
	QPoint mMovePosition;//鼠标按下去的坐标
	QVector<AccountAndPass>accountAndPassVector;//账号和密码的列表

	//获取该账户的下标，若账户不存在则返回-1
	int getAccountIndex(const QString &account);
	//获取该账户在文件中的位置，并判断密码是否一致，若一致则返回-1
	qint64 getAccountPos(const QString &password, const int &index);
	//格式化账号与密码字符串,组合成24位数
	QString formatAccountPassword(int index);
protected:
	//事件过滤器，监听密码框被点击事件
	bool eventFilter(QObject *obj, QEvent *e);
	//重载基类的绘制事件
	void paintEvent(QPaintEvent *event);
	//重载基类的鼠标按下事件处理函数
	virtual void mousePressEvent(QMouseEvent *event);
	//重载基类的鼠标释放事件处理函数
	virtual void mouseReleaseEvent(QMouseEvent *event);
	//重载基类的鼠标移动事件处理函数
	virtual void mouseMoveEvent(QMouseEvent *event);
};
