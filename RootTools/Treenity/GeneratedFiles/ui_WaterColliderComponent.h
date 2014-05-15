/********************************************************************************
** Form generated from reading UI file 'WaterColliderComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WATERCOLLIDERCOMPONENT_H
#define UI_WATERCOLLIDERCOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaterCollider
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_distPower;
    QSpinBox *spinBox_radius;
    QDoubleSpinBox *doubleSpinBox_distInterval;
    QLabel *label_3;

    void setupUi(QWidget *WaterCollider)
    {
        if (WaterCollider->objectName().isEmpty())
            WaterCollider->setObjectName(QStringLiteral("WaterCollider"));
        WaterCollider->resize(357, 130);
        WaterCollider->setMaximumSize(QSize(16777215, 130));
        gridLayout = new QGridLayout(WaterCollider);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(WaterCollider);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(85, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(WaterCollider);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(85, 16777215));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        doubleSpinBox_distPower = new QDoubleSpinBox(WaterCollider);
        doubleSpinBox_distPower->setObjectName(QStringLiteral("doubleSpinBox_distPower"));
        doubleSpinBox_distPower->setMinimum(-500);
        doubleSpinBox_distPower->setMaximum(500);
        doubleSpinBox_distPower->setSingleStep(0.5);

        gridLayout->addWidget(doubleSpinBox_distPower, 1, 1, 1, 1);

        spinBox_radius = new QSpinBox(WaterCollider);
        spinBox_radius->setObjectName(QStringLiteral("spinBox_radius"));
        spinBox_radius->setMinimum(1);
        spinBox_radius->setMaximum(500);

        gridLayout->addWidget(spinBox_radius, 0, 1, 1, 1);

        doubleSpinBox_distInterval = new QDoubleSpinBox(WaterCollider);
        doubleSpinBox_distInterval->setObjectName(QStringLiteral("doubleSpinBox_distInterval"));
        doubleSpinBox_distInterval->setMinimum(0.1);
        doubleSpinBox_distInterval->setSingleStep(0.1);

        gridLayout->addWidget(doubleSpinBox_distInterval, 2, 1, 1, 1);

        label_3 = new QLabel(WaterCollider);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(85, 16777215));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);


        retranslateUi(WaterCollider);

        QMetaObject::connectSlotsByName(WaterCollider);
    } // setupUi

    void retranslateUi(QWidget *WaterCollider)
    {
        WaterCollider->setWindowTitle(QApplication::translate("WaterCollider", "Form", 0));
        label->setText(QApplication::translate("WaterCollider", "Radius", 0));
        label_2->setText(QApplication::translate("WaterCollider", "Disturb power", 0));
        label_3->setText(QApplication::translate("WaterCollider", "Disturb interval", 0));
    } // retranslateUi

};

namespace Ui {
    class WaterCollider: public Ui_WaterCollider {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WATERCOLLIDERCOMPONENT_H
