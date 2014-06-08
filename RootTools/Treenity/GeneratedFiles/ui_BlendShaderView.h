/********************************************************************************
** Form generated from reading UI file 'BlendShaderView.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLENDSHADERVIEW_H
#define UI_BLENDSHADERVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/TextureDrop.h>

QT_BEGIN_NAMESPACE

class Ui_meshBlendedShaderView
{
public:
    QGridLayout *gridLayout;
    QSpinBox *spinBox_tileFactor;
    QLabel *label;
    QSlider *horizontalSlider_tileFactor;
    TextureDrop *textureDrop_diffuseB;
    TextureDrop *textureDrop_diffuseG;
    TextureDrop *textureDrop_diffuseR;
    TextureDrop *textureDrop_blendmap;
    TextureDrop *textureDrop_glow;
    TextureDrop *textureDrop_specular;

    void setupUi(QWidget *meshBlendedShaderView)
    {
        if (meshBlendedShaderView->objectName().isEmpty())
            meshBlendedShaderView->setObjectName(QStringLiteral("meshBlendedShaderView"));
        meshBlendedShaderView->resize(400, 448);
        meshBlendedShaderView->setMinimumSize(QSize(0, 448));
        meshBlendedShaderView->setMaximumSize(QSize(16777215, 448));
        gridLayout = new QGridLayout(meshBlendedShaderView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setHorizontalSpacing(10);
        gridLayout->setVerticalSpacing(0);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        spinBox_tileFactor = new QSpinBox(meshBlendedShaderView);
        spinBox_tileFactor->setObjectName(QStringLiteral("spinBox_tileFactor"));
        spinBox_tileFactor->setMinimumSize(QSize(50, 0));

        gridLayout->addWidget(spinBox_tileFactor, 6, 1, 1, 1);

        label = new QLabel(meshBlendedShaderView);
        label->setObjectName(QStringLiteral("label"));
        label->setMinimumSize(QSize(50, 0));
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 6, 0, 1, 1);

        horizontalSlider_tileFactor = new QSlider(meshBlendedShaderView);
        horizontalSlider_tileFactor->setObjectName(QStringLiteral("horizontalSlider_tileFactor"));
        horizontalSlider_tileFactor->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_tileFactor, 6, 2, 1, 1);

        textureDrop_diffuseB = new TextureDrop(meshBlendedShaderView);
        textureDrop_diffuseB->setObjectName(QStringLiteral("textureDrop_diffuseB"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(textureDrop_diffuseB->sizePolicy().hasHeightForWidth());
        textureDrop_diffuseB->setSizePolicy(sizePolicy);
        textureDrop_diffuseB->setMinimumSize(QSize(0, 71));
        textureDrop_diffuseB->setMaximumSize(QSize(16777215, 16777215));
        textureDrop_diffuseB->setStyleSheet(QStringLiteral("background-color: rgb(90, 90, 90);"));

        gridLayout->addWidget(textureDrop_diffuseB, 5, 0, 1, 3);

        textureDrop_diffuseG = new TextureDrop(meshBlendedShaderView);
        textureDrop_diffuseG->setObjectName(QStringLiteral("textureDrop_diffuseG"));
        sizePolicy.setHeightForWidth(textureDrop_diffuseG->sizePolicy().hasHeightForWidth());
        textureDrop_diffuseG->setSizePolicy(sizePolicy);
        textureDrop_diffuseG->setMinimumSize(QSize(0, 71));
        textureDrop_diffuseG->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));

        gridLayout->addWidget(textureDrop_diffuseG, 4, 0, 1, 3);

        textureDrop_diffuseR = new TextureDrop(meshBlendedShaderView);
        textureDrop_diffuseR->setObjectName(QStringLiteral("textureDrop_diffuseR"));
        sizePolicy.setHeightForWidth(textureDrop_diffuseR->sizePolicy().hasHeightForWidth());
        textureDrop_diffuseR->setSizePolicy(sizePolicy);
        textureDrop_diffuseR->setMinimumSize(QSize(0, 71));
        textureDrop_diffuseR->setMaximumSize(QSize(16777215, 16777215));
        textureDrop_diffuseR->setStyleSheet(QStringLiteral("background-color: rgb(90, 90, 90);"));

        gridLayout->addWidget(textureDrop_diffuseR, 3, 0, 1, 3);

        textureDrop_blendmap = new TextureDrop(meshBlendedShaderView);
        textureDrop_blendmap->setObjectName(QStringLiteral("textureDrop_blendmap"));
        sizePolicy.setHeightForWidth(textureDrop_blendmap->sizePolicy().hasHeightForWidth());
        textureDrop_blendmap->setSizePolicy(sizePolicy);
        textureDrop_blendmap->setMinimumSize(QSize(0, 71));
        textureDrop_blendmap->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));

        gridLayout->addWidget(textureDrop_blendmap, 2, 0, 1, 3);

        textureDrop_glow = new TextureDrop(meshBlendedShaderView);
        textureDrop_glow->setObjectName(QStringLiteral("textureDrop_glow"));
        sizePolicy.setHeightForWidth(textureDrop_glow->sizePolicy().hasHeightForWidth());
        textureDrop_glow->setSizePolicy(sizePolicy);
        textureDrop_glow->setMinimumSize(QSize(0, 71));
        textureDrop_glow->setMaximumSize(QSize(16777215, 16777215));
        textureDrop_glow->setStyleSheet(QStringLiteral("background-color: rgb(90, 90, 90);"));

        gridLayout->addWidget(textureDrop_glow, 1, 0, 1, 3);

        textureDrop_specular = new TextureDrop(meshBlendedShaderView);
        textureDrop_specular->setObjectName(QStringLiteral("textureDrop_specular"));
        sizePolicy.setHeightForWidth(textureDrop_specular->sizePolicy().hasHeightForWidth());
        textureDrop_specular->setSizePolicy(sizePolicy);
        textureDrop_specular->setMinimumSize(QSize(0, 71));
        textureDrop_specular->setStyleSheet(QStringLiteral("background-color: rgb(70, 70, 70);"));

        gridLayout->addWidget(textureDrop_specular, 0, 0, 1, 3);


        retranslateUi(meshBlendedShaderView);
        QObject::connect(horizontalSlider_tileFactor, SIGNAL(valueChanged(int)), spinBox_tileFactor, SLOT(setValue(int)));
        QObject::connect(spinBox_tileFactor, SIGNAL(valueChanged(int)), horizontalSlider_tileFactor, SLOT(setValue(int)));

        QMetaObject::connectSlotsByName(meshBlendedShaderView);
    } // setupUi

    void retranslateUi(QWidget *meshBlendedShaderView)
    {
        meshBlendedShaderView->setWindowTitle(QApplication::translate("meshBlendedShaderView", "Form", 0));
        label->setText(QApplication::translate("meshBlendedShaderView", "Tile factor", 0));
    } // retranslateUi

};

namespace Ui {
    class meshBlendedShaderView: public Ui_meshBlendedShaderView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLENDSHADERVIEW_H
