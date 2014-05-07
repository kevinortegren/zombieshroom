/********************************************************************************
** Form generated from reading UI file 'CollisionComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLLISIONCOMPONENT_H
#define UI_COLLISIONCOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Collision
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLineEdit *lineEdit;

    void setupUi(QWidget *Collision)
    {
        if (Collision->objectName().isEmpty())
            Collision->setObjectName(QStringLiteral("Collision"));
        Collision->resize(153, 50);
        Collision->setMaximumSize(QSize(16777215, 50));
        gridLayout = new QGridLayout(Collision);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(Collision);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit = new QLineEdit(Collision);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 1);


        retranslateUi(Collision);

        QMetaObject::connectSlotsByName(Collision);
    } // setupUi

    void retranslateUi(QWidget *Collision)
    {
        Collision->setWindowTitle(QApplication::translate("Collision", "Collision", 0));
        label->setText(QApplication::translate("Collision", "Model", 0));
    } // retranslateUi

};

namespace Ui {
    class Collision: public Ui_Collision {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLLISIONCOMPONENT_H
