/********************************************************************************
** Form generated from reading UI file 'ParticleComponent.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PARTICLECOMPONENT_H
#define UI_PARTICLECOMPONENT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QWidget>
#include <RootTools/Treenity/Include/DropLineEdit.h>

QT_BEGIN_NAMESPACE

class Ui_Form_ParticleView
{
public:
    QGridLayout *gridLayout;
    QLabel *label_Particle;
    DropLineEdit *lineEdit_Particle;

    void setupUi(QWidget *Form_ParticleView)
    {
        if (Form_ParticleView->objectName().isEmpty())
            Form_ParticleView->setObjectName(QStringLiteral("Form_ParticleView"));
        Form_ParticleView->resize(400, 60);
        gridLayout = new QGridLayout(Form_ParticleView);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_Particle = new QLabel(Form_ParticleView);
        label_Particle->setObjectName(QStringLiteral("label_Particle"));

        gridLayout->addWidget(label_Particle, 0, 0, 1, 1);

        lineEdit_Particle = new DropLineEdit(Form_ParticleView);
        lineEdit_Particle->setObjectName(QStringLiteral("lineEdit_Particle"));

        gridLayout->addWidget(lineEdit_Particle, 0, 1, 1, 1);


        retranslateUi(Form_ParticleView);

        QMetaObject::connectSlotsByName(Form_ParticleView);
    } // setupUi

    void retranslateUi(QWidget *Form_ParticleView)
    {
        Form_ParticleView->setWindowTitle(QApplication::translate("Form_ParticleView", "Form", 0));
        label_Particle->setText(QApplication::translate("Form_ParticleView", "Particle", 0));
    } // retranslateUi

};

namespace Ui {
    class Form_ParticleView: public Ui_Form_ParticleView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PARTICLECOMPONENT_H
