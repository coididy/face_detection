#pragma once
#include <QString>
typedef struct myStruct
{
	uint aSize{0};//账户字节数
	uint pSize{0};//密码字节数
	qint64 pos{0};//文件中的位置
	QString account;
	QString password;
}AccountAndPass;