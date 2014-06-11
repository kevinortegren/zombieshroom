/********************************************************************************
** Form generated from reading UI file 'TerrainTool.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TERRAINTOOL_H
#define UI_TERRAINTOOL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TerrainTool
{
public:
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *spinBox_size;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox_strength;
    QLabel *label_4;
    QCheckBox *checkBox_autosmooth;
    QLabel *label_3;
    QComboBox *comboBox_options;
    QWidget *widget;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *buttonCircle;
    QPushButton *buttonGrid;
    QPushButton *pushButton_smoothAll;

    void setupUi(QWidget *TerrainTool)
    {
        if (TerrainTool->objectName().isEmpty())
            TerrainTool->setObjectName(QStringLiteral("TerrainTool"));
        TerrainTool->resize(739, 32);
        TerrainTool->setMinimumSize(QSize(495, 32));
        TerrainTool->setMaximumSize(QSize(16777215, 32));
        horizontalLayout = new QHBoxLayout(TerrainTool);
        horizontalLayout->setSpacing(3);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(TerrainTool);
        label->setObjectName(QStringLiteral("label"));
        label->setMaximumSize(QSize(25, 32));

        horizontalLayout->addWidget(label);

        spinBox_size = new QSpinBox(TerrainTool);
        spinBox_size->setObjectName(QStringLiteral("spinBox_size"));
        spinBox_size->setMaximumSize(QSize(40, 20));
        spinBox_size->setMinimum(-99);
        spinBox_size->setValue(2);

        horizontalLayout->addWidget(spinBox_size);

        label_2 = new QLabel(TerrainTool);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setMaximumSize(QSize(50, 32));

        horizontalLayout->addWidget(label_2);

        doubleSpinBox_strength = new QDoubleSpinBox(TerrainTool);
        doubleSpinBox_strength->setObjectName(QStringLiteral("doubleSpinBox_strength"));
        doubleSpinBox_strength->setMaximumSize(QSize(55, 20));
        doubleSpinBox_strength->setMinimum(-1000);
        doubleSpinBox_strength->setMaximum(1000);
        doubleSpinBox_strength->setSingleStep(0.5);
        doubleSpinBox_strength->setValue(2);

        horizontalLayout->addWidget(doubleSpinBox_strength);

        label_4 = new QLabel(TerrainTool);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setMaximumSize(QSize(45, 32));

        horizontalLayout->addWidget(label_4);

        checkBox_autosmooth = new QCheckBox(TerrainTool);
        checkBox_autosmooth->setObjectName(QStringLiteral("checkBox_autosmooth"));
        checkBox_autosmooth->setMaximumSize(QSize(16, 32));

        horizontalLayout->addWidget(checkBox_autosmooth);

        label_3 = new QLabel(TerrainTool);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setMaximumSize(QSize(70, 32));

        horizontalLayout->addWidget(label_3);

        comboBox_options = new QComboBox(TerrainTool);
        comboBox_options->setObjectName(QStringLiteral("comboBox_options"));
        comboBox_options->setMaximumSize(QSize(90, 20));

        horizontalLayout->addWidget(comboBox_options);

        widget = new QWidget(TerrainTool);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setMaximumSize(QSize(64, 16777215));
        horizontalLayout_2 = new QHBoxLayout(widget);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        buttonCircle = new QPushButton(widget);
        buttonCircle->setObjectName(QStringLiteral("buttonCircle"));
        buttonCircle->setMinimumSize(QSize(32, 32));
        buttonCircle->setMaximumSize(QSize(32, 32));
        buttonCircle->setCheckable(true);
        buttonCircle->setChecked(true);
        buttonCircle->setAutoExclusive(true);

        horizontalLayout_2->addWidget(buttonCircle);

        buttonGrid = new QPushButton(widget);
        buttonGrid->setObjectName(QStringLiteral("buttonGrid"));
        buttonGrid->setMinimumSize(QSize(32, 32));
        buttonGrid->setMaximumSize(QSize(32, 32));
        buttonGrid->setCheckable(true);
        buttonGrid->setAutoExclusive(true);

        horizontalLayout_2->addWidget(buttonGrid);


        horizontalLayout->addWidget(widget);

        pushButton_smoothAll = new QPushButton(TerrainTool);
        pushButton_smoothAll->setObjectName(QStringLiteral("pushButton_smoothAll"));
        pushButton_smoothAll->setMaximumSize(QSize(80, 20));

        horizontalLayout->addWidget(pushButton_smoothAll, 0, Qt::AlignLeft);


        retranslateUi(TerrainTool);

        QMetaObject::connectSlotsByName(TerrainTool);
    } // setupUi

    void retranslateUi(QWidget *TerrainTool)
    {
        TerrainTool->setWindowTitle(QApplication::translate("TerrainTool", "Form", 0));
        label->setText(QApplication::translate("TerrainTool", "Size:", 0));
#ifndef QT_NO_TOOLTIP
        spinBox_size->setToolTip(QApplication::translate("TerrainTool", "Brush size", 0));
#endif // QT_NO_TOOLTIP
        label_2->setText(QApplication::translate("TerrainTool", "Strength:", 0));
        label_4->setText(QApplication::translate("TerrainTool", "Smooth:", 0));
        checkBox_autosmooth->setText(QString());
        label_3->setText(QApplication::translate("TerrainTool", "Brush options:", 0));
        comboBox_options->clear();
        comboBox_options->insertItems(0, QStringList()
         << QApplication::translate("TerrainTool", "Default", 0)
         << QApplication::translate("TerrainTool", "Flat", 0)
         << QApplication::translate("TerrainTool", "Plateau", 0)
         << QApplication::translate("TerrainTool", "Smooth only", 0)
        );
        buttonCircle->setText(QString());
        buttonGrid->setText(QString());
        pushButton_smoothAll->setText(QApplication::translate("TerrainTool", "Smooth all", 0));
    } // retranslateUi

};

namespace Ui {
    class TerrainTool: public Ui_TerrainTool {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TERRAINTOOL_H
