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
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QWidget *PointLight)
    {
        if (PointLight->objectName().isEmpty())
            PointLight->setObjectName(QStringLiteral("PointLight"));
        PointLight->resize(116, 50);
        PointLight->setMaximumSize(QSize(16777215, 50));
        gridLayout = new QGridLayout(PointLight);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton = new QPushButton(PointLight);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QPalette palette;
        pushButton->setPalette(palette);
        pushButton->setAutoFillBackground(false);
        pushButton->setAutoDefault(false);
        pushButton->setDefault(false);
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 0, 1, 1, 1);

        label = new QLabel(PointLight);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(PointLight);

        QMetaObject::connectSlotsByName(PointLight);
    } // setupUi

    void retranslateUi(QWidget *PointLight)
    {
        PointLight->setWindowTitle(QApplication::translate("PointLight", "PointLight", 0));
        pushButton->setText(QString());
        label->setText(QApplication::translate("PointLight", "Color", 0));
    } // retranslateUi

};

namespace Ui {
    class PointLight: public Ui_PointLight {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_POINTLIGHTCOMPONENT_H
