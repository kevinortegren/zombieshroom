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
    QDoubleSpinBox *doubleSpinBox_velocityZ;
    QLabel *label_2;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_velocityX;
    QLabel *label_3;
    QLabel *label_4;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_mass;

    void setupUi(QWidget *Physics)
    {
        if (Physics->objectName().isEmpty())
            Physics->setObjectName(QStringLiteral("Physics"));
        Physics->resize(132, 122);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Physics->sizePolicy().hasHeightForWidth());
        Physics->setSizePolicy(sizePolicy);
        Physics->setMaximumSize(QSize(16777215, 150));
        gridLayout = new QGridLayout(Physics);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        doubleSpinBox_velocityY = new QDoubleSpinBox(Physics);
        doubleSpinBox_velocityY->setObjectName(QStringLiteral("doubleSpinBox_velocityY"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(doubleSpinBox_velocityY->sizePolicy().hasHeightForWidth());
        doubleSpinBox_velocityY->setSizePolicy(sizePolicy1);
        doubleSpinBox_velocityY->setMinimumSize(QSize(20, 0));

        gridLayout->addWidget(doubleSpinBox_velocityY, 2, 2, 1, 1);

        doubleSpinBox_velocityZ = new QDoubleSpinBox(Physics);
        doubleSpinBox_velocityZ->setObjectName(QStringLiteral("doubleSpinBox_velocityZ"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_velocityZ->sizePolicy().hasHeightForWidth());
        doubleSpinBox_velocityZ->setSizePolicy(sizePolicy1);
        doubleSpinBox_velocityZ->setMinimumSize(QSize(20, 0));

        gridLayout->addWidget(doubleSpinBox_velocityZ, 2, 3, 1, 1);

        label_2 = new QLabel(Physics);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(Physics);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        doubleSpinBox_velocityX = new QDoubleSpinBox(Physics);
        doubleSpinBox_velocityX->setObjectName(QStringLiteral("doubleSpinBox_velocityX"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_velocityX->sizePolicy().hasHeightForWidth());
        doubleSpinBox_velocityX->setSizePolicy(sizePolicy1);
        doubleSpinBox_velocityX->setMinimumSize(QSize(20, 0));

        gridLayout->addWidget(doubleSpinBox_velocityX, 2, 1, 1, 1);

        label_3 = new QLabel(Physics);
        label_3->setObjectName(QStringLiteral("label_3"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy2);
        label_3->setMaximumSize(QSize(16777215, 15));

        gridLayout->addWidget(label_3, 1, 1, 1, 1);

        label_4 = new QLabel(Physics);
        label_4->setObjectName(QStringLiteral("label_4"));
        sizePolicy2.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_4, 1, 2, 1, 1);

        label_5 = new QLabel(Physics);
        label_5->setObjectName(QStringLiteral("label_5"));
        sizePolicy2.setHeightForWidth(label_5->sizePolicy().hasHeightForWidth());
        label_5->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(label_5, 1, 3, 1, 1);

        doubleSpinBox_mass = new QDoubleSpinBox(Physics);
        doubleSpinBox_mass->setObjectName(QStringLiteral("doubleSpinBox_mass"));
        sizePolicy1.setHeightForWidth(doubleSpinBox_mass->sizePolicy().hasHeightForWidth());
        doubleSpinBox_mass->setSizePolicy(sizePolicy1);
        doubleSpinBox_mass->setMinimumSize(QSize(20, 0));

        gridLayout->addWidget(doubleSpinBox_mass, 0, 1, 1, 3);


        retranslateUi(Physics);

        QMetaObject::connectSlotsByName(Physics);
    } // setupUi

    void retranslateUi(QWidget *Physics)
    {
        Physics->setWindowTitle(QApplication::translate("Physics", "Physics", 0));
        label_2->setText(QApplication::translate("Physics", "Velocity", 0));
        label->setText(QApplication::translate("Physics", "Mass", 0));
        label_3->setText(QApplication::translate("Physics", "X", 0));
        label_4->setText(QApplication::translate("Physics", "Y", 0));
        label_5->setText(QApplication::translate("Physics", "Z", 0));
    } // retranslateUi

};

namespace Ui {
    class Physics: public Ui_Physics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHYSICSCOMPONENT_H
