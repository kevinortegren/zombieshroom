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
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AbilityEditorClass
{
public:
    QWidget *centralWidget;
    QTabWidget *abilityWidget;
    QWidget *tabOnCreate;
    QListWidget *entityList;
    QWidget *tabOnCollide;
    QTreeWidget *treeWidget;
    QWidget *tabOnDestroy;
    QTabWidget *toolBoxWidget;
    QWidget *tab_3;
    QWidget *tabComponents;
    QWidget *propertyWidget;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AbilityEditorClass)
    {
        if (AbilityEditorClass->objectName().isEmpty())
            AbilityEditorClass->setObjectName(QStringLiteral("AbilityEditorClass"));
        AbilityEditorClass->resize(592, 682);
        centralWidget = new QWidget(AbilityEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        abilityWidget = new QTabWidget(centralWidget);
        abilityWidget->setObjectName(QStringLiteral("abilityWidget"));
        abilityWidget->setGeometry(QRect(16, 10, 271, 381));
        tabOnCreate = new QWidget();
        tabOnCreate->setObjectName(QStringLiteral("tabOnCreate"));
        entityList = new QListWidget(tabOnCreate);
        new QListWidgetItem(entityList);
        new QListWidgetItem(entityList);
        new QListWidgetItem(entityList);
        entityList->setObjectName(QStringLiteral("entityList"));
        entityList->setGeometry(QRect(0, 0, 261, 351));
        QFont font;
        font.setPointSize(9);
        entityList->setFont(font);
        entityList->setViewMode(QListView::ListMode);
        abilityWidget->addTab(tabOnCreate, QString());
        tabOnCollide = new QWidget();
        tabOnCollide->setObjectName(QStringLiteral("tabOnCollide"));
        treeWidget = new QTreeWidget(tabOnCollide);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem);
        new QTreeWidgetItem(__qtreewidgetitem);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem(treeWidget);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        new QTreeWidgetItem(__qtreewidgetitem1);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        treeWidget->setGeometry(QRect(0, 0, 261, 351));
        treeWidget->setColumnCount(1);
        abilityWidget->addTab(tabOnCollide, QString());
        tabOnDestroy = new QWidget();
        tabOnDestroy->setObjectName(QStringLiteral("tabOnDestroy"));
        abilityWidget->addTab(tabOnDestroy, QString());
        toolBoxWidget = new QTabWidget(centralWidget);
        toolBoxWidget->setObjectName(QStringLiteral("toolBoxWidget"));
        toolBoxWidget->setGeometry(QRect(320, 10, 261, 381));
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        toolBoxWidget->addTab(tab_3, QString());
        tabComponents = new QWidget();
        tabComponents->setObjectName(QStringLiteral("tabComponents"));
        toolBoxWidget->addTab(tabComponents, QString());
        propertyWidget = new QWidget(centralWidget);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));
        propertyWidget->setGeometry(QRect(10, 410, 571, 231));
        AbilityEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AbilityEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 592, 21));
        AbilityEditorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(AbilityEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AbilityEditorClass->setStatusBar(statusBar);

        retranslateUi(AbilityEditorClass);

        abilityWidget->setCurrentIndex(0);
        toolBoxWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AbilityEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AbilityEditorClass)
    {
        AbilityEditorClass->setWindowTitle(QApplication::translate("AbilityEditorClass", "AbilityEditor", 0));

        const bool __sortingEnabled = entityList->isSortingEnabled();
        entityList->setSortingEnabled(false);
        QListWidgetItem *___qlistwidgetitem = entityList->item(0);
        ___qlistwidgetitem->setText(QApplication::translate("AbilityEditorClass", "New Item", 0));
        QListWidgetItem *___qlistwidgetitem1 = entityList->item(1);
        ___qlistwidgetitem1->setText(QApplication::translate("AbilityEditorClass", "New Item", 0));
        QListWidgetItem *___qlistwidgetitem2 = entityList->item(2);
        ___qlistwidgetitem2->setText(QApplication::translate("AbilityEditorClass", "New Item", 0));
        entityList->setSortingEnabled(__sortingEnabled);

        abilityWidget->setTabText(abilityWidget->indexOf(tabOnCreate), QApplication::translate("AbilityEditorClass", "On Create", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("AbilityEditorClass", "Entities", 0));

        const bool __sortingEnabled1 = treeWidget->isSortingEnabled();
        treeWidget->setSortingEnabled(false);
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget->topLevelItem(0);
        ___qtreewidgetitem1->setText(0, QApplication::translate("AbilityEditorClass", "New Item", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = ___qtreewidgetitem1->child(0);
        ___qtreewidgetitem2->setText(0, QApplication::translate("AbilityEditorClass", "New Item", 0));
        QTreeWidgetItem *___qtreewidgetitem3 = ___qtreewidgetitem1->child(1);
        ___qtreewidgetitem3->setText(0, QApplication::translate("AbilityEditorClass", "New Item", 0));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget->topLevelItem(1);
        ___qtreewidgetitem4->setText(0, QApplication::translate("AbilityEditorClass", "New Item", 0));
        QTreeWidgetItem *___qtreewidgetitem5 = ___qtreewidgetitem4->child(0);
        ___qtreewidgetitem5->setText(0, QApplication::translate("AbilityEditorClass", "New Subitem", 0));
        QTreeWidgetItem *___qtreewidgetitem6 = ___qtreewidgetitem4->child(1);
        ___qtreewidgetitem6->setText(0, QApplication::translate("AbilityEditorClass", "New Item", 0));
        QTreeWidgetItem *___qtreewidgetitem7 = ___qtreewidgetitem4->child(2);
        ___qtreewidgetitem7->setText(0, QApplication::translate("AbilityEditorClass", "New Item", 0));
        treeWidget->setSortingEnabled(__sortingEnabled1);

        abilityWidget->setTabText(abilityWidget->indexOf(tabOnCollide), QApplication::translate("AbilityEditorClass", "On Collide", 0));
        abilityWidget->setTabText(abilityWidget->indexOf(tabOnDestroy), QApplication::translate("AbilityEditorClass", "On Destroy", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tab_3), QApplication::translate("AbilityEditorClass", "Tab 1", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabComponents), QApplication::translate("AbilityEditorClass", "Components", 0));
    } // retranslateUi

};

namespace Ui {
    class AbilityEditorClass: public Ui_AbilityEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABILITYEDITOR_H
