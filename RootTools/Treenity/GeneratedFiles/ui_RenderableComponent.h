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
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/DropLineEdit.h>

QT_BEGIN_NAMESPACE

class Ui_Renderable
{
public:
    QVBoxLayout *verticalLayout_5;
    QWidget *widget_overview;
    QGridLayout *gridLayout_4;
    DropLineEdit *lineEdit_modelName;
    QLabel *label_model;
    DropLineEdit *lineEdit_materialName;
    QLabel *label_material;
    QToolButton *toolButton_modelBrowse;
    QToolButton *toolButton_materialBrowse;
    QPushButton *pushButton_newMaterial;
    QWidget *widget_material;
    QGridLayout *gridLayout;
    QWidget *widget_shaderSelect;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox_shaderSelect;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QWidget *Renderable)
    {
        if (Renderable->objectName().isEmpty())
            Renderable->setObjectName(QStringLiteral("Renderable"));
        Renderable->resize(374, 147);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Renderable->sizePolicy().hasHeightForWidth());
        Renderable->setSizePolicy(sizePolicy);
        Renderable->setMinimumSize(QSize(0, 0));
        Renderable->setMaximumSize(QSize(16777215, 16777215));
        Renderable->setAcceptDrops(true);
        verticalLayout_5 = new QVBoxLayout(Renderable);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        verticalLayout_5->setContentsMargins(0, 0, 0, 0);
        widget_overview = new QWidget(Renderable);
        widget_overview->setObjectName(QStringLiteral("widget_overview"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(widget_overview->sizePolicy().hasHeightForWidth());
        widget_overview->setSizePolicy(sizePolicy1);
        gridLayout_4 = new QGridLayout(widget_overview);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        gridLayout_4->setContentsMargins(-1, 9, -1, -1);
        lineEdit_modelName = new DropLineEdit(widget_overview);
        lineEdit_modelName->setObjectName(QStringLiteral("lineEdit_modelName"));
        lineEdit_modelName->setReadOnly(true);

        gridLayout_4->addWidget(lineEdit_modelName, 0, 1, 1, 1);

        label_model = new QLabel(widget_overview);
        label_model->setObjectName(QStringLiteral("label_model"));

        gridLayout_4->addWidget(label_model, 0, 0, 1, 1);

        lineEdit_materialName = new DropLineEdit(widget_overview);
        lineEdit_materialName->setObjectName(QStringLiteral("lineEdit_materialName"));
        lineEdit_materialName->setReadOnly(false);

        gridLayout_4->addWidget(lineEdit_materialName, 1, 1, 1, 1);

        label_material = new QLabel(widget_overview);
        label_material->setObjectName(QStringLiteral("label_material"));

        gridLayout_4->addWidget(label_material, 1, 0, 1, 1);

        toolButton_modelBrowse = new QToolButton(widget_overview);
        toolButton_modelBrowse->setObjectName(QStringLiteral("toolButton_modelBrowse"));

        gridLayout_4->addWidget(toolButton_modelBrowse, 0, 2, 1, 1);

        toolButton_materialBrowse = new QToolButton(widget_overview);
        toolButton_materialBrowse->setObjectName(QStringLiteral("toolButton_materialBrowse"));

        gridLayout_4->addWidget(toolButton_materialBrowse, 1, 2, 1, 1);

        pushButton_newMaterial = new QPushButton(widget_overview);
        pushButton_newMaterial->setObjectName(QStringLiteral("pushButton_newMaterial"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(pushButton_newMaterial->sizePolicy().hasHeightForWidth());
        pushButton_newMaterial->setSizePolicy(sizePolicy2);
        pushButton_newMaterial->setMinimumSize(QSize(0, 0));
        pushButton_newMaterial->setMaximumSize(QSize(32, 22));

        gridLayout_4->addWidget(pushButton_newMaterial, 1, 3, 1, 1);

        label_model->raise();
        label_material->raise();
        lineEdit_modelName->raise();
        lineEdit_materialName->raise();
        toolButton_modelBrowse->raise();
        toolButton_materialBrowse->raise();
        pushButton_newMaterial->raise();

        verticalLayout_5->addWidget(widget_overview);

        widget_material = new QWidget(Renderable);
        widget_material->setObjectName(QStringLiteral("widget_material"));
        sizePolicy.setHeightForWidth(widget_material->sizePolicy().hasHeightForWidth());
        widget_material->setSizePolicy(sizePolicy);
        widget_material->setMaximumSize(QSize(16777215, 16777215));
        widget_material->setLayoutDirection(Qt::LeftToRight);
        gridLayout = new QGridLayout(widget_material);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        widget_shaderSelect = new QWidget(widget_material);
        widget_shaderSelect->setObjectName(QStringLiteral("widget_shaderSelect"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(widget_shaderSelect->sizePolicy().hasHeightForWidth());
        widget_shaderSelect->setSizePolicy(sizePolicy3);
        widget_shaderSelect->setMaximumSize(QSize(16777215, 500));
        verticalLayout = new QVBoxLayout(widget_shaderSelect);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 9, -1, -1);
        comboBox_shaderSelect = new QComboBox(widget_shaderSelect);
        comboBox_shaderSelect->setObjectName(QStringLiteral("comboBox_shaderSelect"));
        sizePolicy1.setHeightForWidth(comboBox_shaderSelect->sizePolicy().hasHeightForWidth());
        comboBox_shaderSelect->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(comboBox_shaderSelect, 0, Qt::AlignTop);


        gridLayout->addWidget(widget_shaderSelect, 1, 0, 1, 1, Qt::AlignTop);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));

        gridLayout->addLayout(verticalLayout_2, 2, 0, 1, 1);


        verticalLayout_5->addWidget(widget_material);


        retranslateUi(Renderable);

        QMetaObject::connectSlotsByName(Renderable);
    } // setupUi

    void retranslateUi(QWidget *Renderable)
    {
        Renderable->setWindowTitle(QApplication::translate("Renderable", "Renderable", 0));
        label_model->setText(QApplication::translate("Renderable", "Model", 0));
        label_material->setText(QApplication::translate("Renderable", "Material", 0));
        toolButton_modelBrowse->setText(QApplication::translate("Renderable", "...", 0));
        toolButton_materialBrowse->setText(QApplication::translate("Renderable", "...", 0));
        pushButton_newMaterial->setText(QApplication::translate("Renderable", "New", 0));
        comboBox_shaderSelect->clear();
        comboBox_shaderSelect->insertItems(0, QStringList()
         << QApplication::translate("Renderable", "Diffuse", 0)
         << QApplication::translate("Renderable", "DiffuseNormal", 0)
         << QApplication::translate("Renderable", "Translucent", 0)
         << QApplication::translate("Renderable", "Refractive", 0)
         << QApplication::translate("Renderable", "RefractiveClear", 0)
         << QApplication::translate("Renderable", "Blend", 0)
        );
    } // retranslateUi

};

namespace Ui {
    class Renderable: public Ui_Renderable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERABLECOMPONENT_H
