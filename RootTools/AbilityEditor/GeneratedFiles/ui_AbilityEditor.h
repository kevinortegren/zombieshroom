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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>
#include "CustomListWidget.h"
#include "CustomTreeWidget.h"

QT_BEGIN_NAMESPACE

class Ui_AbilityEditorClass
{
public:
    QAction *actionSave_As;
    QAction *actionSave;
    QAction *actionLoad;
    QAction *actionQuit;
    QAction *actionGenerate_Script;
    QWidget *centralWidget;
    QTabWidget *abilityWidget;
    QWidget *tabOnCreate;
    CustomTreeWidget *treeOnCreate;
    QWidget *tabOnCollide;
    CustomTreeWidget *treeOnCollide;
    QWidget *tabOnDestroy;
    CustomTreeWidget *treeOnDestroy;
    QTabWidget *toolBoxWidget;
    QWidget *tabAbilities;
    CustomListWidget *listAbilities;
    QWidget *tabComponents;
    CustomListWidget *listComponents;
    QWidget *tabConditions;
    CustomListWidget *listWidget;
    QWidget *propertyWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AbilityEditorClass)
    {
        if (AbilityEditorClass->objectName().isEmpty())
            AbilityEditorClass->setObjectName(QStringLiteral("AbilityEditorClass"));
        AbilityEditorClass->resize(592, 682);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(AbilityEditorClass->sizePolicy().hasHeightForWidth());
        AbilityEditorClass->setSizePolicy(sizePolicy);
        actionSave_As = new QAction(AbilityEditorClass);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionSave = new QAction(AbilityEditorClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionLoad = new QAction(AbilityEditorClass);
        actionLoad->setObjectName(QStringLiteral("actionLoad"));
        actionQuit = new QAction(AbilityEditorClass);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        actionGenerate_Script = new QAction(AbilityEditorClass);
        actionGenerate_Script->setObjectName(QStringLiteral("actionGenerate_Script"));
        centralWidget = new QWidget(AbilityEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        abilityWidget = new QTabWidget(centralWidget);
        abilityWidget->setObjectName(QStringLiteral("abilityWidget"));
        abilityWidget->setGeometry(QRect(10, 10, 271, 381));
        tabOnCreate = new QWidget();
        tabOnCreate->setObjectName(QStringLiteral("tabOnCreate"));
        treeOnCreate = new CustomTreeWidget(tabOnCreate);
        treeOnCreate->setObjectName(QStringLiteral("treeOnCreate"));
        treeOnCreate->setGeometry(QRect(0, 0, 261, 351));
        treeOnCreate->setDragEnabled(true);
        treeOnCreate->setDragDropMode(QAbstractItemView::DragDrop);
        treeOnCreate->setDefaultDropAction(Qt::MoveAction);
        abilityWidget->addTab(tabOnCreate, QString());
        tabOnCollide = new QWidget();
        tabOnCollide->setObjectName(QStringLiteral("tabOnCollide"));
        treeOnCollide = new CustomTreeWidget(tabOnCollide);
        treeOnCollide->setObjectName(QStringLiteral("treeOnCollide"));
        treeOnCollide->setGeometry(QRect(0, 0, 261, 351));
        treeOnCollide->setDragEnabled(true);
        treeOnCollide->setDragDropOverwriteMode(true);
        treeOnCollide->setDragDropMode(QAbstractItemView::DragDrop);
        treeOnCollide->setDefaultDropAction(Qt::IgnoreAction);
        treeOnCollide->setColumnCount(1);
        abilityWidget->addTab(tabOnCollide, QString());
        tabOnDestroy = new QWidget();
        tabOnDestroy->setObjectName(QStringLiteral("tabOnDestroy"));
        treeOnDestroy = new CustomTreeWidget(tabOnDestroy);
        treeOnDestroy->setObjectName(QStringLiteral("treeOnDestroy"));
        treeOnDestroy->setGeometry(QRect(0, 0, 261, 351));
        treeOnDestroy->setDragEnabled(true);
        treeOnDestroy->setDragDropMode(QAbstractItemView::DragDrop);
        treeOnDestroy->setDefaultDropAction(Qt::MoveAction);
        abilityWidget->addTab(tabOnDestroy, QString());
        toolBoxWidget = new QTabWidget(centralWidget);
        toolBoxWidget->setObjectName(QStringLiteral("toolBoxWidget"));
        toolBoxWidget->setGeometry(QRect(320, 10, 261, 381));
        tabAbilities = new QWidget();
        tabAbilities->setObjectName(QStringLiteral("tabAbilities"));
        listAbilities = new CustomListWidget(tabAbilities);
        listAbilities->setObjectName(QStringLiteral("listAbilities"));
        listAbilities->setGeometry(QRect(0, 0, 251, 351));
        listAbilities->setDragEnabled(true);
        listAbilities->setDragDropMode(QAbstractItemView::DragOnly);
        toolBoxWidget->addTab(tabAbilities, QString());
        tabComponents = new QWidget();
        tabComponents->setObjectName(QStringLiteral("tabComponents"));
        listComponents = new CustomListWidget(tabComponents);
        listComponents->setObjectName(QStringLiteral("listComponents"));
        listComponents->setGeometry(QRect(0, 0, 251, 351));
        listComponents->setDragEnabled(true);
        listComponents->setDragDropMode(QAbstractItemView::DragOnly);
        toolBoxWidget->addTab(tabComponents, QString());
        tabConditions = new QWidget();
        tabConditions->setObjectName(QStringLiteral("tabConditions"));
        listWidget = new CustomListWidget(tabConditions);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(0, 0, 251, 351));
        toolBoxWidget->addTab(tabConditions, QString());
        propertyWidget = new QWidget(centralWidget);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));
        propertyWidget->setGeometry(QRect(10, 410, 571, 231));
        AbilityEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AbilityEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 592, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        AbilityEditorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(AbilityEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AbilityEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionLoad);
        menuFile->addAction(actionGenerate_Script);
        menuFile->addAction(actionQuit);

        retranslateUi(AbilityEditorClass);

        abilityWidget->setCurrentIndex(0);
        toolBoxWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AbilityEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AbilityEditorClass)
    {
        AbilityEditorClass->setWindowTitle(QApplication::translate("AbilityEditorClass", "Ability Editor", 0));
        actionSave_As->setText(QApplication::translate("AbilityEditorClass", "Save As...", 0));
        actionSave->setText(QApplication::translate("AbilityEditorClass", "Save", 0));
        actionLoad->setText(QApplication::translate("AbilityEditorClass", "Load...", 0));
        actionQuit->setText(QApplication::translate("AbilityEditorClass", "Quit", 0));
        actionGenerate_Script->setText(QApplication::translate("AbilityEditorClass", "Generate Script...", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeOnCreate->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("AbilityEditorClass", "Entities", 0));
        abilityWidget->setTabText(abilityWidget->indexOf(tabOnCreate), QApplication::translate("AbilityEditorClass", "On Create", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = treeOnCollide->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("AbilityEditorClass", "Entities", 0));
        abilityWidget->setTabText(abilityWidget->indexOf(tabOnCollide), QApplication::translate("AbilityEditorClass", "On Collide", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = treeOnDestroy->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("AbilityEditorClass", "Entities", 0));
        abilityWidget->setTabText(abilityWidget->indexOf(tabOnDestroy), QApplication::translate("AbilityEditorClass", "On Destroy", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabAbilities), QApplication::translate("AbilityEditorClass", "Existing Abilities", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabComponents), QApplication::translate("AbilityEditorClass", "Components", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabConditions), QApplication::translate("AbilityEditorClass", "Conditions", 0));
        menuFile->setTitle(QApplication::translate("AbilityEditorClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class AbilityEditorClass: public Ui_AbilityEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABILITYEDITOR_H
