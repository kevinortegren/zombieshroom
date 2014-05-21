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
#include <RootTools/Treenity/Include/TextureDrop.h>

QT_BEGIN_NAMESPACE

class Ui_diffuseShaderView
{
public:
    QGridLayout *gridLayout;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    TextureDrop *widget_2;
    QPushButton *pushButton;

    void setupUi(QWidget *diffuseShaderView)
    {
        if (diffuseShaderView->objectName().isEmpty())
            diffuseShaderView->setObjectName(QStringLiteral("diffuseShaderView"));
        diffuseShaderView->resize(400, 125);
        gridLayout = new QGridLayout(diffuseShaderView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        widget = new QWidget(diffuseShaderView);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        widget_2 = new TextureDrop(widget);
        widget_2->setObjectName(QStringLiteral("widget_2"));

        gridLayout_2->addWidget(widget_2, 0, 0, 1, 1);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        gridLayout_2->addWidget(pushButton, 1, 0, 1, 1);


        gridLayout->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(diffuseShaderView);

        QMetaObject::connectSlotsByName(diffuseShaderView);
    } // setupUi

    void retranslateUi(QWidget *diffuseShaderView)
    {
        diffuseShaderView->setWindowTitle(QApplication::translate("diffuseShaderView", "Form", 0));
        pushButton->setText(QApplication::translate("diffuseShaderView", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class diffuseShaderView: public Ui_diffuseShaderView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFFUSESHADERVIEW_H
