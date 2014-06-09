/********************************************************************************
** Form generated from reading UI file 'TerrainTextureTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINTEXTURETOOL_H
#define UI_TERRAINTEXTURETOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainTextureTool
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *spinBox_size;
    QSlider *horizontalSlider_opacity;
    QLabel *label_2;
    QSpinBox *spinBox_opacity;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton;

    void setupUi(QWidget *TerrainTextureTool)
    {
        if (TerrainTextureTool->objectName().isEmpty())
            TerrainTextureTool->setObjectName(QStringLiteral("TerrainTextureTool"));
        TerrainTextureTool->resize(297, 153);
        gridLayout = new QGridLayout(TerrainTextureTool);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(TerrainTextureTool);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spinBox_size = new QSpinBox(TerrainTextureTool);
        spinBox_size->setObjectName(QStringLiteral("spinBox_size"));
        spinBox_size->setMinimum(-99);
        spinBox_size->setValue(2);

        gridLayout->addWidget(spinBox_size, 0, 1, 1, 3);

        horizontalSlider_opacity = new QSlider(TerrainTextureTool);
        horizontalSlider_opacity->setObjectName(QStringLiteral("horizontalSlider_opacity"));
        horizontalSlider_opacity->setMinimum(1);
        horizontalSlider_opacity->setMaximum(100);
        horizontalSlider_opacity->setSingleStep(1);
        horizontalSlider_opacity->setSliderPosition(100);
        horizontalSlider_opacity->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(horizontalSlider_opacity, 1, 2, 1, 1);

        label_2 = new QLabel(TerrainTextureTool);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        spinBox_opacity = new QSpinBox(TerrainTextureTool);
        spinBox_opacity->setObjectName(QStringLiteral("spinBox_opacity"));
        spinBox_opacity->setMinimum(1);
        spinBox_opacity->setMaximum(100);
        spinBox_opacity->setSingleStep(1);
        spinBox_opacity->setValue(100);

        gridLayout->addWidget(spinBox_opacity, 1, 1, 1, 1);

        pushButton_2 = new QPushButton(TerrainTextureTool);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setCheckable(true);
        pushButton_2->setAutoExclusive(true);

        gridLayout->addWidget(pushButton_2, 3, 0, 1, 3);

        pushButton_3 = new QPushButton(TerrainTextureTool);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setCheckable(true);
        pushButton_3->setAutoExclusive(true);

        gridLayout->addWidget(pushButton_3, 4, 0, 1, 3);

        pushButton = new QPushButton(TerrainTextureTool);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setCheckable(true);
        pushButton->setChecked(true);
        pushButton->setAutoExclusive(true);

        gridLayout->addWidget(pushButton, 2, 0, 1, 3);


        retranslateUi(TerrainTextureTool);

        QMetaObject::connectSlotsByName(TerrainTextureTool);
    } // setupUi

    void retranslateUi(QWidget *TerrainTextureTool)
    {
        TerrainTextureTool->setWindowTitle(QApplication::translate("TerrainTextureTool", "Form", 0));
        label->setText(QApplication::translate("TerrainTextureTool", "Brush size:", 0));
        label_2->setText(QApplication::translate("TerrainTextureTool", "Opacity %:", 0));
        pushButton_2->setText(QApplication::translate("TerrainTextureTool", "PushButton", 0));
        pushButton_3->setText(QApplication::translate("TerrainTextureTool", "PushButton", 0));
        pushButton->setText(QApplication::translate("TerrainTextureTool", "PushButton", 0));
    } // retranslateUi

};

namespace Ui {
    class TerrainTextureTool: public Ui_TerrainTextureTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINTEXTURETOOL_H
