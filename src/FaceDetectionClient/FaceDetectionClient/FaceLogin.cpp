#include "FaceLogin.h"
#include "QApplication"

FaceLogin::FaceLogin(QWidget *parent)
	: QDialog(parent)
{

	mMoveing = false;
	mMovePosition = QPoint(0, 0);
	accountAndPassVector.clear();

	this->setWindowIcon(QIcon(":/Resources/icon/main.ico"));
	this->setWindowTitle(QString::fromLocal8Bit("人脸识别系统"));

	QPixmap pix(":/Resources/images/background1.png");
	this->setMaximumSize(QSize(pix.width(),pix.height()));
	this->setMinimumSize(QSize(pix.width(), pix.height()));
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setAutoFillBackground(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(pix));
	this->setPalette(palette);

	//显示时间
	showtimeLabel = new QLabel(this);
	showtimeLabel->move(this->width() - 80, this->height() - 50);
	showtimeLabel->setFixedWidth(80);
	showtimeLabel->setFixedHeight(60);
	showtimeLabel->setStyleSheet("QLabel{color:#68696a;font-size:15px}");
	QDateTime time = QDateTime::currentDateTime();
	QString current_date = time.toString("hh:mm:ss");
	showtimeLabel->setText(current_date);

	QTimer *timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, [=]()
	{
		QDateTime time = QDateTime::currentDateTime();
		QString current_date = time.toString("hh:mm:ss");
		showtimeLabel->setText(current_date);
	});
	timer->start(1000);//定时器，1s动一次

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
		this->showMinimized();
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
		this->close();
	});

	//登录按钮
	loginBtn = new QPushButton(this);
	loginBtn->setText(QString::fromLocal8Bit("登录"));
	loginBtn->move(120, 321);
	//loginBtn->move(this->width() / 2 - 310 / 2, this->height() - 50 - 60);
	loginBtn->setFixedSize(310, 60);
	loginBtn->setFlat(true);//窗口透明并不显示边框
	loginBtn->setToolTip(QString::fromLocal8Bit("登录"));
	loginBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/login.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/login_hover.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/login_pressed.png);}"
		"QPushButton{color:white;font-size:25px;}");
	connect(loginBtn, &QPushButton::clicked, this, &FaceLogin::loginBtnClicked);

	//注册按钮
	registBtn = new QPushButton(this);
	registBtn->setText(QString::fromLocal8Bit("注册账号"));
	registBtn->move(5, this->height() - 40);
	registBtn->setFixedSize(90, 35);
	registBtn->setFlat(true);//窗口透明
	registBtn->setCursor(Qt::PointingHandCursor);//设置手形鼠标
	registBtn->setStyleSheet("QPushButton{color:#68696a;}"
		"QPushButton:hover{color:#363839;}"
		"QPushButton:pressed{color:#0068cb;}"
		"QPushButton{font-size:20px;border:none;}");//设置按钮无边框

	QFont font("Microsoft YaHei",20,QFont::Normal);

	//账号编辑框
	acountEdit = new QLineEdit(this);
	acountEdit->move(120,203);
	acountEdit->setFixedSize(310, 42);
	acountEdit->setFont(font);
	acountEdit->setStyleSheet("QLineEdit{background:rgba(255,0,0,0);border:none;}");

	//给QLineEdit左侧LeadingPosition或右侧TrailingPosition添加图标
	QAction *accountAction = new QAction(acountEdit);
	accountAction->setIcon(QIcon(":/Resources/images/accountEdit.png"));
	acountEdit->addAction(accountAction, QLineEdit::LeadingPosition);

	QRegExp regx("[0-9]+$");
	QValidator* validator = new QRegExpValidator(regx,this);
	acountEdit->setValidator(validator);//只能输入整数
	acountEdit->setMaxLength(12);//最多12位数

	//密码编辑框
	passwordEdit = new QLineEdit(this);
	passwordEdit->move(120, 245+10);
	passwordEdit->setFixedSize(310, 42);
	passwordEdit->setFont(font);
	passwordEdit->setStyleSheet("QLineEdit{background:rgba(255,0,0,0);border:none;}");
	QAction *passwordAction = new QAction(passwordEdit);
	passwordAction->setIcon(QIcon(":/Resources/images/passwordEdit.png"));
	passwordEdit->addAction(passwordAction, QLineEdit::LeadingPosition);
	passwordEdit->setEchoMode(QLineEdit::Password);//密码形式
	passwordEdit->setMaxLength(12);//最多12位密码

	passwordEdit->installEventFilter(this);

	//是否记住密码
	checkBox = new QCheckBox(QString::fromLocal8Bit("记住密码"), this);
	checkBox->move(125, 285);
	checkBox->setFixedSize(90, 50);

	//读取账户密码文件中的数据到accountAndPassVector
	QFile file("./AccoundAndPassword.ini");
	if (!file.open(QIODevice::ReadOnly)) return;//如果文件不存在，直接结束构造函数
	QDataStream out(&file);
	while (!out.atEnd())
	{
		AccountAndPass account;
		out >> account.aSize >> account.pSize;
		account.pos = file.pos();
		QString str;
		out >> str;
		account.account = str.mid(0, account.aSize);
		account.password = str.mid(account.aSize, account.pSize);
		accountAndPassVector.push_back(account);
	}
	file.close();
}

