#include "FaceLogin.h"
#include "QApplication"

FaceLogin::FaceLogin(QWidget *parent)
	: QDialog(parent)
{

	mMoveing = false;
	mMovePosition = QPoint(0, 0);
	accountAndPassVector.clear();

	this->setWindowIcon(QIcon(":/Resources/icon/main.ico"));
	this->setWindowTitle(QString::fromLocal8Bit("����ʶ��ϵͳ"));

	QPixmap pix(":/Resources/images/background1.png");
	this->setMaximumSize(QSize(pix.width(),pix.height()));
	this->setMinimumSize(QSize(pix.width(), pix.height()));
	this->setWindowFlags(Qt::FramelessWindowHint);

	this->setAutoFillBackground(true);
	QPalette palette = this->palette();
	palette.setBrush(QPalette::Window, QBrush(pix));
	this->setPalette(palette);

	//��ʾʱ��
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
	timer->start(1000);//��ʱ����1s��һ��

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
		this->showMinimized();
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
		this->close();
	});

	//��¼��ť
	loginBtn = new QPushButton(this);
	loginBtn->setText(QString::fromLocal8Bit("��¼"));
	loginBtn->move(120, 321);
	//loginBtn->move(this->width() / 2 - 310 / 2, this->height() - 50 - 60);
	loginBtn->setFixedSize(310, 60);
	loginBtn->setFlat(true);//����͸��������ʾ�߿�
	loginBtn->setToolTip(QString::fromLocal8Bit("��¼"));
	loginBtn->setStyleSheet("QPushButton{border-image:url(:/Resources/images/login.png);}"
		"QPushButton:hover{border-image:url(:/Resources/images/login_hover.png);}"
		"QPushButton:pressed{border-image:url(:/Resources/images/login_pressed.png);}"
		"QPushButton{color:white;font-size:25px;}");
	connect(loginBtn, &QPushButton::clicked, this, &FaceLogin::loginBtnClicked);

	//ע�ᰴť
	registBtn = new QPushButton(this);
	registBtn->setText(QString::fromLocal8Bit("ע���˺�"));
	registBtn->move(5, this->height() - 40);
	registBtn->setFixedSize(90, 35);
	registBtn->setFlat(true);//����͸��
	registBtn->setCursor(Qt::PointingHandCursor);//�����������
	registBtn->setStyleSheet("QPushButton{color:#68696a;}"
		"QPushButton:hover{color:#363839;}"
		"QPushButton:pressed{color:#0068cb;}"
		"QPushButton{font-size:20px;border:none;}");//���ð�ť�ޱ߿�

	QFont font("Microsoft YaHei",20,QFont::Normal);

	//�˺ű༭��
	acountEdit = new QLineEdit(this);
	acountEdit->move(120,203);
	acountEdit->setFixedSize(310, 42);
	acountEdit->setFont(font);
	acountEdit->setStyleSheet("QLineEdit{background:rgba(255,0,0,0);border:none;}");

	//��QLineEdit���LeadingPosition���Ҳ�TrailingPosition���ͼ��
	QAction *accountAction = new QAction(acountEdit);
	accountAction->setIcon(QIcon(":/Resources/images/accountEdit.png"));
	acountEdit->addAction(accountAction, QLineEdit::LeadingPosition);

	QRegExp regx("[0-9]+$");
	QValidator* validator = new QRegExpValidator(regx,this);
	acountEdit->setValidator(validator);//ֻ����������
	acountEdit->setMaxLength(12);//���12λ��

	//����༭��
	passwordEdit = new QLineEdit(this);
	passwordEdit->move(120, 245+10);
	passwordEdit->setFixedSize(310, 42);
	passwordEdit->setFont(font);
	passwordEdit->setStyleSheet("QLineEdit{background:rgba(255,0,0,0);border:none;}");
	QAction *passwordAction = new QAction(passwordEdit);
	passwordAction->setIcon(QIcon(":/Resources/images/passwordEdit.png"));
	passwordEdit->addAction(passwordAction, QLineEdit::LeadingPosition);
	passwordEdit->setEchoMode(QLineEdit::Password);//������ʽ
	passwordEdit->setMaxLength(12);//���12λ����

	passwordEdit->installEventFilter(this);

	//�Ƿ��ס����
	checkBox = new QCheckBox(QString::fromLocal8Bit("��ס����"), this);
	checkBox->move(125, 285);
	checkBox->setFixedSize(90, 50);

	//��ȡ�˻������ļ��е����ݵ�accountAndPassVector
	QFile file("./AccoundAndPassword.ini");
	if (!file.open(QIODevice::ReadOnly)) return;//����ļ������ڣ�ֱ�ӽ������캯��
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
//���ػ���Ļ����¼�
void FaceLogin::paintEvent(QPaintEvent * event)
{
	QPainter p(this);
	p.setPen(QColor("#888f97"));
	p.drawLine(120+10, 245, 120 + 310-10, 245);
	p.drawLine(120+10, 292+5, 120 + 310-10, 292+5);
	QWidget::paintEvent(event);
}
//���ػ������갴���¼�������
void FaceLogin::mousePressEvent(QMouseEvent * event)
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
void FaceLogin::mouseReleaseEvent(QMouseEvent * event)
{
	mMoveing = false;
	return QWidget::mouseReleaseEvent(event);
}
//���ػ��������ƶ��¼�������
void FaceLogin::mouseMoveEvent(QMouseEvent * event)
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

//�����½
void FaceLogin::loginBtnClicked()
{
	//�˺ź������Ϊ��
	if (!acountEdit->text().isEmpty() && !passwordEdit->text().isEmpty())
	{
		QString account = acountEdit->text();
		QString password = passwordEdit->text();
		//��¼��ť��������˺����벻Ϊ�ա��Ҽ�ס�����ѡ��
		if (checkBox->isChecked())
		{
			int accountIndex = getAccountIndex(account);
			if (accountIndex !=-1)//������-1��ʾ���˻�����
			{
				qint64 pos = getAccountPos(password, accountIndex);
				if (pos != -1)//������-1��ʾ��Ҫ�޸�����
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
			else//�˻�������
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
				accountAndPassVector.push_back(newAccount);//���˺ż�������
				in << newAccount.aSize << newAccount.pSize;
				in << (formatAccountPassword(accountAndPassVector.size() - 1));
				file.close();
			}
		}
	}
	accept();
}
//��ȡ���˻����±꣬���˻��������򷵻�-1
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
//��ȡ���˻����ļ��е�λ�ã����ж������Ƿ�һ�£���һ���򷵻�-1
qint64 FaceLogin::getAccountPos(const QString &password, const int &index)
{
	if (password == accountAndPassVector[index].password)
	{
		accountAndPassVector[index].password = password;//�޸�����
		return -1;
	}
	else return accountAndPassVector[index].pos;
}
//��ʽ���˺��������ַ���,��ϳ�24λ��
QString FaceLogin::formatAccountPassword(int index)
{
	QString str = accountAndPassVector[index].account + accountAndPassVector[index].password;
	for (int i = str.size(); i <= 24; ++i)
	{
		str.append(' ');//ʹ�ÿո�ȫ24λ
	}
	return str;
}
//�¼�����������������򱻵���¼�
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