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

#ifndef SOCKET_H
#define SOCKET_H

#include <string> 

#include "error.h"

class SocketImplIntf;

class Socket 
{

 public:
   Socket (const std::string& i_address, unsigned short portNum, bool ssl); 
   ~Socket();

   EmailError  connect           ();
   EmailError  send              (const std::string& i_msg) const ;
   EmailError  send              (const char* i_msg) const ;
   EmailError  receive           (std::string& o_msg) const ;
   EmailError  close             ();

 private:
   SocketImplIntf*      m_socketImpl;
};

#endif
