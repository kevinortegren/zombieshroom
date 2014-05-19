/********************************************************************************
** Form generated from reading UI file 'AssetBrowser.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ASSETBROWSER_H
#define UI_ASSETBROWSER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QSlider>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QVBoxLayout *verticalLayout;
    QLineEdit *lineEdit_assetSearch;
    QListView *listView_fileBrowser;
    QFormLayout *formLayout;
    QLabel *label;
    QSlider *horizontalSlider_icon;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(330, 457);
        verticalLayout = new QVBoxLayout(Form);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        lineEdit_assetSearch = new QLineEdit(Form);
        lineEdit_assetSearch->setObjectName(QStringLiteral("lineEdit_assetSearch"));
        lineEdit_assetSearch->setStyleSheet(QLatin1String("QLineEdit {\n"
"     border: 2px solid gray;\n"
"     border-radius: 10px;\n"
"     padding: 0 8px;\n"
"     selection-background-color: darkgray;\n"
" }"));
        lineEdit_assetSearch->setClearButtonEnabled(false);

        verticalLayout->addWidget(lineEdit_assetSearch);

        listView_fileBrowser = new QListView(Form);
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

        verticalLayout->addWidget(listView_fileBrowser);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QStringLiteral("formLayout"));
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(50, 16777215));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        horizontalSlider_icon = new QSlider(Form);
        horizontalSlider_icon->setObjectName(QStringLiteral("horizontalSlider_icon"));
        horizontalSlider_icon->setMaximumSize(QSize(150, 16777215));
        horizontalSlider_icon->setMinimum(25);
        horizontalSlider_icon->setMaximum(100);
        horizontalSlider_icon->setValue(50);
        horizontalSlider_icon->setOrientation(Qt::Horizontal);
        horizontalSlider_icon->setTickPosition(QSlider::NoTicks);

        formLayout->setWidget(0, QFormLayout::FieldRole, horizontalSlider_icon);


        verticalLayout->addLayout(formLayout);


        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        lineEdit_assetSearch->setPlaceholderText(QApplication::translate("Form", "Search assets...", 0));
        label->setText(QApplication::translate("Form", "Icon size:", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ASSETBROWSER_H
