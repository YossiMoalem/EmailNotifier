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

#ifndef EMAIL_NOTIFYER
#define EMAIL_NOTIFYER

class emailAccount;


enum AccountType
{
  AT_ERR      = 0,
  AT_POP3     = 1,
  AT_IMAP     = 2,
  AT_HOTMAIL  = 3,
  AT_GMAIL    = 4,
  AT_YAHOO    = 5,
  AT_LAST
};

class emailNotifiableIntf
{
    public:
    virtual void onAccountUpdated (int newMessages) = 0;
    virtual void onUpdateError (int status ) = 0;
};

struct AccountSettings
{
   AccountType    type;
   std::string    host;
   unsigned int   port;
   std::string    login;
   std::string    pass;
   bool           ssl;
   unsigned int   updateInterval;
};

typedef emailAccount* accountHndl;

class emailNotifier 
{
   public:
   accountHndl registerAccount ( AccountSettings accountSettings, emailNotifiableIntf* handler );

   private:
   emailAccount*  createAccount (AccountSettings& settings, emailNotifiableIntf* i_handler);
};
#endif
