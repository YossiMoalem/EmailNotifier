/*
 * Email Notify Version: 0.1
 * Author: Yossi Mualem
 * Email :  ymgetm@gmail.com
 * 
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "configUi.h"
#include "config.h"

#include <QDialog>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QPushButton>

configUi::configUi (QWidget* parent) : QDialog(parent)
{
   m_stdBtn = new QDialogButtonBox(QDialogButtonBox::Save |
                                   QDialogButtonBox::Cancel);

   m_config = new QTabWidget;
   m_accountTab = new accountTab(this);
   m_generalTab = new generalTab(this);
   m_config->addTab(m_generalTab, tr("General"));
   m_config->addTab(m_accountTab, tr("Account"));

   QVBoxLayout* configUiLayout = new QVBoxLayout;
   configUiLayout->addWidget(m_config);
   configUiLayout->addStretch(10);
   configUiLayout->addWidget(m_stdBtn);

   connect (m_stdBtn, SIGNAL(accepted ()),
            this, SLOT(save()));
   connect (m_stdBtn, SIGNAL(rejected()),
            this, SLOT(close()));
   connect (m_accountTab->m_uname, SIGNAL(textChanged(QString)),
            this, SLOT(saveAccessibility(QString)));
   connect (m_accountTab->m_pass, SIGNAL(textChanged(QString)),
            this, SLOT(saveAccessibility(QString)));
   connect (m_accountTab->m_host, SIGNAL(textChanged(QString)),
            this, SLOT(saveAccessibility(QString)));

   saveAccessibility(NULL);
   resize (200, 200);

   setWindowTitle(tr("Configure"));
   setLayout (configUiLayout);
}

void configUi::save ()
{
   qDebug("ConfigUi: Saving configuration");
   m_accountTab->save();
   m_generalTab->save();
   emit (configUpdated());
   this->close();
   //TODO: Update Account
}
void configUi::saveAccessibility (QString /*text*/)
{
   int typeIndex = m_accountTab->m_typeCBox->currentIndex();
   QString type = m_accountTab->m_typeCBox->itemData(typeIndex).toString();
   bool enable = (! m_accountTab->m_host->text().isEmpty() ||
                  type.compare (EMAIL_TYPE_HOTMAIL) == 0 ||
                  type.compare (EMAIL_TYPE_GMAIL) == 0 ||
                  type.compare (EMAIL_TYPE_YAHOO) == 0);

   enable = enable && !m_accountTab->m_uname->text().isEmpty() 
            && !m_accountTab->m_pass->text().isEmpty();
   m_stdBtn->button(QDialogButtonBox::Save)->setEnabled(enable);
}

/*****************************************************************
 *                  Account Tab
 *****************************************************************/
accountTab::accountTab (QWidget* parent) : QWidget(parent)
{
   m_accountLayout = new QVBoxLayout(this);
   /* type */
   QHBoxLayout* typeLayout = new QHBoxLayout;
   m_typeCBox = new QComboBox ();
   m_typeCBox->addItem ("Pop3", EMAIL_TYPE_POP3);
   m_typeCBox->addItem ("Imap", EMAIL_TYPE_IMAP);
   m_typeCBox->addItem ("Hotmail (live)", EMAIL_TYPE_HOTMAIL);
   m_typeCBox->addItem ("Gmail", EMAIL_TYPE_GMAIL);
   m_typeCBox->addItem ("Yahoo", EMAIL_TYPE_YAHOO);
   typeLayout->addWidget(new QLabel (tr("Account Type : ")));
   typeLayout->addWidget(m_typeCBox);

   m_detailsLayout = new QGridLayout();
   m_host       = new QLineEdit();
   m_hostLbl    = new QLabel (tr("Host"));
   m_port       = new QSpinBox();
   m_portLbl    = new QLabel(tr("Port"));
   m_uname      = new QLineEdit();
   m_unameLbl   = new QLabel(tr("Login"));
   m_unameSuffixLbl = new QLabel();
   m_pass       = new QLineEdit();
   m_passLbl    = new QLabel(tr("Password"));
   m_ssl        = new QCheckBox();
   m_sslLbl     = new QLabel(tr("Use SSL"));
   m_port->setMinimum(1);
   m_port->setMaximum(65535);
   m_port->setValue(110);
   m_pass->setEchoMode (QLineEdit::Password);

   m_detailsLayout->addWidget(m_hostLbl, 0, 0);
   m_detailsLayout->addWidget(m_host, 0, 1);
   m_detailsLayout->addWidget(m_portLbl, 1, 0);
   m_detailsLayout->addWidget(m_port, 1, 1);
   m_detailsLayout->addWidget(m_unameLbl, 2, 0);
   m_detailsLayout->addWidget(m_uname, 2, 1);
   m_detailsLayout->addWidget(m_unameSuffixLbl, 2, 2);
   m_detailsLayout->addWidget(m_passLbl, 3, 0);
   m_detailsLayout->addWidget(m_pass, 3, 1);
   m_detailsLayout->addWidget(m_ssl, 4, 0);
   m_detailsLayout->addWidget(m_sslLbl, 4, 1);

   m_detailsLayout->setColumnStretch (1, 10);

   m_accountLayout->addLayout(typeLayout);
   m_accountLayout->addLayout(m_detailsLayout);
   load();

   /* Dialog */
   connect (m_typeCBox, SIGNAL(currentIndexChanged (int)),
         this, SLOT(accountTypeChanged(int)));
   connect (m_ssl, SIGNAL(stateChanged(int)),
            this, SLOT(setPort(int)));


   setLayout(m_accountLayout);
}

