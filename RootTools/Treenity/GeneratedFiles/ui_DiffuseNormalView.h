/********************************************************************************
** Form generated from reading UI file 'DiffuseNormalView.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIFFUSENORMALVIEW_H
#define UI_DIFFUSENORMALVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/TextureDrop.h>

QT_BEGIN_NAMESPACE

class Ui_diffuseNormalView
{
public:
    QGridLayout *gridLayout;
    TextureDrop *textureDrop_diffuse;
    TextureDrop *textureDrop_specular;
    TextureDrop *textureDrop_normal;
    TextureDrop *textureDrop_glow;

    void setupUi(QWidget *diffuseNormalView)
    {
        if (diffuseNormalView->objectName().isEmpty())
            diffuseNormalView->setObjectName(QStringLiteral("diffuseNormalView"));
        diffuseNormalView->resize(369, 284);
        gridLayout = new QGridLayout(diffuseNormalView);
        gridLayout->setSpacing(0);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        textureDrop_diffuse = new TextureDrop(diffuseNormalView);
        textureDrop_diffuse->setObjectName(QStringLiteral("textureDrop_diffuse"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textureDrop_diffuse->sizePolicy().hasHeightForWidth());
        textureDrop_diffuse->setSizePolicy(sizePolicy);
        textureDrop_diffuse->setMinimumSize(QSize(0, 71));
        textureDrop_diffuse->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));

        gridLayout->addWidget(textureDrop_diffuse, 0, 0, 1, 1);

        textureDrop_specular = new TextureDrop(diffuseNormalView);
        textureDrop_specular->setObjectName(QStringLiteral("textureDrop_specular"));
        sizePolicy.setHeightForWidth(textureDrop_specular->sizePolicy().hasHeightForWidth());
        textureDrop_specular->setSizePolicy(sizePolicy);
        textureDrop_specular->setMinimumSize(QSize(0, 71));
        textureDrop_specular->setMaximumSize(QSize(16777215, 16777215));
        textureDrop_specular->setStyleSheet(QStringLiteral("background-color: rgb(90, 90, 90);"));

        gridLayout->addWidget(textureDrop_specular, 1, 0, 1, 1);

        textureDrop_normal = new TextureDrop(diffuseNormalView);
        textureDrop_normal->setObjectName(QStringLiteral("textureDrop_normal"));
        sizePolicy.setHeightForWidth(textureDrop_normal->sizePolicy().hasHeightForWidth());
        textureDrop_normal->setSizePolicy(sizePolicy);
        textureDrop_normal->setMinimumSize(QSize(0, 71));
        textureDrop_normal->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));

        gridLayout->addWidget(textureDrop_normal, 2, 0, 1, 1);

        textureDrop_glow = new TextureDrop(diffuseNormalView);
        textureDrop_glow->setObjectName(QStringLiteral("textureDrop_glow"));
        sizePolicy.setHeightForWidth(textureDrop_glow->sizePolicy().hasHeightForWidth());
        textureDrop_glow->setSizePolicy(sizePolicy);
        textureDrop_glow->setMinimumSize(QSize(0, 71));
        textureDrop_glow->setMaximumSize(QSize(16777215, 16777215));
        textureDrop_glow->setStyleSheet(QStringLiteral("background-color: rgb(90, 90, 90);"));

        gridLayout->addWidget(textureDrop_glow, 3, 0, 1, 1);


        retranslateUi(diffuseNormalView);

        QMetaObject::connectSlotsByName(diffuseNormalView);
    } // setupUi

    void retranslateUi(QWidget *diffuseNormalView)
    {
        diffuseNormalView->setWindowTitle(QApplication::translate("diffuseNormalView", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class diffuseNormalView: public Ui_diffuseNormalView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIFFUSENORMALVIEW_H
