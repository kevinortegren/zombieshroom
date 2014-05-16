/********************************************************************************
** Form generated from reading UI file 'AssetManager.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASSETMANAGER_H
#define UI_ASSETMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AssetManagerUi
{
public:
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_2;
    QTreeView *treeView_assetFileBrowser;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_expandall;
    QPushButton *pushButton_collapseall;
    QGridLayout *gridLayout;
    QLineEdit *lineEdit_assetSearch;
    QPushButton *pushButton_back;
    QLabel *label;
    QPushButton *pushButton_forward;
    QSlider *horizontalSlider_icon;
    QListView *listView_fileBrowser;
    QGridLayout *gridLayout_3;
    QSpacerItem *verticalSpacer;
    QFrame *frame;
    QGridLayout *gridLayout_4;
    QLabel *label_ss;
    QLabel *label_fn;
    QGridLayout *gridLayout_5;
    QLabel *widget_pictureFrame;
    QLabel *label_createdName;
    QLabel *label_fileName;
    QLabel *label_sizeName;
    QLabel *label_cc;
    QLabel *label_res;
    QLabel *label_fileResolution;

    void setupUi(QWidget *AssetManagerUi)
    {
        if (AssetManagerUi->objectName().isEmpty())
            AssetManagerUi->setObjectName(QStringLiteral("AssetManagerUi"));
        AssetManagerUi->resize(1067, 286);
        horizontalLayout = new QHBoxLayout(AssetManagerUi);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(0);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        gridLayout_2->setSizeConstraint(QLayout::SetMaximumSize);
        treeView_assetFileBrowser = new QTreeView(AssetManagerUi);
        treeView_assetFileBrowser->setObjectName(QStringLiteral("treeView_assetFileBrowser"));
        treeView_assetFileBrowser->setMaximumSize(QSize(200, 16777215));

        gridLayout_2->addWidget(treeView_assetFileBrowser, 1, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_expandall = new QPushButton(AssetManagerUi);
        pushButton_expandall->setObjectName(QStringLiteral("pushButton_expandall"));
        pushButton_expandall->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(pushButton_expandall);

        pushButton_collapseall = new QPushButton(AssetManagerUi);
        pushButton_collapseall->setObjectName(QStringLiteral("pushButton_collapseall"));
        pushButton_collapseall->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_2->addWidget(pushButton_collapseall);


        gridLayout_2->addLayout(horizontalLayout_2, 2, 1, 1, 1);


        horizontalLayout->addLayout(gridLayout_2);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lineEdit_assetSearch = new QLineEdit(AssetManagerUi);
        lineEdit_assetSearch->setObjectName(QStringLiteral("lineEdit_assetSearch"));
        lineEdit_assetSearch->setStyleSheet(QLatin1String("QLineEdit {\n"
"     border: 2px solid gray;\n"
"     border-radius: 10px;\n"
"     padding: 0 8px;\n"
"     selection-background-color: darkgray;\n"
" }"));
        lineEdit_assetSearch->setClearButtonEnabled(false);

        gridLayout->addWidget(lineEdit_assetSearch, 0, 2, 1, 1);

        pushButton_back = new QPushButton(AssetManagerUi);
        pushButton_back->setObjectName(QStringLiteral("pushButton_back"));
        pushButton_back->setMinimumSize(QSize(25, 25));
        pushButton_back->setMaximumSize(QSize(25, 25));
        pushButton_back->setStyleSheet(QLatin1String("QPushButton{\n"
"	color: #FF8C00;\n"
"}"));

        gridLayout->addWidget(pushButton_back, 0, 0, 1, 1);

        label = new QLabel(AssetManagerUi);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 0, 1, 2);

        pushButton_forward = new QPushButton(AssetManagerUi);
        pushButton_forward->setObjectName(QStringLiteral("pushButton_forward"));
        pushButton_forward->setMinimumSize(QSize(25, 25));
        pushButton_forward->setMaximumSize(QSize(25, 25));
        pushButton_forward->setStyleSheet(QLatin1String("QPushButton{\n"
"	color: #FF8C00;\n"
"}"));

        gridLayout->addWidget(pushButton_forward, 0, 1, 1, 1);

        horizontalSlider_icon = new QSlider(AssetManagerUi);
        horizontalSlider_icon->setObjectName(QStringLiteral("horizontalSlider_icon"));
        horizontalSlider_icon->setMaximumSize(QSize(150, 16777215));
        horizontalSlider_icon->setMinimum(25);
        horizontalSlider_icon->setMaximum(100);
        horizontalSlider_icon->setValue(50);
        horizontalSlider_icon->setOrientation(Qt::Horizontal);
        horizontalSlider_icon->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(horizontalSlider_icon, 2, 2, 1, 1);

        listView_fileBrowser = new QListView(AssetManagerUi);
        listView_fileBrowser->setObjectName(QStringLiteral("listView_fileBrowser"));
        listView_fileBrowser->setDragDropMode(QAbstractItemView::NoDragDrop);
        listView_fileBrowser->setIconSize(QSize(50, 50));
        listView_fileBrowser->setTextElideMode(Qt::ElideRight);
        listView_fileBrowser->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        listView_fileBrowser->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        listView_fileBrowser->setMovement(QListView::Static);
        listView_fileBrowser->setLayoutMode(QListView::Batched);
        listView_fileBrowser->setGridSize(QSize(80, 80));
        listView_fileBrowser->setViewMode(QListView::IconMode);
        listView_fileBrowser->setBatchSize(5);
        listView_fileBrowser->setWordWrap(true);

        gridLayout->addWidget(listView_fileBrowser, 1, 0, 1, 3);


        horizontalLayout->addLayout(gridLayout);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        gridLayout_3->setSizeConstraint(QLayout::SetNoConstraint);
        verticalSpacer = new QSpacerItem(0, 21, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer, 1, 0, 1, 1);

        frame = new QFrame(AssetManagerUi);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setMinimumSize(QSize(0, 0));
        frame->setMaximumSize(QSize(300, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setLineWidth(1);
        frame->setMidLineWidth(0);
        gridLayout_4 = new QGridLayout(frame);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_ss = new QLabel(frame);
        label_ss->setObjectName(QStringLiteral("label_ss"));
        label_ss->setMaximumSize(QSize(55, 20));

        gridLayout_4->addWidget(label_ss, 2, 0, 1, 1);

        label_fn = new QLabel(frame);
        label_fn->setObjectName(QStringLiteral("label_fn"));
        label_fn->setMaximumSize(QSize(55, 20));

        gridLayout_4->addWidget(label_fn, 1, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        widget_pictureFrame = new QLabel(frame);
        widget_pictureFrame->setObjectName(QStringLiteral("widget_pictureFrame"));
        widget_pictureFrame->setMaximumSize(QSize(180, 180));

        gridLayout_5->addWidget(widget_pictureFrame, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_5, 0, 0, 1, 3);

        label_createdName = new QLabel(frame);
        label_createdName->setObjectName(QStringLiteral("label_createdName"));
        label_createdName->setMaximumSize(QSize(16777215, 16777215));
        label_createdName->setWordWrap(true);

        gridLayout_4->addWidget(label_createdName, 5, 1, 1, 2);

        label_fileName = new QLabel(frame);
        label_fileName->setObjectName(QStringLiteral("label_fileName"));
        label_fileName->setMaximumSize(QSize(16777215, 16777215));
        label_fileName->setWordWrap(true);

        gridLayout_4->addWidget(label_fileName, 1, 1, 1, 2);

        label_sizeName = new QLabel(frame);
        label_sizeName->setObjectName(QStringLiteral("label_sizeName"));
        label_sizeName->setMaximumSize(QSize(16777215, 16777215));
        label_sizeName->setWordWrap(true);

        gridLayout_4->addWidget(label_sizeName, 2, 1, 1, 2);

        label_cc = new QLabel(frame);
        label_cc->setObjectName(QStringLiteral("label_cc"));
        label_cc->setMaximumSize(QSize(55, 20));

        gridLayout_4->addWidget(label_cc, 5, 0, 1, 1);

        label_res = new QLabel(frame);
        label_res->setObjectName(QStringLiteral("label_res"));
        label_res->setMaximumSize(QSize(55, 20));

        gridLayout_4->addWidget(label_res, 6, 0, 1, 1);

        label_fileResolution = new QLabel(frame);
        label_fileResolution->setObjectName(QStringLiteral("label_fileResolution"));

        gridLayout_4->addWidget(label_fileResolution, 6, 1, 1, 2);


        gridLayout_3->addWidget(frame, 0, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout_3);


        retranslateUi(AssetManagerUi);

        QMetaObject::connectSlotsByName(AssetManagerUi);
    } // setupUi

    void retranslateUi(QWidget *AssetManagerUi)
    {
        AssetManagerUi->setWindowTitle(QApplication::translate("AssetManagerUi", "Form", 0));
        pushButton_expandall->setText(QApplication::translate("AssetManagerUi", "Expand all", 0));
        pushButton_collapseall->setText(QApplication::translate("AssetManagerUi", "Collapse all", 0));
        lineEdit_assetSearch->setPlaceholderText(QApplication::translate("AssetManagerUi", "Search assets...", 0));
        pushButton_back->setText(QApplication::translate("AssetManagerUi", "<", 0));
        label->setText(QApplication::translate("AssetManagerUi", "Icon size:", 0));
        pushButton_forward->setText(QApplication::translate("AssetManagerUi", ">", 0));
        label_ss->setText(QApplication::translate("AssetManagerUi", "Size:", 0));
        label_fn->setText(QApplication::translate("AssetManagerUi", "Name:", 0));
        widget_pictureFrame->setText(QString());
        label_createdName->setText(QString());
        label_fileName->setText(QString());
        label_sizeName->setText(QString());
        label_cc->setText(QApplication::translate("AssetManagerUi", "Created:", 0));
        label_res->setText(QApplication::translate("AssetManagerUi", "Resolution:", 0));
        label_fileResolution->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class AssetManagerUi: public Ui_AssetManagerUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSETMANAGER_H
