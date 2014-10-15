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
#include <pthread.h>
#include <ace/Log_Msg.h>

accountHndl emailNotifier::registerAccount( AccountSettings accountSettings, emailNotifiableIntf* handler )
{
   ACE_DEBUG((LM_INFO, "EmailNotify:Reading config\n"));
   EmailError status = Email_no_error;

   emailAccount* newAccount = createAccount(accountSettings, handler);

   if (newAccount == NULL)
   {
     ACE_DEBUG((LM_ERROR, "EmailNotify:Error loading config\n"));
     status = Email_invalid_config_file;
   }

   if (status != Email_no_error)
        handler->onUpdateError(status);
   else
   { 
    pthread_t workerTreag;
    pthread_create(&workerTreag, NULL, startChechingAccount, newAccount);
   }

  return newAccount;
}

emailAccount* emailNotifier::createAccount(AccountSettings& settings, emailNotifiableIntf* i_handler)
{
  emailAccount* newAccount = NULL;


  switch (settings.type)
  {
    case AT_POP3:
      {
        ACE_DEBUG((LM_INFO, "enailnotify:creating pop3 account\n"));
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
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Imap account\n"));
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
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Hotmail account\n"));
        newAccount = new Hotmail( settings.login.c_str(),
            settings.pass.c_str() ,
            settings.updateInterval,
            i_handler); 
        break;
      }
    case AT_GMAIL:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Gmail account\n"));
        newAccount = new Gmail( settings.login.c_str(),
            settings.pass.c_str() ,
            settings.updateInterval,
            i_handler); 
        break;
      } 
    case AT_YAHOO:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Creating Yahoo account\n"));
        newAccount = new Yahoo( settings.login.c_str(),
            settings.pass.c_str(),
            settings.updateInterval,
            i_handler ); 
        break;
      } 
    default:
      {
        ACE_DEBUG((LM_INFO, "EnailNotify:Unknown account type\n"));
        newAccount = NULL;
        break;
      }
  }
  return newAccount;
}

