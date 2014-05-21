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
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/TextureDrop.h>

QT_BEGIN_NAMESPACE

class Ui_diffuseShaderView
{
public:
    QGridLayout *gridLayout;
    TextureDrop *diffuse_textureDrop;
    QGridLayout *gridLayout_2;

    void setupUi(QWidget *diffuseShaderView)
    {
        if (diffuseShaderView->objectName().isEmpty())
            diffuseShaderView->setObjectName(QStringLiteral("diffuseShaderView"));
        diffuseShaderView->resize(400, 125);
        gridLayout = new QGridLayout(diffuseShaderView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        diffuse_textureDrop = new TextureDrop(diffuseShaderView);
        diffuse_textureDrop->setObjectName(QStringLiteral("diffuse_textureDrop"));
        gridLayout_2 = new QGridLayout(diffuse_textureDrop);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));

        gridLayout->addWidget(diffuse_textureDrop, 0, 0, 1, 1);


        retranslateUi(diffuseShaderView);

        QMetaObject::connectSlotsByName(diffuseShaderView);
    } // setupUi

    void retranslateUi(QWidget *diffuseShaderView)
    {
        diffuseShaderView->setWindowTitle(QApplication::translate("diffuseShaderView", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class diffuseShaderView: public Ui_diffuseShaderView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFFUSESHADERVIEW_H
