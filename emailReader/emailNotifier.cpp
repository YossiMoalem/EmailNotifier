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

#include "Pop3.h"
#include "Imap.h"
#include "emailNotifier.h"
#include "error.h"

emailNotifier::emailNotifier() : m_account(NULL)
{ }

void emailNotifier::registerAccount( AccountSettings accountSettings, emailNotifiableIntf* handler )
{
   qDebug("EmailNotify:Reading config");
   EmailError status = Email_no_error;

   m_account = createAccount(accountSettings, handler);

   if (m_account == NULL)
   {
     qDebug("EmailNotify:Error loading config");
     status = Email_invalid_config_file;
   }

   if (status != Email_no_error)
        handler->onUpdateError(status);
   else
    m_account->start();
}

emailAccount* emailNotifier::createAccount(AccountSettings& settings, emailNotifiableIntf* i_handler)
{
  emailAccount* newAccount = NULL;


  switch (settings.type)
  {
    case AT_POP3:
      {
        qDebug("enailnotify:creating pop3 account");
        newAccount = new Pop3(settings.host.c_str(),
            settings.port,
            settings.login.c_str(),
            settings.pass.c_str(),
            settings.ssl,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_IMAP:
      {
        qDebug("EnailNotify:Creating Imap account");
        newAccount = new Imap(settings.host.c_str(),
            settings.port,
            settings.login.c_str(),
            settings.pass.c_str(),
            settings.ssl,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_HOTMAIL:
      {
        qDebug("EnailNotify:Creating Hotmail account");
        newAccount = new Hotmail( settings.login.c_str(),
            settings.pass.c_str() ,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_GMAIL:
      {
        qDebug("EnailNotify:Creating Gmail account");
        newAccount = new Gmail( settings.login.c_str(),
            settings.pass.c_str() ,
            settings.updateInterval,
            i_handler); 
        break;
      } 
    case AT_YAHOO:
      {
        qDebug("EnailNotify:Creating Yahoo account");
        newAccount = new Yahoo( settings.login.c_str(),
            settings.pass.c_str(),
            settings.updateInterval,
            i_handler ); 
        break;
      } 
    default:
      {
        qDebug("EnailNotify:Unknown account type");
        newAccount = NULL;
        break;
      }
  }
  return newAccount;
}

