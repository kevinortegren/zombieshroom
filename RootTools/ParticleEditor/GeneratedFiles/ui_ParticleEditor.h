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
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
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
    QAction *actionNew_Emitter;
    QAction *actionRename_Emitter;
    QAction *actionDelete_Emitter;
    QAction *actionDuplicate_Emitter;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tabProperties;
    QGroupBox *groupBox_3;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QDoubleSpinBox *orbitRadiusSpinBox;
    QLabel *label_17;
    QDoubleSpinBox *spawnTimeSpinBox;
    QLabel *label_15;
    QLabel *label_7;
    QDoubleSpinBox *posSpinBoxX;
    QDoubleSpinBox *posSpinBoxY;
    QLabel *label;
    QDoubleSpinBox *posSpinBoxZ;
    QLabel *label_9;
    QLabel *label_2;
    QDoubleSpinBox *speedMinSpinBox;
    QLabel *label_10;
    QLabel *label_11;
    QDoubleSpinBox *directionSpinBoxY;
    QDoubleSpinBox *gravitySpinBoxZ;
    QLabel *label_8;
    QDoubleSpinBox *gravitySpinBoxY;
    QDoubleSpinBox *gravitySpinBoxX;
    QDoubleSpinBox *directionSpinBoxX;
    QLabel *label_12;
    QDoubleSpinBox *sizeMaxSpinBoxX;
    QDoubleSpinBox *sizeMinSpinBoxX;
    QLabel *label_13;
    QLabel *label_6;
    QDoubleSpinBox *directionSpinBoxZ;
    QDoubleSpinBox *spreadSpinBox;
    QDoubleSpinBox *lifeTimeMaxSpinBox;
    QDoubleSpinBox *lifeTimeMinSpinBox;
    QDoubleSpinBox *speedMaxSpinBox;
    QLabel *label_22;
    QDoubleSpinBox *RotspeedmaxSpinbox;
    QDoubleSpinBox *RotspeedminSpinbox;
    QLabel *label_21;
    QLabel *label_14;
    QDoubleSpinBox *orbitSpeedSpinBox;
    QLabel *label_23;
    QComboBox *blendingComboBox;
    QLabel *label_20;
    QLabel *label_3;
    QDoubleSpinBox *sizeEndSpinBoxX;
    QSlider *spreadSlider;
    QComboBox *relativeComboBox;
    QLabel *label_4;
    QDoubleSpinBox *loopNumberSpinBox;
    QWidget *tab_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QLabel *label_16;
    QLineEdit *textureEmitterLineEdit;
    QTreeView *textureTreeView;
    QWidget *tab;
    QTreeView *modelTreeView;
    QTreeView *modeltexTreeView;
    QPushButton *removeObjectButton;
    QWidget *gridLayoutWidget_3;
    QGridLayout *gridLayout_3;
    QLabel *label_24;
    QLineEdit *modelLineEdit;
    QWidget *gridLayoutWidget_4;
    QGridLayout *gridLayout_4;
    QLabel *label_25;
    QLineEdit *textureModelLineEdit;
    QGroupBox *groupBox;
    QListWidget *listWidget;
    QFrame *frame;
    QDockWidget *newEmitterWidget;
    QWidget *dockWidgetContents_7;
    QLineEdit *nameEmitterLineEdit;
    QPushButton *newEmitterButton;
    QDockWidget *renameWidget;
    QWidget *dockWidgetContents_8;
    QLineEdit *renameEmitterLineEdit;
    QPushButton *renameEmitterButton;
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
    QMenu *menuEmitter;
    QStatusBar *statusBar;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents;
    QDoubleSpinBox *lookAtSpinBoxY;
    QDoubleSpinBox *lookAtSpinBoxX;
    QDoubleSpinBox *lookAtSpinBoxZ;
    QLabel *label_18;
    QLabel *label_19;
    QDoubleSpinBox *particleCountSpinBox;
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
    QPushButton *restartParticleSystemButton;

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
        actionNew_Emitter = new QAction(ParticleEditorClass);
        actionNew_Emitter->setObjectName(QStringLiteral("actionNew_Emitter"));
        actionNew_Emitter->setEnabled(false);
        actionRename_Emitter = new QAction(ParticleEditorClass);
        actionRename_Emitter->setObjectName(QStringLiteral("actionRename_Emitter"));
        actionRename_Emitter->setEnabled(false);
        actionDelete_Emitter = new QAction(ParticleEditorClass);
        actionDelete_Emitter->setObjectName(QStringLiteral("actionDelete_Emitter"));
        actionDelete_Emitter->setEnabled(false);
        actionDuplicate_Emitter = new QAction(ParticleEditorClass);
        actionDuplicate_Emitter->setObjectName(QStringLiteral("actionDuplicate_Emitter"));
        actionDuplicate_Emitter->setEnabled(false);
        centralWidget = new QWidget(ParticleEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1182, 134, 343, 793));
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
        groupBox_3->setGeometry(QRect(0, 0, 335, 757));
        QFont font1;
        font1.setPointSize(10);
        groupBox_3->setFont(font1);
        gridLayoutWidget = new QWidget(groupBox_3);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(4, 16, 330, 737));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        orbitRadiusSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        orbitRadiusSpinBox->setObjectName(QStringLiteral("orbitRadiusSpinBox"));
        orbitRadiusSpinBox->setMinimum(-100);
        orbitRadiusSpinBox->setMaximum(100);
        orbitRadiusSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(orbitRadiusSpinBox, 17, 4, 1, 1);

        label_17 = new QLabel(gridLayoutWidget);
        label_17->setObjectName(QStringLiteral("label_17"));

        gridLayout->addWidget(label_17, 18, 1, 1, 1);

        spawnTimeSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        spawnTimeSpinBox->setObjectName(QStringLiteral("spawnTimeSpinBox"));
        spawnTimeSpinBox->setDecimals(3);
        spawnTimeSpinBox->setMaximum(1024);
        spawnTimeSpinBox->setSingleStep(0.005);
        spawnTimeSpinBox->setValue(0.1);

        gridLayout->addWidget(spawnTimeSpinBox, 18, 4, 1, 1);

        label_15 = new QLabel(gridLayoutWidget);
        label_15->setObjectName(QStringLiteral("label_15"));

        gridLayout->addWidget(label_15, 17, 1, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 10, 1, 1, 1);

        posSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxX->setObjectName(QStringLiteral("posSpinBoxX"));
        posSpinBoxX->setMinimum(-1024);
        posSpinBoxX->setMaximum(1024);
        posSpinBoxX->setSingleStep(0.1);

        gridLayout->addWidget(posSpinBoxX, 0, 4, 1, 1);

        posSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxY->setObjectName(QStringLiteral("posSpinBoxY"));
        posSpinBoxY->setMinimum(-1024);
        posSpinBoxY->setMaximum(1024);
        posSpinBoxY->setSingleStep(0.1);

        gridLayout->addWidget(posSpinBoxY, 0, 5, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        posSpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxZ->setObjectName(QStringLiteral("posSpinBoxZ"));
        posSpinBoxZ->setMinimum(-1024);
        posSpinBoxZ->setMaximum(1024);
        posSpinBoxZ->setSingleStep(0.1);

        gridLayout->addWidget(posSpinBoxZ, 0, 6, 1, 1);

        label_9 = new QLabel(gridLayoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 14, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 5, 1, 1, 1);

        speedMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedMinSpinBox->setObjectName(QStringLiteral("speedMinSpinBox"));
        speedMinSpinBox->setMaximum(1024);
        speedMinSpinBox->setSingleStep(0.1);
        speedMinSpinBox->setValue(1);

        gridLayout->addWidget(speedMinSpinBox, 10, 4, 1, 1);

        label_10 = new QLabel(gridLayoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 14, 5, 1, 1);

        label_11 = new QLabel(gridLayoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 10, 5, 1, 1);

        directionSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxY->setObjectName(QStringLiteral("directionSpinBoxY"));
        directionSpinBoxY->setMinimum(-1024);
        directionSpinBoxY->setMaximum(1024);

        gridLayout->addWidget(directionSpinBoxY, 2, 5, 1, 1);

        gravitySpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxZ->setObjectName(QStringLiteral("gravitySpinBoxZ"));
        gravitySpinBoxZ->setMinimum(-1024);
        gravitySpinBoxZ->setMaximum(1024);
        gravitySpinBoxZ->setSingleStep(0.1);

        gridLayout->addWidget(gravitySpinBoxZ, 1, 6, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 2, 1, 1, 1);

        gravitySpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxY->setObjectName(QStringLiteral("gravitySpinBoxY"));
        gravitySpinBoxY->setMinimum(-1024);
        gravitySpinBoxY->setMaximum(1024);
        gravitySpinBoxY->setSingleStep(0.1);

        gridLayout->addWidget(gravitySpinBoxY, 1, 5, 1, 1);

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

        label_12 = new QLabel(gridLayoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 5, 5, 1, 1);

        sizeMaxSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        sizeMaxSpinBoxX->setObjectName(QStringLiteral("sizeMaxSpinBoxX"));
        sizeMaxSpinBoxX->setMaximum(1024);
        sizeMaxSpinBoxX->setSingleStep(0.01);
        sizeMaxSpinBoxX->setValue(1);

        gridLayout->addWidget(sizeMaxSpinBoxX, 5, 6, 1, 1);

        sizeMinSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        sizeMinSpinBoxX->setObjectName(QStringLiteral("sizeMinSpinBoxX"));
        sizeMinSpinBoxX->setMinimum(0);
        sizeMinSpinBoxX->setMaximum(1024);
        sizeMinSpinBoxX->setSingleStep(0.01);
        sizeMinSpinBoxX->setValue(1);

        gridLayout->addWidget(sizeMinSpinBoxX, 5, 4, 1, 1);

        label_13 = new QLabel(gridLayoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout->addWidget(label_13, 3, 1, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 1, 1, 1, 1);

        directionSpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxZ->setObjectName(QStringLiteral("directionSpinBoxZ"));
        directionSpinBoxZ->setMinimum(-1024);
        directionSpinBoxZ->setMaximum(1024);

        gridLayout->addWidget(directionSpinBoxZ, 2, 6, 1, 1);

        spreadSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        spreadSpinBox->setObjectName(QStringLiteral("spreadSpinBox"));
        spreadSpinBox->setDecimals(0);
        spreadSpinBox->setMinimum(0);
        spreadSpinBox->setMaximum(360);
        spreadSpinBox->setSingleStep(1);
        spreadSpinBox->setValue(10);

        gridLayout->addWidget(spreadSpinBox, 3, 4, 1, 1);

        lifeTimeMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        lifeTimeMaxSpinBox->setObjectName(QStringLiteral("lifeTimeMaxSpinBox"));
        lifeTimeMaxSpinBox->setMaximum(1024);
        lifeTimeMaxSpinBox->setValue(0.1);

        gridLayout->addWidget(lifeTimeMaxSpinBox, 14, 6, 1, 1);

        lifeTimeMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        lifeTimeMinSpinBox->setObjectName(QStringLiteral("lifeTimeMinSpinBox"));
        lifeTimeMinSpinBox->setMaximum(1024);
        lifeTimeMinSpinBox->setValue(0.1);

        gridLayout->addWidget(lifeTimeMinSpinBox, 14, 4, 1, 1);

        speedMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedMaxSpinBox->setObjectName(QStringLiteral("speedMaxSpinBox"));
        speedMaxSpinBox->setMaximum(1024);
        speedMaxSpinBox->setSingleStep(0.1);
        speedMaxSpinBox->setValue(1);

        gridLayout->addWidget(speedMaxSpinBox, 10, 6, 1, 1);

        label_22 = new QLabel(gridLayoutWidget);
        label_22->setObjectName(QStringLiteral("label_22"));

        gridLayout->addWidget(label_22, 15, 5, 1, 1);

        RotspeedmaxSpinbox = new QDoubleSpinBox(gridLayoutWidget);
        RotspeedmaxSpinbox->setObjectName(QStringLiteral("RotspeedmaxSpinbox"));
        RotspeedmaxSpinbox->setMinimum(-30);
        RotspeedmaxSpinbox->setMaximum(30);
        RotspeedmaxSpinbox->setSingleStep(0.1);

        gridLayout->addWidget(RotspeedmaxSpinbox, 15, 6, 1, 1);

        RotspeedminSpinbox = new QDoubleSpinBox(gridLayoutWidget);
        RotspeedminSpinbox->setObjectName(QStringLiteral("RotspeedminSpinbox"));
        RotspeedminSpinbox->setMinimum(-30);
        RotspeedminSpinbox->setMaximum(30);
        RotspeedminSpinbox->setSingleStep(0.1);

        gridLayout->addWidget(RotspeedminSpinbox, 15, 4, 1, 1);

        label_21 = new QLabel(gridLayoutWidget);
        label_21->setObjectName(QStringLiteral("label_21"));

        gridLayout->addWidget(label_21, 15, 1, 1, 1);

        label_14 = new QLabel(gridLayoutWidget);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout->addWidget(label_14, 16, 1, 1, 1);

        orbitSpeedSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        orbitSpeedSpinBox->setObjectName(QStringLiteral("orbitSpeedSpinBox"));
        orbitSpeedSpinBox->setMinimum(-100);
        orbitSpeedSpinBox->setMaximum(100);
        orbitSpeedSpinBox->setSingleStep(0.1);

        gridLayout->addWidget(orbitSpeedSpinBox, 16, 4, 1, 1);

        label_23 = new QLabel(gridLayoutWidget);
        label_23->setObjectName(QStringLiteral("label_23"));

        gridLayout->addWidget(label_23, 20, 1, 1, 1);

        blendingComboBox = new QComboBox(gridLayoutWidget);
        blendingComboBox->setObjectName(QStringLiteral("blendingComboBox"));

        gridLayout->addWidget(blendingComboBox, 20, 4, 1, 2);

        label_20 = new QLabel(gridLayoutWidget);
        label_20->setObjectName(QStringLiteral("label_20"));

        gridLayout->addWidget(label_20, 19, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 6, 1, 1, 1);

        sizeEndSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        sizeEndSpinBoxX->setObjectName(QStringLiteral("sizeEndSpinBoxX"));
        sizeEndSpinBoxX->setMaximum(1024);
        sizeEndSpinBoxX->setSingleStep(0.01);
        sizeEndSpinBoxX->setValue(1);

        gridLayout->addWidget(sizeEndSpinBoxX, 6, 4, 1, 1);

        spreadSlider = new QSlider(gridLayoutWidget);
        spreadSlider->setObjectName(QStringLiteral("spreadSlider"));
        spreadSlider->setMaximum(360);
        spreadSlider->setOrientation(Qt::Horizontal);
        spreadSlider->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(spreadSlider, 3, 5, 1, 2);

        relativeComboBox = new QComboBox(gridLayoutWidget);
        relativeComboBox->setObjectName(QStringLiteral("relativeComboBox"));

        gridLayout->addWidget(relativeComboBox, 19, 4, 1, 2);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 18, 5, 1, 1);

        loopNumberSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        loopNumberSpinBox->setObjectName(QStringLiteral("loopNumberSpinBox"));
        loopNumberSpinBox->setDecimals(0);
        loopNumberSpinBox->setMaximum(50);
        loopNumberSpinBox->setSingleStep(1);
        loopNumberSpinBox->setValue(1);

        gridLayout->addWidget(loopNumberSpinBox, 18, 6, 1, 1);

        tabWidget->addTab(tabProperties, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayoutWidget_2 = new QWidget(tab_2);
        gridLayoutWidget_2->setObjectName(QStringLiteral("gridLayoutWidget_2"));
        gridLayoutWidget_2->setGeometry(QRect(7, 5, 327, 37));
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
        textureTreeView->setGeometry(QRect(8, 52, 329, 703));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        modelTreeView = new QTreeView(tab);
        modelTreeView->setObjectName(QStringLiteral("modelTreeView"));
        modelTreeView->setGeometry(QRect(6, 68, 331, 351));
        modeltexTreeView = new QTreeView(tab);
        modeltexTreeView->setObjectName(QStringLiteral("modeltexTreeView"));
        modeltexTreeView->setGeometry(QRect(6, 458, 331, 295));
        removeObjectButton = new QPushButton(tab);
        removeObjectButton->setObjectName(QStringLiteral("removeObjectButton"));
        removeObjectButton->setGeometry(QRect(6, 4, 329, 23));
        QFont font2;
        font2.setPointSize(9);
        removeObjectButton->setFont(font2);
        gridLayoutWidget_3 = new QWidget(tab);
        gridLayoutWidget_3->setObjectName(QStringLiteral("gridLayoutWidget_3"));
        gridLayoutWidget_3->setGeometry(QRect(6, 28, 327, 37));
        gridLayout_3 = new QGridLayout(gridLayoutWidget_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setContentsMargins(0, 0, 0, 0);
        label_24 = new QLabel(gridLayoutWidget_3);
        label_24->setObjectName(QStringLiteral("label_24"));
        label_24->setFont(font1);

        gridLayout_3->addWidget(label_24, 0, 0, 1, 1);

        modelLineEdit = new QLineEdit(gridLayoutWidget_3);
        modelLineEdit->setObjectName(QStringLiteral("modelLineEdit"));
        modelLineEdit->setFont(font1);
        modelLineEdit->setReadOnly(true);

        gridLayout_3->addWidget(modelLineEdit, 0, 2, 1, 1);

        gridLayoutWidget_4 = new QWidget(tab);
        gridLayoutWidget_4->setObjectName(QStringLiteral("gridLayoutWidget_4"));
        gridLayoutWidget_4->setGeometry(QRect(6, 420, 327, 37));
        gridLayout_4 = new QGridLayout(gridLayoutWidget_4);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(0, 0, 0, 0);
        label_25 = new QLabel(gridLayoutWidget_4);
        label_25->setObjectName(QStringLiteral("label_25"));
        label_25->setFont(font1);

        gridLayout_4->addWidget(label_25, 0, 0, 1, 1);

        textureModelLineEdit = new QLineEdit(gridLayoutWidget_4);
        textureModelLineEdit->setObjectName(QStringLiteral("textureModelLineEdit"));
        textureModelLineEdit->setFont(font1);
        textureModelLineEdit->setReadOnly(true);

        gridLayout_4->addWidget(textureModelLineEdit, 0, 2, 1, 1);

        tabWidget->addTab(tab, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1184, 0, 341, 131));
        groupBox->setFont(font1);
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(6, 16, 329, 111));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(2, 28, 1173, 899));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        newEmitterWidget = new QDockWidget(frame);
        newEmitterWidget->setObjectName(QStringLiteral("newEmitterWidget"));
        newEmitterWidget->setEnabled(true);
        newEmitterWidget->setGeometry(QRect(192, 144, 275, 121));
        newEmitterWidget->setFloating(true);
        newEmitterWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
        newEmitterWidget->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents_7 = new QWidget();
        dockWidgetContents_7->setObjectName(QStringLiteral("dockWidgetContents_7"));
        nameEmitterLineEdit = new QLineEdit(dockWidgetContents_7);
        nameEmitterLineEdit->setObjectName(QStringLiteral("nameEmitterLineEdit"));
        nameEmitterLineEdit->setGeometry(QRect(28, 16, 219, 25));
        newEmitterButton = new QPushButton(dockWidgetContents_7);
        newEmitterButton->setObjectName(QStringLiteral("newEmitterButton"));
        newEmitterButton->setEnabled(true);
        newEmitterButton->setGeometry(QRect(94, 58, 75, 23));
        newEmitterWidget->setWidget(dockWidgetContents_7);
        renameWidget = new QDockWidget(frame);
        renameWidget->setObjectName(QStringLiteral("renameWidget"));
        renameWidget->setGeometry(QRect(190, 274, 275, 121));
        renameWidget->setFloating(true);
        renameWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
        renameWidget->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents_8 = new QWidget();
        dockWidgetContents_8->setObjectName(QStringLiteral("dockWidgetContents_8"));
        renameEmitterLineEdit = new QLineEdit(dockWidgetContents_8);
        renameEmitterLineEdit->setObjectName(QStringLiteral("renameEmitterLineEdit"));
        renameEmitterLineEdit->setGeometry(QRect(28, 16, 219, 25));
        renameEmitterButton = new QPushButton(dockWidgetContents_8);
        renameEmitterButton->setObjectName(QStringLiteral("renameEmitterButton"));
        renameEmitterButton->setEnabled(true);
        renameEmitterButton->setGeometry(QRect(94, 60, 75, 23));
        renameWidget->setWidget(dockWidgetContents_8);
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
        helpWidget->setGeometry(QRect(574, 0, 417, 279));
        helpWidget->setFloating(true);
        helpWidget->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetFloatable);
        helpWidget->setAllowedAreas(Qt::NoDockWidgetArea);
        dockWidgetContents_6 = new QWidget();
        dockWidgetContents_6->setObjectName(QStringLiteral("dockWidgetContents_6"));
        textBrowser_2 = new QTextBrowser(dockWidgetContents_6);
        textBrowser_2->setObjectName(QStringLiteral("textBrowser_2"));
        textBrowser_2->setGeometry(QRect(2, 0, 417, 277));
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
        menuEmitter = new QMenu(menuBar);
        menuEmitter->setObjectName(QStringLiteral("menuEmitter"));
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
        lookAtSpinBoxY = new QDoubleSpinBox(dockWidgetContents);
        lookAtSpinBoxY->setObjectName(QStringLiteral("lookAtSpinBoxY"));
        lookAtSpinBoxY->setGeometry(QRect(102, 2, 47, 22));
        lookAtSpinBoxY->setReadOnly(true);
        lookAtSpinBoxY->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lookAtSpinBoxY->setDecimals(3);
        lookAtSpinBoxY->setMinimum(-1024);
        lookAtSpinBoxY->setMaximum(1024);
        lookAtSpinBoxX = new QDoubleSpinBox(dockWidgetContents);
        lookAtSpinBoxX->setObjectName(QStringLiteral("lookAtSpinBoxX"));
        lookAtSpinBoxX->setGeometry(QRect(56, 2, 47, 22));
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
        lookAtSpinBoxZ->setGeometry(QRect(148, 2, 47, 22));
        lookAtSpinBoxZ->setReadOnly(true);
        lookAtSpinBoxZ->setButtonSymbols(QAbstractSpinBox::NoButtons);
        lookAtSpinBoxZ->setDecimals(3);
        lookAtSpinBoxZ->setMinimum(-1024);
        lookAtSpinBoxZ->setMaximum(1024);
        label_18 = new QLabel(dockWidgetContents);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setGeometry(QRect(4, -4, 63, 31));
        QFont font3;
        font3.setPointSize(11);
        label_18->setFont(font3);
        label_19 = new QLabel(dockWidgetContents);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setGeometry(QRect(4, 26, 85, 21));
        label_19->setFont(font1);
        particleCountSpinBox = new QDoubleSpinBox(dockWidgetContents);
        particleCountSpinBox->setObjectName(QStringLiteral("particleCountSpinBox"));
        particleCountSpinBox->setGeometry(QRect(92, 26, 103, 22));
        particleCountSpinBox->setWrapping(false);
        particleCountSpinBox->setFrame(true);
        particleCountSpinBox->setReadOnly(true);
        particleCountSpinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        particleCountSpinBox->setKeyboardTracking(true);
        particleCountSpinBox->setDecimals(0);
        particleCountSpinBox->setMinimum(0);
        particleCountSpinBox->setMaximum(1000);
        particleCountSpinBox->setValue(0);
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
        gridDockWidget->setMinimumSize(QSize(200, 200));
        gridDockWidget->setMaximumSize(QSize(200, 200));
        gridDockWidget->setFeatures(QDockWidget::DockWidgetFloatable|QDockWidget::DockWidgetMovable);
        gridDockWidget->setAllowedAreas(Qt::RightDockWidgetArea);
        dockWidgetContents_5 = new QWidget();
        dockWidgetContents_5->setObjectName(QStringLiteral("dockWidgetContents_5"));
        gridSpaceSpinBox = new QDoubleSpinBox(dockWidgetContents_5);
        gridSpaceSpinBox->setObjectName(QStringLiteral("gridSpaceSpinBox"));
        gridSpaceSpinBox->setGeometry(QRect(93, 24, 97, 22));
        gridSpaceSpinBox->setSingleStep(0.25);
        gridSpaceSpinBox->setValue(1);
        label_5 = new QLabel(dockWidgetContents_5);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(18, 26, 69, 16));
        label_5->setFont(font1);
        restartParticleSystemButton = new QPushButton(dockWidgetContents_5);
        restartParticleSystemButton->setObjectName(QStringLiteral("restartParticleSystemButton"));
        restartParticleSystemButton->setGeometry(QRect(18, 60, 173, 23));
        gridDockWidget->setWidget(dockWidgetContents_5);
        ParticleEditorClass->addDockWidget(static_cast<Qt::DockWidgetArea>(2), gridDockWidget);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEmitter->menuAction());
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
        menuEmitter->addAction(actionNew_Emitter);
        menuEmitter->addAction(actionRename_Emitter);
        menuEmitter->addAction(actionDelete_Emitter);
        menuEmitter->addAction(actionDuplicate_Emitter);

        retranslateUi(ParticleEditorClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ParticleEditorClass);
    } // setupUi

    void retranslateUi(QMainWindow *ParticleEditorClass)
    {
        ParticleEditorClass->setWindowTitle(QApplication::translate("ParticleEditorClass", "ParticleEditor", 0));
        actionNew->setText(QApplication::translate("ParticleEditorClass", "New Project", 0));
        actionNew->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+N", 0));
        actionOpen->setText(QApplication::translate("ParticleEditorClass", "Open Project...", 0));
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
        actionNew_Emitter->setText(QApplication::translate("ParticleEditorClass", "New Emitter", 0));
        actionRename_Emitter->setText(QApplication::translate("ParticleEditorClass", "Rename Emitter", 0));
        actionDelete_Emitter->setText(QApplication::translate("ParticleEditorClass", "Delete Emitter", 0));
        actionDuplicate_Emitter->setText(QApplication::translate("ParticleEditorClass", "Duplicate Emitter", 0));
        groupBox_3->setTitle(QApplication::translate("ParticleEditorClass", "Properties", 0));
        label_17->setText(QApplication::translate("ParticleEditorClass", "SpawnTime:", 0));
        label_15->setText(QApplication::translate("ParticleEditorClass", "Orbit radius", 0));
        label_7->setText(QApplication::translate("ParticleEditorClass", "SpeedMin:", 0));
        label->setText(QApplication::translate("ParticleEditorClass", "Pos:", 0));
        label_9->setText(QApplication::translate("ParticleEditorClass", "LifeMin:", 0));
        label_2->setText(QApplication::translate("ParticleEditorClass", "SizeMin:", 0));
        label_10->setText(QApplication::translate("ParticleEditorClass", "LifeMax:", 0));
        label_11->setText(QApplication::translate("ParticleEditorClass", "SpeedMax:", 0));
        label_8->setText(QApplication::translate("ParticleEditorClass", "Direction:", 0));
        label_12->setText(QApplication::translate("ParticleEditorClass", "SizeMax:", 0));
        label_13->setText(QApplication::translate("ParticleEditorClass", "Spread:", 0));
        label_6->setText(QApplication::translate("ParticleEditorClass", "Gravity:", 0));
        label_22->setText(QApplication::translate("ParticleEditorClass", "Rotspdmax:", 0));
        label_21->setText(QApplication::translate("ParticleEditorClass", "Rotspdmin:", 0));
        label_14->setText(QApplication::translate("ParticleEditorClass", "Orbit speed", 0));
        label_23->setText(QApplication::translate("ParticleEditorClass", "Blending:", 0));
        blendingComboBox->clear();
        blendingComboBox->insertItems(0, QStringList()
         << QApplication::translate("ParticleEditorClass", "Additive", 0)
         << QApplication::translate("ParticleEditorClass", "Alpha", 0)
        );
        label_20->setText(QApplication::translate("ParticleEditorClass", "Relative pos:", 0));
        label_3->setText(QApplication::translate("ParticleEditorClass", "SizeEnd:", 0));
        relativeComboBox->clear();
        relativeComboBox->insertItems(0, QStringList()
         << QApplication::translate("ParticleEditorClass", "NonRelative", 0)
         << QApplication::translate("ParticleEditorClass", "Relative", 0)
        );
        label_4->setText(QApplication::translate("ParticleEditorClass", "# per spawn:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabProperties), QApplication::translate("ParticleEditorClass", "Properties", 0));
        label_16->setText(QApplication::translate("ParticleEditorClass", "Texture:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ParticleEditorClass", "Textures", 0));
        removeObjectButton->setText(QApplication::translate("ParticleEditorClass", "Remove model", 0));
        label_24->setText(QApplication::translate("ParticleEditorClass", "Model:", 0));
        label_25->setText(QApplication::translate("ParticleEditorClass", "Texture:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ParticleEditorClass", "Models", 0));
        groupBox->setTitle(QApplication::translate("ParticleEditorClass", "Emitters", 0));
        newEmitterWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "New Emitter", 0));
        nameEmitterLineEdit->setText(QString());
        nameEmitterLineEdit->setPlaceholderText(QApplication::translate("ParticleEditorClass", "EmitterName", 0));
        newEmitterButton->setText(QApplication::translate("ParticleEditorClass", "Create", 0));
        newEmitterButton->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+N", 0));
        renameWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Rename Emitter", 0));
        renameEmitterLineEdit->setText(QString());
        renameEmitterLineEdit->setPlaceholderText(QApplication::translate("ParticleEditorClass", "EmitterName", 0));
        renameEmitterButton->setText(QApplication::translate("ParticleEditorClass", "Rename", 0));
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
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">F	Focus on selected particle</span></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">LMB-click"
                        " on particle origin to select it</span></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt;\">Drag an axis to move a particle emitter</span></p>\n"
"<p align=\"center\" style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px; font-size:10pt;\"><br /></p></body></html>", 0));
        menuFile->setTitle(QApplication::translate("ParticleEditorClass", "File", 0));
        menuHelp->setTitle(QApplication::translate("ParticleEditorClass", "Help", 0));
        menuView->setTitle(QApplication::translate("ParticleEditorClass", "View", 0));
        menuEmitter->setTitle(QApplication::translate("ParticleEditorClass", "Emitter", 0));
        dockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Status", 0));
        label_18->setText(QApplication::translate("ParticleEditorClass", "LookAt:", 0));
        label_19->setText(QApplication::translate("ParticleEditorClass", "Particle count:", 0));
        colorDockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Start Color", 0));
        endcolorDockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "End Color", 0));
#ifndef QT_NO_ACCESSIBILITY
        gridDockWidget->setAccessibleName(QString());
#endif // QT_NO_ACCESSIBILITY
        gridDockWidget->setWindowTitle(QApplication::translate("ParticleEditorClass", "Settings", 0));
        label_5->setText(QApplication::translate("ParticleEditorClass", "Grid space:", 0));
        restartParticleSystemButton->setText(QApplication::translate("ParticleEditorClass", "Restart particle system", 0));
    } // retranslateUi

};

namespace Ui {
    class ParticleEditorClass: public Ui_ParticleEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARTICLEEDITOR_H
