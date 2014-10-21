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

#include <QObject>
#include "emailNotifier.h"

#include <QPaintEvent>
class QLabel;
class QPainter;
class QRect;
class QPushButton;
class QTextEdit;
class configUi;
class QWidget;

class emailNotifierUi : public QObject, public EmailNotifiableIntf
{
   Q_OBJECT

   public:
      emailNotifierUi();
      QWidget* widget();
      const char* statusStr () const;
      void onAccountUpdated (int newMsgs);
    void onUpdateError (EmailError status);

      public slots:
         void showConfig();

   private:
      QString*       m_accountString;
      QPushButton*   m_config;
      QTextEdit*     m_accountStatus;
      QWidget*       m_widget;
      EmailNotifier* m_ntf;

   private slots:
      void accountUpdated (int newMsgs);
   void registerAccount();

   signals:
      void sigAccountUpdated(int newMsg);

};
