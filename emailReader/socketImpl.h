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

#ifndef SOCKET_IMPL_H
#define SOCKET_IMPL_H

#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/Log_Msg.h>
#include <ace/SSL/SSL_SOCK_Stream.h>
#include <ace/SSL/SSL_SOCK_Connector.h>
#define BUFF_SIZE     512

class SocketImplIntf
{
 public:
   virtual int connect  () = 0;
   virtual int send     ( const char* i_msg) = 0;
   virtual int recv     (char out_msg_buff[])const = 0;
   virtual int close    () = 0;
};

template <typename SOCK_STREAM, typename CONNECTOR>
class SocketImpl : public SocketImplIntf
{
 public:
   SocketImpl(const char* in_address, unsigned short portNum) :
                        m_server(portNum, in_address)
   {}
   virtual int connect  ();
   virtual int send     ( const char* i_msg);
   virtual int recv     (char out_msg_buff[])const;
   virtual int close    ();

   protected:
   ACE_INET_Addr        m_server;
   SOCK_STREAM          m_server_stream;
};

typedef SocketImpl<ACE_SOCK_Stream, ACE_SOCK_Connector> RegularSocketImpl;
typedef SocketImpl<ACE_SSL_SOCK_Stream, ACE_SSL_SOCK_Connector> SslSocketImpl;

template <typename SOCK_STREAM, typename CONNECTOR>
inline int SocketImpl<SOCK_STREAM, CONNECTOR>::recv(char out_msg[])const 
{
  if (out_msg == NULL)
  {
    return -1;
  } else {
    m_server_stream.recv(out_msg, BUFF_SIZE);
    ACE_DEBUG((LM_INFO, "Recieved %s\n", out_msg));
  }
  return 0;
}

template <typename SOCK_STREAM, typename CONNECTOR>
inline int SocketImpl<SOCK_STREAM, CONNECTOR>::send( const char* i_msg)
{
  int len = strlen (i_msg);
  ACE_DEBUG((LM_INFO, "Going to send %s \n", i_msg));
  m_server_stream.send_n(i_msg, len);
  return 0;
}

template <typename SOCK_STREAM, typename CONNECTOR>
inline int SocketImpl<SOCK_STREAM, CONNECTOR>::connect()
{
  CONNECTOR connector;
  return connector.connect(m_server_stream, m_server);
}

template <typename SOCK_STREAM, typename CONNECTOR>
inline int SocketImpl<SOCK_STREAM, CONNECTOR>::close()
{
  m_server_stream.close();
}
#endif
