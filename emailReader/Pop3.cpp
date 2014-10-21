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

#include "Pop3.h"
#include "Socket.h"
#include <string.h>
#include <sstream>
#include <ace/Log_Msg.h>

const char* Pop3::mUserCommand = "USER ";
const char* Pop3::mPasswordCommand = "PASS ";
const char* Pop3::mQuitCommand = "QUIT";
const char* Pop3::mStatCommand = "STAT";
/* Some values from RFC1939 */
#define POP3_RESPONSE_MAX_LEM 512

Pop3::Pop3(const std::string& i_serverAddress, 
            int port, 
            const std::string& i_uname, 
            const std::string& i_password, 
            bool ssl, 
            int updateInterval, 
            EmailNotifiableIntf* i_handler) : 
        EmailAccount(i_serverAddress, 
                        port, 
                        i_uname, 
                        i_password, 
                        ssl, 
                        updateInterval, 
                        i_handler)
{
   ACE_DEBUG((LM_INFO, "Pop3:Creating account for: %s@%s\n", i_uname.c_str(), i_serverAddress.c_str()));
}
Pop3::~Pop3 ()
{ }


EmailError Pop3::authenticate ()
{
   EmailError  status = Email_no_error;
   std::string answer;
   answer.reserve(POP3_RESPONSE_MAX_LEM);

   ACE_DEBUG((LM_INFO, "Pop3:Starting Authentication\n"));
   if (m_socket->send(mUserCommand + m_uname ) != Email_no_error || m_socket->receive (answer) != Email_no_error)
      status = Email_general_connection_error ;
   else 
      status = check_response(answer, Email_authentication_error);
   if (status == Email_no_error)
   {
      if (m_socket->send(mPasswordCommand + m_pass ) != Email_no_error || m_socket->receive (answer) != Email_no_error)
         status = Email_general_connection_error;
      else 
         status = check_response(answer, Email_authentication_error);
   }
   return status;
}

EmailError Pop3::getNumOfNewMsgs (int& r_newMsgs)
{
   EmailError status = Email_no_error;
   std::string answer;
   answer.reserve(POP3_RESPONSE_MAX_LEM);
   ACE_DEBUG((LM_INFO, "Pop3:Checking the account status\n"));
   if (m_socket->send( mStatCommand ) != Email_no_error ||m_socket->receive (answer) != Email_no_error)
      status = Email_general_connection_error;
   else
   {
      status = check_response(answer, Email_connection_invalid_response);
      if (status == Email_no_error )
      {
          const char* answerStr = answer.c_str();
         sscanf (answerStr, "+OK %d", &r_newMsgs);
         ACE_DEBUG((LM_INFO, "Pop3: got %d new messages\n", r_newMsgs));
      }
   }
   return status;
}

void Pop3::logout ()
{
   ACE_DEBUG((LM_INFO, "Pop3:Loging out\n"));
   m_socket->send(mQuitCommand);
}

const char* Pop3::okResponse() const
{
  return "OK";
}

const char* Pop3::errResponse() const
{
  return "NO";
}

/********************************************************\
 *                Hotmail
\ ********************************************************/

Hotmail::Hotmail(const std::string& i_uname, 
                    const std::string& i_password,
                    int updateInterval,  
                    EmailNotifiableIntf* i_handler) 
    : Pop3 ("pop3.live.com", 995, i_uname, i_password, true, updateInterval, i_handler) 
{
   m_uname += "@hotmail.com";
}

Hotmail::~Hotmail () {}

/********************************************************\
 *                     Yahoo 
\ ********************************************************/

Yahoo::Yahoo (const std::string& i_uname, 
                const std::string& i_password, 
                int updateInterval, 
                EmailNotifiableIntf* i_handler) : 
        Pop3("pop.mail.yahoo.com", 995, i_uname, i_password, true, updateInterval, i_handler) 
{
   m_uname += "@yahoo.com";
}

Yahoo::~Yahoo()
{}
