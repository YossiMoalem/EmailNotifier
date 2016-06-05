/*
 * Email Notify Version: 0.2
 * Author: Yossi Moalem
 * Email :  moalem.yossi@gmail.com
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

#include <pthread.h>
#include <ace/Log_Msg.h>

#include "error.h"
#include "emailNotifier.h"
#include "Pop3.h"
#include "Imap.h"

accountHndl EmailNotifier::registerAccount( const AccountSettings &  accountSettings, EmailNotifiableIntf* handler ) const
{
   EmailAccount* newAccount = createAccount(accountSettings, handler);

   if (newAccount == NULL)
   {
       ACE_DEBUG((LM_ERROR, "EmailNotify:Error creating acount \n"));
       handler->onUpdateError(Email_invalid_account_detils ); //TODO: replace error
   } else { 
    pthread_t workerTread;
    pthread_create(&workerTread, NULL, startChechingAccount, newAccount);
   }
  return newAccount;
}

EmailAccount* EmailNotifier::createAccount(const AccountSettings& settings, EmailNotifiableIntf* i_handler) const
{
  EmailAccount* newAccount = NULL;
  switch (settings.type)
  {
    case AT_POP3:
      {
        ACE_DEBUG((LM_INFO, "enailnotify:creating pop3 account\n"));
        newAccount = new Pop3(settings.host,
            settings.port,
            settings.login,
            settings.pass,
            settings.ssl,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_IMAP:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Imap account\n"));
        newAccount = new Imap(settings.host,
            settings.port,
            settings.login,
            settings.pass,
            settings.ssl,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_HOTMAIL:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Hotmail account\n"));
        newAccount = new Hotmail( settings.login,
            settings.pass ,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_GMAIL:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Gmail account\n"));
        newAccount = new Gmail( settings.login,
            settings.pass ,
            settings.updateInterval,
            i_handler); 
        break;
      } 
    case AT_YAHOO:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Yahoo account\n"));
        newAccount = new Yahoo( settings.login,
            settings.pass,
            settings.updateInterval,
            i_handler ); 
        break;
      } 
    default:
      {
        ACE_DEBUG((LM_ERROR, "EnailNotify:Unknown account type\n"));
        newAccount = NULL;
        break;
      }
  }
  return newAccount;
}
