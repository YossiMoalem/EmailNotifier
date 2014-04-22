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

#ifndef IMAP 
#define IMAP

#include "emailAccount.h"

class Imap : public emailAccount
{
   Q_OBJECT

   protected:
      virtual EmailError  authenticate();
      virtual EmailError  getNumOfNewMsgs(int* r_numOfNewMsgs);
      virtual void        logout();
      virtual EmailError check_response (char* in_buff, EmailError in_error_msg) const;

   public:
   Imap(const char* in_server_address, int in_port, const char* in_uname, const char* in_pass, bool in_ssl, QObject* parent = NULL);
   ~Imap();
};

class Gmail : public Imap
{
   public:
   Gmail (const char* in_uname, const char* in_pass, QObject* parent = NULL);
   virtual ~Gmail ();
};


#endif
