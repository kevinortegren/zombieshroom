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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDial>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeView>
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
    QAction *actionRed;
    QAction *actionGreen;
    QAction *actionYellow;
    QAction *actionWhite;
    QAction *actionGrid_2;
    QAction *actionColor_Triangle;
    QAction *actionEndColor_Triangle;
    QAction *actionHelp;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tabProperties;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_5;
    QGroupBox *groupBox_2;
    QComboBox *templateComboBox;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QWidget *page_3;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QDial *orbitSpeedDial;
    QDial *orbitRadiusDial;
    QDoubleSpinBox *orbitSpeedSpinBox;
    QDoubleSpinBox *orbitRadiusSpinBox;
    QLabel *label_15;
    QLabel *label_14;
    QComboBox *orbitAxisComboBox;
    QLabel *label_19;
    QWidget *page_4;
    QWidget *page_5;
    QWidget *page_2;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_7;
    QLabel *label_9;
    QDoubleSpinBox *posSpinBoxY;
    QDoubleSpinBox *posSpinBoxX;
    QDoubleSpinBox *speedMinSpinBox;
    QLabel *label_2;
    QLabel *label;
    QDoubleSpinBox *posSpinBoxZ;
    QDoubleSpinBox *spawnTimeSpinBox;
    QLabel *label_10;
    QLabel *label_17;
    QLabel *label_11;
    QDoubleSpinBox *gravitySpinBoxY;
    QLabel *label_8;
    QDoubleSpinBox *gravitySpinBoxZ;
    QDoubleSpinBox *gravitySpinBoxX;
    QDoubleSpinBox *directionSpinBoxX;
    QDoubleSpinBox *directionSpinBoxY;
    QDoubleSpinBox *directionSpinBoxZ;
    QLabel *label_12;
    QDoubleSpinBox *sizeMaxSpinBoxX;
    QLabel *label_3;
    QDoubleSpinBox *sizeEndSpinBoxX;
    QLabel *label_6;
    QLabel *label_13;
    QDoubleSpinBox *spreadSpinBox;
    QDoubleSpinBox *sizeMinSpinBoxX;
    QDoubleSpinBox *lifeTimeMinSpinBox;
    QDoubleSpinBox *speedMaxSpinBox;
    QDoubleSpinBox *lifeTimeMaxSpinBox;
    QPushButton *focusButton;
    QWidget *tab_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_16;
    QLineEdit *textureEmitterLineEdit;
    QTreeView *textureTreeView;
    QGroupBox *groupBox;
    QListWidget *listWidget;
    QFrame *frame;
    QGroupBox *groupBox_6;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QPushButton *newEmitterButton;
    QPushButton *renameEmitterButton;
    QPushButton *deleteEmitterButton;
    QLineEdit *nameEmitterLineEdit;
    QDockWidget *aboutWidget;
    QWidget *dockWidgetContents_2;
    QTextBrowser *textBrowser;
    QDockWidget *helpWidget;
    QWidget *dockWidgetContents_6;
    QTextBrowser *textBrowser_2;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QLCDNumber *FPSlcdNumber;
    QLabel *label_4;
    QDoubleSpinBox *lookAtSpinBoxY;
    QDoubleSpinBox *lookAtSpinBoxX;
    QDoubleSpinBox *lookAtSpinBoxZ;
    QLabel *label_18;
    QDockWidget *colorDockWidget;
    QWidget *dockWidgetContents_3;
    QSlider *colorAlphaSlider;
    QDoubleSpinBox *colorSpinBoxR;
    QDoubleSpinBox *colorSpinBoxG;
    QDoubleSpinBox *colorSpinBoxB;
    QDoubleSpinBox *colorSpinBoxA;
    QDockWidget *endcolorDockWidget;
    QWidget *dockWidgetContents_4;
    QSlider *endcolorAlphaSlider;
    QDoubleSpinBox *endcolorSpinBoxG;
    QDoubleSpinBox *endcolorSpinBoxB;
    QDoubleSpinBox *endcolorSpinBoxR;
    QDoubleSpinBox *endcolorSpinBoxA;
    QDockWidget *gridDockWidget;
    QWidget *dockWidgetContents_5;
    QDoubleSpinBox *gridSpaceSpinBox;
    QLabel *label_5;

    void setupUi(QMainWindow *ParticleEditorClass)
    {
        if (ParticleEditorClass->objectName().isEmpty())
            ParticleEditorClass->setObjectName(QStringLiteral("ParticleEditorClass"));
        ParticleEditorClass->resize(1731, 970);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(227, 227, 227, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Base, brush2);
        QBrush brush3(QColor(105, 105, 105, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Shadow, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::Shadow, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Midlight, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Shadow, brush3);
        ParticleEditorClass->setPalette(palette);
        actionNew = new QAction(ParticleEditorClass);
        actionNew->setObjectName(QStringLiteral("actionNew"));
        actionNew->setShortcutContext(Qt::ApplicationShortcut);
        actionOpen = new QAction(ParticleEditorClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen->setShortcutContext(Qt::ApplicationShortcut);
        actionSave = new QAction(ParticleEditorClass);
        actionSave->setObjectName(QStringLiteral("actionSave"));
        actionSave->setShortcutContext(Qt::ApplicationShortcut);
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
        actionEmitter->setShortcutContext(Qt::ApplicationShortcut);
        actionRed = new QAction(ParticleEditorClass);
        actionRed->setObjectName(QStringLiteral("actionRed"));
        actionGreen = new QAction(ParticleEditorClass);
        actionGreen->setObjectName(QStringLiteral("actionGreen"));
        actionYellow = new QAction(ParticleEditorClass);
        actionYellow->setObjectName(QStringLiteral("actionYellow"));
        actionWhite = new QAction(ParticleEditorClass);
        actionWhite->setObjectName(QStringLiteral("actionWhite"));
        actionGrid_2 = new QAction(ParticleEditorClass);
        actionGrid_2->setObjectName(QStringLiteral("actionGrid_2"));
        actionColor_Triangle = new QAction(ParticleEditorClass);
        actionColor_Triangle->setObjectName(QStringLiteral("actionColor_Triangle"));
        actionEndColor_Triangle = new QAction(ParticleEditorClass);
        actionEndColor_Triangle->setObjectName(QStringLiteral("actionEndColor_Triangle"));
        actionHelp = new QAction(ParticleEditorClass);
        actionHelp->setObjectName(QStringLiteral("actionHelp"));
        centralWidget = new QWidget(ParticleEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1008, 124, 517, 803));
        QFont font;
        font.setPointSize(16);
        tabWidget->setFont(font);
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setDocumentMode(false);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tabProperties = new QWidget();
        tabProperties->setObjectName(QStringLiteral("tabProperties"));
        groupBox_3 = new QGroupBox(tabProperties);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(2, 0, 507, 761));
        QFont font1;
        font1.setPointSize(10);
        groupBox_3->setFont(font1);
        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(5, 328, 497, 429));
        groupBox_2 = new QGroupBox(groupBox_5);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(2, 14, 265, 57));
        groupBox_2->setFont(font1);
        templateComboBox = new QComboBox(groupBox_2);
        templateComboBox->setObjectName(QStringLiteral("templateComboBox"));
        templateComboBox->setGeometry(QRect(12, 24, 241, 22));
        stackedWidget = new QStackedWidget(groupBox_5);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(4, 72, 489, 355));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        gridLayoutWidget_3 = new QWidget(page_3);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(6, 4, 479, 347));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        orbitSpeedDial = new QDial(gridLayoutWidget_3);
        orbitSpeedDial->setObjectName(QStringLiteral("orbitSpeedDial"));

        gridLayout_3->addWidget(orbitSpeedDial, 0, 2, 1, 1);

        orbitRadiusDial = new QDial(gridLayoutWidget_3);
        orbitRadiusDial->setObjectName(QStringLiteral("orbitRadiusDial"));
        orbitRadiusDial->setWrapping(true);

        gridLayout_3->addWidget(orbitRadiusDial, 1, 2, 1, 1);

        orbitSpeedSpinBox = new QDoubleSpinBox(gridLayoutWidget_3);
        orbitSpeedSpinBox->setObjectName(QStringLiteral("orbitSpeedSpinBox"));
        orbitSpeedSpinBox->setSingleStep(0.1);

        gridLayout_3->addWidget(orbitSpeedSpinBox, 0, 1, 1, 1);

        orbitRadiusSpinBox = new QDoubleSpinBox(gridLayoutWidget_3);
        orbitRadiusSpinBox->setObjectName(QStringLiteral("orbitRadiusSpinBox"));
        orbitRadiusSpinBox->setSingleStep(0.1);

        gridLayout_3->addWidget(orbitRadiusSpinBox, 1, 1, 1, 1);

        label_15 = new QLabel(gridLayoutWidget_3);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout_3->addWidget(label_15, 1, 0, 1, 1);

        label_14 = new QLabel(gridLayoutWidget_3);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout_3->addWidget(label_14, 0, 0, 1, 1);

        orbitAxisComboBox = new QComboBox(gridLayoutWidget_3);
        orbitAxisComboBox->setObjectName(QStringLiteral("orbitAxisComboBox"));

        gridLayout_3->addWidget(orbitAxisComboBox, 2, 1, 1, 1);

        label_19 = new QLabel(gridLayoutWidget_3);
        label_19->setObjectName(QStringLiteral("label_19"));

        gridLayout_3->addWidget(label_19, 2, 0, 1, 1);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName(QStringLiteral("page_4"));
        stackedWidget->addWidget(page_4);
        page_5 = new QWidget();
        page_5->setObjectName(QStringLiteral("page_5"));
        stackedWidget->addWidget(page_5);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        stackedWidget->addWidget(page_2);
        gridLayoutWidget = new QWidget(groupBox_3);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(2, 16, 499, 307));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 9, 1, 1, 1);

        label_9 = new QLabel(gridLayoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 13, 1, 1, 1);

        posSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxY->setObjectName(QStringLiteral("posSpinBoxY"));
        posSpinBoxY->setMinimum(-1024);
        posSpinBoxY->setMaximum(1024);
        posSpinBoxY->setSingleStep(0.1);

        gridLayout->addWidget(posSpinBoxY, 0, 5, 1, 1);

        posSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxX->setObjectName(QStringLiteral("posSpinBoxX"));
        posSpinBoxX->setMinimum(-1024);
        posSpinBoxX->setMaximum(1024);
        posSpinBoxX->setSingleStep(0.1);

        gridLayout->addWidget(posSpinBoxX, 0, 4, 1, 1);

        speedMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedMinSpinBox->setObjectName(QStringLiteral("speedMinSpinBox"));
        speedMinSpinBox->setMaximum(1024);
        speedMinSpinBox->setSingleStep(0.1);
        speedMinSpinBox->setValue(1);

        gridLayout->addWidget(speedMinSpinBox, 9, 4, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 5, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        posSpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxZ->setObjectName(QStringLiteral("posSpinBoxZ"));
        posSpinBoxZ->setMinimum(-1024);
        posSpinBoxZ->setMaximum(1024);
        posSpinBoxZ->setSingleStep(0.1);

        gridLayout->addWidget(posSpinBoxZ, 0, 6, 1, 1);

        spawnTimeSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        spawnTimeSpinBox->setObjectName(QStringLiteral("spawnTimeSpinBox"));
        spawnTimeSpinBox->setMaximum(1024);
        spawnTimeSpinBox->setSingleStep(0.01);
        spawnTimeSpinBox->setValue(0.1);

        gridLayout->addWidget(spawnTimeSpinBox, 13, 8, 1, 1);

        label_10 = new QLabel(gridLayoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 13, 5, 1, 1);

        label_17 = new QLabel(gridLayoutWidget);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout->addWidget(label_17, 13, 7, 1, 1);

        label_11 = new QLabel(gridLayoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 9, 5, 1, 1);

        gravitySpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxY->setObjectName(QStringLiteral("gravitySpinBoxY"));
        gravitySpinBoxY->setMinimum(-1024);
        gravitySpinBoxY->setMaximum(1024);
        gravitySpinBoxY->setSingleStep(0.1);

        gridLayout->addWidget(gravitySpinBoxY, 1, 5, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 2, 1, 1, 1);

        gravitySpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxZ->setObjectName(QStringLiteral("gravitySpinBoxZ"));
        gravitySpinBoxZ->setMinimum(-1024);
        gravitySpinBoxZ->setMaximum(1024);
        gravitySpinBoxZ->setSingleStep(0.1);

        gridLayout->addWidget(gravitySpinBoxZ, 1, 6, 1, 1);

        gravitySpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxX->setObjectName(QStringLiteral("gravitySpinBoxX"));
        gravitySpinBoxX->setMinimum(-1024);
        gravitySpinBoxX->setMaximum(1024);
        gravitySpinBoxX->setSingleStep(0.1);

        gridLayout->addWidget(gravitySpinBoxX, 1, 4, 1, 1);

        directionSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxX->setObjectName(QStringLiteral("directionSpinBoxX"));
        directionSpinBoxX->setMinimum(-1024);
        directionSpinBoxX->setMaximum(1024);
        directionSpinBoxX->setValue(0);

        gridLayout->addWidget(directionSpinBoxX, 2, 4, 1, 1);

        directionSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxY->setObjectName(QStringLiteral("directionSpinBoxY"));
        directionSpinBoxY->setMinimum(-1024);
        directionSpinBoxY->setMaximum(1024);

        gridLayout->addWidget(directionSpinBoxY, 2, 5, 1, 1);

        directionSpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxZ->setObjectName(QStringLiteral("directionSpinBoxZ"));
        directionSpinBoxZ->setMinimum(-1024);
        directionSpinBoxZ->setMaximum(1024);

        gridLayout->addWidget(directionSpinBoxZ, 2, 6, 1, 1);

        label_12 = new QLabel(gridLayoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 5, 5, 1, 1);

        sizeMaxSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        sizeMaxSpinBoxX->setObjectName(QStringLiteral("sizeMaxSpinBoxX"));
        sizeMaxSpinBoxX->setMaximum(1024);
        sizeMaxSpinBoxX->setSingleStep(0.01);
        sizeMaxSpinBoxX->setValue(1);

        gridLayout->addWidget(sizeMaxSpinBoxX, 5, 6, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 5, 7, 1, 1);

        sizeEndSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        sizeEndSpinBoxX->setObjectName(QStringLiteral("sizeEndSpinBoxX"));
        sizeEndSpinBoxX->setMaximum(1024);
        sizeEndSpinBoxX->setSingleStep(0.01);
        sizeEndSpinBoxX->setValue(1);

        gridLayout->addWidget(sizeEndSpinBoxX, 5, 8, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        label_13 = new QLabel(gridLayoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout->addWidget(label_13, 3, 1, 1, 1);

        spreadSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        spreadSpinBox->setObjectName(QStringLiteral("spreadSpinBox"));
        spreadSpinBox->setMinimum(-1024);
        spreadSpinBox->setMaximum(1024);
        spreadSpinBox->setSingleStep(0.1);
        spreadSpinBox->setValue(10);

        gridLayout->addWidget(spreadSpinBox, 3, 4, 1, 1);

        sizeMinSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        sizeMinSpinBoxX->setObjectName(QStringLiteral("sizeMinSpinBoxX"));
        sizeMinSpinBoxX->setMinimum(0);
        sizeMinSpinBoxX->setMaximum(1024);
        sizeMinSpinBoxX->setSingleStep(0.01);
        sizeMinSpinBoxX->setValue(1);

        gridLayout->addWidget(sizeMinSpinBoxX, 5, 4, 1, 1);

        lifeTimeMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        lifeTimeMinSpinBox->setObjectName(QStringLiteral("lifeTimeMinSpinBox"));
        lifeTimeMinSpinBox->setMaximum(1024);
        lifeTimeMinSpinBox->setValue(0.1);

        gridLayout->addWidget(lifeTimeMinSpinBox, 13, 4, 1, 1);

        speedMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedMaxSpinBox->setObjectName(QStringLiteral("speedMaxSpinBox"));
        speedMaxSpinBox->setMaximum(1024);
        speedMaxSpinBox->setSingleStep(0.1);
        speedMaxSpinBox->setValue(1);

        gridLayout->addWidget(speedMaxSpinBox, 9, 6, 1, 1);

        lifeTimeMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        lifeTimeMaxSpinBox->setObjectName(QStringLiteral("lifeTimeMaxSpinBox"));
        lifeTimeMaxSpinBox->setMaximum(1024);
        lifeTimeMaxSpinBox->setValue(0.1);

        gridLayout->addWidget(lifeTimeMaxSpinBox, 13, 6, 1, 1);

        focusButton = new QPushButton(gridLayoutWidget);
        focusButton->setObjectName(QStringLiteral("focusButton"));

        gridLayout->addWidget(focusButton, 0, 7, 1, 1);

        tabWidget->addTab(tabProperties, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayoutWidget_2 = new QWidget(tab_2);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(7, 5, 501, 37));
        gridLayout_2 = new QGridLayout(gridLayoutWidget_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        label_16 = new QLabel(gridLayoutWidget_2);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setFont(font1);

        gridLayout_2->addWidget(label_16, 0, 0, 1, 1);

        textureEmitterLineEdit = new QLineEdit(gridLayoutWidget_2);
        textureEmitterLineEdit->setObjectName(QStringLiteral("textureEmitterLineEdit"));
        textureEmitterLineEdit->setFont(font1);
        textureEmitterLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(textureEmitterLineEdit, 0, 2, 1, 1);

        textureTreeView = new QTreeView(tab_2);
        textureTreeView->setObjectName(QStringLiteral("textureTreeView"));
        textureTreeView->setGeometry(QRect(8, 52, 497, 597));
        tabWidget->addTab(tab_2, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1248, 0, 277, 119));
        groupBox->setFont(font1);
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(12, 16, 253, 97));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(2, 28, 999, 899));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(1004, 2, 239, 117));
        formLayoutWidget = new QWidget(groupBox_6);
        formLayoutWidget->setObjectName(QStringLiteral("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(6, 14, 227, 99));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        newEmitterButton = new QPushButton(formLayoutWidget);
        newEmitterButton->setObjectName(QStringLiteral("newEmitterButton"));
        newEmitterButton->setEnabled(false);

        formLayout->setWidget(0, QFormLayout::LabelRole, newEmitterButton);

        renameEmitterButton = new QPushButton(formLayoutWidget);
        renameEmitterButton->setObjectName(QStringLiteral("renameEmitterButton"));
        renameEmitterButton->setEnabled(false);

        formLayout->setWidget(1, QFormLayout::LabelRole, renameEmitterButton);

        deleteEmitterButton = new QPushButton(formLayoutWidget);
        deleteEmitterButton->setObjectName(QStringLiteral("deleteEmitterButton"));
        deleteEmitterButton->setEnabled(false);

        formLayout->setWidget(2, QFormLayout::LabelRole, deleteEmitterButton);

        nameEmitterLineEdit = new QLineEdit(formLayoutWidget);
        nameEmitterLineEdit->setObjectName(QStringLiteral("nameEmitterLineEdit"));

        formLayout->setWidget(0, QFormLayout::FieldRole, nameEmitterLineEdit);

        aboutWidget = new QDockWidget(centralWidget);
        aboutWidget->setObjectName(QStringLiteral("aboutWidget"));
        aboutWidget->setGeometry(QRect(268, -2, 301, 115));
        aboutWidget->setFloating(true);
        aboutWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
        aboutWidget->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QStringLiteral("dockWidgetContents_2"));
        textBrowser = new QTextBrowser(dockWidgetContents_2);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(2, 0, 301, 131));
        aboutWidget->setWidget(dockWidgetContents_2);
        helpWidget = new QDockWidget(centralWidget);
        helpWidget->setObjectName(QStringLiteral("helpWidget"));
        helpWidget->setGeometry(QRect(574, 0, 417, 221));
        helpWidget->setFloating(true);
        helpWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
        helpWidget->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        textBrowser_2 = new QTextBrowser(dockWidgetContents_6);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));
        textBrowser_2->setGeometry(QRect(2, 0, 417, 235));
        helpWidget->setWidget(dockWidgetContents_6);
        ParticleEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ParticleEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1731, 21));
        QPalette palette1;
        QBrush brush4(QColor(51, 153, 255, 255));
        brush4.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::Highlight, brush4);
        palette1.setBrush(QPalette::Active, QPalette::HighlightedText, brush2);
        QBrush brush5(QColor(246, 246, 246, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::AlternateBase, brush5);
        palette1.setBrush(QPalette::Inactive, QPalette::Highlight, brush4);
        palette1.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush5);
        palette1.setBrush(QPalette::Disabled, QPalette::Highlight, brush4);
        palette1.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush5);
        menuBar->setPalette(palette1);
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        QBrush brush6(QColor(120, 120, 120, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        menuFile->setPalette(palette2);
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QStringLiteral("menuView"));
        ParticleEditorClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(ParticleEditorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setSizeGripEnabled(false);
        ParticleEditorClass->setStatusBar(statusBar);
        dockWidget = new QDockWidget(ParticleEditorClass);
        dockWidget->setObjectName(QStringLiteral("dockWidget"));
        dockWidget->setMinimumSize(QSize(200, 150));
        dockWidget->setMaximumSize(QSize(200, 150));
        dockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        dockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QStringLiteral("dockWidgetContents"));
        FPSlcdNumber = new QLCDNumber(dockWidgetContents);
        FPSlcdNumber->setObjectName(QStringLiteral("FPSlcdNumber"));
        FPSlcdNumber->setGeometry(QRect(56, 2, 139, 37));
        label_4 = new QLabel(dockWidgetContents);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 0, 59, 39));
        QFont font2;
        font2.setPointSize(15);
        label_4->setFont(font2);
        lookAtSpinBoxY = new QDoubleSpinBox(dockWidgetContents);
        lookAtSpinBoxY->setObjectName(QStringLiteral("lookAtSpinBoxY"));
        lookAtSpinBoxY->setGeometry(QRect(102, 58, 47, 22));
        lookAtSpinBoxY->setReadOnly(true);
        lookAtSpinBoxY->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lookAtSpinBoxY->setDecimals(3);
        lookAtSpinBoxY->setMinimum(-1024);
        lookAtSpinBoxY->setMaximum(1024);
        lookAtSpinBoxX = new QDoubleSpinBox(dockWidgetContents);
        lookAtSpinBoxX->setObjectName(QStringLiteral("lookAtSpinBoxX"));
        lookAtSpinBoxX->setGeometry(QRect(56, 58, 47, 22));
        lookAtSpinBoxX->setWrapping(false);
        lookAtSpinBoxX->setFrame(true);
        lookAtSpinBoxX->setReadOnly(true);
        lookAtSpinBoxX->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lookAtSpinBoxX->setKeyboardTracking(true);
        lookAtSpinBoxX->setDecimals(3);
        lookAtSpinBoxX->setMinimum(-1024);
        lookAtSpinBoxX->setValue(0);
        lookAtSpinBoxZ = new QDoubleSpinBox(dockWidgetContents);
        lookAtSpinBoxZ->setObjectName(QStringLiteral("lookAtSpinBoxZ"));
        lookAtSpinBoxZ->setGeometry(QRect(148, 58, 47, 22));
        lookAtSpinBoxZ->setReadOnly(true);
        lookAtSpinBoxZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lookAtSpinBoxZ->setDecimals(3);
        lookAtSpinBoxZ->setMinimum(-1024);
        lookAtSpinBoxZ->setMaximum(1024);
        label_18 = new QLabel(dockWidgetContents);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(4, 52, 63, 31));
        QFont font3;
        font3.setPointSize(11);
        label_18->setFont(font3);
        dockWidget->setWidget(dockWidgetContents);
        ParticleEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), dockWidget);
        colorDockWidget = new QDockWidget(ParticleEditorClass);
        colorDockWidget->setObjectName(QStringLiteral("colorDockWidget"));
        colorDockWidget->setMinimumSize(QSize(200, 280));
        colorDockWidget->setMaximumSize(QSize(200, 280));
        colorDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        colorDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_3 = new QWidget();
        dockWidgetContents_3->setObjectName(QStringLiteral("dockWidgetContents_3"));
        colorAlphaSlider = new QSlider(dockWidgetContents_3);
        colorAlphaSlider->setObjectName(QStringLiteral("colorAlphaSlider"));
        colorAlphaSlider->setGeometry(QRect(8, 232, 185, 22));
        colorAlphaSlider->setMaximum(255);
        colorAlphaSlider->setTracking(true);
        colorAlphaSlider->setOrientation(Qt::Horizontal);
        colorAlphaSlider->setInvertedAppearance(false);
        colorAlphaSlider->setInvertedControls(false);
        colorAlphaSlider->setTickPosition(QSlider::NoTicks);
        colorSpinBoxR = new QDoubleSpinBox(dockWidgetContents_3);
        colorSpinBoxR->setObjectName(QStringLiteral("colorSpinBoxR"));
        colorSpinBoxR->setGeometry(QRect(8, 208, 47, 22));
        colorSpinBoxR->setWrapping(false);
        colorSpinBoxR->setFrame(true);
        colorSpinBoxR->setReadOnly(true);
        colorSpinBoxR->setButtonSymbols(QAbstractSpinBox::NoButtons);
        colorSpinBoxR->setKeyboardTracking(true);
        colorSpinBoxR->setDecimals(3);
        colorSpinBoxR->setMinimum(0);
        colorSpinBoxR->setMaximum(1);
        colorSpinBoxR->setValue(0);
        colorSpinBoxG = new QDoubleSpinBox(dockWidgetContents_3);
        colorSpinBoxG->setObjectName(QStringLiteral("colorSpinBoxG"));
        colorSpinBoxG->setGeometry(QRect(54, 208, 47, 22));
        colorSpinBoxG->setReadOnly(true);
        colorSpinBoxG->setButtonSymbols(QAbstractSpinBox::NoButtons);
        colorSpinBoxG->setDecimals(3);
        colorSpinBoxG->setMinimum(0);
        colorSpinBoxG->setMaximum(1);
        colorSpinBoxB = new QDoubleSpinBox(dockWidgetContents_3);
        colorSpinBoxB->setObjectName(QStringLiteral("colorSpinBoxB"));
        colorSpinBoxB->setGeometry(QRect(100, 208, 47, 22));
        colorSpinBoxB->setReadOnly(true);
        colorSpinBoxB->setButtonSymbols(QAbstractSpinBox::NoButtons);
        colorSpinBoxB->setDecimals(3);
        colorSpinBoxB->setMinimum(0);
        colorSpinBoxB->setMaximum(1);
        colorSpinBoxA = new QDoubleSpinBox(dockWidgetContents_3);
        colorSpinBoxA->setObjectName(QStringLiteral("colorSpinBoxA"));
        colorSpinBoxA->setGeometry(QRect(146, 208, 47, 22));
        colorSpinBoxA->setReadOnly(true);
        colorSpinBoxA->setButtonSymbols(QAbstractSpinBox::NoButtons);
        colorSpinBoxA->setDecimals(3);
        colorSpinBoxA->setMinimum(0);
        colorSpinBoxA->setMaximum(1);
        colorDockWidget->setWidget(dockWidgetContents_3);
        ParticleEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), colorDockWidget);
        endcolorDockWidget = new QDockWidget(ParticleEditorClass);
        endcolorDockWidget->setObjectName(QStringLiteral("endcolorDockWidget"));
        endcolorDockWidget->setMinimumSize(QSize(200, 280));
        endcolorDockWidget->setMaximumSize(QSize(200, 280));
        endcolorDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        endcolorDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_4 = new QWidget();
        dockWidgetContents_4->setObjectName(QStringLiteral("dockWidgetContents_4"));
        endcolorAlphaSlider = new QSlider(dockWidgetContents_4);
        endcolorAlphaSlider->setObjectName(QStringLiteral("endcolorAlphaSlider"));
        endcolorAlphaSlider->setGeometry(QRect(6, 232, 185, 22));
        endcolorAlphaSlider->setMaximum(255);
        endcolorAlphaSlider->setTracking(true);
        endcolorAlphaSlider->setOrientation(Qt::Horizontal);
        endcolorAlphaSlider->setInvertedAppearance(false);
        endcolorAlphaSlider->setInvertedControls(false);
        endcolorAlphaSlider->setTickPosition(QSlider::NoTicks);
        endcolorSpinBoxG = new QDoubleSpinBox(dockWidgetContents_4);
        endcolorSpinBoxG->setObjectName(QStringLiteral("endcolorSpinBoxG"));
        endcolorSpinBoxG->setGeometry(QRect(54, 208, 47, 22));
        endcolorSpinBoxG->setReadOnly(true);
        endcolorSpinBoxG->setButtonSymbols(QAbstractSpinBox::NoButtons);
        endcolorSpinBoxG->setDecimals(3);
        endcolorSpinBoxG->setMinimum(0);
        endcolorSpinBoxG->setMaximum(1);
        endcolorSpinBoxB = new QDoubleSpinBox(dockWidgetContents_4);
        endcolorSpinBoxB->setObjectName(QStringLiteral("endcolorSpinBoxB"));
        endcolorSpinBoxB->setGeometry(QRect(100, 208, 47, 22));
        endcolorSpinBoxB->setReadOnly(true);
        endcolorSpinBoxB->setButtonSymbols(QAbstractSpinBox::NoButtons);
        endcolorSpinBoxB->setDecimals(3);
        endcolorSpinBoxB->setMinimum(0);
        endcolorSpinBoxB->setMaximum(1);
        endcolorSpinBoxR = new QDoubleSpinBox(dockWidgetContents_4);
        endcolorSpinBoxR->setObjectName(QStringLiteral("endcolorSpinBoxR"));
        endcolorSpinBoxR->setGeometry(QRect(8, 208, 47, 22));
        endcolorSpinBoxR->setWrapping(false);
        endcolorSpinBoxR->setFrame(true);
        endcolorSpinBoxR->setReadOnly(true);
        endcolorSpinBoxR->setButtonSymbols(QAbstractSpinBox::NoButtons);
        endcolorSpinBoxR->setKeyboardTracking(true);
        endcolorSpinBoxR->setDecimals(3);
        endcolorSpinBoxR->setMinimum(0);
        endcolorSpinBoxR->setMaximum(1);
        endcolorSpinBoxR->setValue(0);
        endcolorSpinBoxA = new QDoubleSpinBox(dockWidgetContents_4);
        endcolorSpinBoxA->setObjectName(QStringLiteral("endcolorSpinBoxA"));
        endcolorSpinBoxA->setGeometry(QRect(146, 208, 47, 22));
        endcolorSpinBoxA->setReadOnly(true);
        endcolorSpinBoxA->setButtonSymbols(QAbstractSpinBox::NoButtons);
        endcolorSpinBoxA->setDecimals(3);
        endcolorSpinBoxA->setMinimum(0);
        endcolorSpinBoxA->setMaximum(1);
        endcolorDockWidget->setWidget(dockWidgetContents_4);
        ParticleEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), endcolorDockWidget);
        gridDockWidget = new QDockWidget(ParticleEditorClass);
        gridDockWidget->setObjectName(QStringLiteral("gridDockWidget"));
        gridDockWidget->setMinimumSize(QSize(200, 38));
        gridDockWidget->setMaximumSize(QSize(200, 100));
        gridDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        gridDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        gridSpaceSpinBox = new QDoubleSpinBox(dockWidgetContents_5);
        gridSpaceSpinBox->setObjectName(QStringLiteral("gridSpaceSpinBox"));
        gridSpaceSpinBox->setGeometry(QRect(93, 24, 77, 22));
        gridSpaceSpinBox->setSingleStep(0.25);
        gridSpaceSpinBox->setValue(0.5);
        label_5 = new QLabel(dockWidgetContents_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(18, 26, 59, 16));
        gridDockWidget->setWidget(dockWidgetContents_5);
        ParticleEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), gridDockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionHelp);
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionStats);
        menuView->addAction(actionGrid_2);
        menuView->addAction(actionColor_Triangle);
        menuView->addAction(actionEndColor_Triangle);

        retranslateUi(ParticleEditorClass);
        QObject::connect(templateComboBox, SIGNAL(currentIndexChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        tabWidget->setCurrentIndex(0);
        stackedWidget->setCurrentIndex(0);
        orbitAxisComboBox->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(ParticleEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ParticleEditorClass)
    {
        ParticleEditorClass->setWindowTitle(QApplication::translate("ParticleEditorClass", "ParticleEditor", 0));
        actionNew->setText(QApplication::translate("ParticleEditorClass", "New", 0));
        actionNew->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+N", 0));
        actionOpen->setText(QApplication::translate("ParticleEditorClass", "Open...", 0));
        actionOpen->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+O", 0));
        actionSave->setText(QApplication::translate("ParticleEditorClass", "Save", 0));
        actionSave->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+S", 0));
        actionSave_As->setText(QApplication::translate("ParticleEditorClass", "Save As...", 0));
        actionExit->setText(QApplication::translate("ParticleEditorClass", "Exit", 0));
        actionAbout->setText(QApplication::translate("ParticleEditorClass", "About", 0));
        actionStats->setText(QApplication::translate("ParticleEditorClass", "Stats", 0));
        actionEmitter->setText(QApplication::translate("ParticleEditorClass", "Emitter", 0));
        actionEmitter->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+E", 0));
        actionRed->setText(QApplication::translate("ParticleEditorClass", "Red", 0));
        actionGreen->setText(QApplication::translate("ParticleEditorClass", "Green", 0));
        actionYellow->setText(QApplication::translate("ParticleEditorClass", "Yellow", 0));
        actionWhite->setText(QApplication::translate("ParticleEditorClass", "White", 0));
        actionGrid_2->setText(QApplication::translate("ParticleEditorClass", "Grid", 0));
        actionColor_Triangle->setText(QApplication::translate("ParticleEditorClass", "Color Triangle", 0));
        actionEndColor_Triangle->setText(QApplication::translate("ParticleEditorClass", "EndColor Triangle", 0));
        actionHelp->setText(QApplication::translate("ParticleEditorClass", "Help", 0));
        groupBox_3->setTitle(QApplication::translate("ParticleEditorClass", "Properties", 0));
        groupBox_5->setTitle(QApplication::translate("ParticleEditorClass", "Template properties", 0));
        groupBox_2->setTitle(QApplication::translate("ParticleEditorClass", "Template", 0));
        templateComboBox->clear();
        templateComboBox->insertItems(0, QStringList()
         << QApplication::translate("ParticleEditorClass", "Normal", 0)
         << QApplication::translate("ParticleEditorClass", "Spiral", 0)
         << QApplication::translate("ParticleEditorClass", "Vortex", 0)
         << QApplication::translate("ParticleEditorClass", "Pulsing", 0)
         << QApplication::translate("ParticleEditorClass", "Explosion", 0)
        );
        label_15->setText(QApplication::translate("ParticleEditorClass", "Orbit radius", 0));
        label_14->setText(QApplication::translate("ParticleEditorClass", "Orbit speed", 0));
        orbitAxisComboBox->clear();
        orbitAxisComboBox->insertItems(0, QStringList()
         << QApplication::translate("ParticleEditorClass", "X", 0)
         << QApplication::translate("ParticleEditorClass", "Y", 0)
         << QApplication::translate("ParticleEditorClass", "Z", 0)
        );
        label_19->setText(QApplication::translate("ParticleEditorClass", "Orbit axis", 0));
        label_7->setText(QApplication::translate("ParticleEditorClass", "SpeedMin:", 0));
        label_9->setText(QApplication::translate("ParticleEditorClass", "LifeMin:", 0));
        label_2->setText(QApplication::translate("ParticleEditorClass", "SizeMin:", 0));
        label->setText(QApplication::translate("ParticleEditorClass", "Pos:", 0));
        label_10->setText(QApplication::translate("ParticleEditorClass", "LifeMax:", 0));
        label_17->setText(QApplication::translate("ParticleEditorClass", "SpawnTime:", 0));
        label_11->setText(QApplication::translate("ParticleEditorClass", "SpeedMax:", 0));
        label_8->setText(QApplication::translate("ParticleEditorClass", "Direction:", 0));
        label_12->setText(QApplication::translate("ParticleEditorClass", "SizeMax:", 0));
        label_3->setText(QApplication::translate("ParticleEditorClass", "SizeEnd:", 0));
        label_6->setText(QApplication::translate("ParticleEditorClass", "Gravity:", 0));
        label_13->setText(QApplication::translate("ParticleEditorClass", "Spread:", 0));
        focusButton->setText(QApplication::translate("ParticleEditorClass", "Look At", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabProperties), QApplication::translate("ParticleEditorClass", "Properties", 0));
        label_16->setText(QApplication::translate("ParticleEditorClass", "Texture:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ParticleEditorClass", "Resources", 0));
        groupBox->setTitle(QApplication::translate("ParticleEditorClass", "Emitters", 0));
        groupBox_6->setTitle(QApplication::translate("ParticleEditorClass", "Manage emitters", 0));
        newEmitterButton->setText(QApplication::translate("ParticleEditorClass", "New", 0));
        newEmitterButton->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+N", 0));
        renameEmitterButton->setText(QApplication::translate("ParticleEditorClass", "Rename", 0));
        deleteEmitterButton->setText(QApplication::translate("ParticleEditorClass", "Delete", 0));
        nameEmitterLineEdit->setPlaceholderText(QApplication::translate("ParticleEditorClass", "EmitterName", 0));
        aboutWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "About", 0));
        textBrowser->setHtml(QApplication::translate("ParticleEditorClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600;\">Root Engine Particle Editor</span></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Version 0.99</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p align=\"center\" style=\" margin-top:0px; margin-botto"
                        "m:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Created by Group 6 @ BTH 2014</span></p></body></html>", 0));
        helpWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Help", 0));
        textBrowser_2->setHtml(QApplication::translate("ParticleEditorClass", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt; font-weight:600;\">How-to</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">Shortcuts:</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600; font-style:italic;\">QT</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; ma"
                        "rgin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Ctrl+N 	New project</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Ctrl+O  	Open project</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Ctrl+S  	Save project</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600; font-style:italic;\">Viewer window</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Alt+LMB  	Rotation around look-at point</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-"
                        "indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Alt+MMB  	Move look-at point(panning)</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Alt+RMB  	Zoom</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Scroll wheel 	Zoom</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p></body></html>", 0));
        menuFile->setTitle(QApplication::translate("ParticleEditorClass", "File", 0));
        menuHelp->setTitle(QApplication::translate("ParticleEditorClass", "Help", 0));
        menuView->setTitle(QApplication::translate("ParticleEditorClass", "View", 0));
        dockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Status", 0));
        label_4->setText(QApplication::translate("ParticleEditorClass", "FPS:", 0));
        label_18->setText(QApplication::translate("ParticleEditorClass", "LookAt:", 0));
        colorDockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Start Color", 0));
        endcolorDockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "End Color", 0));
#ifndef QT_NO_ACCESSIBILITY
        gridDockWidget->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        gridDockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Grid Settings", 0));
        label_5->setText(QApplication::translate("ParticleEditorClass", "Grid space:", 0));
    } // retranslateUi

};

namespace Ui {
    class ParticleEditorClass: public Ui_ParticleEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARTICLEEDITOR_H
