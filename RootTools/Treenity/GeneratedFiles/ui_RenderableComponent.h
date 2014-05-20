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
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/DropLabel.h>
#include <RootTools/Treenity/Include/DropLineEdit.h>

QT_BEGIN_NAMESPACE

class Ui_Renderable
{
public:
    QGridLayout *gridLayout;
    QWidget *widget_overview;
    QGridLayout *gridLayout_4;
    DropLineEdit *lineEdit_modelName;
    QLabel *label_model;
    QLineEdit *lineEdit_materialName;
    QLabel *label_material;
    QToolButton *toolButton_modelBrowse;
    QToolButton *toolButton_materialBrowse;
    QFrame *frame_material;
    QGridLayout *gridLayout_5;
    QWidget *widget_shaderSelect;
    QVBoxLayout *verticalLayout;
    QComboBox *comboBox_shaderSelect;
    QStackedWidget *stackedWidget_shaderProperties;
    QWidget *page_diffuse;
    QGridLayout *gridLayout_3;
    QLabel *label_diffuse;
    QLabel *label_specular;
    DropLabel *label_image_specular;
    QLabel *label_glow;
    DropLabel *label_image_glow;
    DropLabel *label_image_diffuse;
    QWidget *page_diffuse_normal;
    QGridLayout *gridLayout_2;
    QLabel *label_diffuse_1;
    DropLabel *label_image_diffuse_1;
    QLabel *label_normal_1;
    DropLabel *label_image_normal_1;
    QLabel *label_specular_1;
    DropLabel *label_image_specular_1;
    QLabel *label_glow_1;
    DropLabel *label_image_glow_1;

    void setupUi(QWidget *Renderable)
    {
        if (Renderable->objectName().isEmpty())
            Renderable->setObjectName(QStringLiteral("Renderable"));
        Renderable->resize(379, 500);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Renderable->sizePolicy().hasHeightForWidth());
        Renderable->setSizePolicy(sizePolicy);
        Renderable->setMinimumSize(QSize(0, 500));
        Renderable->setMaximumSize(QSize(16777215, 500));
        Renderable->setAcceptDrops(true);
        gridLayout = new QGridLayout(Renderable);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(-1, 0, -1, -1);
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

        lineEdit_materialName = new QLineEdit(widget_overview);
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

        label_model->raise();
        label_material->raise();
        lineEdit_modelName->raise();
        lineEdit_materialName->raise();
        toolButton_modelBrowse->raise();
        toolButton_materialBrowse->raise();

        gridLayout->addWidget(widget_overview, 0, 0, 1, 1, Qt::AlignTop);

