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

#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>
#include <QString>

#include <sstream>
#include <stdlib.h> //for calloc
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>


#include "config.h"

#define DOC_NAME                 "EmailNotifySettings"

/* Account Settingg */
#define ACCOUNTS_ROOT            "Accounts"
#define ACCOUNT_ELEM_NAME        "Account"
#define ACCOUNT_TYPE_ELEM_NAME   "AccoutType"
#define HOST_ELEM_NAME           "Host"
#define PORT_ELEM_NAME           "Port"
#define LOGIN_ELEM_NAME          "Login"
#define PASS_ELEM_NAME           "Pass"
#define SSL_ELEM_NAME            "UseSsl"

/* General Settings */
#define GENERAL_ELEM             "GeneralSettings"
#define UPDATE_INTERVAL          "UpdateInterval"
#define PLAY_SOUND               "PlaySound"

static void _read_elem_val (QDomElement &root, const char* elem_name, std::string &r_value);
static void _add_elem (QDomDocument &doc, QDomElement &root, const char* elem_name, const char* elem_val);
static ConfigurationError _load_xml (QDomDocument &dox);

const char* get_settings_config_name ()
{
   static const char* config_name = ".emailNotifier.xml";
   static char* config_path = NULL;
   if (config_path == NULL)
   {
      struct passwd *pw = getpwuid(getuid());
      const char *homedir = pw->pw_dir;
      //TODO: convert to sstream
      int config_name_len = strlen(homedir) + strlen (config_name) + 1;
      config_path = (char*)calloc (config_name_len, sizeof(char));
      if (config_path != NULL)
         sprintf (config_path, "%s/%s", homedir, config_name);
   }
   return config_path;
}
/*********************************************************\
 *          Account Settings
 \*********************************************************/
ConfigurationError accountSettings_save(const AccountSettings* settings)
{
   /* init */

   std::ostringstream sout;
   const char* useSsl; 
   std::string portStr;
   if (settings->port > 0)
   {
      sout << settings->port;
      portStr = sout.str();
      useSsl = (settings->ssl) ? "TRUE" : "FALSE";
   } else {
      portStr = '\0';
      useSsl = "\0";
   }


   /* Prepare XML */
   QDomDocument doc (DOC_NAME);
   _load_xml (doc); 

   QDomElement root  = doc.firstChildElement (DOC_NAME);
   if (root.isNull())
      root = doc.createElement(DOC_NAME);
   QDomElement accountRoot  = root.firstChildElement (ACCOUNTS_ROOT);
   if (accountRoot.isNull())
   {
      accountRoot = doc.createElement(ACCOUNTS_ROOT);
      root.appendChild(accountRoot);
   }

   QDomElement account  = accountRoot.firstChildElement (ACCOUNT_ELEM_NAME);
   if (!account.isNull())
      accountRoot.removeChild(account);
   account = doc.createElement(ACCOUNT_ELEM_NAME);

   _add_elem (doc, account, ACCOUNT_TYPE_ELEM_NAME,   AccountTypeName [settings->type] );
   _add_elem (doc, account, HOST_ELEM_NAME,           settings->host.c_str());
   _add_elem (doc, account, PORT_ELEM_NAME,           portStr.c_str());
   _add_elem (doc, account, LOGIN_ELEM_NAME,          settings->login.c_str());
   _add_elem (doc, account, PASS_ELEM_NAME,           settings->pass.c_str());
   _add_elem (doc, account, SSL_ELEM_NAME,            useSsl);

   accountRoot.appendChild(account);
   doc.appendChild (root);
   /* Save XML */
   QString xml = doc.toString();
   QFile file(get_settings_config_name());
   if(!file.open(QIODevice::WriteOnly ))
      return E_cannot_write_config_file ;

   QTextStream ts( &file );
   ts << doc.toString();


   file.close();
   return E_ok;
}

ConfigurationError accountSettings_load(AccountSettings* r_settings)
{
   ConfigurationError status = E_ok;

   QDomDocument doc(DOC_NAME);
   status = _load_xml (doc);

   /* Read Settings */
   if (status == E_ok)
   {
      QDomElement root = doc.firstChildElement(DOC_NAME);
      QDomElement accountsRoot = root.firstChildElement(ACCOUNTS_ROOT);
      QDomElement account = accountsRoot.firstChildElement(ACCOUNT_ELEM_NAME);

      if (account.isNull())
      {
         status = E_invalid_config_file;
      } else {
         std::string portStr;
         std::string sslStr;
         std::string accountName;
         _read_elem_val (account, ACCOUNT_TYPE_ELEM_NAME,   accountName);
         _read_elem_val (account, HOST_ELEM_NAME,           r_settings->host);
         _read_elem_val (account, PORT_ELEM_NAME,           portStr);
         _read_elem_val (account, LOGIN_ELEM_NAME,          r_settings->login);
         _read_elem_val (account, PASS_ELEM_NAME,           r_settings->pass);
         _read_elem_val (account, SSL_ELEM_NAME,            sslStr);

         std::istringstream sout (portStr);
         sout >> r_settings->port;
         r_settings->ssl = (sslStr.compare("TRUE") == 0) ? true : false; 
         r_settings->type = accountNameToType(accountName);
//TODO: Add updateInterval
      }
   }
   return status;
}