void accountTab::setPort (int sslState)
{
   if (m_typeCBox->currentText().compare("Pop3") == 0)
   {
      if (sslState == Qt::Checked)
         m_port->setValue (995);
      else
         m_port->setValue (110);
   } else if (m_typeCBox->currentText().compare("Imap") == 0)
   {
      if (sslState == Qt::Checked)
         m_port->setValue (993);
      else
         m_port->setValue (143);
   } 
}

void  accountTab::clear()
{
   m_host->clear();
   m_uname->clear();
   m_pass->clear();
   m_ssl->setChecked(false);
}

void accountTab::setElemsVis( int index) 
{

   QString type = m_typeCBox->itemData(index).toString();
   for (int i = 0; i < m_detailsLayout->count(); ++i)
      m_detailsLayout->itemAt(i)->widget()->hide();

   m_uname->show();
   m_unameLbl->show();
   m_unameSuffixLbl->show();
   m_pass->show();
   m_passLbl->show();

   if (type.compare (EMAIL_TYPE_POP3) == 0 ||
         type.compare (EMAIL_TYPE_IMAP) == 0)
   {
      m_host->show();
      m_host->show();
      m_hostLbl->show();
      m_port->show();
      m_portLbl->show();
      m_ssl->show();
      m_sslLbl->show();
   }
}

void accountTab::accountTypeChanged(int index)
{
   clear();
   updateUnameSuffix(index);
   setPort (m_ssl->checkState ());
   setElemsVis(index);
}
void accountTab::updateUnameSuffix (int index)
{
   QString type = m_typeCBox->itemData(index).toString();

   if (type.compare (EMAIL_TYPE_POP3) == 0 ||
         type.compare (EMAIL_TYPE_IMAP) == 0)
   {
      m_unameSuffixLbl->setText ("");
   } else if (type.compare(EMAIL_TYPE_HOTMAIL) == 0)
   {
       m_unameSuffixLbl->setText ("@hotmail.com");
   } else if (type.compare(EMAIL_TYPE_GMAIL) == 0)
   { 
      m_unameSuffixLbl->setText("@gmail.com"  );
   } else if (type.compare(EMAIL_TYPE_YAHOO) == 0)
   {
      m_unameSuffixLbl->setText ("@yahoo.com");
   }
}

