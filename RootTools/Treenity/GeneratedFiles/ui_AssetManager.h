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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
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
    QPushButton *pushButton_back;
    QLineEdit *lineEdit_assetSearch;
    QPushButton *pushButton_forward;
    QListView *listView_fileBrowser;
    QSlider *horizontalSlider_icon;
    QLabel *label;

    void setupUi(QWidget *AssetManagerUi)
    {
        if (AssetManagerUi->objectName().isEmpty())
            AssetManagerUi->setObjectName(QStringLiteral("AssetManagerUi"));
        AssetManagerUi->resize(1005, 286);
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
        pushButton_back = new QPushButton(AssetManagerUi);
        pushButton_back->setObjectName(QStringLiteral("pushButton_back"));
        pushButton_back->setMinimumSize(QSize(25, 25));
        pushButton_back->setMaximumSize(QSize(25, 25));
        pushButton_back->setStyleSheet(QLatin1String("QPushButton{\n"
"	color: #FF8C00;\n"
"}"));

        gridLayout->addWidget(pushButton_back, 0, 0, 1, 1);

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

        pushButton_forward = new QPushButton(AssetManagerUi);
        pushButton_forward->setObjectName(QStringLiteral("pushButton_forward"));
        pushButton_forward->setMinimumSize(QSize(25, 25));
        pushButton_forward->setMaximumSize(QSize(25, 25));
        pushButton_forward->setStyleSheet(QLatin1String("QPushButton{\n"
"	color: #FF8C00;\n"
"}"));

        gridLayout->addWidget(pushButton_forward, 0, 1, 1, 1);

        listView_fileBrowser = new QListView(AssetManagerUi);
        listView_fileBrowser->setObjectName(QStringLiteral("listView_fileBrowser"));
        listView_fileBrowser->setDragDropMode(QAbstractItemView::NoDragDrop);
        listView_fileBrowser->setIconSize(QSize(50, 50));
        listView_fileBrowser->setTextElideMode(Qt::ElideRight);
        listView_fileBrowser->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
        listView_fileBrowser->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
        listView_fileBrowser->setMovement(QListView::Static);
        listView_fileBrowser->setGridSize(QSize(80, 80));
        listView_fileBrowser->setViewMode(QListView::IconMode);
        listView_fileBrowser->setWordWrap(true);

        gridLayout->addWidget(listView_fileBrowser, 1, 0, 1, 3);

        horizontalSlider_icon = new QSlider(AssetManagerUi);
        horizontalSlider_icon->setObjectName(QStringLiteral("horizontalSlider_icon"));
        horizontalSlider_icon->setMaximumSize(QSize(150, 16777215));
        horizontalSlider_icon->setMinimum(25);
        horizontalSlider_icon->setMaximum(100);
        horizontalSlider_icon->setValue(50);
        horizontalSlider_icon->setOrientation(Qt::Horizontal);
        horizontalSlider_icon->setTickPosition(QSlider::NoTicks);

        gridLayout->addWidget(horizontalSlider_icon, 2, 2, 1, 1);

        label = new QLabel(AssetManagerUi);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 2, 0, 1, 2);


        horizontalLayout->addLayout(gridLayout);


        retranslateUi(AssetManagerUi);

        QMetaObject::connectSlotsByName(AssetManagerUi);
    } // setupUi

    void retranslateUi(QWidget *AssetManagerUi)
    {
        AssetManagerUi->setWindowTitle(QApplication::translate("AssetManagerUi", "Form", 0));
        pushButton_expandall->setText(QApplication::translate("AssetManagerUi", "Expand all", 0));
        pushButton_collapseall->setText(QApplication::translate("AssetManagerUi", "Collapse all", 0));
        pushButton_back->setText(QApplication::translate("AssetManagerUi", "<", 0));
        lineEdit_assetSearch->setPlaceholderText(QApplication::translate("AssetManagerUi", "Search assets...", 0));
        pushButton_forward->setText(QApplication::translate("AssetManagerUi", ">", 0));
        label->setText(QApplication::translate("AssetManagerUi", "Icon size:", 0));
    } // retranslateUi

};

namespace Ui {
    class AssetManagerUi: public Ui_AssetManagerUi {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSETMANAGER_H