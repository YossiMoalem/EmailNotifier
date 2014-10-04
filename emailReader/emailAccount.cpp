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

/* Some values from RFC1939 (Pop3)*/
#define ARG_MAX_LEN 40
#define RESPONSE_MAX_LEM 512
emailAccount::emailAccount (const char* in_server_address, int in_port, const char* in_uname,
      const char* in_pass, bool in_ssl, emailNotifiableIntf* i_handler , QObject* parent) : QObject (parent), m_port (in_port), m_ssl(in_ssl), m_connected(false), m_handler(i_handler)

{
   if (in_server_address)
      m_server_address = in_server_address;

   if (in_uname)
      m_uname = in_uname;

   if (in_pass)
      m_pass = in_pass;
   std::ostringstream sout;
   std::string portStr;
   sout << m_port;
   portStr = sout.str();
   m_socket = new Socket(m_server_address.c_str(), portStr.c_str(), m_ssl);
}

emailAccount::~emailAccount()
{
   delete m_socket;
}

EmailError emailAccount::connect ()
{
   qDebug ("EmailAccount:Connecting to server (%s)", m_server_address.c_str());
   EmailError status = Email_no_error;
   char buff[RESPONSE_MAX_LEM] = {0};
   /* Connect to Server */
   if (m_socket->connect() != Email_no_error)
   {
      qDebug ("EmailAccount:Cannot connect Socket");
      status = Email_connection_failed;
   }
   if (status == Email_no_error )
   { 
      if (m_socket->receive (buff) != Email_no_error)
         status = Email_connection_failed;
      else
         status = check_response(buff, Email_connection_failed);
   }
   return status;
}

void emailAccount::checkAccount() 
{
   qDebug ("EmailAccount: Checking Account");
   EmailError  status      = Email_no_error;

   int         new_msgs    = -1;
   if (m_connected == false) //TODO: use state of socket instead of this member
   {
      status = connect();
      if (status != Email_no_error)
      {
         qDebug ("EmailAccount: Error Connecting to server");
      } else {
         status = authenticate();
         if (status != Email_no_error )
         {
            qDebug ("EmailAccount: Error authenticating");
         } else {
            status = getNumOfNewMsgs(&new_msgs);
            if (status == Email_no_error)
               m_connected = true;
         }
      }
   } else { /* Already connected */
      qDebug ("EmailAccount: Attempting to use existing account");
      status = getNumOfNewMsgs(&new_msgs);
      if (status != Email_no_error)
      {
         qDebug("EmailAccount:Existing connection may have dropped. Attempting to re-connect");
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
      qDebug ("EmailAccount:Checking account finished successfully. Emiting %d", new_msgs);
      accountUpdated(new_msgs);
   } else {
      qDebug ("EmailAccount:Checking account finished with error. Emiting %d",status);
      accountUpdated(status); //TODO:
   }
}

void emailAccount::accountUpdated(int newMsgs)
{
   qDebug("EnailAccount:Account status has changed. Emiting new status(%d)", newMsgs);
    m_handler->onAccountUpdated(newMsgs);
}
