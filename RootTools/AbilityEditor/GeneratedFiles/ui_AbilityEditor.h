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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

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
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QWidget *propertyWidget;
    QTabWidget *toolBoxWidget;
    QWidget *tabEntities;
    QVBoxLayout *verticalLayout_4;
    QListWidget *listEntities;
    QWidget *tabComponents;
    QVBoxLayout *verticalLayout_5;
    QListWidget *listComponents;
    QWidget *tabConditions;
    QVBoxLayout *verticalLayout_6;
    QListWidget *listConditions;
    QListWidget *listAbilityComponents;
    QTabWidget *toolBoxWidget_2;
    QWidget *tabOnCreate;
    QVBoxLayout *verticalLayout;
    QListWidget *listOnCreate;
    QWidget *tabOnCollide;
    QVBoxLayout *verticalLayout_2;
    QListWidget *listOnCollide;
    QWidget *tabOnDestroy;
    QVBoxLayout *verticalLayout_3;
    QListWidget *listOnDestroy;
    QPushButton *buttonNameBrowser;
    QLabel *labelComponent;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *AbilityEditorClass)
    {
        if (AbilityEditorClass->objectName().isEmpty())
            AbilityEditorClass->setObjectName(QStringLiteral("AbilityEditorClass"));
        AbilityEditorClass->resize(783, 738);
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
        centralWidget = new QWidget(AbilityEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        propertyWidget = new QWidget(centralWidget);
        propertyWidget->setObjectName(QStringLiteral("propertyWidget"));

        gridLayout->addWidget(propertyWidget, 3, 1, 1, 1);

        toolBoxWidget = new QTabWidget(centralWidget);
        toolBoxWidget->setObjectName(QStringLiteral("toolBoxWidget"));
        tabEntities = new QWidget();
        tabEntities->setObjectName(QStringLiteral("tabEntities"));
        verticalLayout_4 = new QVBoxLayout(tabEntities);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        listEntities = new QListWidget(tabEntities);
        listEntities->setObjectName(QStringLiteral("listEntities"));

        verticalLayout_4->addWidget(listEntities);

        toolBoxWidget->addTab(tabEntities, QString());
        tabComponents = new QWidget();
        tabComponents->setObjectName(QStringLiteral("tabComponents"));
        verticalLayout_5 = new QVBoxLayout(tabComponents);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        listComponents = new QListWidget(tabComponents);
        listComponents->setObjectName(QStringLiteral("listComponents"));

        verticalLayout_5->addWidget(listComponents);

        toolBoxWidget->addTab(tabComponents, QString());
        tabConditions = new QWidget();
        tabConditions->setObjectName(QStringLiteral("tabConditions"));
        verticalLayout_6 = new QVBoxLayout(tabConditions);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        listConditions = new QListWidget(tabConditions);
        listConditions->setObjectName(QStringLiteral("listConditions"));

        verticalLayout_6->addWidget(listConditions);

        toolBoxWidget->addTab(tabConditions, QString());

        gridLayout->addWidget(toolBoxWidget, 1, 1, 1, 1);

        listAbilityComponents = new QListWidget(centralWidget);
        listAbilityComponents->setObjectName(QStringLiteral("listAbilityComponents"));

        gridLayout->addWidget(listAbilityComponents, 1, 0, 1, 1);

        toolBoxWidget_2 = new QTabWidget(centralWidget);
        toolBoxWidget_2->setObjectName(QStringLiteral("toolBoxWidget_2"));
        tabOnCreate = new QWidget();
        tabOnCreate->setObjectName(QStringLiteral("tabOnCreate"));
        verticalLayout = new QVBoxLayout(tabOnCreate);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        listOnCreate = new QListWidget(tabOnCreate);
        listOnCreate->setObjectName(QStringLiteral("listOnCreate"));

        verticalLayout->addWidget(listOnCreate);

        toolBoxWidget_2->addTab(tabOnCreate, QString());
        tabOnCollide = new QWidget();
        tabOnCollide->setObjectName(QStringLiteral("tabOnCollide"));
        verticalLayout_2 = new QVBoxLayout(tabOnCollide);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        listOnCollide = new QListWidget(tabOnCollide);
        listOnCollide->setObjectName(QStringLiteral("listOnCollide"));

        verticalLayout_2->addWidget(listOnCollide);

        toolBoxWidget_2->addTab(tabOnCollide, QString());
        tabOnDestroy = new QWidget();
        tabOnDestroy->setObjectName(QStringLiteral("tabOnDestroy"));
        verticalLayout_3 = new QVBoxLayout(tabOnDestroy);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        listOnDestroy = new QListWidget(tabOnDestroy);
        listOnDestroy->setObjectName(QStringLiteral("listOnDestroy"));

        verticalLayout_3->addWidget(listOnDestroy);

        toolBoxWidget_2->addTab(tabOnDestroy, QString());

        gridLayout->addWidget(toolBoxWidget_2, 3, 0, 1, 1);

        buttonNameBrowser = new QPushButton(centralWidget);
        buttonNameBrowser->setObjectName(QStringLiteral("buttonNameBrowser"));

        gridLayout->addWidget(buttonNameBrowser, 0, 1, 1, 1, Qt::AlignRight);

        labelComponent = new QLabel(centralWidget);
        labelComponent->setObjectName(QStringLiteral("labelComponent"));
        labelComponent->setFrameShape(QFrame::Box);

        gridLayout->addWidget(labelComponent, 0, 0, 1, 1, Qt::AlignHCenter);

        AbilityEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(AbilityEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 783, 21));
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

        toolBoxWidget->setCurrentIndex(0);
        toolBoxWidget_2->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AbilityEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *AbilityEditorClass)
    {
        AbilityEditorClass->setWindowTitle(QApplication::translate("AbilityEditorClass", "Ability Editor", 0));
        actionSave_As->setText(QApplication::translate("AbilityEditorClass", "Save As...", 0));
        actionSave->setText(QApplication::translate("AbilityEditorClass", "Save", 0));
        actionLoad->setText(QApplication::translate("AbilityEditorClass", "Open...", 0));
        actionQuit->setText(QApplication::translate("AbilityEditorClass", "Quit", 0));
        actionGenerate_Script->setText(QApplication::translate("AbilityEditorClass", "Generate Script Folder...", 0));
        actionEntity->setText(QApplication::translate("AbilityEditorClass", "Entity", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabEntities), QApplication::translate("AbilityEditorClass", "Entities", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabComponents), QApplication::translate("AbilityEditorClass", "Components", 0));
        toolBoxWidget->setTabText(toolBoxWidget->indexOf(tabConditions), QApplication::translate("AbilityEditorClass", "Conditions", 0));
        toolBoxWidget_2->setTabText(toolBoxWidget_2->indexOf(tabOnCreate), QApplication::translate("AbilityEditorClass", "OnCreate", 0));
        toolBoxWidget_2->setTabText(toolBoxWidget_2->indexOf(tabOnCollide), QApplication::translate("AbilityEditorClass", "OnCollide", 0));
        toolBoxWidget_2->setTabText(toolBoxWidget_2->indexOf(tabOnDestroy), QApplication::translate("AbilityEditorClass", "OnDestroy", 0));
        buttonNameBrowser->setText(QApplication::translate("AbilityEditorClass", "Browse name for property manager", 0));
        labelComponent->setText(QApplication::translate("AbilityEditorClass", "Ability Components", 0));
        menuFile->setTitle(QApplication::translate("AbilityEditorClass", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class AbilityEditorClass: public Ui_AbilityEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABILITYEDITOR_H
