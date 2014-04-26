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
   Email_cannot_init_ssl               = -105,
   Email_cannor_resolve_host           = -106,
   Email_connection_closed             = -107,
   Email_recive_error                  = -108,
   Email_no_connection                 = -109,
   /* Config Errors */
   Email_missing_config_file           = -201,
   Email_invalid_config_file           = -202,
   Email_cannot_write_config_file      = -203,

   /* General Errors */
   Email_invalid_input                 = -301,
   Email_last_err                      = -302

};

const char* EmailErrorToStr (EmailError error);

#endif
