/********************************************************************************
** Form generated from reading UI file 'TransformComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TRANSFORMCOMPONENT_H
#define UI_TRANSFORMCOMPONENT_H

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

class Ui_Transform
{
public:
    QGridLayout *gridLayout;
    QDoubleSpinBox *doubleSpinBox_orientationY;
    QDoubleSpinBox *doubleSpinBox_scaleY;
    QDoubleSpinBox *doubleSpinBox_orientationX;
    QDoubleSpinBox *doubleSpinBox_scaleX;
    QDoubleSpinBox *doubleSpinBox_scaleZ;
    QDoubleSpinBox *doubleSpinBox_orientationZ;
    QDoubleSpinBox *doubleSpinBox_translationZ;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox_translationX;
    QDoubleSpinBox *doubleSpinBox_translationY;
    QLabel *label;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;

    void setupUi(QWidget *Transform)
    {
        if (Transform->objectName().isEmpty())
            Transform->setObjectName(QStringLiteral("Transform"));
        Transform->resize(351, 146);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Transform->sizePolicy().hasHeightForWidth());
        Transform->setSizePolicy(sizePolicy);
        Transform->setMaximumSize(QSize(16777215, 150));
        gridLayout = new QGridLayout(Transform);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        doubleSpinBox_orientationY = new QDoubleSpinBox(Transform);
        doubleSpinBox_orientationY->setObjectName(QStringLiteral("doubleSpinBox_orientationY"));
        doubleSpinBox_orientationY->setMinimum(-360);
        doubleSpinBox_orientationY->setMaximum(360);

        gridLayout->addWidget(doubleSpinBox_orientationY, 2, 2, 1, 1);

        doubleSpinBox_scaleY = new QDoubleSpinBox(Transform);
        doubleSpinBox_scaleY->setObjectName(QStringLiteral("doubleSpinBox_scaleY"));
        doubleSpinBox_scaleY->setMinimum(0.01);
        doubleSpinBox_scaleY->setMaximum(100);
        doubleSpinBox_scaleY->setValue(1);

        gridLayout->addWidget(doubleSpinBox_scaleY, 3, 2, 1, 1);

        doubleSpinBox_orientationX = new QDoubleSpinBox(Transform);
        doubleSpinBox_orientationX->setObjectName(QStringLiteral("doubleSpinBox_orientationX"));
        doubleSpinBox_orientationX->setMinimum(-360);
        doubleSpinBox_orientationX->setMaximum(360);

        gridLayout->addWidget(doubleSpinBox_orientationX, 2, 1, 1, 1);

        doubleSpinBox_scaleX = new QDoubleSpinBox(Transform);
        doubleSpinBox_scaleX->setObjectName(QStringLiteral("doubleSpinBox_scaleX"));
        doubleSpinBox_scaleX->setMinimum(0.01);
        doubleSpinBox_scaleX->setMaximum(100);
        doubleSpinBox_scaleX->setValue(1);

        gridLayout->addWidget(doubleSpinBox_scaleX, 3, 1, 1, 1);

        doubleSpinBox_scaleZ = new QDoubleSpinBox(Transform);
        doubleSpinBox_scaleZ->setObjectName(QStringLiteral("doubleSpinBox_scaleZ"));
        doubleSpinBox_scaleZ->setMinimum(0.01);
        doubleSpinBox_scaleZ->setMaximum(100);
        doubleSpinBox_scaleZ->setValue(1);

        gridLayout->addWidget(doubleSpinBox_scaleZ, 3, 3, 1, 1);

        doubleSpinBox_orientationZ = new QDoubleSpinBox(Transform);
        doubleSpinBox_orientationZ->setObjectName(QStringLiteral("doubleSpinBox_orientationZ"));
        doubleSpinBox_orientationZ->setMinimum(-360);
        doubleSpinBox_orientationZ->setMaximum(360);

        gridLayout->addWidget(doubleSpinBox_orientationZ, 2, 3, 1, 1);

        doubleSpinBox_translationZ = new QDoubleSpinBox(Transform);
        doubleSpinBox_translationZ->setObjectName(QStringLiteral("doubleSpinBox_translationZ"));
        doubleSpinBox_translationZ->setMinimum(-1000);
        doubleSpinBox_translationZ->setMaximum(1000);

        gridLayout->addWidget(doubleSpinBox_translationZ, 1, 3, 1, 1);

        label_2 = new QLabel(Transform);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 3, 0, 1, 1);

        label_3 = new QLabel(Transform);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        doubleSpinBox_translationX = new QDoubleSpinBox(Transform);
        doubleSpinBox_translationX->setObjectName(QStringLiteral("doubleSpinBox_translationX"));
        doubleSpinBox_translationX->setMinimum(-1000);
        doubleSpinBox_translationX->setMaximum(1000);

        gridLayout->addWidget(doubleSpinBox_translationX, 1, 1, 1, 1);

        doubleSpinBox_translationY = new QDoubleSpinBox(Transform);
        doubleSpinBox_translationY->setObjectName(QStringLiteral("doubleSpinBox_translationY"));
        doubleSpinBox_translationY->setMinimum(-1000);
        doubleSpinBox_translationY->setMaximum(1000);

        gridLayout->addWidget(doubleSpinBox_translationY, 1, 2, 1, 1);

        label = new QLabel(Transform);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 1, 0, 1, 1);

        label_4 = new QLabel(Transform);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(16777215, 15));
        label_4->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_4, 0, 1, 1, 1);

        label_5 = new QLabel(Transform);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_5, 0, 2, 1, 1);

        label_6 = new QLabel(Transform);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(label_6, 0, 3, 1, 1);


        retranslateUi(Transform);

        QMetaObject::connectSlotsByName(Transform);
    } // setupUi

    void retranslateUi(QWidget *Transform)
    {
        Transform->setWindowTitle(QApplication::translate("Transform", "Transform", 0));
        label_2->setText(QApplication::translate("Transform", "Scale", 0));
        label_3->setText(QApplication::translate("Transform", "Orientation", 0));
        label->setText(QApplication::translate("Transform", "Position", 0));
        label_4->setText(QApplication::translate("Transform", "X", 0));
        label_5->setText(QApplication::translate("Transform", "Y", 0));
        label_6->setText(QApplication::translate("Transform", "Z", 0));
    } // retranslateUi

};

namespace Ui {
    class Transform: public Ui_Transform {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSFORMCOMPONENT_H
