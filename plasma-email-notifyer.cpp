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

#include "plasma-email-notifyer.h"
 
#include <plasma/svg.h>
#include <plasma/theme.h>
#include <QGridLayout>
#include <QString>
#include <QPainter>
#include <QWidget>
 
#include "common/emailNotifyerUi.h"
#include "common/configUi.h"
#include <sstream>

PlasmaEmailNotifyer::PlasmaEmailNotifyer(QObject *parent, const QVariantList &args)
    : Plasma::PopupApplet(parent, args),
    m_svg( this)
{
    setHasConfigurationInterface(true);
    //m_svg.setImagePath("widgets/background");
    //setBackgroundHints(DefaultBackground);
    resize(200, 100);
}
 
 
PlasmaEmailNotifyer::~PlasmaEmailNotifyer()
{
    if (m_ntf != NULL)
       delete m_ntf;
}
 
void PlasmaEmailNotifyer::init()
{

  m_ntf = new emailNotifyerUi() ;
  connect (m_ntf, SIGNAL(sigAccountChanged(int)),
            this, SLOT(accountUpdated(int)));
} 
 
 
void PlasmaEmailNotifyer::paintInterface(QPainter *p,
        const QStyleOptionGraphicsItem *option, const QRect &contentsRect)
{
   qDebug ("Repainting the plasmoind");
   p->setRenderHint(QPainter::SmoothPixmapTransform);
   p->setRenderHint(QPainter::Antialiasing);

   m_svg.resize((int)contentsRect.width(), (int)contentsRect.height());
   m_svg.paint(p, (int)contentsRect.left(), (int)contentsRect.top());

   p->save();
   p->setPen(Qt::black);
   p->setFont(QFont("Currier", 20, QFont::Bold, false));
   p->drawText (contentsRect, Qt::AlignTop | Qt::AlignLeft| Qt::TextWordWrap, m_ntf->statusStr());
   p->restore();
}
 
void PlasmaEmailNotifyer::accountUpdated(int newMsgs)
{
   qDebug ("Ui:Updating acount status");
   QString msgStr ;
   std::ostringstream sout;
   if (newMsgs >= 0)
   {
      qDebug("Ui:New account status has %d messages",newMsgs);
   } else {
      qDebug("Ui:Error code %d was recieved", newMsgs);
   }
   (*m_accountString) = m_ntf->statusStr();
   update();
}

QWidget* PlasmaEmailNotifyer::widget ()
{
   return m_ntf->widget();
}
void PlasmaEmailNotifyer::showConfigurationInterface()
{
   m_ntf->showConfig();
}

// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(email_notifyer, PlasmaEmailNotifyer)
 
#include "plasma-email-notifyer.moc"
