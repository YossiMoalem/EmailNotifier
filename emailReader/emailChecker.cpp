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

#include "emailChecker.h"
#include "emailAccount.h"

#define MAX_SIZE_QUEUE 10

emailChecker::emailChecker() : m_accounts() 
{}


EmailError emailChecker::checkAccount (emailAccount* acount)
{
   EmailError status = Email_no_error;
   int size = m_accounts.size();
   if (size > MAX_SIZE_QUEUE)
   {
      qDebug ("emailChecker: Queue is full, not adding new task");
   } else {
      qDebug ("emailChecker: Currently there are %d elements in queue. Adding new one", size);
      m_accounts.push(acount);
   }
   return status;
}

void emailChecker::run ()
{
   while(!m_accounts.empty()) m_accounts.pop(); /* When re-run after changing account */
qDebug ("Starting emailChecker main loop");
   while (true)
   {
      if (m_accounts.empty())
      {
         sleep(1);
      } else {
         qDebug ("emailChecker: starting to check account");
         emailAccount* account = m_accounts.front();
         account->checkAccount();
         m_accounts.pop();
      }
   }
}
#undef MAX_SIZE_QUEUE
