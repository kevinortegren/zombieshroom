/********************************************************************************
** Form generated from reading UI file 'RenderableComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENDERABLECOMPONENT_H
#define UI_RENDERABLECOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Renderable
{
public:
    QGridLayout *gridLayout;
    QToolButton *toolButton_modelBrowse;
    QLineEdit *lineEdit_modelName;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_materialName;
    QToolButton *toolButton_materialBrowse;

    void setupUi(QWidget *Renderable)
    {
        if (Renderable->objectName().isEmpty())
            Renderable->setObjectName(QStringLiteral("Renderable"));
        Renderable->resize(400, 149);
        Renderable->setMaximumSize(QSize(16777215, 150));
        gridLayout = new QGridLayout(Renderable);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        toolButton_modelBrowse = new QToolButton(Renderable);
        toolButton_modelBrowse->setObjectName(QStringLiteral("toolButton_modelBrowse"));

        gridLayout->addWidget(toolButton_modelBrowse, 0, 3, 1, 1);

        lineEdit_modelName = new QLineEdit(Renderable);
        lineEdit_modelName->setObjectName(QStringLiteral("lineEdit_modelName"));

        gridLayout->addWidget(lineEdit_modelName, 0, 2, 1, 1);

        label = new QLabel(Renderable);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(Renderable);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        lineEdit_materialName = new QLineEdit(Renderable);
        lineEdit_materialName->setObjectName(QStringLiteral("lineEdit_materialName"));

        gridLayout->addWidget(lineEdit_materialName, 1, 2, 1, 1);

        toolButton_materialBrowse = new QToolButton(Renderable);
        toolButton_materialBrowse->setObjectName(QStringLiteral("toolButton_materialBrowse"));

        gridLayout->addWidget(toolButton_materialBrowse, 1, 3, 1, 1);


        retranslateUi(Renderable);

        QMetaObject::connectSlotsByName(Renderable);
    } // setupUi

    void retranslateUi(QWidget *Renderable)
    {
        Renderable->setWindowTitle(QApplication::translate("Renderable", "Renderable", 0));
        toolButton_modelBrowse->setText(QApplication::translate("Renderable", "...", 0));
        label->setText(QApplication::translate("Renderable", "Model", 0));
        label_2->setText(QApplication::translate("Renderable", "Material", 0));
        toolButton_materialBrowse->setText(QApplication::translate("Renderable", "...", 0));
    } // retranslateUi

};

namespace Ui {
    class Renderable: public Ui_Renderable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERABLECOMPONENT_H
