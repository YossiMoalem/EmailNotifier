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

#include "Imap.h"
#include "Socket.h"

#define IMAP_RESPONSE_MAX_LEM 512
/* Response from Server */
#define OK "OK"
#define ERR "NO"

Imap::Imap(const char* in_server_address, int in_port, const char* in_uname, const char* in_pass, bool in_ssl, emailNotifiableIntf* i_handler, QObject* parent) : 
               emailAccount(in_server_address, in_port, in_uname, in_pass, in_ssl, i_handler, parent)
{
   qDebug ("Imap:Creating account for: %s @ %s", in_uname, in_server_address);
}

Imap::~Imap() {}
EmailError  Imap::authenticate() 
{
   char buff[IMAP_RESPONSE_MAX_LEM] = {0};
   EmailError status = Email_no_error;
   qDebug ("Imap:Starting Authentication");
   sprintf(buff, ". login %s %s \r\n", m_uname.c_str(), m_pass.c_str());
   if (m_socket->send(buff) != Email_no_error || m_socket->receive (buff) != Email_no_error)
      status = Email_general_connection_error ;
   else 
      status = check_response(buff, Email_authentication_error);
   return status;
}

EmailError  Imap::getNumOfNewMsgs(int* r_newMsgs)
{
   EmailError status = Email_no_error;
   char buff[IMAP_RESPONSE_MAX_LEM] = {0};
   qDebug("Imap:Checking the account status");
   if (r_newMsgs == NULL)
   {
      status = Email_invalid_input;
   } else if (m_socket->send(". status INBOX (unseen)\r\n") != Email_no_error ||m_socket->receive (buff) != Email_no_error)
      status = Email_general_connection_error;
   else
   {
      if (status == Email_no_error )
      {
         sscanf (buff, "* STATUS \"INBOX\" (UNSEEN %d)", r_newMsgs);
         //TODO: Check msg
         qDebug ("Imap: got %d new messages", *r_newMsgs);
      }
   }
   return status;
}
void Imap::logout()
{
   char buff[IMAP_RESPONSE_MAX_LEM] = {0};
   qDebug ("Imap:Loging out");
   sprintf(buff, ". logout" /*getSetIdentifyer()*/);
   m_socket->send(buff);
}

EmailError Imap::check_response(char* in_buff, EmailError in_err_msg) const
{
   EmailError status = Email_no_error;
   if (strstr (in_buff, ERR) )
      status = in_err_msg;
   else if (strstr(in_buff, OK) == NULL)
      status = Email_connection_invalid_response;

   return status;
}

/********************************************************\
 *                      Gmail
\ ********************************************************/

Gmail::Gmail(const char* in_uname, const char* in_pass, emailNotifiableIntf* i_handler, QObject* parent) : Imap("imap.gmail.com", 993, in_uname, in_pass, true, i_handler, parent) 
{
   m_uname += "@gmail.com";
}

Gmail::~Gmail()
{}
