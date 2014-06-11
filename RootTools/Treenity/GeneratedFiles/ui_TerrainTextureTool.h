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
#include <QtWidgets/QHBoxLayout>
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
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox_size;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QLabel *label_2;
    QSpinBox *spinBox_opacity;
    QSlider *horizontalSlider_opacity;

    void setupUi(QWidget *TerrainTextureTool)
    {
        if (TerrainTextureTool->objectName().isEmpty())
            TerrainTextureTool->setObjectName(QStringLiteral("TerrainTextureTool"));
        TerrainTextureTool->resize(467, 32);
        TerrainTextureTool->setMinimumSize(QSize(467, 32));
        TerrainTextureTool->setMaximumSize(QSize(16777215, 32));
        horizontalLayout = new QHBoxLayout(TerrainTextureTool);
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(TerrainTextureTool);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        spinBox_size = new QSpinBox(TerrainTextureTool);
        spinBox_size->setObjectName(QStringLiteral("spinBox_size"));
        spinBox_size->setMinimum(-99);
        spinBox_size->setValue(2);

        horizontalLayout->addWidget(spinBox_size);

        pushButton = new QPushButton(TerrainTextureTool);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setMinimumSize(QSize(32, 32));
        pushButton->setMaximumSize(QSize(32, 32));
        pushButton->setStyleSheet(QLatin1String("QPushButton \n"
"{\n"
"background-color: red;\n"
"}\n"
"\n"
"QPushButton:checked \n"
"{ \n"
"border: 2px solid orange;\n"
"}"));
        pushButton->setCheckable(true);
        pushButton->setChecked(true);
        pushButton->setAutoExclusive(true);

        horizontalLayout->addWidget(pushButton);

        pushButton_2 = new QPushButton(TerrainTextureTool);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setMinimumSize(QSize(32, 32));
        pushButton_2->setMaximumSize(QSize(32, 32));
        pushButton_2->setStyleSheet(QLatin1String("QPushButton {\n"
"background-color: green\n"
"}\n"
"QPushButton:checked \n"
"{ \n"
"border: 2px solid orange;\n"
"}"));
        pushButton_2->setCheckable(true);
        pushButton_2->setAutoExclusive(true);

        horizontalLayout->addWidget(pushButton_2);

        pushButton_3 = new QPushButton(TerrainTextureTool);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
        pushButton_3->setMinimumSize(QSize(32, 32));
        pushButton_3->setMaximumSize(QSize(32, 32));
        pushButton_3->setStyleSheet(QLatin1String("QPushButton {\n"
"background-color: blue\n"
"}\n"
"\n"
"QPushButton:checked \n"
"{ \n"
"border: 2px solid orange;\n"
"}"));
        pushButton_3->setCheckable(true);
        pushButton_3->setAutoExclusive(true);

        horizontalLayout->addWidget(pushButton_3);

        label_2 = new QLabel(TerrainTextureTool);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout->addWidget(label_2);

        spinBox_opacity = new QSpinBox(TerrainTextureTool);
        spinBox_opacity->setObjectName(QStringLiteral("spinBox_opacity"));
        spinBox_opacity->setMinimum(1);
        spinBox_opacity->setMaximum(100);
        spinBox_opacity->setSingleStep(1);
        spinBox_opacity->setValue(100);

        horizontalLayout->addWidget(spinBox_opacity);

        horizontalSlider_opacity = new QSlider(TerrainTextureTool);
        horizontalSlider_opacity->setObjectName(QStringLiteral("horizontalSlider_opacity"));
        horizontalSlider_opacity->setMinimum(1);
        horizontalSlider_opacity->setMaximum(100);
        horizontalSlider_opacity->setSingleStep(1);
        horizontalSlider_opacity->setSliderPosition(100);
        horizontalSlider_opacity->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider_opacity);


        retranslateUi(TerrainTextureTool);

        QMetaObject::connectSlotsByName(TerrainTextureTool);
    } // setupUi

    void retranslateUi(QWidget *TerrainTextureTool)
    {
        TerrainTextureTool->setWindowTitle(QApplication::translate("TerrainTextureTool", "Form", 0));
        label->setText(QApplication::translate("TerrainTextureTool", "Size:", 0));
        pushButton->setText(QString());
        pushButton_2->setText(QString());
        pushButton_3->setText(QString());
        label_2->setText(QApplication::translate("TerrainTextureTool", "Opacity %:", 0));
    } // retranslateUi

};

namespace Ui {
    class TerrainTextureTool: public Ui_TerrainTextureTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINTEXTURETOOL_H