        frame_material = new QFrame(Renderable);
        frame_material->setObjectName(QStringLiteral("frame_material"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(frame_material->sizePolicy().hasHeightForWidth());
        frame_material->setSizePolicy(sizePolicy2);
        frame_material->setMaximumSize(QSize(16777215, 16777215));
        frame_material->setLayoutDirection(Qt::LeftToRight);
        frame_material->setFrameShape(QFrame::StyledPanel);
        frame_material->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(frame_material);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        gridLayout_5->setSizeConstraint(QLayout::SetDefaultConstraint);
        gridLayout_5->setContentsMargins(-1, 0, -1, -1);
        widget_shaderSelect = new QWidget(frame_material);
        widget_shaderSelect->setObjectName(QStringLiteral("widget_shaderSelect"));
        QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
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

        verticalLayout->addWidget(comboBox_shaderSelect);


        gridLayout_5->addWidget(widget_shaderSelect, 0, 0, 1, 1, Qt::AlignTop);

        stackedWidget_shaderProperties = new QStackedWidget(frame_material);
        stackedWidget_shaderProperties->setObjectName(QStringLiteral("stackedWidget_shaderProperties"));
        sizePolicy.setHeightForWidth(stackedWidget_shaderProperties->sizePolicy().hasHeightForWidth());
        stackedWidget_shaderProperties->setSizePolicy(sizePolicy);
        stackedWidget_shaderProperties->setMinimumSize(QSize(0, 69));
        stackedWidget_shaderProperties->setMaximumSize(QSize(16777215, 800));
        page_diffuse = new QWidget();
        page_diffuse->setObjectName(QStringLiteral("page_diffuse"));
        sizePolicy.setHeightForWidth(page_diffuse->sizePolicy().hasHeightForWidth());
        page_diffuse->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(page_diffuse);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_diffuse = new QLabel(page_diffuse);
        label_diffuse->setObjectName(QStringLiteral("label_diffuse"));

        gridLayout_3->addWidget(label_diffuse, 0, 0, 1, 1);

        label_specular = new QLabel(page_diffuse);
        label_specular->setObjectName(QStringLiteral("label_specular"));

        gridLayout_3->addWidget(label_specular, 1, 0, 1, 1);

        label_image_specular = new DropLabel(page_diffuse);
        label_image_specular->setObjectName(QStringLiteral("label_image_specular"));
        label_image_specular->setMinimumSize(QSize(64, 64));
        label_image_specular->setMaximumSize(QSize(64, 64));

        gridLayout_3->addWidget(label_image_specular, 1, 1, 1, 1);

        label_glow = new QLabel(page_diffuse);
        label_glow->setObjectName(QStringLiteral("label_glow"));

        gridLayout_3->addWidget(label_glow, 2, 0, 1, 1);

        label_image_glow = new DropLabel(page_diffuse);
        label_image_glow->setObjectName(QStringLiteral("label_image_glow"));
        label_image_glow->setMinimumSize(QSize(64, 64));
        label_image_glow->setMaximumSize(QSize(64, 64));

        gridLayout_3->addWidget(label_image_glow, 2, 1, 1, 1);

        label_image_diffuse = new DropLabel(page_diffuse);
        label_image_diffuse->setObjectName(QStringLiteral("label_image_diffuse"));
        label_image_diffuse->setMinimumSize(QSize(64, 64));
        label_image_diffuse->setMaximumSize(QSize(64, 64));

        gridLayout_3->addWidget(label_image_diffuse, 0, 1, 1, 1);

        stackedWidget_shaderProperties->addWidget(page_diffuse);
        label_diffuse->raise();
        label_image_specular->raise();
        label_specular->raise();
        label_glow->raise();
        label_image_glow->raise();
        label_image_diffuse->raise();
        page_diffuse_normal = new QWidget();
        page_diffuse_normal->setObjectName(QStringLiteral("page_diffuse_normal"));
        gridLayout_2 = new QGridLayout(page_diffuse_normal);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_diffuse_1 = new QLabel(page_diffuse_normal);
        label_diffuse_1->setObjectName(QStringLiteral("label_diffuse_1"));

        gridLayout_2->addWidget(label_diffuse_1, 0, 0, 1, 1);

        label_image_diffuse_1 = new DropLabel(page_diffuse_normal);
        label_image_diffuse_1->setObjectName(QStringLiteral("label_image_diffuse_1"));
        label_image_diffuse_1->setMinimumSize(QSize(64, 64));
        label_image_diffuse_1->setMaximumSize(QSize(64, 64));
        label_image_diffuse_1->setLayoutDirection(Qt::LeftToRight);
        label_image_diffuse_1->setScaledContents(false);

        gridLayout_2->addWidget(label_image_diffuse_1, 0, 1, 1, 1);

        label_normal_1 = new QLabel(page_diffuse_normal);
        label_normal_1->setObjectName(QStringLiteral("label_normal_1"));

        gridLayout_2->addWidget(label_normal_1, 1, 0, 1, 1);

        label_image_normal_1 = new DropLabel(page_diffuse_normal);
        label_image_normal_1->setObjectName(QStringLiteral("label_image_normal_1"));
        label_image_normal_1->setMinimumSize(QSize(64, 64));
        label_image_normal_1->setMaximumSize(QSize(64, 64));

        gridLayout_2->addWidget(label_image_normal_1, 1, 1, 1, 1);

        label_specular_1 = new QLabel(page_diffuse_normal);
        label_specular_1->setObjectName(QStringLiteral("label_specular_1"));

        gridLayout_2->addWidget(label_specular_1, 2, 0, 1, 1);

        label_image_specular_1 = new DropLabel(page_diffuse_normal);
        label_image_specular_1->setObjectName(QStringLiteral("label_image_specular_1"));
        label_image_specular_1->setMinimumSize(QSize(64, 64));
        label_image_specular_1->setMaximumSize(QSize(64, 64));

        gridLayout_2->addWidget(label_image_specular_1, 2, 1, 1, 1);

        label_glow_1 = new QLabel(page_diffuse_normal);
        label_glow_1->setObjectName(QStringLiteral("label_glow_1"));

        gridLayout_2->addWidget(label_glow_1, 3, 0, 1, 1);

        label_image_glow_1 = new DropLabel(page_diffuse_normal);
        label_image_glow_1->setObjectName(QStringLiteral("label_image_glow_1"));
        label_image_glow_1->setMinimumSize(QSize(64, 64));
        label_image_glow_1->setMaximumSize(QSize(64, 64));

        gridLayout_2->addWidget(label_image_glow_1, 3, 1, 1, 1);

        stackedWidget_shaderProperties->addWidget(page_diffuse_normal);

        gridLayout_5->addWidget(stackedWidget_shaderProperties, 1, 0, 1, 1);


        gridLayout->addWidget(frame_material, 1, 0, 1, 1);


        retranslateUi(Renderable);
        QObject::connect(comboBox_shaderSelect, SIGNAL(currentIndexChanged(int)), stackedWidget_shaderProperties, SLOT(setCurrentIndex(int)));

        stackedWidget_shaderProperties->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Renderable);
    } // setupUi

