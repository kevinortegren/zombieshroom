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
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/include/Canvas3D.h>
#include <RootTools/Treenity/include/EntityOutliner.h>

QT_BEGIN_NAMESPACE

class Ui_TreenityClass
{
public:
    QAction *actionAdd_entity;
    QAction *actionExit;
    QAction *actionSave_As;
    QAction *actionSave;
    QAction *actionRotate;
    QAction *actionResize;
    QAction *actionTranslate;
    QAction *actionPlay;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    Canvas3D *widget_canvas3D;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuEntity;
    QMenu *menuComponent;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_4;
    QWidget *dockWidgetContents_4;
    QHBoxLayout *horizontalLayout_3;
    EntityOutliner *treeView_entityOutliner;
    QDockWidget *dockWidget_6;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLineEdit *lineEdit;
    QLabel *label;
    QToolBox *toolBox;
    QWidget *page_2;
    QToolBar *toolBar;

    void setupUi(QMainWindow *TreenityClass)
    {
        if (TreenityClass->objectName().isEmpty())
            TreenityClass->setObjectName(QStringLiteral("TreenityClass"));
        TreenityClass->resize(1292, 902);
        TreenityClass->setStyleSheet(QStringLiteral(""));
        actionAdd_entity = new QAction(TreenityClass);
        actionAdd_entity->setObjectName(QStringLiteral("actionAdd_entity"));
        actionExit = new QAction(TreenityClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionSave_As = new QAction(TreenityClass);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionSave = new QAction(TreenityClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionRotate = new QAction(TreenityClass);
        actionRotate->setObjectName(QStringLiteral("actionRotate"));
        actionRotate->setCheckable(true);
        QIcon icon;
        icon.addFile(QStringLiteral("Resources/rotateButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionRotate->setIcon(icon);
        actionResize = new QAction(TreenityClass);
        actionResize->setObjectName(QStringLiteral("actionResize"));
        actionResize->setCheckable(true);
        QIcon icon1;
        icon1.addFile(QStringLiteral("Resources/resizeButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionResize->setIcon(icon1);
        actionTranslate = new QAction(TreenityClass);
        actionTranslate->setObjectName(QStringLiteral("actionTranslate"));
        actionTranslate->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QStringLiteral("Resources/translateButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionTranslate->setIcon(icon2);
        actionPlay = new QAction(TreenityClass);
        actionPlay->setObjectName(QStringLiteral("actionPlay"));
        QIcon icon3;
        icon3.addFile(QStringLiteral("Resources/playButton.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionPlay->setIcon(icon3);
        centralWidget = new QWidget(TreenityClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(0);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        widget_canvas3D = new Canvas3D(centralWidget);
        widget_canvas3D->setObjectName(QStringLiteral("widget_canvas3D"));
        widget_canvas3D->setMinimumSize(QSize(375, 375));
        widget_canvas3D->setStyleSheet(QStringLiteral("background-color: black;"));

        horizontalLayout->addWidget(widget_canvas3D);

        TreenityClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(TreenityClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1292, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        menuEntity = new QMenu(menuBar);
        menuEntity->setObjectName(QStringLiteral("menuEntity"));
        menuComponent = new QMenu(menuBar);
        menuComponent->setObjectName(QStringLiteral("menuComponent"));
        TreenityClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(TreenityClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        TreenityClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(TreenityClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setSizeGripEnabled(true);
        TreenityClass->setStatusBar(statusBar);
        dockWidget_4 = new QDockWidget(TreenityClass);
        dockWidget_4->setObjectName(QStringLiteral("dockWidget_4"));
        dockWidget_4->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        horizontalLayout_3 = new QHBoxLayout(dockWidgetContents_4);
        horizontalLayout_3->setSpacing(0);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        treeView_entityOutliner = new EntityOutliner(dockWidgetContents_4);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("Entities"));
        treeView_entityOutliner->setHeaderItem(__qtreewidgetitem);
        treeView_entityOutliner->setObjectName(QStringLiteral("treeView_entityOutliner"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(treeView_entityOutliner->sizePolicy().hasHeightForWidth());
        treeView_entityOutliner->setSizePolicy(sizePolicy);
        treeView_entityOutliner->setMinimumSize(QSize(0, 0));
        treeView_entityOutliner->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_3->addWidget(treeView_entityOutliner);

        dockWidget_4->setWidget(dockWidgetContents_4);
        TreenityClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_4);
        dockWidget_6 = new QDockWidget(TreenityClass);
        dockWidget_6->setObjectName(QStringLiteral("dockWidget_6"));
        dockWidget_6->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        verticalLayout = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(dockWidgetContents_6);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 120));
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);


        verticalLayout->addWidget(groupBox);

        toolBox = new QToolBox(dockWidgetContents_6);
        toolBox->setObjectName(QStringLiteral("toolBox"));
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        page_2->setGeometry(QRect(0, 0, 186, 701));
        toolBox->addItem(page_2, QStringLiteral("Renderable"));

        verticalLayout->addWidget(toolBox);

        dockWidget_6->setWidget(dockWidgetContents_6);
        TreenityClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_6);
        toolBar = new QToolBar(TreenityClass);
        toolBar->setObjectName(QStringLiteral("toolBar"));
        TreenityClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuEntity->menuAction());
        menuBar->addAction(menuComponent->menuAction());
        menuFile->addAction(actionSave_As);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionExit);
        menuEntity->addAction(actionAdd_entity);
        mainToolBar->addAction(actionPlay);
        toolBar->addAction(actionTranslate);
        toolBar->addAction(actionRotate);
        toolBar->addAction(actionResize);

        retranslateUi(TreenityClass);

        toolBox->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(TreenityClass);
    } // setupUi

    void retranslateUi(QMainWindow *TreenityClass)
    {
        TreenityClass->setWindowTitle(QApplication::translate("TreenityClass", "Treenity", 0));
        actionAdd_entity->setText(QApplication::translate("TreenityClass", "Add entity", 0));
        actionExit->setText(QApplication::translate("TreenityClass", "Exit", 0));
        actionSave_As->setText(QApplication::translate("TreenityClass", "Save As...", 0));
        actionSave->setText(QApplication::translate("TreenityClass", "Save", 0));
        actionRotate->setText(QApplication::translate("TreenityClass", "Rotate", 0));
#ifndef QT_NO_TOOLTIP
        actionRotate->setToolTip(QApplication::translate("TreenityClass", "Rotate things", 0));
#endif // QT_NO_TOOLTIP
        actionResize->setText(QApplication::translate("TreenityClass", "Resize", 0));
#ifndef QT_NO_TOOLTIP
        actionResize->setToolTip(QApplication::translate("TreenityClass", "Resize things", 0));
#endif // QT_NO_TOOLTIP
        actionTranslate->setText(QApplication::translate("TreenityClass", "Translate", 0));
#ifndef QT_NO_TOOLTIP
        actionTranslate->setToolTip(QApplication::translate("TreenityClass", "Move things", 0));
#endif // QT_NO_TOOLTIP
        actionPlay->setText(QApplication::translate("TreenityClass", "Play", 0));
#ifndef QT_NO_TOOLTIP
        actionPlay->setToolTip(QApplication::translate("TreenityClass", "Play the game", 0));
#endif // QT_NO_TOOLTIP
        menuFile->setTitle(QApplication::translate("TreenityClass", "File", 0));
        menuEdit->setTitle(QApplication::translate("TreenityClass", "Edit", 0));
        menuEntity->setTitle(QApplication::translate("TreenityClass", "Entity", 0));
        menuComponent->setTitle(QApplication::translate("TreenityClass", "Component", 0));
        dockWidget_4->setWindowTitle(QApplication::translate("TreenityClass", "Outline", 0));
        dockWidget_6->setWindowTitle(QApplication::translate("TreenityClass", "Properties", 0));
        groupBox->setTitle(QApplication::translate("TreenityClass", "General", 0));
        label->setText(QApplication::translate("TreenityClass", "Name", 0));
        toolBox->setItemText(toolBox->indexOf(page_2), QApplication::translate("TreenityClass", "Renderable", 0));
        toolBar->setWindowTitle(QApplication::translate("TreenityClass", "toolBar", 0));
    } // retranslateUi

};

namespace Ui {
    class TreenityClass: public Ui_TreenityClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREENITY_H
