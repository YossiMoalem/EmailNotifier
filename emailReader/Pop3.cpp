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
#include "Socket.h"
#include <string.h>
#include <sstream>


/* Some values from RFC1939 (Pop3)*/
#define ARG_MAX_LEN 40
#define POP3_RESPONSE_MAX_LEM 512
/* Response from Server */
#define OK "+OK"
#define ERR "-ERR"

Pop3::Pop3(const char* in_server_address, int in_port, const char* in_uname, const char* in_pass, bool in_ssl, emailNotifiableIntf* i_handler) : 
               emailAccount(in_server_address, in_port, in_uname, in_pass, in_ssl, i_handler)
{
   qDebug ("Pop3:Creating account for: %s @ %s", in_uname, in_server_address);
}
Pop3::~Pop3 ()
{ }


EmailError Pop3::authenticate ()
{
   EmailError  status                  = Email_no_error;
   char        buff[POP3_RESPONSE_MAX_LEM]  = {0};

   qDebug ("Pop3:Starting Authentication");
   sprintf(buff, "USER %s\r\n", m_uname.c_str());
   if (m_socket->send(buff) != Email_no_error || m_socket->receive (buff) != Email_no_error)
      status = Email_general_connection_error ;
   else 
      status = check_response(buff, Email_authentication_error);
   if (status == Email_no_error)
   {
      sprintf(buff, "PASS %s\r\n", m_pass.c_str());
      if (m_socket->send(buff) != Email_no_error || m_socket->receive (buff) != Email_no_error)
         status = Email_general_connection_error;
      else 
         status = check_response(buff, Email_authentication_error);
   }
   return status;
}

EmailError Pop3::getNumOfNewMsgs (int* r_newMsgs)
{
   EmailError status = Email_no_error;
   char buff[POP3_RESPONSE_MAX_LEM] = {0};
   qDebug("Pop3:Checking the account status");
   if (r_newMsgs == NULL)
   {
      status = Email_invalid_input;
   } else if (m_socket->send("STAT\r\n") != Email_no_error ||m_socket->receive (buff) != Email_no_error)
      status = Email_general_connection_error;
   else
   {
      status = check_response(buff, Email_connection_invalid_response);
      if (status == Email_no_error )
      {
         sscanf (buff, "+OK %d", r_newMsgs);
         qDebug ("Pop3: got %d new messages", *r_newMsgs);
      }
   }
   return status;
}

void Pop3::logout ()
{
   qDebug ("Pop3:Loging out");
   m_socket->send("QUIT\r\n");
}
EmailError Pop3::check_response(char* in_buff, EmailError in_err_msg) const
{
   *(strchr(in_buff, '\n')) = '\0';
   *(strchr(in_buff, '\r')) = '\0';
   EmailError status = Email_no_error;
   if (strstr (in_buff, ERR) == in_buff)
      status = in_err_msg;
   else if (strstr(in_buff, OK) != in_buff)
      status = Email_connection_invalid_response;

   return status;
}
/********************************************************\
 *                Hotmail
\ ********************************************************/

Hotmail::Hotmail(const char* in_uname, const char* in_pass, emailNotifiableIntf* i_handler) : Pop3 ("pop3.live.com", 995, in_uname, in_pass, true, i_handler) 
{
   m_uname += "@hotmail.com";
}

Hotmail::~Hotmail () {}

/********************************************************\
 *                     Yahoo 
\ ********************************************************/

Yahoo::Yahoo (const char* in_uname, const char* in_pass, emailNotifiableIntf* i_handler) : Pop3("pop.mail.yahoo.com", 995, in_uname, in_pass, true, i_handler) 
{
   m_uname += "@yahoo.com";
}

Yahoo::~Yahoo()
{}
