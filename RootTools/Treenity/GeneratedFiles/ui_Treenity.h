/********************************************************************************
** Form generated from reading UI file 'Treenity.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TREENITY_H
#define UI_TREENITY_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TreenityClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TreenityClass)
    {
        if (TreenityClass->objectName().isEmpty())
            TreenityClass->setObjectName(QStringLiteral("TreenityClass"));
        TreenityClass->resize(600, 400);
        menuBar = new QMenuBar(TreenityClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        TreenityClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TreenityClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TreenityClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(TreenityClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        TreenityClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(TreenityClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TreenityClass->setStatusBar(statusBar);

        retranslateUi(TreenityClass);

        QMetaObject::connectSlotsByName(TreenityClass);
    } // setupUi

    void retranslateUi(QMainWindow *TreenityClass)
    {
        TreenityClass->setWindowTitle(QApplication::translate("TreenityClass", "Treenity", 0));
    } // retranslateUi

};

namespace Ui {
    class TreenityClass: public Ui_TreenityClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREENITY_H
