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

#include "Socket.h"
#include <netdb.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>


#define SOC_TIMEOUT   7
#define BUFF_SIZE     512

#ifdef DEBUG
#include <stdio.h>
#define debug printf
#else
#define debug noop
void noop (const char*, ...) {}
#endif
bool Socket::s_sslLoaded(false);

Socket::Socket (const char* in_address, const char* in_port, bool in_ssl) : 
                        m_ssl(in_ssl), 
                        m_socfd(NULL), 
                        m_ctx(NULL),
                        m_sslfd(NULL), 
                        m_srv_addr_info(NULL)
{
   if (in_address)
      m_address = in_address;

   if (in_port)
      m_port = in_port;
}

Socket::~Socket ()
{
   close();
   if (m_srv_addr_info != NULL)
      freeaddrinfo (m_srv_addr_info);
   if (m_ctx)
      SSL_CTX_free(m_ctx);
   if (m_sslfd)
      SSL_free (m_sslfd);
}


EmailError Socket::resolveHost ()
{
   debug ("Socket: Resolving Host\n");
   EmailError       status            = Email_no_error; 
   struct addrinfo   hints;
   memset (&hints, 0, sizeof(hints));

   hints.ai_family      = AF_UNSPEC;
   hints.ai_socktype    = SOCK_STREAM;

   if (m_srv_addr_info != NULL)
      freeaddrinfo (m_srv_addr_info);

   if (m_address.empty() || m_port.empty())
      status = Email_invalid_input;
   if (status == Email_no_error)
   {
      if (getaddrinfo(m_address.c_str(), m_port.c_str(), &hints, &m_srv_addr_info) != 0)
         status = Email_cannor_resolve_host;
   }
   return status;
}

EmailError Socket::connect ()
{
   EmailError       status            = Email_no_error; 
   bool              connected        = false;

   if (m_srv_addr_info == NULL)
      status = resolveHost();

   if (status == Email_no_error)
   {
      debug ("Socket:trying to connect to server\n");
      struct addrinfo*  cur_srv          = m_srv_addr_info;
      while ( cur_srv != NULL && !connected)
      {
         int socfd = socket(cur_srv->ai_family, cur_srv->ai_socktype, cur_srv->ai_protocol);
         if (socfd > 0)
         {
            struct timeval tv;
            tv.tv_sec = SOC_TIMEOUT;
            tv.tv_usec = 0 ;
            if (setsockopt (socfd, SOL_SOCKET, SO_RCVTIMEO, (void*)&tv, sizeof tv) != 0 ||
                setsockopt (socfd, SOL_SOCKET, SO_SNDTIMEO, (void*)&tv, sizeof tv))
            {
               debug ("Socket: Error setting timeout\n");
            }
            if (::connect(socfd, cur_srv->ai_addr, cur_srv->ai_addrlen) == -1 )
            {
               ::close(socfd);
               cur_srv = cur_srv->ai_next;
            } else {
               m_socfd = socfd;
               connected = true;
            }
         }
      }
      if (!connected)
      {
         debug("Socket: Connection to server failed\n");
         status = Email_connection_failed;
      } else if (m_ssl) {
         debug("Socket: Initializing SSL\n");
        status = setSsl();
      }
   }
   return status;
}

int Socket::send (const char* in_msg)const 
{
   debug ("Soket:sending %s", in_msg);
   int len = strlen (in_msg);
   int sent = (m_ssl) ? SSL_write (m_sslfd, in_msg, len) : ::send (m_socfd, in_msg, len, 0);
   return sent - len; 
}

EmailError Socket::receive (char out_msg[])const 
{
   EmailError status = Email_no_error;
   if (out_msg == NULL)
   {
      status = Email_invalid_input;
   } else {

      char buff[BUFF_SIZE+1] = {0};
      int recved = (m_ssl) ? SSL_read(m_sslfd, buff, BUFF_SIZE ) : ::recv(m_socfd, buff, BUFF_SIZE, 0) ;
      if (recved == 0)
       {
          status = Email_connection_closed;
          out_msg[0] = '\0';
       } else if ( recved == -1) {
           status = Email_recive_error;
           out_msg[0] = '\0';
      } else {
         buff[recved+1] = '\0';
         strcpy(out_msg, buff);
      }
   }
   debug ("Soket:received %s", out_msg);
   return status; 
}
EmailError Socket::close()
{
   EmailError status = Email_no_error;

   if ( m_socfd <= 0 )
   {
      status = Email_no_connection;
   } else {
      ::close(m_socfd);
      m_socfd = -1;
   }
   return status; 
}


EmailError Socket::setSsl()
{
   EmailError status = Email_no_error;
   if ( ! s_sslLoaded)
   {
      SSL_library_init();
      s_sslLoaded = true;
   }
   SSL_load_error_strings();
   /* TODO: check other methods */
   m_ctx =  SSL_CTX_new(SSLv23_client_method());
   if (m_ctx == NULL)
   {
      status = Email_cannot_init_ssl;
      ::close(m_socfd);
   } else
   {
      //SSL_CTX_load_verify_locations
      //SSL_CTX_set_verify
      m_sslfd = SSL_new(m_ctx);
      if (m_sslfd == NULL)
      {
         status = Email_cannot_init_ssl;
         ::close(m_socfd);
         SSL_CTX_free(m_ctx);
      }else {
         SSL_set_fd (m_sslfd, m_socfd);
         SSL_connect (m_sslfd);
      }
   }
   return status;
}
