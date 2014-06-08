/********************************************************************************
** Form generated from reading UI file 'RefractiveView.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFRACTIVEVIEW_H
#define UI_REFRACTIVEVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/TextureDrop.h>

QT_BEGIN_NAMESPACE

class Ui_refractiveView
{
public:
    QGridLayout *gridLayout;
    TextureDrop *textureDrop_diffuse;

    void setupUi(QWidget *refractiveView)
    {
        if (refractiveView->objectName().isEmpty())
            refractiveView->setObjectName(QStringLiteral("refractiveView"));
        refractiveView->resize(400, 71);
        gridLayout = new QGridLayout(refractiveView);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textureDrop_diffuse = new TextureDrop(refractiveView);
        textureDrop_diffuse->setObjectName(QStringLiteral("textureDrop_diffuse"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textureDrop_diffuse->sizePolicy().hasHeightForWidth());
        textureDrop_diffuse->setSizePolicy(sizePolicy);
        textureDrop_diffuse->setMinimumSize(QSize(0, 71));
        textureDrop_diffuse->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));

        gridLayout->addWidget(textureDrop_diffuse, 0, 0, 1, 1);


        retranslateUi(refractiveView);

        QMetaObject::connectSlotsByName(refractiveView);
    } // setupUi

    void retranslateUi(QWidget *refractiveView)
    {
        refractiveView->setWindowTitle(QApplication::translate("refractiveView", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class refractiveView: public Ui_refractiveView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFRACTIVEVIEW_H
