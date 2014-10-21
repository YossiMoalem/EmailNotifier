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

#ifndef IMAP 
#define IMAP

#include <string>

#include "emailAccount.h"

class Imap : public EmailAccount
{
   public:
   Imap(const std::string& i_serverAddress, 
        int port, 
        const std::string& i_uname, 
        const std::string& i_password, 
        bool ssl, 
        int updateInterval, 
        EmailNotifiableIntf* i_handler);
   ~Imap();
   
   protected:
      virtual EmailError  authenticate();
      virtual EmailError  getNumOfNewMsgs(int& r_numOfNewMsgs);
      virtual void        logout();
      virtual const char* okResponse() const ;
      virtual const char* errResponse() const;

    protected:
    static const char* const mLoginCommand;
    static const char* const mStatusCommand;
    static const char* const mLogoutCommand;
};

class Gmail : public Imap
{
   public:
   Gmail (const std::string& i_uname, const std::string& i_password, int updateInterval, EmailNotifiableIntf* i_handler);
   virtual ~Gmail ();
};


#endif
