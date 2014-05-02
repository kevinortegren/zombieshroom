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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Transform
{
public:
    QGridLayout *gridLayout;
    QSpinBox *spinBox_translationX;
    QSpinBox *spinBox_scaleY;
    QLabel *label;
    QSpinBox *spinBox_translationY;
    QSpinBox *spinBox_translationZ;
    QSpinBox *spinBox_scaleX;
    QLabel *label_2;
    QSpinBox *spinBox_scaleZ;
    QLabel *label_3;
    QSpinBox *spinBox_orientationX;
    QSpinBox *spinBox_orientationY;
    QSpinBox *spinBox_orientationZ;

    void setupUi(QWidget *Transform)
    {
        if (Transform->objectName().isEmpty())
            Transform->setObjectName(QStringLiteral("Transform"));
        Transform->resize(227, 146);
        Transform->setMaximumSize(QSize(16777215, 150));
        gridLayout = new QGridLayout(Transform);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        spinBox_translationX = new QSpinBox(Transform);
        spinBox_translationX->setObjectName(QStringLiteral("spinBox_translationX"));

        gridLayout->addWidget(spinBox_translationX, 0, 1, 1, 1);

        spinBox_scaleY = new QSpinBox(Transform);
        spinBox_scaleY->setObjectName(QStringLiteral("spinBox_scaleY"));

        gridLayout->addWidget(spinBox_scaleY, 2, 2, 1, 1);

        label = new QLabel(Transform);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        spinBox_translationY = new QSpinBox(Transform);
        spinBox_translationY->setObjectName(QStringLiteral("spinBox_translationY"));

        gridLayout->addWidget(spinBox_translationY, 0, 2, 1, 1);

        spinBox_translationZ = new QSpinBox(Transform);
        spinBox_translationZ->setObjectName(QStringLiteral("spinBox_translationZ"));

        gridLayout->addWidget(spinBox_translationZ, 0, 3, 1, 1);

        spinBox_scaleX = new QSpinBox(Transform);
        spinBox_scaleX->setObjectName(QStringLiteral("spinBox_scaleX"));

        gridLayout->addWidget(spinBox_scaleX, 2, 1, 1, 1);

        label_2 = new QLabel(Transform);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        spinBox_scaleZ = new QSpinBox(Transform);
        spinBox_scaleZ->setObjectName(QStringLiteral("spinBox_scaleZ"));

        gridLayout->addWidget(spinBox_scaleZ, 2, 3, 1, 1);

        label_3 = new QLabel(Transform);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 1, 0, 1, 1);

        spinBox_orientationX = new QSpinBox(Transform);
        spinBox_orientationX->setObjectName(QStringLiteral("spinBox_orientationX"));

        gridLayout->addWidget(spinBox_orientationX, 1, 1, 1, 1);

        spinBox_orientationY = new QSpinBox(Transform);
        spinBox_orientationY->setObjectName(QStringLiteral("spinBox_orientationY"));

        gridLayout->addWidget(spinBox_orientationY, 1, 2, 1, 1);

        spinBox_orientationZ = new QSpinBox(Transform);
        spinBox_orientationZ->setObjectName(QStringLiteral("spinBox_orientationZ"));

        gridLayout->addWidget(spinBox_orientationZ, 1, 3, 1, 1);


        retranslateUi(Transform);

        QMetaObject::connectSlotsByName(Transform);
    } // setupUi

    void retranslateUi(QWidget *Transform)
    {
        Transform->setWindowTitle(QApplication::translate("Transform", "Transform", 0));
        label->setText(QApplication::translate("Transform", "Position", 0));
        label_2->setText(QApplication::translate("Transform", "Scale", 0));
        label_3->setText(QApplication::translate("Transform", "Orientation", 0));
    } // retranslateUi

};

namespace Ui {
    class Transform: public Ui_Transform {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TRANSFORMCOMPONENT_H
