/********************************************************************************
** Form generated from reading UI file 'TerrainTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINTOOL_H
#define UI_TERRAINTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainTool
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QCheckBox *checkBox_flat;
    QDoubleSpinBox *doubleSpinBox_strength;
    QPushButton *pushButton_smoothAll;
    QSpinBox *spinBox_size;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_5;
    QCheckBox *checkBox_autoSmooth;
    QCheckBox *checkBox_smoothOnly;
    QLabel *label_4;
    QComboBox *comboBox_shape;
    QLabel *label_6;

    void setupUi(QWidget *TerrainTool)
    {
        if (TerrainTool->objectName().isEmpty())
            TerrainTool->setObjectName(QStringLiteral("TerrainTool"));
        TerrainTool->resize(400, 301);
        gridLayout = new QGridLayout(TerrainTool);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_3 = new QLabel(TerrainTool);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        checkBox_flat = new QCheckBox(TerrainTool);
        checkBox_flat->setObjectName(QStringLiteral("checkBox_flat"));

        gridLayout->addWidget(checkBox_flat, 5, 1, 1, 1);

        doubleSpinBox_strength = new QDoubleSpinBox(TerrainTool);
        doubleSpinBox_strength->setObjectName(QStringLiteral("doubleSpinBox_strength"));
        doubleSpinBox_strength->setMinimum(-1000);
        doubleSpinBox_strength->setMaximum(1000);
        doubleSpinBox_strength->setSingleStep(0.5);
        doubleSpinBox_strength->setValue(2);

        gridLayout->addWidget(doubleSpinBox_strength, 1, 1, 1, 1);

        pushButton_smoothAll = new QPushButton(TerrainTool);
        pushButton_smoothAll->setObjectName(QStringLiteral("pushButton_smoothAll"));

        gridLayout->addWidget(pushButton_smoothAll, 6, 0, 1, 2);

        spinBox_size = new QSpinBox(TerrainTool);
        spinBox_size->setObjectName(QStringLiteral("spinBox_size"));
        spinBox_size->setMinimum(-99);
        spinBox_size->setValue(2);

        gridLayout->addWidget(spinBox_size, 0, 1, 1, 1);

        label = new QLabel(TerrainTool);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(TerrainTool);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(90, 16777215));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_5 = new QLabel(TerrainTool);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        checkBox_autoSmooth = new QCheckBox(TerrainTool);
        checkBox_autoSmooth->setObjectName(QStringLiteral("checkBox_autoSmooth"));

        gridLayout->addWidget(checkBox_autoSmooth, 3, 1, 1, 1);

        checkBox_smoothOnly = new QCheckBox(TerrainTool);
        checkBox_smoothOnly->setObjectName(QStringLiteral("checkBox_smoothOnly"));

        gridLayout->addWidget(checkBox_smoothOnly, 4, 1, 1, 1);

        label_4 = new QLabel(TerrainTool);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 0, 1, 1);

        comboBox_shape = new QComboBox(TerrainTool);
        comboBox_shape->setObjectName(QStringLiteral("comboBox_shape"));

        gridLayout->addWidget(comboBox_shape, 2, 1, 1, 1);

        label_6 = new QLabel(TerrainTool);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 2, 0, 1, 1);


        retranslateUi(TerrainTool);

        QMetaObject::connectSlotsByName(TerrainTool);
    } // setupUi

    void retranslateUi(QWidget *TerrainTool)
    {
        TerrainTool->setWindowTitle(QApplication::translate("TerrainTool", "Form", 0));
        label_3->setText(QApplication::translate("TerrainTool", "Auto smooth:", 0));
        checkBox_flat->setText(QString());
        pushButton_smoothAll->setText(QApplication::translate("TerrainTool", "Smooth all", 0));
        label->setText(QApplication::translate("TerrainTool", "Brush size:", 0));
        label_2->setText(QApplication::translate("TerrainTool", "Brush strength:", 0));
        label_5->setText(QApplication::translate("TerrainTool", "Flat:", 0));
        checkBox_autoSmooth->setText(QString());
        checkBox_smoothOnly->setText(QString());
        label_4->setText(QApplication::translate("TerrainTool", "Smooth only:", 0));
        comboBox_shape->clear();
        comboBox_shape->insertItems(0, QStringList()
         << QApplication::translate("TerrainTool", "Circle", 0)
        );
        label_6->setText(QApplication::translate("TerrainTool", "Brush shape:", 0));
    } // retranslateUi

};

namespace Ui {
    class TerrainTool: public Ui_TerrainTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINTOOL_H
