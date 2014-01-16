/********************************************************************************
** Form generated from reading UI file 'ParticleEditor.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARTICLEEDITOR_H
#define UI_PARTICLEEDITOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ParticleEditorClass
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionStats;
    QAction *actionEmitter;
    QWidget *centralWidget;
    QFrame *frame;
    QTabWidget *tabWidget;
    QWidget *tabProperties;
    QGroupBox *groupBox;
    QPushButton *pushButton;
    QListWidget *listWidget;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QMenu *menuCreate;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ParticleEditorClass)
    {
        if (ParticleEditorClass->objectName().isEmpty())
            ParticleEditorClass->setObjectName(QStringLiteral("ParticleEditorClass"));
        ParticleEditorClass->resize(1736, 970);
        actionNew = new QAction(ParticleEditorClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionOpen = new QAction(ParticleEditorClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionSave = new QAction(ParticleEditorClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave_As = new QAction(ParticleEditorClass);
        actionSave_As->setObjectName(QStringLiteral("actionSave_As"));
        actionExit = new QAction(ParticleEditorClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAbout = new QAction(ParticleEditorClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionStats = new QAction(ParticleEditorClass);
        actionStats->setObjectName(QStringLiteral("actionStats"));
        actionEmitter = new QAction(ParticleEditorClass);
        actionEmitter->setObjectName(QStringLiteral("actionEmitter"));
        centralWidget = new QWidget(ParticleEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(-1, -1, 1461, 921));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1460, 0, 271, 921));
        QFont font;
        font.setPointSize(16);
        tabWidget->setFont(font);
        tabProperties = new QWidget();
        tabProperties->setObjectName(QStringLiteral("tabProperties"));
        groupBox = new QGroupBox(tabProperties);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 261, 201));
        QFont font1;
        font1.setPointSize(10);
        groupBox->setFont(font1);
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(200, 20, 61, 23));
        pushButton->setFont(font1);
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(0, 20, 181, 181));
        tabWidget->addTab(tabProperties, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());
        ParticleEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ParticleEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1736, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        menuCreate = new QMenu(menuBar);
        menuCreate->setObjectName(QStringLiteral("menuCreate"));
        ParticleEditorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ParticleEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ParticleEditorClass->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuCreate->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionStats);
        menuCreate->addAction(actionEmitter);

        retranslateUi(ParticleEditorClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ParticleEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ParticleEditorClass)
    {
        ParticleEditorClass->setWindowTitle(QApplication::translate("ParticleEditorClass", "ParticleEditor", 0));
        actionNew->setText(QApplication::translate("ParticleEditorClass", "New", 0));
        actionOpen->setText(QApplication::translate("ParticleEditorClass", "Open...", 0));
        actionSave->setText(QApplication::translate("ParticleEditorClass", "Save", 0));
        actionSave_As->setText(QApplication::translate("ParticleEditorClass", "Save As...", 0));
        actionExit->setText(QApplication::translate("ParticleEditorClass", "Exit", 0));
        actionAbout->setText(QApplication::translate("ParticleEditorClass", "About", 0));
        actionStats->setText(QApplication::translate("ParticleEditorClass", "Stats", 0));
        actionEmitter->setText(QApplication::translate("ParticleEditorClass", "Emitter", 0));
        groupBox->setTitle(QApplication::translate("ParticleEditorClass", "Emitters", 0));
        pushButton->setText(QApplication::translate("ParticleEditorClass", "Delete", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabProperties), QApplication::translate("ParticleEditorClass", "Properties", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ParticleEditorClass", "Tab 2", 0));
        menuFile->setTitle(QApplication::translate("ParticleEditorClass", "File", 0));
        menuHelp->setTitle(QApplication::translate("ParticleEditorClass", "Help", 0));
        menuView->setTitle(QApplication::translate("ParticleEditorClass", "View", 0));
        menuCreate->setTitle(QApplication::translate("ParticleEditorClass", "Create", 0));
    } // retranslateUi

};

namespace Ui {
    class ParticleEditorClass: public Ui_ParticleEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARTICLEEDITOR_H
