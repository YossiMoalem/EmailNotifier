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

#include <QTimer>
#include "Pop3.h"
#include "Imap.h"
#include "emailNotifier.h"
#include "error.h"
#include "emailChecker.h"

//TODO: Convert to Singelton
emailNotifier::emailNotifier(QObject* parent) : QObject(parent), m_account(NULL), m_checkTimer(new QTimer(this)), m_emailChecker(new emailChecker)
{
   connect (m_checkTimer, SIGNAL(timeout()),
         this, SLOT(checkAccount()));
}

void emailNotifier::registerAccount( AccountSettings accountSettings, emailNotifiableIntf* handler )
{
   qDebug("EmailNotify:Reading config");
   EmailError status = Email_no_error;
   
   if (m_emailChecker)
      m_emailChecker->terminate();

   createAccount(accountSettings);
  //TODO: this should be ret account!
    m_updateInterval = accountSettings.updateInterval;
    m_handler = handler;

   if (m_account == NULL)
   {
     qDebug("EmailNotify:Error loading config");
     status = Email_invalid_config_file;
   }

   if (status != Email_no_error)
        m_handler->onAccountUpdated(status);// TODO: Replace with onUpdateError()
   else
      checkAccount ();
}
void emailNotifier::checkAccount ()
{
   if (m_account != NULL)
   {
      qDebug("EnailNotify:Checking Account");
      if (m_checkTimer->isActive())
         m_checkTimer->stop();
      m_emailChecker->start();
      m_emailChecker->checkAccount(m_account);
      m_checkTimer->start(m_updateInterval * 1000);
   }
}

void emailNotifier::createAccount(AccountSettings& settings)
{
  //TODO : add updateInterval
   if (m_account != NULL)
   {
      delete (m_account);
      m_account = NULL;
   }
   if (settings.type == AT_POP3)
   {
      qDebug("enailnotify:creating pop3 account");
      m_account = new Pop3(settings.host.c_str(),
            settings.port,
            settings.login.c_str(),
            settings.pass.c_str(),
            settings.ssl); 
   }
   else if (settings.type == AT_IMAP)
   {
      qDebug("EnailNotify:Creating Imap account");
      m_account = new Imap(settings.host.c_str(),
            settings.port,
            settings.login.c_str(),
            settings.pass.c_str(),
            settings.ssl); 
   } else if ( settings.type == AT_HOTMAIL)
   {
      qDebug("EnailNotify:Creating Hotmail account");
      m_account = new Hotmail( settings.login.c_str(),
            settings.pass.c_str() ); 
   } else if ( settings.type == AT_GMAIL)
   {
      qDebug("EnailNotify:Creating Gmail account");
      m_account = new Gmail( settings.login.c_str(),
            settings.pass.c_str() ); 
   } else if ( settings.type == AT_YAHOO)
   {
      qDebug("EnailNotify:Creating Yahoo account");
      m_account = new Yahoo( settings.login.c_str(),
            settings.pass.c_str() ); 
   } else {
      qDebug("EnailNotify:Unknown account type");
      m_account = NULL;
   }
   if (m_account != NULL)
      connect (m_account, SIGNAL(accountUpdated(int)), 
            this, SLOT(accountUpdated(int)), Qt::DirectConnection);
}

void emailNotifier::accountUpdated(int newMsgs)
{
   qDebug("EnailNotify:Got new account status");
   qDebug("EnailNotify:Account status has changed. Emiting new status(%d)", newMsgs);
    m_handler->onAccountUpdated(newMsgs);
}
