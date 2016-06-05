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

#ifndef ERROR_H
#define ERROR_H

 /**************************************************
 *
 * IMPORTANT: 
 * Do not forget to add entry in emailErrorToStr()
 * for EVERY entry you add here!
 *
 \*************************************************/
enum EmailError
{
   Email_no_error                      = 0,
   /*Connection Errors */
   Email_connection_failed             = -101,
   Email_connection_invalid_response   = -102,
   Email_authentication_error          = -103,
   Email_general_connection_error      = -104,
   Email_connection_closed             = -105,
   Email_recive_error                  = -106,
   Email_time_out                      = -107,
   Email_invalid_account_detils        = -108
   /* Config Errors */
};

const char* EmailErrorToStr (EmailError error);

#endif
