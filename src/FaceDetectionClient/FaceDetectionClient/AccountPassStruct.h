#pragma once
#include <QString>
typedef struct myStruct
{
	uint aSize{0};//�˻��ֽ���
	uint pSize{0};//�����ֽ���
	qint64 pos{0};//�ļ��е�λ��
	QString account;
	QString password;
}AccountAndPass;