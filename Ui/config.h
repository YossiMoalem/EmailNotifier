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

#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include "error.h"
#include "emailNotifier.h"

#define EMAIL_TYPE_POP3       "pop3"
#define EMAIL_TYPE_IMAP       "imap"
#define EMAIL_TYPE_HOTMAIL    "hotmail"
#define EMAIL_TYPE_GMAIL      "gmail"
#define EMAIL_TYPE_YAHOO      "yahoo"

struct GeneralSettings
{
   unsigned int         updateIntervalMin;
   unsigned int         updateIntervalSec;
   bool                 playNotification;
};
static const char * const AccountTypeName[AT_LAST] = { "ERR", EMAIL_TYPE_POP3, EMAIL_TYPE_IMAP, EMAIL_TYPE_HOTMAIL, EMAIL_TYPE_GMAIL, EMAIL_TYPE_YAHOO};

class AccountSettings;

EmailError accountSettings_save(const AccountSettings* settings);
EmailError accountSettings_load(AccountSettings* r_settings);

EmailError generalSettings_save (const GeneralSettings* settings);
EmailError generalSettings_load (GeneralSettings* r_settings);
AccountType accountNameToType (const std::string& i_name);

#endif
