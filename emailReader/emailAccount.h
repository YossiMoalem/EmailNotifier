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

#ifndef EMAIL_ACCOUNT
#define EMAIL_ACCOUNT

#include <QObject>
#include <string>
#include "error.h"

class Socket;
using namespace std;

class emailAccount : public QObject
{
   Q_OBJECT
   protected:
      string       m_server_address;
      int          m_port;
      string       m_uname;
      string       m_pass;
      bool         m_ssl;
      bool         m_connected;

      Socket*      m_socket;


      EmailError  connect();
      virtual EmailError  authenticate() = 0;
      virtual EmailError  getNumOfNewMsgs(int* r_numOfNewMsgs) = 0;
      virtual void        logout() = 0;
      virtual EmailError check_response (char* in_buff, EmailError in_error_msg) const = 0;

   public:
      emailAccount (const char* in_server_address, int in_port, const char* in_uname,
            const char* in_pass, bool in_ssl, QObject* parent ); 
      virtual ~emailAccount() = 0;
      virtual void checkAccount();

   signals:
      void accountUpdated (int mewMsgs);
};

#endif
