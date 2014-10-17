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
#include "socketImpl.h"
#include <ace/Log_Msg.h>

Socket::Socket (const char* in_address, unsigned short portNum, bool useSsl)
{
  if (useSsl)
  {
    m_socketImpl = new SslSocketImpl(in_address, portNum);
  } else {
    m_socketImpl = new RegularSocketImpl(in_address, portNum);
  }

}

Socket::~Socket ()
{
   close();
}

EmailError Socket::connect ()
{
  EmailError status = Email_no_error; 

  if (Email_no_error != m_socketImpl->connect() )
  {
    ACE_DEBUG((LM_ERROR, "Socket: Failed to connect \n"));
    status = Email_connection_failed;
  } else {
    ACE_DEBUG((LM_INFO, "Socket: Conected!"));
  }
  return status;
}

EmailError Socket::send (const char* in_msg)const 
{
   m_socketImpl->send(in_msg);
   return Email_no_error;
}

EmailError Socket::receive (std::string& o_msg) const 
{
   return m_socketImpl->recv(o_msg);
}
EmailError Socket::close()
{
  m_socketImpl->close();
}
