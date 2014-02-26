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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "CustomCompListWidget.h"
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
    QAction *actionEntity;
    QAction *actionNew_Ability;
    QWidget *centralWidget;
    QWidget *layoutWidget;
    QGridLayout *mainLayout;
    CustomCompListWidget *listAbilityComponents;
    QTabWidget *toolBoxWidget;
    QWidget *tabComponents;
    QVBoxLayout *verticalLayout_5;
    CustomListWidget *listComponents;
    QWidget *tabEntities;
    QVBoxLayout *verticalLayout_4;
    QTreeView *listEntities;
    QWidget *tabConditions;
    QVBoxLayout *verticalLayout;
    CustomListWidget *listConditions;
    QTabWidget *OnTabWidget;
    QWidget *tabOnCreate;
    QVBoxLayout *verticalLayout_6;
    CustomTreeWidget *treeOnCreate;
    QWidget *tabOnCollide;
    QVBoxLayout *verticalLayout_2;
    CustomTreeWidget *treeOnCollide;
    QWidget *tabOnDestroy;
    QVBoxLayout *verticalLayout_3;
    CustomTreeWidget *treeOnDestroy;
    QWidget *propertyWidget;
    QWidget *layoutWidget1;
    QHBoxLayout *variableLayout;
    QLabel *chargesLable;
    QSpinBox *chargesSpinbox;
    QLabel *chargingLable;
    QDoubleSpinBox *chargingSpinbox;
    QLabel *channelingLable;
    QDoubleSpinBox *channelingSpingbox;
    QLabel *durationLable;
    QDoubleSpinBox *durationSpinbox;
    QLabel *cooldownLabel;
    QDoubleSpinBox *cooldownSpinbox;
    QLineEdit *abilityNameEdit;
    QPushButton *buttonNameBrowser;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AbilityEditorClass)
    {
        if (AbilityEditorClass->objectName().isEmpty())
            AbilityEditorClass->setObjectName(QStringLiteral("AbilityEditorClass"));
        AbilityEditorClass->resize(791, 738);
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
        actionEntity = new QAction(AbilityEditorClass);
        actionEntity->setObjectName(QStringLiteral("actionEntity"));
        actionNew_Ability = new QAction(AbilityEditorClass);
        actionNew_Ability->setObjectName(QStringLiteral("actionNew_Ability"));
        centralWidget = new QWidget(AbilityEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        layoutWidget = new QWidget(centralWidget);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 69, 771, 631));
        mainLayout = new QGridLayout(layoutWidget);
        mainLayout->setSpacing(6);
        mainLayout->setContentsMargins(11, 11, 11, 11);
        mainLayout->setObjectName(QStringLiteral("mainLayout"));
        mainLayout->setContentsMargins(0, 0, 0, 0);
        listAbilityComponents = new CustomCompListWidget(layoutWidget);
        listAbilityComponents->setObjectName(QStringLiteral("listAbilityComponents"));

        mainLayout->addWidget(listAbilityComponents, 0, 0, 1, 1);

        toolBoxWidget = new QTabWidget(layoutWidget);
        toolBoxWidget->setObjectName(QStringLiteral("toolBoxWidget"));
        tabComponents = new QWidget();
        tabComponents->setObjectName(QStringLiteral("tabComponents"));
        verticalLayout_5 = new QVBoxLayout(tabComponents);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        listComponents = new CustomListWidget(tabComponents);
        listComponents->setObjectName(QStringLiteral("listComponents"));

        verticalLayout_5->addWidget(listComponents);

        toolBoxWidget->addTab(tabComponents, QString());
        tabEntities = new QWidget();
        tabEntities->setObjectName(QStringLiteral("tabEntities"));
        verticalLayout_4 = new QVBoxLayout(tabEntities);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        listEntities = new QTreeView(tabEntities);
        listEntities->setObjectName(QStringLiteral("listEntities"));

        verticalLayout_4->addWidget(listEntities);

        toolBoxWidget->addTab(tabEntities, QString());
        tabConditions = new QWidget();
        tabConditions->setObjectName(QStringLiteral("tabConditions"));
        verticalLayout = new QVBoxLayout(tabConditions);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listConditions = new CustomListWidget(tabConditions);
        listConditions->setObjectName(QStringLiteral("listConditions"));

        verticalLayout->addWidget(listConditions);

        toolBoxWidget->addTab(tabConditions, QString());

        mainLayout->addWidget(toolBoxWidget, 0, 1, 1, 1);

        OnTabWidget = new QTabWidget(layoutWidget);
        OnTabWidget->setObjectName(QStringLiteral("OnTabWidget"));
        tabOnCreate = new QWidget();
        tabOnCreate->setObjectName(QStringLiteral("tabOnCreate"));
        verticalLayout_6 = new QVBoxLayout(tabOnCreate);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        treeOnCreate = new CustomTreeWidget(tabOnCreate);
        treeOnCreate->setObjectName(QStringLiteral("treeOnCreate"));

        verticalLayout_6->addWidget(treeOnCreate);

        OnTabWidget->addTab(tabOnCreate, QString());
        tabOnCollide = new QWidget();
        tabOnCollide->setObjectName(QStringLiteral("tabOnCollide"));
        verticalLayout_2 = new QVBoxLayout(tabOnCollide);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        treeOnCollide = new CustomTreeWidget(tabOnCollide);
        treeOnCollide->setObjectName(QStringLiteral("treeOnCollide"));

        verticalLayout_2->addWidget(treeOnCollide);

        OnTabWidget->addTab(tabOnCollide, QString());
        tabOnDestroy = new QWidget();
        tabOnDestroy->setObjectName(QStringLiteral("tabOnDestroy"));
        verticalLayout_3 = new QVBoxLayout(tabOnDestroy);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        treeOnDestroy = new CustomTreeWidget(tabOnDestroy);
        treeOnDestroy->setObjectName(QStringLiteral("treeOnDestroy"));

        verticalLayout_3->addWidget(treeOnDestroy);

        OnTabWidget->addTab(tabOnDestroy, QString());

        mainLayout->addWidget(OnTabWidget, 1, 0, 1, 1);

        propertyWidget = new QWidget(layoutWidget);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));

        mainLayout->addWidget(propertyWidget, 1, 1, 1, 1);

        layoutWidget1 = new QWidget(centralWidget);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        layoutWidget1->setGeometry(QRect(10, 40, 771, 25));
        variableLayout = new QHBoxLayout(layoutWidget1);
        variableLayout->setSpacing(6);
        variableLayout->setContentsMargins(11, 11, 11, 11);
        variableLayout->setObjectName(QStringLiteral("variableLayout"));
        variableLayout->setContentsMargins(0, 0, 0, 0);
        chargesLable = new QLabel(layoutWidget1);
        chargesLable->setObjectName(QStringLiteral("chargesLable"));

        variableLayout->addWidget(chargesLable, 0, Qt::AlignRight);

        chargesSpinbox = new QSpinBox(layoutWidget1);
        chargesSpinbox->setObjectName(QStringLiteral("chargesSpinbox"));

        variableLayout->addWidget(chargesSpinbox);

        chargingLable = new QLabel(layoutWidget1);
        chargingLable->setObjectName(QStringLiteral("chargingLable"));

        variableLayout->addWidget(chargingLable, 0, Qt::AlignRight);

        chargingSpinbox = new QDoubleSpinBox(layoutWidget1);
        chargingSpinbox->setObjectName(QStringLiteral("chargingSpinbox"));

        variableLayout->addWidget(chargingSpinbox);

        channelingLable = new QLabel(layoutWidget1);
        channelingLable->setObjectName(QStringLiteral("channelingLable"));

        variableLayout->addWidget(channelingLable, 0, Qt::AlignRight);

        channelingSpingbox = new QDoubleSpinBox(layoutWidget1);
        channelingSpingbox->setObjectName(QStringLiteral("channelingSpingbox"));

        variableLayout->addWidget(channelingSpingbox);

        durationLable = new QLabel(layoutWidget1);
        durationLable->setObjectName(QStringLiteral("durationLable"));

        variableLayout->addWidget(durationLable, 0, Qt::AlignRight);

        durationSpinbox = new QDoubleSpinBox(layoutWidget1);
        durationSpinbox->setObjectName(QStringLiteral("durationSpinbox"));

        variableLayout->addWidget(durationSpinbox);

        cooldownLabel = new QLabel(layoutWidget1);
        cooldownLabel->setObjectName(QStringLiteral("cooldownLabel"));

        variableLayout->addWidget(cooldownLabel, 0, Qt::AlignRight);

        cooldownSpinbox = new QDoubleSpinBox(layoutWidget1);
        cooldownSpinbox->setObjectName(QStringLiteral("cooldownSpinbox"));

        variableLayout->addWidget(cooldownSpinbox);

        abilityNameEdit = new QLineEdit(centralWidget);
        abilityNameEdit->setObjectName(QStringLiteral("abilityNameEdit"));
        abilityNameEdit->setGeometry(QRect(10, 10, 141, 20));
        buttonNameBrowser = new QPushButton(centralWidget);
        buttonNameBrowser->setObjectName(QStringLiteral("buttonNameBrowser"));
        buttonNameBrowser->setGeometry(QRect(590, 10, 189, 23));
        AbilityEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AbilityEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 791, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        AbilityEditorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(AbilityEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        AbilityEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew_Ability);
        menuFile->addSeparator();
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionSave);
        menuFile->addSeparator();
        menuFile->addAction(actionLoad);
        menuFile->addSeparator();
        menuFile->addAction(actionGenerate_Script);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit);

        retranslateUi(AbilityEditorClass);

        toolBoxWidget->setCurrentIndex(0);
        OnTabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AbilityEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AbilityEditorClass)
    {
        AbilityEditorClass->setWindowTitle(QApplication::translate("AbilityEditorClass", "Ability Editor", 0));
        actionSave_As->setText(QApplication::translate("AbilityEditorClass", "Save As...", 0));
        actionSave->setText(QApplication::translate("AbilityEditorClass", "Save", 0));
        actionLoad->setText(QApplication::translate("AbilityEditorClass", "Open...", 0));
        actionQuit->setText(QApplication::translate("AbilityEditorClass", "Quit", 0));
        actionGenerate_Script->setText(QApplication::translate("AbilityEditorClass", "Generate Script...", 0));
        actionEntity->setText(QApplication::translate("AbilityEditorClass", "Entity", 0));
        actionNew_Ability->setText(QApplication::translate("AbilityEditorClass", "New Ability", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabComponents), QApplication::translate("AbilityEditorClass", "Components", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabEntities), QApplication::translate("AbilityEditorClass", "Entities", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabConditions), QApplication::translate("AbilityEditorClass", "Conditions", 0));
        QTreeWidgetItem *___qtreewidgetitem = treeOnCreate->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("AbilityEditorClass", "Conditions & Entities", 0));
        OnTabWidget->setTabText(OnTabWidget->indexOf(tabOnCreate), QApplication::translate("AbilityEditorClass", "OnCreate", 0));
        QTreeWidgetItem *___qtreewidgetitem1 = treeOnCollide->headerItem();
        ___qtreewidgetitem1->setText(0, QApplication::translate("AbilityEditorClass", "Conditions & Entities", 0));
        OnTabWidget->setTabText(OnTabWidget->indexOf(tabOnCollide), QApplication::translate("AbilityEditorClass", "OnCollide", 0));
        QTreeWidgetItem *___qtreewidgetitem2 = treeOnDestroy->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("AbilityEditorClass", "Conditions & Entities", 0));
        OnTabWidget->setTabText(OnTabWidget->indexOf(tabOnDestroy), QApplication::translate("AbilityEditorClass", "OnDestroy", 0));
        chargesLable->setText(QApplication::translate("AbilityEditorClass", "Charges", 0));
        chargingLable->setText(QApplication::translate("AbilityEditorClass", "Charging Time", 0));
        channelingLable->setText(QApplication::translate("AbilityEditorClass", "Channeling Time", 0));
        durationLable->setText(QApplication::translate("AbilityEditorClass", "Duration", 0));
        cooldownLabel->setText(QApplication::translate("AbilityEditorClass", "Cooldown", 0));
        abilityNameEdit->setText(QApplication::translate("AbilityEditorClass", "AbilityName", 0));
        buttonNameBrowser->setText(QApplication::translate("AbilityEditorClass", "Browse name for property manager...", 0));
        menuFile->setTitle(QApplication::translate("AbilityEditorClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class AbilityEditorClass: public Ui_AbilityEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABILITYEDITOR_H