FaceLogin::~FaceLogin()
{
}
//重载基类的绘制事件
void FaceLogin::paintEvent(QPaintEvent * event)
{
	QPainter p(this);
	p.setPen(QColor("#888f97"));
	p.drawLine(120+10, 245, 120 + 310-10, 245);
	p.drawLine(120+10, 292+5, 120 + 310-10, 292+5);
	QWidget::paintEvent(event);
}
//重载基类的鼠标按下事件处理函数
void FaceLogin::mousePressEvent(QMouseEvent * event)
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
void FaceLogin::mouseReleaseEvent(QMouseEvent * event)
{
	mMoveing = false;
	return QWidget::mouseReleaseEvent(event);
}
//重载基类的鼠标移动事件处理函数
void FaceLogin::mouseMoveEvent(QMouseEvent * event)
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

//点击登陆
void FaceLogin::loginBtnClicked()
{
	//账号和密码框不为空
	if (!acountEdit->text().isEmpty() && !passwordEdit->text().isEmpty())
	{
		QString account = acountEdit->text();
		QString password = passwordEdit->text();
		//登录按钮被点击、账号密码不为空、且记住密码框被选中
		if (checkBox->isChecked())
		{
			int accountIndex = getAccountIndex(account);
			if (accountIndex !=-1)//不等于-1表示该账户存在
			{
				qint64 pos = getAccountPos(password, accountIndex);
				if (pos != -1)//不等于-1表示需要修改密码
				{
					QFile file("./AccoundAndPassword.ini");
					file.open(QIODevice::ReadWrite | QIODevice::Append);
					QDataStream in(&file);
					in.setVersion(QDataStream::Qt_5_11);
					file.seek(pos);
					in << accountAndPassVector[accountIndex].aSize << accountAndPassVector[accountIndex].pSize;
					in << formatAccountPassword(accountIndex);
					file.close();
				}
			}
			else//账户不存在
			{
				QFile file("./AccoundAndPassword.ini");
				file.open(QIODevice::ReadWrite | QIODevice::Append);
				QDataStream in(&file);
				in.setVersion(QDataStream::Qt_5_11);
				AccountAndPass newAccount;
				newAccount.aSize = account.size();
				newAccount.pSize = password.size();
				newAccount.pos = file.pos();
				newAccount.account = account;
				newAccount.password = password;
				accountAndPassVector.push_back(newAccount);//新账号加入数组
				in << newAccount.aSize << newAccount.pSize;
				in << (formatAccountPassword(accountAndPassVector.size() - 1));
				file.close();
			}
		}
	}
	accept();
}
//获取该账户的下标，若账户不存在则返回-1
int FaceLogin::getAccountIndex(const QString &account)
{
	for (int i = 0; i < accountAndPassVector.size(); ++i)
	{
		if (account == accountAndPassVector[i].account)
		{
			return i;
		}
	}
	return -1;
}
//获取该账户在文件中的位置，并判断密码是否一致，若一致则返回-1
qint64 FaceLogin::getAccountPos(const QString &password, const int &index)
{
	if (password == accountAndPassVector[index].password)
	{
		accountAndPassVector[index].password = password;//修改密码
		return -1;
	}
	else return accountAndPassVector[index].pos;
}
//格式化账号与密码字符串,组合成24位数
QString FaceLogin::formatAccountPassword(int index)
{
	QString str = accountAndPassVector[index].account + accountAndPassVector[index].password;
	for (int i = str.size(); i <= 24; ++i)
	{
		str.append(' ');//使用空格补全24位
	}
	return str;
}
//事件过滤器，监听密码框被点击事件
bool FaceLogin::eventFilter(QObject *obj, QEvent *e)
{
	if (obj == passwordEdit) 
	{
		if (e->type() == QEvent::MouseButtonPress) 
		{
			int accountIndex = getAccountIndex(acountEdit->text());
			qDebug() << accountIndex << endl;
			if (accountIndex != -1)
			{
				passwordEdit->setText(accountAndPassVector[accountIndex].password);
			}
			return true;
		}
	}
	return QWidget::eventFilter(obj, e);
}