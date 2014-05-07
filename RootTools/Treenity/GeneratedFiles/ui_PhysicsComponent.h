/********************************************************************************
** Form generated from reading UI file 'PhysicsComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PHYSICSCOMPONENT_H
#define UI_PHYSICSCOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Physics
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBox_velocityY;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_velocityZ;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_velocityX;
    QDoubleSpinBox *doubleSpinBox_mass;
    QLabel *label;
    QLabel *label_5;
    QLabel *label_4;

    void setupUi(QWidget *Physics)
    {
        if (Physics->objectName().isEmpty())
            Physics->setObjectName(QStringLiteral("Physics"));
        Physics->resize(260, 122);
        gridLayout = new QGridLayout(Physics);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        doubleSpinBox_velocityY = new QDoubleSpinBox(Physics);
        doubleSpinBox_velocityY->setObjectName(QStringLiteral("doubleSpinBox_velocityY"));

        gridLayout->addWidget(doubleSpinBox_velocityY, 2, 2, 1, 1);

        label_3 = new QLabel(Physics);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(16777215, 15));

        gridLayout->addWidget(label_3, 1, 1, 1, 1);

        doubleSpinBox_velocityZ = new QDoubleSpinBox(Physics);
        doubleSpinBox_velocityZ->setObjectName(QStringLiteral("doubleSpinBox_velocityZ"));

        gridLayout->addWidget(doubleSpinBox_velocityZ, 2, 3, 1, 1);

        label_2 = new QLabel(Physics);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        doubleSpinBox_velocityX = new QDoubleSpinBox(Physics);
        doubleSpinBox_velocityX->setObjectName(QStringLiteral("doubleSpinBox_velocityX"));

        gridLayout->addWidget(doubleSpinBox_velocityX, 2, 1, 1, 1);

        doubleSpinBox_mass = new QDoubleSpinBox(Physics);
        doubleSpinBox_mass->setObjectName(QStringLiteral("doubleSpinBox_mass"));

        gridLayout->addWidget(doubleSpinBox_mass, 0, 1, 1, 3);

        label = new QLabel(Physics);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_5 = new QLabel(Physics);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 1, 3, 1, 1);

        label_4 = new QLabel(Physics);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 1, 2, 1, 1);


        retranslateUi(Physics);

        QMetaObject::connectSlotsByName(Physics);
    } // setupUi

    void retranslateUi(QWidget *Physics)
    {
        Physics->setWindowTitle(QApplication::translate("Physics", "Physics", 0));
        label_3->setText(QApplication::translate("Physics", "X", 0));
        label_2->setText(QApplication::translate("Physics", "Velocity", 0));
        label->setText(QApplication::translate("Physics", "Mass", 0));
        label_5->setText(QApplication::translate("Physics", "Z", 0));
        label_4->setText(QApplication::translate("Physics", "Y", 0));
    } // retranslateUi

};

namespace Ui {
    class Physics: public Ui_Physics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHYSICSCOMPONENT_H
