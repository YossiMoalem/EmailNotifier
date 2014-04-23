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

#include <QPushButton>
#include <QVBoxLayout>
#include <QVBoxLayout>

#include "notifierApp.h"

notifierApp::notifierApp ()
{
   m_emailNotifier = new emailNotifierUi() ;
   m_config = new QPushButton("Configure", this);

   QVBoxLayout*  appLayout = new QVBoxLayout();
   appLayout->addWidget(m_emailNotifier->widget());
   appLayout->addWidget(m_config);

   connect (m_config, SIGNAL (clicked()),
            m_emailNotifier, SLOT(showConfig()));
   setLayout (appLayout);
}

