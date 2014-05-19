/********************************************************************************
** Form generated from reading UI file 'ScriptComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCRIPTCOMPONENT_H
#define UI_SCRIPTCOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/DropLineEdit.h>

QT_BEGIN_NAMESPACE

class Ui_Script
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    DropLineEdit *lineEdit_name;
    QToolButton *toolButton;

    void setupUi(QWidget *Script)
    {
        if (Script->objectName().isEmpty())
            Script->setObjectName(QStringLiteral("Script"));
        Script->resize(250, 83);
        Script->setMaximumSize(QSize(16777215, 150));
        gridLayout = new QGridLayout(Script);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label = new QLabel(Script);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(30, 16777215));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lineEdit_name = new DropLineEdit(Script);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));
        lineEdit_name->setReadOnly(true);

        gridLayout->addWidget(lineEdit_name, 0, 1, 1, 1);

        toolButton = new QToolButton(Script);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        gridLayout->addWidget(toolButton, 0, 2, 1, 1);


        retranslateUi(Script);

        QMetaObject::connectSlotsByName(Script);
    } // setupUi

    void retranslateUi(QWidget *Script)
    {
        Script->setWindowTitle(QApplication::translate("Script", "Form", 0));
        label->setText(QApplication::translate("Script", "Script:", 0));
        toolButton->setText(QApplication::translate("Script", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class Script: public Ui_Script {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCRIPTCOMPONENT_H
