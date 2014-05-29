/********************************************************************************
** Form generated from reading UI file 'WaterTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATERTOOL_H
#define UI_WATERTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaterUI
{
public:
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_3;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_2;
    QPushButton *pushButton_resetWater;
    QPushButton *pushButton_initWater;
    QDoubleSpinBox *doubleSpinBox_waterSpeed;
    QCheckBox *checkBox_waterRefractions;
    QDoubleSpinBox *doubleSpinBox_waterDepth;
    QPushButton *pushButton_waterColor;
    QCheckBox *checkBox_waterNormals;
    QCheckBox *checkBox_waterReflections;
    QLabel *label_7;
    QCheckBox *checkBox_waterPause;

    void setupUi(QWidget *WaterUI)
    {
        if (WaterUI->objectName().isEmpty())
            WaterUI->setObjectName(QStringLiteral("WaterUI"));
        WaterUI->resize(419, 347);
        gridLayout = new QGridLayout(WaterUI);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_5 = new QLabel(WaterUI);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        label_6 = new QLabel(WaterUI);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);

        label_3 = new QLabel(WaterUI);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label = new QLabel(WaterUI);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(100, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_4 = new QLabel(WaterUI);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        label_2 = new QLabel(WaterUI);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pushButton_resetWater = new QPushButton(WaterUI);
        pushButton_resetWater->setObjectName(QStringLiteral("pushButton_resetWater"));

        gridLayout->addWidget(pushButton_resetWater, 8, 0, 1, 2);

        pushButton_initWater = new QPushButton(WaterUI);
        pushButton_initWater->setObjectName(QStringLiteral("pushButton_initWater"));

        gridLayout->addWidget(pushButton_initWater, 7, 0, 1, 2);

        doubleSpinBox_waterSpeed = new QDoubleSpinBox(WaterUI);
        doubleSpinBox_waterSpeed->setObjectName(QStringLiteral("doubleSpinBox_waterSpeed"));
        doubleSpinBox_waterSpeed->setMinimum(1);
        doubleSpinBox_waterSpeed->setMaximum(50);

        gridLayout->addWidget(doubleSpinBox_waterSpeed, 0, 1, 1, 1);

        checkBox_waterRefractions = new QCheckBox(WaterUI);
        checkBox_waterRefractions->setObjectName(QStringLiteral("checkBox_waterRefractions"));
        checkBox_waterRefractions->setChecked(true);

        gridLayout->addWidget(checkBox_waterRefractions, 3, 1, 1, 1);

        doubleSpinBox_waterDepth = new QDoubleSpinBox(WaterUI);
        doubleSpinBox_waterDepth->setObjectName(QStringLiteral("doubleSpinBox_waterDepth"));
        doubleSpinBox_waterDepth->setDecimals(3);
        doubleSpinBox_waterDepth->setMaximum(5);
        doubleSpinBox_waterDepth->setSingleStep(0.01);

        gridLayout->addWidget(doubleSpinBox_waterDepth, 1, 1, 1, 1);

        pushButton_waterColor = new QPushButton(WaterUI);
        pushButton_waterColor->setObjectName(QStringLiteral("pushButton_waterColor"));
        pushButton_waterColor->setMinimumSize(QSize(0, 40));

        gridLayout->addWidget(pushButton_waterColor, 2, 1, 1, 1);

        checkBox_waterNormals = new QCheckBox(WaterUI);
        checkBox_waterNormals->setObjectName(QStringLiteral("checkBox_waterNormals"));
        checkBox_waterNormals->setChecked(true);

        gridLayout->addWidget(checkBox_waterNormals, 5, 1, 1, 1);

        checkBox_waterReflections = new QCheckBox(WaterUI);
        checkBox_waterReflections->setObjectName(QStringLiteral("checkBox_waterReflections"));
        checkBox_waterReflections->setChecked(true);

        gridLayout->addWidget(checkBox_waterReflections, 4, 1, 1, 1);

        label_7 = new QLabel(WaterUI);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 6, 0, 1, 1);

        checkBox_waterPause = new QCheckBox(WaterUI);
        checkBox_waterPause->setObjectName(QStringLiteral("checkBox_waterPause"));

        gridLayout->addWidget(checkBox_waterPause, 6, 1, 1, 1);


        retranslateUi(WaterUI);

        QMetaObject::connectSlotsByName(WaterUI);
    } // setupUi

    void retranslateUi(QWidget *WaterUI)
    {
        WaterUI->setWindowTitle(QApplication::translate("WaterUI", "Form", 0));
        label_5->setText(QApplication::translate("WaterUI", "Water normals:", 0));
        label_6->setText(QApplication::translate("WaterUI", "Water color:", 0));
        label_3->setText(QApplication::translate("WaterUI", "Water refractions:", 0));
        label->setText(QApplication::translate("WaterUI", "Water speed:", 0));
        label_4->setText(QApplication::translate("WaterUI", "Water reflections:", 0));
        label_2->setText(QApplication::translate("WaterUI", "Water depth:", 0));
        pushButton_resetWater->setText(QApplication::translate("WaterUI", "Reset water", 0));
        pushButton_initWater->setText(QApplication::translate("WaterUI", "Init water", 0));
        checkBox_waterRefractions->setText(QString());
        pushButton_waterColor->setText(QString());
        checkBox_waterNormals->setText(QString());
        checkBox_waterReflections->setText(QString());
        label_7->setText(QApplication::translate("WaterUI", "Water pause:", 0));
        checkBox_waterPause->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class WaterUI: public Ui_WaterUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATERTOOL_H
