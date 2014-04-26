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

#include "config.h"
#include "emailAccount.h"
#include <QObject>

class QTimer;
class emailChecker;

class emailNotifier : public QObject
{
   Q_OBJECT

   public:
   emailNotifier(QObject* parent = NULL);
   void createAccount (AccountSettings& settings);

   private:
   emailAccount*  m_account;
   QTimer*        m_checkTimer;
   bool           m_playNotification;
   int            m_updateInterval;
   emailChecker*  m_emailChecker;

   public slots: 
   void checkAccount();
   void accountUpdated(int newMsgs);
   void readConfigAndUpdate();

   signals:
   void accountChanged(int newMsgs);
};

#endif