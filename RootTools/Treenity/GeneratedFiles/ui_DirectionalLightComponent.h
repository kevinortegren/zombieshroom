/********************************************************************************
** Form generated from reading UI file 'DirectionalLightComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIRECTIONALLIGHTCOMPONENT_H
#define UI_DIRECTIONALLIGHTCOMPONENT_H

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

class Ui_DirectionalLight
{
public:
    QGridLayout *gridLayout;
    QPushButton *pushButton;
    QLabel *label;

    void setupUi(QWidget *DirectionalLight)
    {
        if (DirectionalLight->objectName().isEmpty())
            DirectionalLight->setObjectName(QStringLiteral("DirectionalLight"));
        DirectionalLight->resize(116, 50);
        DirectionalLight->setMaximumSize(QSize(16777215, 50));
        gridLayout = new QGridLayout(DirectionalLight);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        pushButton = new QPushButton(DirectionalLight);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QPalette palette;
        pushButton->setPalette(palette);
        pushButton->setAutoFillBackground(false);
        pushButton->setAutoDefault(false);
        pushButton->setDefault(false);
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 0, 2, 1, 1);

        label = new QLabel(DirectionalLight);
        label->setObjectName(QStringLiteral("label"));
        label->setEnabled(true);
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(DirectionalLight);

        QMetaObject::connectSlotsByName(DirectionalLight);
    } // setupUi

    void retranslateUi(QWidget *DirectionalLight)
    {
        DirectionalLight->setWindowTitle(QApplication::translate("DirectionalLight", "DirectionalLight", 0));
        pushButton->setText(QString());
        label->setText(QApplication::translate("DirectionalLight", "Color", 0));
    } // retranslateUi

};

namespace Ui {
    class DirectionalLight: public Ui_DirectionalLight {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIRECTIONALLIGHTCOMPONENT_H
