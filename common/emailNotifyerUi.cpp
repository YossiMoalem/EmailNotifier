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

#include "emailNotifyerUi.h"
#include "configUi.h"
#include "emailNotifyer.h"

static const char* checkingStatus = "Checking";

emailNotifyerUi::emailNotifyerUi() : QObject (), m_widget(new QWidget), m_ntf(new emailNotifyer)
{
   m_accountString = new QString(checkingStatus);
   m_accountStatus = new QTextEdit(m_widget);
   m_accountStatus->setReadOnly(true);
   m_accountStatus->setCurrentFont(QFont("Currier", 15, true));

   QVBoxLayout*  accountLayout = new QVBoxLayout(m_widget);
   accountLayout->addWidget(m_accountStatus);

   connect (m_ntf, SIGNAL(accountChanged(int)),
            this, SLOT(accountUpdated(int)));
   updateAccount();
   m_widget->setLayout(accountLayout);
   m_widget->resize (200, 200);
   m_widget->update();
}

void emailNotifyerUi::accountUpdated(int newMsgs)
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

void emailNotifyerUi::showConfig()
{
   qDebug ("Ui:Show Config Ui");

   configUi*  cnf = new configUi(m_widget);
   connect (cnf, SIGNAL(configUpdated()),
            this, SLOT(updateAccount()));
   cnf->show();
}

void emailNotifyerUi::updateAccount()
{
   m_accountStatus->clear();
   m_accountStatus->insertPlainText(checkingStatus);
   m_widget->update();

   m_ntf->readConfigAndUpdate();
}

const char* emailNotifyerUi::statusStr () const
{
   return m_accountString->toUtf8();
}

QWidget* emailNotifyerUi::widget()
{
   return m_widget;
}

