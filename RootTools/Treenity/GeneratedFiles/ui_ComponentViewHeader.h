/********************************************************************************
** Form generated from reading UI file 'ComponentViewHeader.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COMPONENTVIEWHEADER_H
#define UI_COMPONENTVIEWHEADER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_componentHeader
{
public:
    QHBoxLayout *horizontalLayout;
    QToolButton *toolButton_collapse;
    QLabel *label_componentName;
    QToolButton *toolButton_remove;

    void setupUi(QWidget *Form_componentHeader)
    {
        if (Form_componentHeader->objectName().isEmpty())
            Form_componentHeader->setObjectName(QStringLiteral("Form_componentHeader"));
        Form_componentHeader->resize(400, 22);
        Form_componentHeader->setStyleSheet(QLatin1String("#Form_componentHeader {\n"
"	border-radius: 2px;\n"
"	background-color: #888;\n"
"}\n"
"\n"
"QToolButton:hover {\n"
"	 background: transparent; \n"
"}\n"
"\n"
"QToolButton:pressed {\n"
"	 background: transparent; \n"
"}\n"
"\n"
"QToolButton:selected {\n"
"	 background: transparent; \n"
"}\n"
"\n"
"\n"
""));
        horizontalLayout = new QHBoxLayout(Form_componentHeader);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(2, 0, 2, 0);
        toolButton_collapse = new QToolButton(Form_componentHeader);
        toolButton_collapse->setObjectName(QStringLiteral("toolButton_collapse"));
        toolButton_collapse->setAutoRaise(true);
        toolButton_collapse->setArrowType(Qt::RightArrow);

        horizontalLayout->addWidget(toolButton_collapse);

        label_componentName = new QLabel(Form_componentHeader);
        label_componentName->setObjectName(QStringLiteral("label_componentName"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label_componentName->setFont(font);
        label_componentName->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_componentName);

        toolButton_remove = new QToolButton(Form_componentHeader);
        toolButton_remove->setObjectName(QStringLiteral("toolButton_remove"));
        toolButton_remove->setStyleSheet(QLatin1String("QToolButton::menu-indicator { image: none; }\n"
"\n"
"QToolButton:pressed {\n"
"	background-color: #666;\n"
"}\n"
"\n"
"QToolButton:hover {\n"
"	background-color: #666;\n"
"}"));
        toolButton_remove->setPopupMode(QToolButton::InstantPopup);
        toolButton_remove->setToolButtonStyle(Qt::ToolButtonIconOnly);
        toolButton_remove->setAutoRaise(true);

        horizontalLayout->addWidget(toolButton_remove);


        retranslateUi(Form_componentHeader);

        QMetaObject::connectSlotsByName(Form_componentHeader);
    } // setupUi

    void retranslateUi(QWidget *Form_componentHeader)
    {
        Form_componentHeader->setWindowTitle(QApplication::translate("Form_componentHeader", "Form", 0));
        toolButton_collapse->setText(QApplication::translate("Form_componentHeader", "...", 0));
        label_componentName->setText(QApplication::translate("Form_componentHeader", "TextLabel", 0));
        toolButton_remove->setText(QApplication::translate("Form_componentHeader", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class Form_componentHeader: public Ui_Form_componentHeader {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COMPONENTVIEWHEADER_H
