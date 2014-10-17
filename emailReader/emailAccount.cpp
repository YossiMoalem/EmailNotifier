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

#include "emailAccount.h"
#include "Socket.h"
#include <sstream>
#include <ace/Log_Msg.h>

void* startChechingAccount (void* account)
{
   for (;;)
   {
     ((emailAccount*)account)->checkAccount();
     sleep(((emailAccount*)account)->m_updateInterval);
   }
   return NULL;
}


/* Some values from RFC1939 (Pop3)*/
#define ARG_MAX_LEN 40
#define RESPONSE_MAX_LEM 512
emailAccount::emailAccount (const char* in_server_address, int in_port, const char* in_uname,
      const char* in_pass, bool in_ssl, int updateInterval, emailNotifiableIntf* i_handler ) 
: m_port (in_port), m_ssl(in_ssl), m_connected(false), m_updateInterval (updateInterval), m_handler(i_handler)

{
   if (in_server_address)
      m_server_address = in_server_address;

   if (in_uname)
   {
      m_uname = in_uname;
     m_uname.erase(m_uname.find_last_not_of(" \n\r\t")+1);
   }

   if (in_pass)
      m_pass = in_pass;
   m_socket = new Socket(m_server_address.c_str(), m_port, m_ssl);
}

emailAccount::~emailAccount()
{
   delete m_socket;
}

EmailError emailAccount::connect ()
{
   EmailError status = Email_no_error;
   ACE_DEBUG((LM_INFO, "EmailAccount:Connecting to server (%s:%d)\n", m_server_address.c_str(), m_port));
   std::string answer;
   /* Connect to Server */
   if (m_socket->connect() != Email_no_error)
   {
      ACE_DEBUG((LM_ERROR, "EmailAccount:Cannot connect Socket\n"));
      status = Email_connection_failed;
   }
   if (status == Email_no_error )
   { 
      if (m_socket->receive (answer) != Email_no_error)
         status = Email_connection_failed;
      else
         status = check_response(answer, Email_connection_failed);
   }
   return status;
}

void emailAccount::checkAccount() 
{
   ACE_DEBUG((LM_INFO, "EmailAccount: Checking Account\n"));
   EmailError  status      = Email_no_error;

   int         new_msgs    = -1;
   if (m_connected == false) //TODO: use state of socket instead of this member
   {
      status = connect();
      if (status != Email_no_error)
      {
         ACE_DEBUG((LM_ERROR, "EmailAccount: Error Connecting to server\n"));
      } else {
         status = authenticate();
         if (status != Email_no_error )
         {
            ACE_DEBUG((LM_WARNING, "EmailAccount: Error authenticating\n"));
         } else {
            status = getNumOfNewMsgs(&new_msgs);
            if (status == Email_no_error)
               m_connected = true;
         }
      }
   } else { /* Already connected */
      ACE_DEBUG((LM_INFO, "EmailAccount: Attempting to use existing account\n"));
      status = getNumOfNewMsgs(&new_msgs);
      if (status != Email_no_error)
      {
         ACE_DEBUG((LM_INFO, "EmailAccount:Existing connection may have dropped. Attempting to re-connect\n"));
         logout();
         m_socket->close();
         m_connected = false;
         checkAccount();
      }
   }
#if 0 // TODO: This should be under config/member should_Disconnect
   logout();
   m_socket->close();
   m_connected = false;
#endif
   if (status == Email_no_error)
   {
      ACE_DEBUG((LM_INFO, "EmailAccount:Checking account finished successfully. Emiting %d\n", new_msgs));
      accountUpdated(new_msgs);
   } else {
      ACE_DEBUG((LM_WARNING, "EmailAccount:Checking account finished with error. Emiting %d\n",status));
      accountUpdated(status); //TODO:
   }
}

void emailAccount::accountUpdated(int newMsgs)
{
   ACE_DEBUG((LM_INFO, "EnailAccount:Account status has changed. Emiting new status(%d)\n", newMsgs));
    m_handler->onAccountUpdated(newMsgs);
}
