/********************************************************************************
** Form generated from reading UI file 'MaterialEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MATERIALEDITOR_H
#define UI_MATERIALEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_materialEffect;
    QLineEdit *lineEdit_materialName;
    QLabel *label;
    QLabel *label_2;
    QPushButton *pushButton_reset;
    QPushButton *pushButton_createMaterial;
    QListWidget *listWidget_materialList;
    QWidget *widget_textureForm;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(914, 424);
        gridLayout = new QGridLayout(Form);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lineEdit_materialEffect = new QLineEdit(Form);
        lineEdit_materialEffect->setObjectName(QStringLiteral("lineEdit_materialEffect"));

        gridLayout->addWidget(lineEdit_materialEffect, 1, 2, 1, 1);

        lineEdit_materialName = new QLineEdit(Form);
        lineEdit_materialName->setObjectName(QStringLiteral("lineEdit_materialName"));

        gridLayout->addWidget(lineEdit_materialName, 0, 2, 1, 1);

        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(50, 16777215));

        gridLayout->addWidget(label_2, 1, 1, 1, 1);

        pushButton_reset = new QPushButton(Form);
        pushButton_reset->setObjectName(QStringLiteral("pushButton_reset"));

        gridLayout->addWidget(pushButton_reset, 3, 1, 1, 1);

        pushButton_createMaterial = new QPushButton(Form);
        pushButton_createMaterial->setObjectName(QStringLiteral("pushButton_createMaterial"));

        gridLayout->addWidget(pushButton_createMaterial, 3, 2, 1, 1);

        listWidget_materialList = new QListWidget(Form);
        listWidget_materialList->setObjectName(QStringLiteral("listWidget_materialList"));

        gridLayout->addWidget(listWidget_materialList, 0, 0, 4, 1);

        widget_textureForm = new QWidget(Form);
        widget_textureForm->setObjectName(QStringLiteral("widget_textureForm"));
        verticalLayout = new QVBoxLayout(widget_textureForm);
        verticalLayout->setSpacing(0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        scrollArea = new QScrollArea(widget_textureForm);
        scrollArea->setObjectName(QStringLiteral("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 483, 323));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);


        gridLayout->addWidget(widget_textureForm, 2, 1, 1, 2);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        label->setText(QApplication::translate("Form", "Name", 0));
        label_2->setText(QApplication::translate("Form", "Effect", 0));
        pushButton_reset->setText(QApplication::translate("Form", "Reset", 0));
        pushButton_createMaterial->setText(QApplication::translate("Form", "Create", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MATERIALEDITOR_H
