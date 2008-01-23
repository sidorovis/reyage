/********************************************************************************
** Form generated from reading ui file 'command_list.ui'
**
** Created: Wed 23. Jan 20:22:22 2008
**      by: Qt User Interface Compiler version 4.3.1
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_COMMAND_LIST_H
#define UI_COMMAND_LIST_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QListWidget>

class Ui_commandList
{
public:
    QGridLayout *gridLayout;
    QListWidget *listWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *commandList)
    {
    if (commandList->objectName().isEmpty())
        commandList->setObjectName(QString::fromUtf8("commandList"));
    commandList->resize(215, 325);
    gridLayout = new QGridLayout(commandList);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    listWidget = new QListWidget(commandList);
    listWidget->setObjectName(QString::fromUtf8("listWidget"));

    gridLayout->addWidget(listWidget, 0, 0, 1, 1);

    buttonBox = new QDialogButtonBox(commandList);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::NoButton|QDialogButtonBox::Ok);

    gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


    retranslateUi(commandList);
    QObject::connect(buttonBox, SIGNAL(accepted()), commandList, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), commandList, SLOT(reject()));

    QMetaObject::connectSlotsByName(commandList);
    } // setupUi

    void retranslateUi(QDialog *commandList)
    {
    commandList->setWindowTitle(QApplication::translate("commandList", "Possible commands", 0, QApplication::UnicodeUTF8));
    Q_UNUSED(commandList);
    } // retranslateUi

};

namespace Ui {
    class commandList: public Ui_commandList {};
} // namespace Ui

#endif // UI_COMMAND_LIST_H
