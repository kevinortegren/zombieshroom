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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/include/Canvas3D.h>
#include <RootTools/Treenity/include/EntityOutliner.h>

QT_BEGIN_NAMESPACE

class Ui_TreenityClass
{
public:
    QAction *action_addEntity;
    QAction *actionExit;
    QAction *action_saveAs;
    QAction *action_save;
    QAction *actionRotate;
    QAction *actionResize;
    QAction *actionTranslate;
    QAction *actionPlay;
    QAction *actionLog;
    QAction *action_removeEntity;
    QAction *action_addRenderable;
    QAction *action_addPhysics;
    QAction *actionNew;
    QAction *actionOpen_Project;
    QAction *action_addWaterCollider;
    QAction *action_addScript;
    QAction *action_collisionResponder;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    Canvas3D *widget_canvas3D;
    QWidget *treedeetoolbar;
    QPushButton *pushButton_translateMode;
    QPushButton *pushButton_rotateMode;
    QPushButton *pushButton_scaleMode;
    QComboBox *comboBox;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuEntity;
    QMenu *menuComponent;
    QMenu *menu_addComponent;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;
    QDockWidget *dockWidget_4;
    QWidget *dockWidgetContents_4;
    QVBoxLayout *verticalLayout_3;
    EntityOutliner *treeView_entityOutliner;
    QWidget *widget;
    QPushButton *pushButton_addEntity;
    QPushButton *pushButton_removeEntity;
    QDockWidget *dockWidget_6;
    QWidget *dockWidgetContents_6;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLineEdit *lineEdit_entityName;
    QLabel *label;
    QDockWidget *dockWidget_assetManager;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_5;
    QVBoxLayout *verticalLayout_assetManager;

