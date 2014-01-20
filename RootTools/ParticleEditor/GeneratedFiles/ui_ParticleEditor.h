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
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFormLayout>
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
#include <QtWidgets/QStackedWidget>
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
    QAction *actionRed;
    QAction *actionGreen;
    QAction *actionYellow;
    QAction *actionWhite;
    QAction *actionGrid_2;
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tabProperties;
    QGroupBox *groupBox_3;
    QGroupBox *groupBox_5;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QLabel *label_15;
    QWidget *page_3;
    QWidget *page_4;
    QWidget *page_5;
    QWidget *page_2;
    QGroupBox *groupBox_2;
    QComboBox *templateComboBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label_12;
    QLabel *label_11;
    QLabel *label_10;
    QDoubleSpinBox *sizeMinSpinBox;
    QDoubleSpinBox *sizeMaxSpinBox;
    QDoubleSpinBox *speedMaxSpinBox;
    QDoubleSpinBox *spreadSpinBox;
    QDoubleSpinBox *lifeTimeMinSpinBox;
    QDoubleSpinBox *lifeTimeMaxSpinBox;
    QLabel *label_6;
    QDoubleSpinBox *gravitySpinBoxY;
    QDoubleSpinBox *gravitySpinBoxX;
    QPushButton *pushButton;
    QLabel *label_7;
    QDoubleSpinBox *gravitySpinBoxZ;
    QDoubleSpinBox *speedMinSpinBox;
    QLabel *label_8;
    QDoubleSpinBox *directionSpinBoxX;
    QDoubleSpinBox *directionSpinBoxY;
    QDoubleSpinBox *directionSpinBoxZ;
    QDoubleSpinBox *posSpinBoxX;
    QDoubleSpinBox *posSpinBoxY;
    QLabel *label_9;
    QLabel *label_2;
    QLabel *label_4;
    QLabel *label;
    QDoubleSpinBox *posSpinBoxZ;
    QLabel *label_13;
    QLabel *label_3;
    QDoubleSpinBox *sizeEndSpinBox;
    QLabel *label_14;
    QDoubleSpinBox *speedEndSpinBox;
    QPushButton *pushButton_2;
    QLabel *label_5;
    QWidget *tab_2;
    QWidget *gridLayoutWidget_2;
    QGridLayout *gridLayout_2;
    QPushButton *browseEmitterTextureButton;
    QLineEdit *textureEmitterLineEdit;
    QLabel *label_16;
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
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QMenu *menuView;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ParticleEditorClass)
    {
        if (ParticleEditorClass->objectName().isEmpty())
            ParticleEditorClass->setObjectName(QStringLiteral("ParticleEditorClass"));
        ParticleEditorClass->resize(1736, 970);
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
        centralWidget = new QWidget(ParticleEditorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(1218, 126, 517, 803));
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
        groupBox_3->setGeometry(QRect(2, 0, 507, 687));
        QFont font1;
        font1.setPointSize(10);
        groupBox_3->setFont(font1);
        groupBox_5 = new QGroupBox(groupBox_3);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        groupBox_5->setGeometry(QRect(5, 328, 497, 353));
        stackedWidget = new QStackedWidget(groupBox_5);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        stackedWidget->setGeometry(QRect(2, 74, 489, 275));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        label_15 = new QLabel(page);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setGeometry(QRect(14, 24, 61, 25));
        stackedWidget->addWidget(page);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
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
        groupBox_2 = new QGroupBox(groupBox_5);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(2, 14, 265, 57));
        groupBox_2->setFont(font1);
        templateComboBox = new QComboBox(groupBox_2);
        templateComboBox->setObjectName(QStringLiteral("templateComboBox"));
        templateComboBox->setGeometry(QRect(12, 24, 241, 22));
        gridLayoutWidget = new QWidget(groupBox_3);
        gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
        gridLayoutWidget->setGeometry(QRect(6, 18, 495, 307));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_12 = new QLabel(gridLayoutWidget);
        label_12->setObjectName(QStringLiteral("label_12"));

        gridLayout->addWidget(label_12, 2, 5, 1, 1);

        label_11 = new QLabel(gridLayoutWidget);
        label_11->setObjectName(QStringLiteral("label_11"));

        gridLayout->addWidget(label_11, 6, 5, 1, 1);

        label_10 = new QLabel(gridLayoutWidget);
        label_10->setObjectName(QStringLiteral("label_10"));

        gridLayout->addWidget(label_10, 10, 5, 1, 1);

        sizeMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        sizeMinSpinBox->setObjectName(QStringLiteral("sizeMinSpinBox"));
        sizeMinSpinBox->setMinimum(0);
        sizeMinSpinBox->setMaximum(1024);
        sizeMinSpinBox->setValue(1);

        gridLayout->addWidget(sizeMinSpinBox, 2, 4, 1, 1);

        sizeMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        sizeMaxSpinBox->setObjectName(QStringLiteral("sizeMaxSpinBox"));
        sizeMaxSpinBox->setMaximum(1024);
        sizeMaxSpinBox->setValue(1);

        gridLayout->addWidget(sizeMaxSpinBox, 2, 6, 1, 1);

        speedMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedMaxSpinBox->setObjectName(QStringLiteral("speedMaxSpinBox"));
        speedMaxSpinBox->setMaximum(1024);
        speedMaxSpinBox->setValue(1);

        gridLayout->addWidget(speedMaxSpinBox, 6, 6, 1, 1);

        spreadSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        spreadSpinBox->setObjectName(QStringLiteral("spreadSpinBox"));
        spreadSpinBox->setMinimum(-1024);
        spreadSpinBox->setMaximum(1024);
        spreadSpinBox->setValue(10);

        gridLayout->addWidget(spreadSpinBox, 9, 4, 1, 1);

        lifeTimeMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        lifeTimeMinSpinBox->setObjectName(QStringLiteral("lifeTimeMinSpinBox"));
        lifeTimeMinSpinBox->setMaximum(1024);
        lifeTimeMinSpinBox->setValue(1);

        gridLayout->addWidget(lifeTimeMinSpinBox, 10, 4, 1, 1);

        lifeTimeMaxSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        lifeTimeMaxSpinBox->setObjectName(QStringLiteral("lifeTimeMaxSpinBox"));
        lifeTimeMaxSpinBox->setMaximum(1024);
        lifeTimeMaxSpinBox->setValue(1);

        gridLayout->addWidget(lifeTimeMaxSpinBox, 10, 6, 1, 1);

        label_6 = new QLabel(gridLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 5, 1, 1, 1);

        gravitySpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxY->setObjectName(QStringLiteral("gravitySpinBoxY"));
        gravitySpinBoxY->setMinimum(-1024);
        gravitySpinBoxY->setMaximum(1024);

        gridLayout->addWidget(gravitySpinBoxY, 5, 5, 1, 1);

        gravitySpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxX->setObjectName(QStringLiteral("gravitySpinBoxX"));
        gravitySpinBoxX->setMinimum(-1024);
        gravitySpinBoxX->setMaximum(1024);

        gridLayout->addWidget(gravitySpinBoxX, 5, 4, 1, 1);

        pushButton = new QPushButton(gridLayoutWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setDefault(false);
        pushButton->setFlat(false);

        gridLayout->addWidget(pushButton, 4, 4, 1, 1);

        label_7 = new QLabel(gridLayoutWidget);
        label_7->setObjectName(QStringLiteral("label_7"));

        gridLayout->addWidget(label_7, 6, 1, 1, 1);

        gravitySpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        gravitySpinBoxZ->setObjectName(QStringLiteral("gravitySpinBoxZ"));
        gravitySpinBoxZ->setMinimum(-1024);
        gravitySpinBoxZ->setMaximum(1024);

        gridLayout->addWidget(gravitySpinBoxZ, 5, 6, 1, 1);

        speedMinSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedMinSpinBox->setObjectName(QStringLiteral("speedMinSpinBox"));
        speedMinSpinBox->setMaximum(1024);
        speedMinSpinBox->setValue(1);

        gridLayout->addWidget(speedMinSpinBox, 6, 4, 1, 1);

        label_8 = new QLabel(gridLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));

        gridLayout->addWidget(label_8, 8, 1, 1, 1);

        directionSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxX->setObjectName(QStringLiteral("directionSpinBoxX"));
        directionSpinBoxX->setMinimum(-1024);
        directionSpinBoxX->setMaximum(1024);
        directionSpinBoxX->setValue(1);

        gridLayout->addWidget(directionSpinBoxX, 8, 4, 1, 1);

        directionSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxY->setObjectName(QStringLiteral("directionSpinBoxY"));
        directionSpinBoxY->setMinimum(-1024);
        directionSpinBoxY->setMaximum(1024);

        gridLayout->addWidget(directionSpinBoxY, 8, 5, 1, 1);

        directionSpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        directionSpinBoxZ->setObjectName(QStringLiteral("directionSpinBoxZ"));
        directionSpinBoxZ->setMinimum(-1024);
        directionSpinBoxZ->setMaximum(1024);

        gridLayout->addWidget(directionSpinBoxZ, 8, 6, 1, 1);

        posSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxX->setObjectName(QStringLiteral("posSpinBoxX"));
        posSpinBoxX->setMinimum(-1024);
        posSpinBoxX->setMaximum(1024);

        gridLayout->addWidget(posSpinBoxX, 0, 4, 1, 1);

        posSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxY->setObjectName(QStringLiteral("posSpinBoxY"));
        posSpinBoxY->setMinimum(-1024);
        posSpinBoxY->setMaximum(1024);

        gridLayout->addWidget(posSpinBoxY, 0, 5, 1, 1);

        label_9 = new QLabel(gridLayoutWidget);
        label_9->setObjectName(QStringLiteral("label_9"));

        gridLayout->addWidget(label_9, 10, 1, 1, 1);

        label_2 = new QLabel(gridLayoutWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 2, 1, 1, 1);

        label_4 = new QLabel(gridLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 4, 1, 1, 1);

        label = new QLabel(gridLayoutWidget);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 1, 1, 1);

        posSpinBoxZ = new QDoubleSpinBox(gridLayoutWidget);
        posSpinBoxZ->setObjectName(QStringLiteral("posSpinBoxZ"));
        posSpinBoxZ->setMinimum(-1024);
        posSpinBoxZ->setMaximum(1024);

        gridLayout->addWidget(posSpinBoxZ, 0, 6, 1, 1);

        label_13 = new QLabel(gridLayoutWidget);
        label_13->setObjectName(QStringLiteral("label_13"));

        gridLayout->addWidget(label_13, 9, 1, 1, 1);

        label_3 = new QLabel(gridLayoutWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 7, 1, 1);

        sizeEndSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        sizeEndSpinBox->setObjectName(QStringLiteral("sizeEndSpinBox"));
        sizeEndSpinBox->setMaximum(1024);
        sizeEndSpinBox->setValue(1);

        gridLayout->addWidget(sizeEndSpinBox, 2, 8, 1, 1);

        label_14 = new QLabel(gridLayoutWidget);
        label_14->setObjectName(QStringLiteral("label_14"));

        gridLayout->addWidget(label_14, 6, 7, 1, 1);

        speedEndSpinBox = new QDoubleSpinBox(gridLayoutWidget);
        speedEndSpinBox->setObjectName(QStringLiteral("speedEndSpinBox"));
        speedEndSpinBox->setMaximum(1024);
        speedEndSpinBox->setValue(1);

        gridLayout->addWidget(speedEndSpinBox, 6, 8, 1, 1);

        pushButton_2 = new QPushButton(gridLayoutWidget);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        gridLayout->addWidget(pushButton_2, 4, 8, 1, 1);

        label_5 = new QLabel(gridLayoutWidget);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 4, 7, 1, 1);

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
        browseEmitterTextureButton = new QPushButton(gridLayoutWidget_2);
        browseEmitterTextureButton->setObjectName(QStringLiteral("browseEmitterTextureButton"));
        browseEmitterTextureButton->setFont(font1);

        gridLayout_2->addWidget(browseEmitterTextureButton, 0, 1, 1, 1);

        textureEmitterLineEdit = new QLineEdit(gridLayoutWidget_2);
        textureEmitterLineEdit->setObjectName(QStringLiteral("textureEmitterLineEdit"));
        textureEmitterLineEdit->setFont(font1);
        textureEmitterLineEdit->setReadOnly(true);

        gridLayout_2->addWidget(textureEmitterLineEdit, 0, 2, 1, 1);

        label_16 = new QLabel(gridLayoutWidget_2);
        label_16->setObjectName(QStringLiteral("label_16"));
        label_16->setFont(font1);

        gridLayout_2->addWidget(label_16, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(1458, -2, 277, 157));
        groupBox->setFont(font1);
        listWidget = new QListWidget(groupBox);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(12, 16, 253, 135));
        frame = new QFrame(centralWidget);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setGeometry(QRect(2, 28, 1209, 899));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        groupBox_6 = new QGroupBox(centralWidget);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        groupBox_6->setGeometry(QRect(1214, 4, 239, 117));
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

        ParticleEditorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(ParticleEditorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1736, 21));
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
        menuHelp->addAction(actionAbout);
        menuView->addAction(actionStats);
        menuView->addAction(actionGrid_2);

        retranslateUi(ParticleEditorClass);
        QObject::connect(templateComboBox, SIGNAL(currentIndexChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

        tabWidget->setCurrentIndex(0);
        stackedWidget->setCurrentIndex(0);


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
        groupBox_3->setTitle(QApplication::translate("ParticleEditorClass", "Properties", 0));
        groupBox_5->setTitle(QApplication::translate("ParticleEditorClass", "Template properties", 0));
        label_15->setText(QApplication::translate("ParticleEditorClass", "Hej", 0));
        groupBox_2->setTitle(QApplication::translate("ParticleEditorClass", "Template", 0));
        templateComboBox->clear();
        templateComboBox->insertItems(0, QStringList()
         << QApplication::translate("ParticleEditorClass", "Normal", 0)
         << QApplication::translate("ParticleEditorClass", "Spiral", 0)
         << QApplication::translate("ParticleEditorClass", "Vortex", 0)
         << QApplication::translate("ParticleEditorClass", "Pulsing", 0)
         << QApplication::translate("ParticleEditorClass", "Explosion", 0)
        );
        label_12->setText(QApplication::translate("ParticleEditorClass", "SizeMax:", 0));
        label_11->setText(QApplication::translate("ParticleEditorClass", "SpeedMax:", 0));
        label_10->setText(QApplication::translate("ParticleEditorClass", "LifeMax:", 0));
        label_6->setText(QApplication::translate("ParticleEditorClass", "Gravity:", 0));
        pushButton->setText(QString());
        label_7->setText(QApplication::translate("ParticleEditorClass", "SpeedMin:", 0));
        label_8->setText(QApplication::translate("ParticleEditorClass", "Direction:", 0));
        label_9->setText(QApplication::translate("ParticleEditorClass", "LifeMin:", 0));
        label_2->setText(QApplication::translate("ParticleEditorClass", "SizeMin:", 0));
        label_4->setText(QApplication::translate("ParticleEditorClass", "Color:", 0));
        label->setText(QApplication::translate("ParticleEditorClass", "Pos:", 0));
        label_13->setText(QApplication::translate("ParticleEditorClass", "Spread:", 0));
        label_3->setText(QApplication::translate("ParticleEditorClass", "SizeEnd:", 0));
        label_14->setText(QApplication::translate("ParticleEditorClass", "SpeedEnd:", 0));
        pushButton_2->setText(QString());
        label_5->setText(QApplication::translate("ParticleEditorClass", "ColorEnd:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tabProperties), QApplication::translate("ParticleEditorClass", "Properties", 0));
        browseEmitterTextureButton->setText(QApplication::translate("ParticleEditorClass", "Browse...", 0));
        label_16->setText(QApplication::translate("ParticleEditorClass", "Texture:", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ParticleEditorClass", "Resources", 0));
        groupBox->setTitle(QApplication::translate("ParticleEditorClass", "Emitters", 0));
        groupBox_6->setTitle(QApplication::translate("ParticleEditorClass", "Manage emitters", 0));
        newEmitterButton->setText(QApplication::translate("ParticleEditorClass", "New", 0));
        newEmitterButton->setShortcut(QApplication::translate("ParticleEditorClass", "Ctrl+N", 0));
        renameEmitterButton->setText(QApplication::translate("ParticleEditorClass", "Rename", 0));
        deleteEmitterButton->setText(QApplication::translate("ParticleEditorClass", "Delete", 0));
        nameEmitterLineEdit->setPlaceholderText(QApplication::translate("ParticleEditorClass", "EmitterName", 0));
        menuFile->setTitle(QApplication::translate("ParticleEditorClass", "File", 0));
        menuHelp->setTitle(QApplication::translate("ParticleEditorClass", "Help", 0));
        menuView->setTitle(QApplication::translate("ParticleEditorClass", "View", 0));
    } // retranslateUi

};

namespace Ui {
    class ParticleEditorClass: public Ui_ParticleEditorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARTICLEEDITOR_H
