/********************************************************************************
** Form generated from reading UI file 'Settings.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QDialogButtonBox *buttonBox;
    QGroupBox *WriteOptionsGroupBox;
    QCheckBox *FlashProgramMemorycheckBox;
    QCheckBox *ConfigBitsCheckBox;
    QCheckBox *EepromCheckBox;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QString::fromUtf8("Settings"));
        Settings->resize(402, 190);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/MainWindow/img/microchip.png"), QSize(), QIcon::Normal, QIcon::Off);
        Settings->setWindowIcon(icon);
        buttonBox = new QDialogButtonBox(Settings);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 150, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        WriteOptionsGroupBox = new QGroupBox(Settings);
        WriteOptionsGroupBox->setObjectName(QString::fromUtf8("WriteOptionsGroupBox"));
        WriteOptionsGroupBox->setGeometry(QRect(20, 10, 361, 131));
        FlashProgramMemorycheckBox = new QCheckBox(WriteOptionsGroupBox);
        FlashProgramMemorycheckBox->setObjectName(QString::fromUtf8("FlashProgramMemorycheckBox"));
        FlashProgramMemorycheckBox->setGeometry(QRect(17, 30, 201, 19));
        ConfigBitsCheckBox = new QCheckBox(WriteOptionsGroupBox);
        ConfigBitsCheckBox->setObjectName(QString::fromUtf8("ConfigBitsCheckBox"));
        ConfigBitsCheckBox->setGeometry(QRect(17, 60, 201, 19));
        EepromCheckBox = new QCheckBox(WriteOptionsGroupBox);
        EepromCheckBox->setObjectName(QString::fromUtf8("EepromCheckBox"));
        EepromCheckBox->setGeometry(QRect(16, 90, 201, 19));

        retranslateUi(Settings);
        QObject::connect(buttonBox, SIGNAL(accepted()), Settings, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Settings, SLOT(reject()));

        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QApplication::translate("Settings", "Settings", 0, QApplication::UnicodeUTF8));
        WriteOptionsGroupBox->setTitle(QApplication::translate("Settings", "Write Options", 0, QApplication::UnicodeUTF8));
        FlashProgramMemorycheckBox->setText(QApplication::translate("Settings", "FLASH Program Memory", 0, QApplication::UnicodeUTF8));
        ConfigBitsCheckBox->setText(QApplication::translate("Settings", "Config Bits", 0, QApplication::UnicodeUTF8));
        EepromCheckBox->setText(QApplication::translate("Settings", "EEPROM", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
