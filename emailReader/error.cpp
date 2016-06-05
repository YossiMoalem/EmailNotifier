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
      case Email_invalid_account_detils:
         str = (char*)"Account detailes are invalid";
         break;
      case Email_time_out:
        str=(char*)"Opperation timed out";
        break;
      case Email_connection_closed :
         str = (char*)"Connection to host closed unexpectedly";
         break;
      case Email_recive_error:
         str = (char*)"General Error (receive error)";
         break;
      default:
         str = (char*)"Unknown Error (AKA I was too lazy to specify the error)";
         break;

   }
   return str;
}