    void setupUi(QMainWindow *TreenityClass)
    {
        if (TreenityClass->objectName().isEmpty())
            TreenityClass->setObjectName(QStringLiteral("TreenityClass"));
        TreenityClass->resize(1292, 878);
        TreenityClass->setStyleSheet(QStringLiteral(""));
        TreenityClass->setTabShape(QTabWidget::Rounded);
        TreenityClass->setDockOptions(QMainWindow::AllowNestedDocks|QMainWindow::AllowTabbedDocks|QMainWindow::AnimatedDocks);
        action_addEntity = new QAction(TreenityClass);
        action_addEntity->setObjectName(QStringLiteral("action_addEntity"));
        actionExit = new QAction(TreenityClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        action_saveAs = new QAction(TreenityClass);
        action_saveAs->setObjectName(QStringLiteral("action_saveAs"));
        action_save = new QAction(TreenityClass);
        action_save->setObjectName(QStringLiteral("action_save"));
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
        actionLog = new QAction(TreenityClass);
        actionLog->setObjectName(QStringLiteral("actionLog"));
        action_removeEntity = new QAction(TreenityClass);
        action_removeEntity->setObjectName(QStringLiteral("action_removeEntity"));
        action_addRenderable = new QAction(TreenityClass);
        action_addRenderable->setObjectName(QStringLiteral("action_addRenderable"));
        action_addPhysics = new QAction(TreenityClass);
        action_addPhysics->setObjectName(QStringLiteral("action_addPhysics"));
        actionNew = new QAction(TreenityClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen_Project = new QAction(TreenityClass);
        actionOpen_Project->setObjectName(QStringLiteral("actionOpen_Project"));
        action_addWaterCollider = new QAction(TreenityClass);
        action_addWaterCollider->setObjectName(QStringLiteral("action_addWaterCollider"));
        action_addScript = new QAction(TreenityClass);
        action_addScript->setObjectName(QStringLiteral("action_addScript"));
        action_collisionResponder = new QAction(TreenityClass);
        action_collisionResponder->setObjectName(QStringLiteral("action_collisionResponder"));
        centralWidget = new QWidget(TreenityClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        widget_canvas3D = new Canvas3D(centralWidget);
        widget_canvas3D->setObjectName(QStringLiteral("widget_canvas3D"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(widget_canvas3D->sizePolicy().hasHeightForWidth());
        widget_canvas3D->setSizePolicy(sizePolicy);
        widget_canvas3D->setStyleSheet(QStringLiteral("background-color: #000000;"));

        verticalLayout_2->addWidget(widget_canvas3D);

        treedeetoolbar = new QWidget(centralWidget);
        treedeetoolbar->setObjectName(QStringLiteral("treedeetoolbar"));
        treedeetoolbar->setMinimumSize(QSize(480, 32));
        treedeetoolbar->setMaximumSize(QSize(16777215, 32));
        pushButton_translateMode = new QPushButton(treedeetoolbar);
        pushButton_translateMode->setObjectName(QStringLiteral("pushButton_translateMode"));
        pushButton_translateMode->setGeometry(QRect(0, 0, 32, 32));
        pushButton_translateMode->setMinimumSize(QSize(32, 32));
        pushButton_translateMode->setMaximumSize(QSize(32, 32));
        pushButton_translateMode->setIcon(icon2);
        pushButton_translateMode->setCheckable(true);
        pushButton_translateMode->setChecked(true);
        pushButton_translateMode->setAutoExclusive(true);
        pushButton_rotateMode = new QPushButton(treedeetoolbar);
        pushButton_rotateMode->setObjectName(QStringLiteral("pushButton_rotateMode"));
        pushButton_rotateMode->setGeometry(QRect(32, 0, 32, 32));
        pushButton_rotateMode->setMinimumSize(QSize(32, 32));
        pushButton_rotateMode->setMaximumSize(QSize(32, 32));
        pushButton_rotateMode->setIcon(icon);
        pushButton_rotateMode->setCheckable(true);
        pushButton_rotateMode->setAutoExclusive(true);
        pushButton_scaleMode = new QPushButton(treedeetoolbar);
        pushButton_scaleMode->setObjectName(QStringLiteral("pushButton_scaleMode"));
        pushButton_scaleMode->setGeometry(QRect(64, 0, 32, 32));
        pushButton_scaleMode->setMinimumSize(QSize(32, 32));
        pushButton_scaleMode->setMaximumSize(QSize(32, 32));
        pushButton_scaleMode->setStyleSheet(QStringLiteral(""));
        pushButton_scaleMode->setIcon(icon1);
        pushButton_scaleMode->setCheckable(true);
        pushButton_scaleMode->setAutoExclusive(true);
        comboBox = new QComboBox(treedeetoolbar);
        comboBox->setObjectName(QStringLiteral("comboBox"));
        comboBox->setGeometry(QRect(96, 0, 70, 32));
        comboBox->setMinimumSize(QSize(70, 32));
        comboBox->setStyleSheet(QStringLiteral(""));

        verticalLayout_2->addWidget(treedeetoolbar);

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
        menu_addComponent = new QMenu(menuComponent);
        menu_addComponent->setObjectName(QStringLiteral("menu_addComponent"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
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
        verticalLayout_3 = new QVBoxLayout(dockWidgetContents_4);
        verticalLayout_3->setSpacing(0);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        treeView_entityOutliner = new EntityOutliner(dockWidgetContents_4);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeView_entityOutliner->setHeaderItem(__qtreewidgetitem);
        treeView_entityOutliner->setObjectName(QStringLiteral("treeView_entityOutliner"));
        sizePolicy.setHeightForWidth(treeView_entityOutliner->sizePolicy().hasHeightForWidth());
        treeView_entityOutliner->setSizePolicy(sizePolicy);
        treeView_entityOutliner->setMinimumSize(QSize(0, 0));
        treeView_entityOutliner->setMaximumSize(QSize(16777215, 16777215));
        treeView_entityOutliner->setSelectionMode(QAbstractItemView::ExtendedSelection);
        treeView_entityOutliner->header()->setVisible(false);

        verticalLayout_3->addWidget(treeView_entityOutliner);

        widget = new QWidget(dockWidgetContents_4);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMinimumSize(QSize(64, 32));
        widget->setMaximumSize(QSize(16777215, 32));
        pushButton_addEntity = new QPushButton(widget);
        pushButton_addEntity->setObjectName(QStringLiteral("pushButton_addEntity"));
        pushButton_addEntity->setGeometry(QRect(0, 0, 32, 32));
        pushButton_addEntity->setMinimumSize(QSize(32, 32));
        pushButton_addEntity->setMaximumSize(QSize(32, 32));
        pushButton_removeEntity = new QPushButton(widget);
        pushButton_removeEntity->setObjectName(QStringLiteral("pushButton_removeEntity"));
        pushButton_removeEntity->setGeometry(QRect(32, 0, 32, 32));
        pushButton_removeEntity->setMinimumSize(QSize(32, 32));
        pushButton_removeEntity->setMaximumSize(QSize(32, 32));

        verticalLayout_3->addWidget(widget);

        dockWidget_4->setWidget(dockWidgetContents_4);
        TreenityClass->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget_4);
        dockWidget_6 = new QDockWidget(TreenityClass);
        dockWidget_6->setObjectName(QStringLiteral("dockWidget_6"));
        dockWidget_6->setMinimumSize(QSize(200, 75));
        dockWidget_6->setAcceptDrops(true);
        dockWidget_6->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        dockWidgetContents_6->setAcceptDrops(true);
        verticalLayout = new QVBoxLayout(dockWidgetContents_6);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(dockWidgetContents_6);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setMaximumSize(QSize(16777215, 120));
        groupBox->setAcceptDrops(true);
        groupBox->setFlat(false);
        groupBox->setCheckable(false);
        formLayout = new QFormLayout(groupBox);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        lineEdit_entityName = new QLineEdit(groupBox);
        lineEdit_entityName->setObjectName(QStringLiteral("lineEdit_entityName"));

        formLayout->setWidget(0, QFormLayout::FieldRole, lineEdit_entityName);

        label = new QLabel(groupBox);
        label->setObjectName(QStringLiteral("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);


        verticalLayout->addWidget(groupBox);

        dockWidget_6->setWidget(dockWidgetContents_6);
        TreenityClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget_6);
        dockWidget_assetManager = new QDockWidget(TreenityClass);
        dockWidget_assetManager->setObjectName(QStringLiteral("dockWidget_assetManager"));
        dockWidget_assetManager->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        verticalLayout_5 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_5->setSpacing(0);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        verticalLayout_assetManager = new QVBoxLayout();
        verticalLayout_assetManager->setSpacing(0);
        verticalLayout_assetManager->setObjectName(QStringLiteral("verticalLayout_assetManager"));

        verticalLayout_5->addLayout(verticalLayout_assetManager);

        dockWidget_assetManager->setWidget(dockWidgetContents);
        TreenityClass->addDockWidget(static_cast<Qt::DockWidgetArea>(8), dockWidget_assetManager);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuEntity->menuAction());
        menuBar->addAction(menuComponent->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen_Project);
        menuFile->addAction(action_saveAs);
        menuFile->addAction(action_save);
        menuFile->addAction(actionExit);
        menuEntity->addAction(action_addEntity);
        menuEntity->addAction(action_removeEntity);
        menuComponent->addAction(menu_addComponent->menuAction());
        menu_addComponent->addAction(action_addRenderable);
        menu_addComponent->addAction(action_addPhysics);
        menu_addComponent->addAction(action_addWaterCollider);
        menu_addComponent->addAction(action_addScript);
        menu_addComponent->addAction(action_collisionResponder);
        menuView->addAction(actionLog);
        mainToolBar->addAction(actionPlay);

        retranslateUi(TreenityClass);

        QMetaObject::connectSlotsByName(TreenityClass);
    } // setupUi

    void retranslateUi(QMainWindow *TreenityClass)
    {
        TreenityClass->setWindowTitle(QApplication::translate("TreenityClass", "Treenity", 0));
        action_addEntity->setText(QApplication::translate("TreenityClass", "Add entity", 0));
        actionExit->setText(QApplication::translate("TreenityClass", "Exit", 0));
        actionExit->setShortcut(QApplication::translate("TreenityClass", "Ctrl+D", 0));
        action_saveAs->setText(QApplication::translate("TreenityClass", "Save As...", 0));
        action_save->setText(QApplication::translate("TreenityClass", "Save", 0));
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
        actionLog->setText(QApplication::translate("TreenityClass", "Log", 0));
        action_removeEntity->setText(QApplication::translate("TreenityClass", "Remove entity", 0));
        action_addRenderable->setText(QApplication::translate("TreenityClass", "Renderable", 0));
#ifndef QT_NO_TOOLTIP
        action_addRenderable->setToolTip(QApplication::translate("TreenityClass", "Add a Renderable component to the selected entity.", 0));
#endif // QT_NO_TOOLTIP
        action_addPhysics->setText(QApplication::translate("TreenityClass", "Physics", 0));
#ifndef QT_NO_TOOLTIP
        action_addPhysics->setToolTip(QApplication::translate("TreenityClass", "Add mass and velocity to the selected entity.", 0));
#endif // QT_NO_TOOLTIP
        actionNew->setText(QApplication::translate("TreenityClass", "New", 0));
        actionOpen_Project->setText(QApplication::translate("TreenityClass", "Open Project...", 0));
        action_addWaterCollider->setText(QApplication::translate("TreenityClass", "Water Collider", 0));
        action_addScript->setText(QApplication::translate("TreenityClass", "Script", 0));
        action_collisionResponder->setText(QApplication::translate("TreenityClass", "Collision responder", 0));
        pushButton_translateMode->setText(QString());
        pushButton_rotateMode->setText(QString());
        pushButton_scaleMode->setText(QString());
        comboBox->clear();
        comboBox->insertItems(0, QStringList()
         << QApplication::translate("TreenityClass", "Global", 0)
         << QApplication::translate("TreenityClass", "Local", 0)
        );
        menuFile->setTitle(QApplication::translate("TreenityClass", "File", 0));
        menuEdit->setTitle(QApplication::translate("TreenityClass", "Edit", 0));
        menuEntity->setTitle(QApplication::translate("TreenityClass", "Entity", 0));
        menuComponent->setTitle(QApplication::translate("TreenityClass", "Component", 0));
        menu_addComponent->setTitle(QApplication::translate("TreenityClass", "Add component", 0));
        menuView->setTitle(QApplication::translate("TreenityClass", "View", 0));
        mainToolBar->setWindowTitle(QApplication::translate("TreenityClass", "Play toolbar", 0));
        dockWidget_4->setWindowTitle(QApplication::translate("TreenityClass", "Outline", 0));
        pushButton_addEntity->setText(QApplication::translate("TreenityClass", "+", 0));
        pushButton_removeEntity->setText(QApplication::translate("TreenityClass", "-", 0));
        dockWidget_6->setWindowTitle(QApplication::translate("TreenityClass", "Properties", 0));
        groupBox->setTitle(QApplication::translate("TreenityClass", "General", 0));
        label->setText(QApplication::translate("TreenityClass", "Name", 0));
        dockWidget_assetManager->setWindowTitle(QApplication::translate("TreenityClass", "Asset browser", 0));
    } // retranslateUi

};

namespace Ui {
    class TreenityClass: public Ui_TreenityClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TREENITY_H
