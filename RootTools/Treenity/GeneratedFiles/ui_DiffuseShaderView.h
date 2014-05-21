/********************************************************************************
** Form generated from reading UI file 'DiffuseShaderView.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIFFUSESHADERVIEW_H
#define UI_DIFFUSESHADERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_diffuseShaderView
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton_2;
    QPushButton *pushButton;

    void setupUi(QWidget *diffuseShaderView)
    {
        if (diffuseShaderView->objectName().isEmpty())
            diffuseShaderView->setObjectName(QStringLiteral("diffuseShaderView"));
        diffuseShaderView->resize(400, 628);
        gridLayout = new QGridLayout(diffuseShaderView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton_2 = new QPushButton(diffuseShaderView);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(0, 300));

        gridLayout->addWidget(pushButton_2, 2, 0, 1, 1);

        pushButton = new QPushButton(diffuseShaderView);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout->addWidget(pushButton, 1, 0, 1, 1);


        retranslateUi(diffuseShaderView);

        QMetaObject::connectSlotsByName(diffuseShaderView);
    } // setupUi

    void retranslateUi(QWidget *diffuseShaderView)
    {
        diffuseShaderView->setWindowTitle(QApplication::translate("diffuseShaderView", "Form", 0));
        pushButton_2->setText(QApplication::translate("diffuseShaderView", "PushButton", 0));
        pushButton->setText(QApplication::translate("diffuseShaderView", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class diffuseShaderView: public Ui_diffuseShaderView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFFUSESHADERVIEW_H
