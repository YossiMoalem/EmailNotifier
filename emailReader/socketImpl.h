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

#ifndef SOCKET_IMPL_H
#define SOCKET_IMPL_H

#include <string>

#include <ace/INET_Addr.h>
#include <ace/SOCK_Stream.h>
#include <ace/SOCK_Connector.h>
#include <ace/Log_Msg.h>
#include <ace/SSL/SSL_SOCK_Stream.h>
#include <ace/SSL/SSL_SOCK_Connector.h>

#define NL "\r\n"

/********************************************************\
 *                      Interface
\ ********************************************************/
class SocketImplIntf
{
 public:
  SocketImplIntf() : m_connectionTimeout(5) ,
                      m_sendTimeout(2),
                      m_recvTimeout(2)
  {}
   virtual EmailError  connect  () = 0;
   virtual EmailError  send     ( const char* i_msg ) = 0;
   virtual EmailError  recv     ( std::string& o_msg ) const = 0;
   virtual EmailError  close    () = 0;

   protected:
   unsigned int m_connectionTimeout;
   unsigned int m_sendTimeout;
   unsigned int m_recvTimeout;
};

/********************************************************\
 *                      Declerations
\ ********************************************************/
template <typename SOCK_STREAM, typename CONNECTOR>
class SocketImpl : public SocketImplIntf
{
 public:
   SocketImpl(const std::string& i_address, unsigned short portNum) :
                        m_server(portNum, i_address.c_str())
   {}
   virtual EmailError connect  ();
   virtual EmailError send     ( const char* i_msg);
   virtual EmailError recv     (std::string& o_msg) const;
   virtual EmailError close    ();

   protected:
   ACE_INET_Addr        m_server;
   SOCK_STREAM          m_server_stream;

   private:
   EmailError handleError(const char* opperation) const;
};

/********************************************************/
typedef SocketImpl<ACE_SOCK_Stream, ACE_SOCK_Connector> RegularSocketImpl;
typedef SocketImpl<ACE_SSL_SOCK_Stream, ACE_SSL_SOCK_Connector> SslSocketImpl;


/********************************************************\
 *                      Implementations
\ ********************************************************/
template <typename SOCK_STREAM, typename CONNECTOR>
inline EmailError SocketImpl<SOCK_STREAM, CONNECTOR>::connect()
{
  ACE_OS::last_error(0);
  ACE_Time_Value connectionTimeout(m_connectionTimeout);
  CONNECTOR connector;
  int status = connector.connect(m_server_stream, m_server, &connectionTimeout);
  if (status != 0)
  {
    return handleError("connecting");
  }
  return Email_no_error;
}

/********************************************************/
template <typename SOCK_STREAM, typename CONNECTOR>
inline EmailError SocketImpl<SOCK_STREAM, CONNECTOR>::recv(std::string& o_msg)const 
{
  ACE_OS::last_error(0);
  ACE_Time_Value recvTimeout(m_recvTimeout);
  EmailError status = Email_no_error;
  iovec response;
  int recvStatus = m_server_stream.recvv(&response, &recvTimeout);
  if (recvStatus < 0)
  {
    status = handleError("recieving");
  } else {
    o_msg = reinterpret_cast<char*>( response.iov_base );
    ACE_DEBUG((LM_INFO, "Recieved %s\n", o_msg.c_str()));
  }
  delete [] (reinterpret_cast<char*> (response.iov_base));
  return status;
}

/********************************************************/
template <typename SOCK_STREAM, typename CONNECTOR>
inline EmailError SocketImpl<SOCK_STREAM, CONNECTOR>::send( const char* i_msg)
{
  ACE_OS::last_error(0);
  ACE_Time_Value sendTimeout(m_sendTimeout);
  unsigned int commandLength = strlen(i_msg) + strlen(NL);
  char command[commandLength + 1];
  strcpy(command, i_msg);
  strcat(command, NL);
  ACE_DEBUG((LM_INFO, "Going to send |%s|\n", command));
  int status = m_server_stream.send_n(command, commandLength, &sendTimeout);
  if (status < 0)
  {
    return handleError("sending");
  } 
  return Email_no_error;
}

/********************************************************/
template <typename SOCK_STREAM, typename CONNECTOR>
inline EmailError SocketImpl<SOCK_STREAM, CONNECTOR>::close()
{
  m_server_stream.close();
  return Email_no_error;
}

/********************************************************/
template <typename SOCK_STREAM, typename CONNECTOR>
inline EmailError SocketImpl<SOCK_STREAM, CONNECTOR>::handleError(const char* opperation) const
{
    int lastError = ACE_OS::last_error();
    if (lastError == ETIME)
    {
      ACE_DEBUG((LM_WARNING, "SocketImpl: Time out while %s\n", opperation));
      return Email_time_out;
    } else {
      ACE_DEBUG((LM_ERROR, "SocketImpl: Error %s. Last error = %d\n", opperation, lastError));
      return Email_connection_failed;
    }
}
#undef NL
#endif