/*********************************************************\
 *          Applications Settings
 \*********************************************************/
ConfigurationError generalSettings_save (const GeneralSettings* settings)
{
   /* init */
   const char* playSound = (settings->playNotification) ? "TRUE" : "FALSE";
   int interval = settings->updateIntervalMin * 60 + settings->updateIntervalSec;
   std::ostringstream sout;
   sout << interval;
   std::string intervalStr = sout.str();

   /* Prepare XML */
   QDomDocument doc (DOC_NAME);
   _load_xml (doc); 

   QDomElement root  = doc.firstChildElement (DOC_NAME);
   if (root.isNull())
      root = doc.createElement(DOC_NAME);
   QDomElement generalSettingsElem = root.firstChildElement (GENERAL_ELEM);
   if (!generalSettingsElem.isNull())
      root.removeChild(generalSettingsElem);

   generalSettingsElem = doc.createElement(GENERAL_ELEM);
   root.appendChild(generalSettingsElem);

   _add_elem (doc, generalSettingsElem, UPDATE_INTERVAL, intervalStr.c_str());
   _add_elem (doc, generalSettingsElem, PLAY_SOUND, playSound);

   doc.appendChild (root);
   /* Save XML */
   QString xml = doc.toString();
   QFile file(get_settings_config_name());
   if(!file.open(QIODevice::WriteOnly ))
      return E_cannot_write_config_file ;

   QTextStream ts( &file );
   ts << doc.toString();

   file.close();
   return E_ok;
}
ConfigurationError generalSettings_load (GeneralSettings* r_settings)
{
   ConfigurationError status = E_ok;

   QDomDocument doc(DOC_NAME);
   status = _load_xml (doc);

   /* Read Settings */
   if (status == E_ok)
   {
      QDomElement root = doc.firstChildElement(DOC_NAME);
      QDomElement generalSettingsElem = root.firstChildElement(GENERAL_ELEM);

      if (generalSettingsElem.isNull())
      {
         status = E_invalid_config_file;
      } else {
         std::string playSoundStr;
         std::string updateStr;
         int uptareInterval = 0;

         _read_elem_val (generalSettingsElem, PLAY_SOUND, playSoundStr);
         _read_elem_val (generalSettingsElem, UPDATE_INTERVAL, updateStr); 

         std::istringstream sout (updateStr);
         sout >> uptareInterval;
         r_settings->updateIntervalMin = (int) uptareInterval/60;
         r_settings->updateIntervalSec = (int) uptareInterval%60;

         r_settings->playNotification = (playSoundStr.compare("TRUE") == 0) ? true : false; 
      }
   }
   return status;
}

/*********************************************************\
 *        Static Functions 
 \*********************************************************/
static void _add_elem (QDomDocument &doc, QDomElement &root, const char* elem_name, const char* elem_val)
{
   if (elem_val[0] != '\0')
   {
      QDomElement new_elem = doc.createElement (elem_name);
      root.appendChild(new_elem);
      QDomText new_val = doc.createTextNode(elem_val);
      new_elem.appendChild(new_val);
   }
}
static void _read_elem_val (QDomElement &root, const char* elem_name, std::string &r_value)
{
   QDomElement elem = root.firstChildElement(elem_name);
   QString elem_val = elem.firstChild().toText().data();
   r_value = elem_val.toUtf8().constData();
}

static ConfigurationError _load_xml (QDomDocument &doc)
{

   ConfigurationError status = E_ok;
   QFile file(get_settings_config_name());
   if (!file.open(QIODevice::ReadOnly))
      status = E_missing_config_file;
   if (status == E_ok)
   {
      if ( !doc.setContent (&file))
      {
         status = E_invalid_config_file; 
      }
      file.close();
   }
   return status;
}

AccountType accountNameToType (const std::string& i_name)
{
  if (i_name.compare(EMAIL_TYPE_POP3)) return AT_POP3;
  if (i_name.compare(EMAIL_TYPE_IMAP)) return AT_IMAP;
  if (i_name.compare(EMAIL_TYPE_GMAIL)) return AT_GMAIL;
  if (i_name.compare(EMAIL_TYPE_YAHOO)) return AT_YAHOO;
  if (i_name.compare(EMAIL_TYPE_HOTMAIL)) return AT_HOTMAIL;
  return AT_ERR;
}
