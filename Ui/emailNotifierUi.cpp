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

#include <QApplication>
#include <QVBoxLayout>
#include <QPainter>
#include <QPushButton>
#include <QTextEdit>
#include <QFont>
#include <sstream>

#include "emailNotifierUi.h"
#include "configUi.h"
#include "config.h"
#include "error.h"

static const char* checkingStatus = "Checking";

emailNotifierUi::emailNotifierUi() : QObject (), m_widget(new QWidget), m_ntf(new EmailNotifier)
{
   m_accountString = new QString(checkingStatus);
   m_accountStatus = new QTextEdit(m_widget);
   m_accountStatus->setReadOnly(true);
   m_accountStatus->setCurrentFont(QFont("Currier", 15, true));

   QVBoxLayout*  accountLayout = new QVBoxLayout(m_widget);
   accountLayout->addWidget(m_accountStatus);

   registerAccount();
   m_widget->setLayout(accountLayout);
   m_widget->resize (200, 200);
   m_widget->update();

  connect (this, SIGNAL(sigAccountUpdated(int)),
          this, SLOT(accountUpdated(int)));
}

void emailNotifierUi::onUpdateError(EmailError newStatus)
{
  //m_curStatus = newStatus; 
  emit (sigAccountUpdated (newStatus));
}
void emailNotifierUi::onAccountUpdated(int newMsgs)
{
  //m_curMsgs = newNsgs;
  emit(sigAccountUpdated(newMsgs));
}

void emailNotifierUi::accountUpdated(int newMsgs)
{
   qDebug ("Ui:Updating acount status");
   std::ostringstream sout;
   if (newMsgs >= 0)
   {
      qDebug("Ui:New account status has %d messages",newMsgs);
      sout <<"You have " << newMsgs << " new Messages";
   } else {
      qDebug("Ui:Error code %d was received", newMsgs);
      sout <<"Error : " << EmailErrorToStr((EmailError)newMsgs);
   }
   (*m_accountString) = sout.str().c_str();

   m_accountStatus->clear();
   m_accountStatus->insertPlainText(*m_accountString);
   m_widget->update();
}

void emailNotifierUi::showConfig()
{
   qDebug ("Ui:Show Config Ui");

   configUi*  cnf = new configUi(m_widget);
   connect (cnf, SIGNAL(configUpdated()),
            this, SLOT(registerAccount()));
   cnf->show();
}

void emailNotifierUi::registerAccount()
{
   m_accountStatus->clear();
   m_accountStatus->insertPlainText(checkingStatus);
   m_widget->update();

   AccountSettings accountSettings;
   ConfigurationError status =  accountSettings_load(&accountSettings); 
   if (status != Email_no_error)
   {
      qDebug("EmailNotify:Failed to load config");
   } else {
    //TODO: this should be saved per account, not as general setings
    GeneralSettings generalSettings;
    //TODO: retval
    generalSettings_load (&generalSettings);

     accountSettings.updateInterval = generalSettings.updateIntervalMin * 60 + generalSettings.updateIntervalSec;
     accountSettings.type = AT_GMAIL;
     accountSettings.login="moalem.yossi";
     accountSettings.pass="ymgetm01";
     m_ntf->registerAccount(accountSettings, this);
   }
}

const char* emailNotifierUi::statusStr () const
{
   return m_accountString->toUtf8();
}

QWidget* emailNotifierUi::widget()
{
   return m_widget;
}