void accountTab::save ()
{
   AccountSettings settings;
   settings.login = m_uname->text().toUtf8().constData();
   settings.pass  = m_pass->text().toUtf8().constData();
   std::string accountName = m_typeCBox->itemData(m_typeCBox->currentIndex()).toString().toUtf8().constData();
   settings.type  = accountNameToType(accountName);
   /*TODO: This shouod be transfered to config.cpp */
   if (settings.type == AT_POP3 || settings.type == AT_IMAP )
   {
      settings.host  = m_host->text().toUtf8().constData();
      settings.port  = (unsigned int)m_port->value();
      settings.ssl   = (m_ssl->checkState() > 0 );
   } else {
      settings.host  = '\0';
      settings.port  = 0;
      settings.ssl   = 0;
   }
   if (accountSettings_save(&settings) != E_ok)
      qDebug ("configUi:ERROR: Error when trying to write config file");
}
void accountTab::load ()
{
   AccountSettings settings;
   if ( accountSettings_load(&settings) == E_ok)
   {
      m_uname->setText(QString(settings.login.c_str()));
      m_pass->setText(QString(settings.pass.c_str()));
      const char* type = AccountTypeName[settings.type];
      int index = m_typeCBox->findData(type);
      if (index >= 0)
         m_typeCBox->setCurrentIndex(index);
      m_host->setText(QString(settings.host.c_str()));
      m_port->setValue(settings.port);
      m_ssl->setChecked (settings.ssl);

      updateUnameSuffix(index);
      setElemsVis(index);
   }
}

/*****************************************************************\
 *                  General Tab
\*****************************************************************/

generalTab::generalTab(QWidget* parent) : QWidget(parent)
{
   /* Update Interval */
   m_intervalMin = new QSpinBox;
   m_intervalSec = new QSpinBox;

   m_intervalMin->setMaximum (99);
   m_intervalSec->setMaximum (59);
   m_intervalMin->setMinimum (0);
   m_intervalSec->setMinimum (0);
   m_intervalMin->setValue(10);

   connect (m_intervalMin, SIGNAL(valueChanged(int)),
            this, SLOT(intervalMinChanged(int)));
   connect (m_intervalSec, SIGNAL(valueChanged(int)),
            this, SLOT(intervalSecChanged(int)));

   QHBoxLayout* intervalLayout = new QHBoxLayout;
   intervalLayout->addWidget(new QLabel (tr("Update every:")));
   intervalLayout->addWidget(m_intervalMin);
   intervalLayout->addWidget(new QLabel (tr("Min")));
   intervalLayout->addWidget(m_intervalSec);
   intervalLayout->addWidget(new QLabel (tr("Sec")));

   /* Play Sound */
   m_playSound = new QCheckBox(tr("Play Sound"));
   QGridLayout* playSoundLayout = new QGridLayout;
   QGroupBox* playSound = new QGroupBox (tr("Notifucation"));
   playSoundLayout->addWidget(m_playSound, 0, 0);
   playSound->setLayout(playSoundLayout);
   /*

   m_playDefault = new QRadioButton (tr("Default Sound"));
   m_playFile = new QRadioButton (tr("Choose:"));

   QGridLayout* playSoundFileLayout = new QGridLayout;
   playSoundFileLayout->addWidget(m_playDefault, 0, 0);
   playSoundFileLayout->addWidget(m_playFile, 1, 0);
   playSoundFileLayout->addWidget(new QLabel(tr("Choose2...")), 1, 1);
   playSoundFileLayout->setColumnStretch (1, 10);

   playSoundLayout->addLayout(playSoundFileLayout, 1, 1);
*/
   QGridLayout* generalTabLayout = new QGridLayout;
   generalTabLayout->addLayout (intervalLayout, 0, 0);
   generalTabLayout->addWidget (playSound, 1, 0);

   load();
   setLayout(generalTabLayout);

}
void generalTab::save()
{
   GeneralSettings settings;
   settings.updateIntervalMin = (unsigned int)m_intervalMin->value();
   settings.updateIntervalSec = (unsigned int)m_intervalSec->value();
   settings.playNotification = (m_playSound->checkState() > 0);
   if (generalSettings_save (&settings) != E_ok)
      qDebug ("configUi:ERROR: Error when trying to write config file");
}
void generalTab::load()
{
   GeneralSettings settings;
   if (generalSettings_load(&settings) == E_ok)
   {
      m_intervalMin->setValue(settings.updateIntervalMin);
      m_intervalSec->setValue(settings.updateIntervalSec);
      m_playSound->setChecked (settings.playNotification);
   }
}
void generalTab::intervalMinChanged(int newVal)
{
   if (newVal == 0)
      m_intervalSec->setMinimum(1);
   else 
      m_intervalSec->setMinimum(0);
}
void generalTab::intervalSecChanged(int newVal)
{
   if (newVal == 0)
      m_intervalMin->setMinimum(1);
   else 
      m_intervalMin->setMinimum(0);
}
