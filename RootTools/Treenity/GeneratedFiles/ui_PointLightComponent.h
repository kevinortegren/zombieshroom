/********************************************************************************
** Form generated from reading UI file 'PointLightComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_POINTLIGHTCOMPONENT_H
#define UI_POINTLIGHTCOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PointLight
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_attX;
    QDoubleSpinBox *doubleSpinBox_attY;
    QDoubleSpinBox *doubleSpinBox_attZ;
    QLabel *label_2;
    QPushButton *pushButton_color;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_range;

    void setupUi(QWidget *PointLight)
    {
        if (PointLight->objectName().isEmpty())
            PointLight->setObjectName(QStringLiteral("PointLight"));
        PointLight->resize(354, 130);
        PointLight->setMaximumSize(QSize(16777215, 130));
        gridLayout = new QGridLayout(PointLight);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(PointLight);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        doubleSpinBox_attX = new QDoubleSpinBox(PointLight);
        doubleSpinBox_attX->setObjectName(QStringLiteral("doubleSpinBox_attX"));
        doubleSpinBox_attX->setDecimals(3);
        doubleSpinBox_attX->setSingleStep(0.01);

        gridLayout->addWidget(doubleSpinBox_attX, 1, 2, 1, 1);

        doubleSpinBox_attY = new QDoubleSpinBox(PointLight);
        doubleSpinBox_attY->setObjectName(QStringLiteral("doubleSpinBox_attY"));
        doubleSpinBox_attY->setDecimals(3);
        doubleSpinBox_attY->setSingleStep(0.01);

        gridLayout->addWidget(doubleSpinBox_attY, 1, 3, 1, 1);

        doubleSpinBox_attZ = new QDoubleSpinBox(PointLight);
        doubleSpinBox_attZ->setObjectName(QStringLiteral("doubleSpinBox_attZ"));
        doubleSpinBox_attZ->setDecimals(3);
        doubleSpinBox_attZ->setSingleStep(0.01);

        gridLayout->addWidget(doubleSpinBox_attZ, 1, 4, 1, 1);

        label_2 = new QLabel(PointLight);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        pushButton_color = new QPushButton(PointLight);
        pushButton_color->setObjectName(QStringLiteral("pushButton_color"));
        QPalette palette;
        pushButton_color->setPalette(palette);
        pushButton_color->setAutoFillBackground(false);
        pushButton_color->setAutoDefault(false);
        pushButton_color->setDefault(false);
        pushButton_color->setFlat(false);

        gridLayout->addWidget(pushButton_color, 0, 2, 1, 3);

        label_3 = new QLabel(PointLight);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        doubleSpinBox_range = new QDoubleSpinBox(PointLight);
        doubleSpinBox_range->setObjectName(QStringLiteral("doubleSpinBox_range"));

        gridLayout->addWidget(doubleSpinBox_range, 2, 2, 1, 1);


        retranslateUi(PointLight);

        QMetaObject::connectSlotsByName(PointLight);
    } // setupUi

    void retranslateUi(QWidget *PointLight)
    {
        PointLight->setWindowTitle(QApplication::translate("PointLight", "PointLight", 0));
        label->setText(QApplication::translate("PointLight", "Color", 0));
        label_2->setText(QApplication::translate("PointLight", "Attenuation", 0));
        pushButton_color->setText(QString());
        label_3->setText(QApplication::translate("PointLight", "Range", 0));
    } // retranslateUi

};

namespace Ui {
    class PointLight: public Ui_PointLight {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTLIGHTCOMPONENT_H
