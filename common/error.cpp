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

#include "error.h"

const char* EmailErrorToStr (EmailError error)
{
   char* str = 0;
   switch (error)
   {
      case Email_no_error:
         str = (char*)"Everything is OK";
         break;
      case Email_connection_failed:
         str = (char*)"Connection to server failed";
         break;
      case Email_connection_invalid_response:
         str = (char*)"Got an invalid response from the server.";
         break;
      case Email_authentication_error:
         str = (char*)"Invalid user name or password";
         break;
      case Email_general_connection_error:
         str = (char*)"General connection Error";
         break;
      case Email_missing_config_file:
         str = (char*)"Please add an account";
         break;
      case Email_invalid_config_file:
         str = (char*)"Configuration file is invalid. Please update settings";
         break;
      case Email_cannot_write_config_file:
         str = (char*)"Cannot write the configuration file";
      case Email_cannot_init_ssl:
         str = (char*)"Cannot initialize SSL connection";
         break;
      case Email_invalid_input:
         str = (char*)"General Error (invalid input)";
         break;
      case Email_cannor_resolve_host:
         str = (char*)"Cannot resolve host";
         break;
      case Email_connection_closed :
         str = (char*)"Connection to host closed unexpectedly";
         break;
      case Email_recive_error:
         str = (char*)"General Error (receive error)";
         break;
      case Email_no_connection:
         str = (char*)"General ERror (no connection)";
         break;


      default:
         str = (char*)"Unknown Error (AKA I was too lazy to specify the error)";
         break;

   }
   return str;
}
