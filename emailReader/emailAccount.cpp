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

#include "emailAccount.h"
#include "Socket.h"
#include <sstream>
#include <ace/Log_Msg.h>

void* startChechingAccount (void* account)
{
   for (;;)
   {
     ((EmailAccount*)account)->checkAccount();
     sleep(((EmailAccount*)account)->m_updateInterval);
   }
   return NULL;
}


/* Some values from RFC1939 (Pop3)*/
#define ARG_MAX_LEN 40
#define RESPONSE_MAX_LEM 512
EmailAccount::EmailAccount (const std::string& i_serverAddress, 
                            int port, 
                            const std::string&  i_uname,
                            const std::string& i_pass, 
                            bool ssl, 
                            int updateInterval, 
                            EmailNotifiableIntf* i_handler ) 
    : m_port (port), 
    m_ssl(ssl), 
    m_server_address (i_serverAddress),
    m_pass( i_pass),
    m_connected(false), 
    m_updateInterval (updateInterval), 
    m_handler(i_handler)
{
    m_uname = i_uname;
    m_uname.erase(m_uname.find_last_not_of(" \n\r\t")+1);

   m_socket = new Socket(m_server_address.c_str(), m_port, m_ssl);
}

EmailAccount::~EmailAccount()
{
   delete m_socket;
}

EmailError EmailAccount::connectToServer ()
{
   EmailError status = Email_no_error;
   ACE_DEBUG((LM_INFO, "EmailAccount:Connecting to server (%s:%d)\n", m_server_address.c_str(), m_port));
   std::string answer;
   /* Connect to Server */
   if (m_socket->connect() != Email_no_error)
   {
      ACE_DEBUG((LM_ERROR, "EmailAccount:Cannot connect Socket\n"));
      status = Email_connection_failed;
   } else { 
      if (m_socket->receive (answer) != Email_no_error)
         status = Email_connection_failed;
      else
         status = check_response(answer, Email_connection_failed);
   }
   return status;
}

EmailError EmailAccount::connectToAccount()
{
      EmailError status = connectToServer();
      if (status != Email_no_error)
      {
         ACE_DEBUG((LM_ERROR, "EmailAccount: Error Connecting to server\n"));
      } else {
         status = authenticate();
         if (status != Email_no_error )
         {
            ACE_DEBUG((LM_WARNING, "EmailAccount: Error authenticating\n"));
         } else {
              m_connected = true;
         }
      }
      return status;
}
void EmailAccount::checkAccount() 
{
   ACE_DEBUG((LM_INFO, "EmailAccount: Checking Account\n"));
   EmailError  status      = Email_no_error;
   int         newMsgs    = -1;

   if (m_connected == false) //TODO: use state of socket instead of this member
   {
       status = connectToAccount();
        if (status == Email_no_error)
        {
            status = getNumOfNewMsgs(newMsgs);
        }
   } else { /* Already connected */
      ACE_DEBUG((LM_INFO, "EmailAccount: Attempting to use existing account\n"));
      status = getNumOfNewMsgs(newMsgs);
      if (status != Email_no_error)
      {
         ACE_DEBUG((LM_INFO, "EmailAccount:Existing connection may have dropped. Attempting to re-connect\n"));
         disconnect();
         checkAccount();
      }
   }
   if (status == Email_no_error)
   {
      ACE_DEBUG((LM_INFO, "EmailAccount:Checking account finished successfully. Emiting %d\n", newMsgs));
        m_handler->onAccountUpdated(newMsgs);
   } else {
      ACE_DEBUG((LM_WARNING, "EmailAccount:Checking account finished with error. Emiting %d\n",status));
        m_handler->onUpdateError(status);
   }
}

void EmailAccount::disconnect()
{
    logout();
    m_socket->close();
    m_connected = false;
}
EmailError EmailAccount::check_response(const std::string& response, EmailError i_err_msg) const
{
   EmailError status = Email_no_error;
   if (response.find( errResponse() ) != std::string::npos)
      status = i_err_msg;
   else if (response.find( okResponse() ) == std::string::npos)
      status = Email_connection_invalid_response;

   return status;
}