    void retranslateUi(QWidget *Renderable)
    {
        Renderable->setWindowTitle(QApplication::translate("Renderable", "Renderable", 0));
        label_model->setText(QApplication::translate("Renderable", "Model", 0));
        label_material->setText(QApplication::translate("Renderable", "Material", 0));
        toolButton_modelBrowse->setText(QApplication::translate("Renderable", "...", 0));
        toolButton_materialBrowse->setText(QApplication::translate("Renderable", "...", 0));
        comboBox_shaderSelect->clear();
        comboBox_shaderSelect->insertItems(0, QStringList()
         << QApplication::translate("Renderable", "Diffuse", 0)
         << QApplication::translate("Renderable", "Diffuse_Normal", 0)
        );
        label_diffuse->setText(QApplication::translate("Renderable", "Diffuse", 0));
        label_specular->setText(QApplication::translate("Renderable", "Specular", 0));
        label_image_specular->setText(QApplication::translate("Renderable", "TextLabel", 0));
        label_glow->setText(QApplication::translate("Renderable", "Glow", 0));
        label_image_glow->setText(QApplication::translate("Renderable", "TextLabel", 0));
        label_image_diffuse->setText(QApplication::translate("Renderable", "TextLabel", 0));
        label_diffuse_1->setText(QApplication::translate("Renderable", "Diffuse", 0));
        label_image_diffuse_1->setText(QApplication::translate("Renderable", "TextLabel", 0));
        label_normal_1->setText(QApplication::translate("Renderable", "Normal", 0));
        label_image_normal_1->setText(QApplication::translate("Renderable", "TextLabel", 0));
        label_specular_1->setText(QApplication::translate("Renderable", "Specular", 0));
        label_image_specular_1->setText(QApplication::translate("Renderable", "TextLabel", 0));
        label_glow_1->setText(QApplication::translate("Renderable", "Glow", 0));
        label_image_glow_1->setText(QApplication::translate("Renderable", "TextLabel", 0));
    } // retranslateUi

};

namespace Ui {
    class Renderable: public Ui_Renderable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENDERABLECOMPONENT_H
