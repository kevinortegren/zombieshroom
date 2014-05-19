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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Physics
{
public:
    QGridLayout *gridLayout;
    QComboBox *comboBox_shape;
    QLabel *label_7;
    QStackedWidget *stackedWidget_2;
    QWidget *page_5;
    QGridLayout *gridLayout_5;
    QLabel *label_3;
    QCheckBox *checkBox_collideWithStatic;
    QCheckBox *checkBox_collideWithWorld;
    QLabel *label_4;
    QDoubleSpinBox *doubleSpinBox_gravityX;
    QDoubleSpinBox *doubleSpinBox_gravityY;
    QDoubleSpinBox *doubleSpinBox_gravityZ;
    QLabel *label_5;
    QDoubleSpinBox *doubleSpinBox_mass;
    QLabel *label_2;
    QWidget *page_6;
    QLabel *label;
    QComboBox *comboBox_type;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_4;
    QDoubleSpinBox *doubleSpinBox_sphereRadius;
    QLabel *label_6;
    QWidget *page_3;
    QGridLayout *gridLayout_2;
    QLabel *label_9;
    QLabel *label_8;
    QDoubleSpinBox *doubleSpinBox_coneHeight;
    QDoubleSpinBox *doubleSpinBox_coneRadius;
    QWidget *page_4;
    QGridLayout *gridLayout_3;
    QLabel *label_10;
    QDoubleSpinBox *doubleSpinBox_cylinderRadius;
    QLabel *label_11;
    QDoubleSpinBox *doubleSpinBox_cylinderHeight;
    QWidget *page_2;
    QGridLayout *gridLayout_6;
    QLabel *label_12;
    QLineEdit *lineEdit_physicsMesh;
    QToolButton *toolButton_physicsMesh;

    void setupUi(QWidget *Physics)
    {
        if (Physics->objectName().isEmpty())
            Physics->setObjectName(QStringLiteral("Physics"));
        Physics->resize(280, 300);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Physics->sizePolicy().hasHeightForWidth());
        Physics->setSizePolicy(sizePolicy);
        Physics->setMaximumSize(QSize(16777215, 300));
        gridLayout = new QGridLayout(Physics);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetNoConstraint);
        comboBox_shape = new QComboBox(Physics);
        comboBox_shape->setObjectName(QStringLiteral("comboBox_shape"));

        gridLayout->addWidget(comboBox_shape, 2, 1, 1, 3);

        label_7 = new QLabel(Physics);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 2, 0, 1, 1);

        stackedWidget_2 = new QStackedWidget(Physics);
        stackedWidget_2->setObjectName(QStringLiteral("stackedWidget_2"));
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        gridLayout_5 = new QGridLayout(page_5);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        label_3 = new QLabel(page_5);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMinimumSize(QSize(0, 20));

        gridLayout_5->addWidget(label_3, 1, 0, 1, 1);

        checkBox_collideWithStatic = new QCheckBox(page_5);
        checkBox_collideWithStatic->setObjectName(QStringLiteral("checkBox_collideWithStatic"));

        gridLayout_5->addWidget(checkBox_collideWithStatic, 1, 1, 1, 1);

        checkBox_collideWithWorld = new QCheckBox(page_5);
        checkBox_collideWithWorld->setObjectName(QStringLiteral("checkBox_collideWithWorld"));

        gridLayout_5->addWidget(checkBox_collideWithWorld, 0, 1, 1, 1);

        label_4 = new QLabel(page_5);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout_5->addWidget(label_4, 2, 0, 1, 1);

        doubleSpinBox_gravityX = new QDoubleSpinBox(page_5);
        doubleSpinBox_gravityX->setObjectName(QStringLiteral("doubleSpinBox_gravityX"));

        gridLayout_5->addWidget(doubleSpinBox_gravityX, 2, 1, 1, 1);

        doubleSpinBox_gravityY = new QDoubleSpinBox(page_5);
        doubleSpinBox_gravityY->setObjectName(QStringLiteral("doubleSpinBox_gravityY"));

        gridLayout_5->addWidget(doubleSpinBox_gravityY, 2, 2, 1, 1);

        doubleSpinBox_gravityZ = new QDoubleSpinBox(page_5);
        doubleSpinBox_gravityZ->setObjectName(QStringLiteral("doubleSpinBox_gravityZ"));

        gridLayout_5->addWidget(doubleSpinBox_gravityZ, 2, 3, 1, 1);

        label_5 = new QLabel(page_5);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout_5->addWidget(label_5, 3, 0, 1, 1);

        doubleSpinBox_mass = new QDoubleSpinBox(page_5);
        doubleSpinBox_mass->setObjectName(QStringLiteral("doubleSpinBox_mass"));

        gridLayout_5->addWidget(doubleSpinBox_mass, 3, 1, 1, 1);

        label_2 = new QLabel(page_5);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMinimumSize(QSize(0, 20));
        label_2->setMaximumSize(QSize(100, 16777215));

        gridLayout_5->addWidget(label_2, 0, 0, 1, 1);

        stackedWidget_2->addWidget(page_5);
        page_6 = new QWidget();
        page_6->setObjectName(QStringLiteral("page_6"));
        stackedWidget_2->addWidget(page_6);

        gridLayout->addWidget(stackedWidget_2, 1, 0, 1, 4);

        label = new QLabel(Physics);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(30, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        comboBox_type = new QComboBox(Physics);
        comboBox_type->setObjectName(QStringLiteral("comboBox_type"));

        gridLayout->addWidget(comboBox_type, 0, 1, 1, 3);

        stackedWidget = new QStackedWidget(Physics);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout_4 = new QGridLayout(page);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        doubleSpinBox_sphereRadius = new QDoubleSpinBox(page);
        doubleSpinBox_sphereRadius->setObjectName(QStringLiteral("doubleSpinBox_sphereRadius"));

        gridLayout_4->addWidget(doubleSpinBox_sphereRadius, 0, 1, 1, 1);

        label_6 = new QLabel(page);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setMaximumSize(QSize(60, 16777215));

        gridLayout_4->addWidget(label_6, 0, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        gridLayout_2 = new QGridLayout(page_3);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_9 = new QLabel(page_3);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout_2->addWidget(label_9, 1, 0, 1, 1);

        label_8 = new QLabel(page_3);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setMaximumSize(QSize(40, 16777215));

        gridLayout_2->addWidget(label_8, 0, 0, 1, 1);

        doubleSpinBox_coneHeight = new QDoubleSpinBox(page_3);
        doubleSpinBox_coneHeight->setObjectName(QStringLiteral("doubleSpinBox_coneHeight"));

        gridLayout_2->addWidget(doubleSpinBox_coneHeight, 1, 1, 1, 1);

        doubleSpinBox_coneRadius = new QDoubleSpinBox(page_3);
        doubleSpinBox_coneRadius->setObjectName(QStringLiteral("doubleSpinBox_coneRadius"));

        gridLayout_2->addWidget(doubleSpinBox_coneRadius, 0, 1, 1, 1);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        gridLayout_3 = new QGridLayout(page_4);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_10 = new QLabel(page_4);
        label_10->setObjectName(QStringLiteral("label_10"));
        label_10->setMaximumSize(QSize(40, 16777215));

        gridLayout_3->addWidget(label_10, 0, 0, 1, 1);

        doubleSpinBox_cylinderRadius = new QDoubleSpinBox(page_4);
        doubleSpinBox_cylinderRadius->setObjectName(QStringLiteral("doubleSpinBox_cylinderRadius"));

        gridLayout_3->addWidget(doubleSpinBox_cylinderRadius, 0, 1, 1, 1);

        label_11 = new QLabel(page_4);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout_3->addWidget(label_11, 1, 0, 1, 1);

        doubleSpinBox_cylinderHeight = new QDoubleSpinBox(page_4);
        doubleSpinBox_cylinderHeight->setObjectName(QStringLiteral("doubleSpinBox_cylinderHeight"));

        gridLayout_3->addWidget(doubleSpinBox_cylinderHeight, 1, 1, 1, 1);

        stackedWidget->addWidget(page_4);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        gridLayout_6 = new QGridLayout(page_2);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        label_12 = new QLabel(page_2);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout_6->addWidget(label_12, 0, 0, 1, 1);

        lineEdit_physicsMesh = new QLineEdit(page_2);
        lineEdit_physicsMesh->setObjectName(QStringLiteral("lineEdit_physicsMesh"));

        gridLayout_6->addWidget(lineEdit_physicsMesh, 0, 1, 1, 1);

        toolButton_physicsMesh = new QToolButton(page_2);
        toolButton_physicsMesh->setObjectName(QStringLiteral("toolButton_physicsMesh"));

        gridLayout_6->addWidget(toolButton_physicsMesh, 0, 2, 1, 1);

        stackedWidget->addWidget(page_2);

        gridLayout->addWidget(stackedWidget, 3, 0, 1, 4);


        retranslateUi(Physics);
        QObject::connect(comboBox_type, SIGNAL(currentIndexChanged(int)), stackedWidget_2, SLOT(setCurrentIndex(int)));
        QObject::connect(comboBox_shape, SIGNAL(currentIndexChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Physics);
    } // setupUi

    void retranslateUi(QWidget *Physics)
    {
        Physics->setWindowTitle(QApplication::translate("Physics", "Physics", 0));
        comboBox_shape->clear();
        comboBox_shape->insertItems(0, QStringList()
         << QApplication::translate("Physics", "Sphere", 0)
         << QApplication::translate("Physics", "Cone", 0)
         << QApplication::translate("Physics", "Cylinder", 0)
         << QApplication::translate("Physics", "Mesh", 0)
        );
        label_7->setText(QApplication::translate("Physics", "Shape", 0));
        label_3->setText(QApplication::translate("Physics", "Static collision", 0));
        checkBox_collideWithStatic->setText(QString());
        checkBox_collideWithWorld->setText(QString());
        label_4->setText(QApplication::translate("Physics", "Gravity", 0));
        label_5->setText(QApplication::translate("Physics", "Mass", 0));
        label_2->setText(QApplication::translate("Physics", "World collision", 0));
        label->setText(QApplication::translate("Physics", "Type", 0));
        comboBox_type->clear();
        comboBox_type->insertItems(0, QStringList()
         << QApplication::translate("Physics", "Dynamic", 0)
         << QApplication::translate("Physics", "Static", 0)
        );
        label_6->setText(QApplication::translate("Physics", "Radius", 0));
        label_9->setText(QApplication::translate("Physics", "Height", 0));
        label_8->setText(QApplication::translate("Physics", "Radius", 0));
        label_10->setText(QApplication::translate("Physics", "Radius", 0));
        label_11->setText(QApplication::translate("Physics", "Height", 0));
        label_12->setText(QApplication::translate("Physics", "Physics Mesh", 0));
        toolButton_physicsMesh->setText(QApplication::translate("Physics", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class Physics: public Ui_Physics {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PHYSICSCOMPONENT_H
