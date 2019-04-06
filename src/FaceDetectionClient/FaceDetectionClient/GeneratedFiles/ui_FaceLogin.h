/********************************************************************************
** Form generated from reading UI file 'FaceLogin.ui'
**
** Created by: Qt User Interface Compiler version 5.11.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACELOGIN_H
#define UI_FACELOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_FaceLogin
{
public:
    QPushButton *loginBtn;

    void setupUi(QDialog *FaceLogin)
    {
        if (FaceLogin->objectName().isEmpty())
            FaceLogin->setObjectName(QStringLiteral("FaceLogin"));
        FaceLogin->resize(400, 300);
        loginBtn = new QPushButton(FaceLogin);
        loginBtn->setObjectName(QStringLiteral("loginBtn"));
        loginBtn->setGeometry(QRect(140, 180, 93, 28));

        retranslateUi(FaceLogin);
        QObject::connect(loginBtn, SIGNAL(clicked()), FaceLogin, SLOT(faceWindowShow()));

        QMetaObject::connectSlotsByName(FaceLogin);
    } // setupUi

    void retranslateUi(QDialog *FaceLogin)
    {
        FaceLogin->setWindowTitle(QApplication::translate("FaceLogin", "FaceLogin", nullptr));
        loginBtn->setText(QApplication::translate("FaceLogin", "\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FaceLogin: public Ui_FaceLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACELOGIN_H
