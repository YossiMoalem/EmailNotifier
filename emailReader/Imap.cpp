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

#include "Imap.h"
#include "Socket.h"
#include <ace/Log_Msg.h>

const char* const Imap::mLoginCommand = ". login ";
const char* const Imap::mStatusCommand = ". status INBOX (unseen)";
const char* const Imap::mLogoutCommand = ". logout ";

#define IMAP_RESPONSE_MAX_LEM 512

Imap::Imap(const std::string& i_serverAddress, 
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
   ACE_DEBUG((LM_INFO, "Imap:Creating account for: %s@%s\n", i_uname.c_str(), i_serverAddress.c_str()));
}

Imap::~Imap() {}

EmailError  Imap::authenticate() 
{
   std::string loginCommand (mLoginCommand);
   std::string answer;
   answer.reserve(IMAP_RESPONSE_MAX_LEM);
   EmailError status = Email_no_error;
   ACE_DEBUG((LM_INFO, "Imap:Starting Authentication\n"));
   if (m_socket->send( loginCommand + m_uname + " " + m_pass ) != Email_no_error || m_socket->receive (answer) != Email_no_error)
      status = Email_general_connection_error ;
   else 
      status = check_response(answer, Email_authentication_error);
   return status;
}

EmailError  Imap::getNumOfNewMsgs(int& r_newMsgs)
{
   EmailError status = Email_no_error;
   std::string answer;
   ACE_DEBUG((LM_INFO, "Imap:Checking the account status\n"));
   if (m_socket->send( mStatusCommand ) != Email_no_error ||m_socket->receive (answer) != Email_no_error)
  {
      status = Email_general_connection_error;
  }
   else
   {
       const char* answerStr = answer.c_str();
       sscanf (answerStr, "* STATUS \"INBOX\" (UNSEEN %d)", &r_newMsgs);
       ACE_DEBUG((LM_INFO, "Imap: got %d new messages\n", r_newMsgs));
   }
   return status;
}
void Imap::logout()
{
   ACE_DEBUG((LM_INFO, "Imap:Loging out\n"));
   m_socket->send( mLogoutCommand );
}

const char* Imap::okResponse() const
{
  return "OK";
}

const char* Imap::errResponse() const
{
  return "NO";
}

/********************************************************\
 *                      Gmail
\ ********************************************************/

//Gmail::Gmail(const char* i_uname, const char* i_pass, int updateInterval, EmailNotifiableIntf* i_handler) : Imap("imap.gmail.com", 993, i_uname, i_pass, true, updateInterval, i_handler) 
Gmail::Gmail(const std::string& i_uname, 
            const std::string& i_password, 
            int updateInterval, 
            EmailNotifiableIntf* i_handler) 
    : Imap("74.125.136.108", 993, i_uname, i_password, true, updateInterval, i_handler) 
{
   m_uname += "@gmail.com";
}

Gmail::~Gmail()
{}
