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

#ifndef EmailNotifyer_HEADER
#define EmailNotifyer_HEADER

#include <KIcon>

#include <Plasma/PopupApplet>
#include <Plasma/Svg>

class emailNotifyerUi;
class QSizeF;
class QString;
class QWidget;

class PlasmaEmailNotifyer : public Plasma::PopupApplet
{
   Q_OBJECT
   public:
      PlasmaEmailNotifyer(QObject *parent, const QVariantList &args);
      ~PlasmaEmailNotifyer();
      QWidget* widget();

      void paintInterface(QPainter *p,
            const QStyleOptionGraphicsItem *option,
            const QRect& contentsRect);
      void init();

   public slots:
   void accountUpdated(int newMsgs);

   private:
      Plasma::Svg       m_svg;
      emailNotifyerUi*  m_ntf;
      QString*          m_accountString;


   private slots:
   void showConfigurationInterface();

};

#endif
