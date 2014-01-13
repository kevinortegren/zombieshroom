/********************************************************************************
** Form generated from reading UI file 'AbilityEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABILITYEDITOR_H
#define UI_ABILITYEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDial>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AbilityEditorClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton;
    QLCDNumber *lcdNumber;
    QDial *dial;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AbilityEditorClass)
    {
        if (AbilityEditorClass->objectName().isEmpty())
            AbilityEditorClass->setObjectName(QStringLiteral("AbilityEditorClass"));
        AbilityEditorClass->resize(960, 699);
        centralWidget = new QWidget(AbilityEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(580, 270, 75, 23));
        lcdNumber = new QLCDNumber(centralWidget);
        lcdNumber->setObjectName(QStringLiteral("lcdNumber"));
        lcdNumber->setGeometry(QRect(470, 210, 64, 23));
        dial = new QDial(centralWidget);
        dial->setObjectName(QStringLiteral("dial"));
        dial->setGeometry(QRect(320, 60, 151, 131));
        tableWidget = new QTableWidget(centralWidget);
        if (tableWidget->columnCount() < 2)
            tableWidget->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        if (tableWidget->rowCount() < 1)
            tableWidget->setRowCount(1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem2);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(280, 400, 256, 192));
        AbilityEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AbilityEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 960, 21));
        AbilityEditorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(AbilityEditorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        AbilityEditorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(AbilityEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AbilityEditorClass->setStatusBar(statusBar);

        retranslateUi(AbilityEditorClass);

        QMetaObject::connectSlotsByName(AbilityEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AbilityEditorClass)
    {
        AbilityEditorClass->setWindowTitle(QApplication::translate("AbilityEditorClass", "AbilityEditor", 0));
#ifndef QT_NO_ACCESSIBILITY
        AbilityEditorClass->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
#ifndef QT_NO_ACCESSIBILITY
        AbilityEditorClass->setAccessibleDescription(QString());
#endif // QT_NO_ACCESSIBILITY
        pushButton->setText(QApplication::translate("AbilityEditorClass", "PushButton", 0));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("AbilityEditorClass", "Props", 0));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("AbilityEditorClass", "Valuz", 0));
    } // retranslateUi

};

namespace Ui {
    class AbilityEditorClass: public Ui_AbilityEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABILITYEDITOR_H
