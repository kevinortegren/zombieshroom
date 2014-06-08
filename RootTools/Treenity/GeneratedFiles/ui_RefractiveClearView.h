/********************************************************************************
** Form generated from reading UI file 'RefractiveClearView.ui'
**
** Created by: Qt User Interface Compiler version 5.2.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REFRACTIVECLEARVIEW_H
#define UI_REFRACTIVECLEARVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_refractiveClearView
{
public:

    void setupUi(QWidget *refractiveClearView)
    {
        if (refractiveClearView->objectName().isEmpty())
            refractiveClearView->setObjectName(QStringLiteral("refractiveClearView"));
        refractiveClearView->resize(400, 123);

        retranslateUi(refractiveClearView);

        QMetaObject::connectSlotsByName(refractiveClearView);
    } // setupUi

    void retranslateUi(QWidget *refractiveClearView)
    {
        refractiveClearView->setWindowTitle(QApplication::translate("refractiveClearView", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class refractiveClearView: public Ui_refractiveClearView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REFRACTIVECLEARVIEW_H
