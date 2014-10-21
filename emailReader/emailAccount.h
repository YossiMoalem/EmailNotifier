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

#ifndef EMAIL_ACCOUNT
#define EMAIL_ACCOUNT

#include <string>

#include "error.h"
#include "emailNotifier.h"

class Socket;

void* startChechingAccount (void* account);

class EmailAccount
{
friend void* startChechingAccount(void*);
   public:
      EmailAccount (const std::string& i_serverAddress, 
                    int port, 
                    const std::string& i_uname,
                    const std::string& i_password, 
                    bool ssl, 
                    int updateInterval, 
                    EmailNotifiableIntf* i_handler); 
      virtual ~EmailAccount() = 0;
      virtual void checkAccount();

   protected:
      std::string       m_server_address;
      unsigned short    m_port;
      std::string       m_uname;
      std::string       m_pass;
      bool              m_ssl;
      bool              m_connected;
      int               m_updateInterval;
      EmailNotifiableIntf* m_handler;
      Socket*           m_socket;

   protected:
      EmailError  connectToServer();
      EmailError  connectToAccount();
      virtual EmailError check_response (const std::string& response, EmailError i_error_msg) const;
      virtual EmailError  authenticate() = 0;
      virtual EmailError  getNumOfNewMsgs(int& r_numOfNewMsgs) = 0;
      virtual void        logout() = 0;
      virtual const char* okResponse() const = 0;
      virtual const char* errResponse() const = 0;

  private:
    void disconnect ();
};

#endif
