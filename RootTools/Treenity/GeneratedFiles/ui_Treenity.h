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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/include/EntityOutliner.h>

QT_BEGIN_NAMESPACE

class Ui_TreenityClass
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_createEntity;
    EntityOutliner *treeWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *TreenityClass)
    {
        if (TreenityClass->objectName().isEmpty())
            TreenityClass->setObjectName(QStringLiteral("TreenityClass"));
        TreenityClass->resize(899, 581);
        centralWidget = new QWidget(TreenityClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        pushButton_createEntity = new QPushButton(centralWidget);
        pushButton_createEntity->setObjectName(QStringLiteral("pushButton_createEntity"));
        pushButton_createEntity->setGeometry(QRect(400, 160, 75, 23));
        treeWidget = new EntityOutliner(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(10, 10, 256, 192));
        TreenityClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TreenityClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 899, 21));
        TreenityClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TreenityClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TreenityClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TreenityClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        TreenityClass->setStatusBar(statusBar);

        retranslateUi(TreenityClass);

        QMetaObject::connectSlotsByName(TreenityClass);
    } // setupUi

    void retranslateUi(QMainWindow *TreenityClass)
    {
        TreenityClass->setWindowTitle(QApplication::translate("TreenityClass", "Treenity", 0));
        pushButton_createEntity->setText(QApplication::translate("TreenityClass", "Create Entity", 0));
    } // retranslateUi

};

namespace Ui {
    class TreenityClass: public Ui_TreenityClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREENITY_H