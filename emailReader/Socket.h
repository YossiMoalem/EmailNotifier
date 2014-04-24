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

#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <openssl/ssl.h>
#include "error.h"


using namespace std;

class Socket
{
   private:
   string      m_address;
   string      m_port;
   bool        m_ssl;
   int         m_socfd;
   SSL_CTX*    m_ctx;
   SSL*        m_sslfd;
   struct addrinfo*  m_srv_addr_info;
   static bool s_sslLoaded;

   EmailError    resolveHost();
   EmailError    setSsl();

   public:
   Socket (const char* in_address, const char* in_port, bool ssl); 
   ~Socket();

   EmailError  connect           ();
   int         send              (const char* in_msg)const ;
   EmailError  receive           (char out_msg_buff[])const ;
   EmailError  close             ();
};

#endif
