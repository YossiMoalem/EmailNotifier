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

#ifndef CONFIG_UI_H
#define CONFIG_UI_H

#include <QDialog>
#include <QWidget>

class QComboBox;
class QLineEdit;
class QSpinBox;
class QLineEdit;
class QString;
class QCheckBox;
class QTabWidget;
class QRadioButton;
class QGridLayout;
class QVBoxLayout;
class QLabel;
class QDialogButtonBox;

class accountTab : public QWidget
{
   Q_OBJECT
   public:
   accountTab(QWidget* parent = NULL);

   private slots:
   void accountTypeChanged(int index);
   void setPort(int SslState);

   public slots:
   void save ();
   void load();

   protected:
   QComboBox*     m_typeCBox;
   QLineEdit*     m_host; 
   QLabel*        m_hostLbl;
   QSpinBox *     m_port;
   QLabel*        m_portLbl;
   QLineEdit*     m_uname;
   QLabel*        m_unameLbl;
   QLabel*        m_unameSuffixLbl;
   QLineEdit*     m_pass;
   QLabel*        m_passLbl;
   QCheckBox*     m_ssl;
   QLabel*        m_sslLbl;
   QVBoxLayout*   m_accountLayout;
   QGridLayout*   m_detailsLayout;

   protected:
   void clear();
   void setElemsVis(int index);
   void updateUnameSuffix (int index);

   friend class configUi;
};

class generalTab : public QWidget
{
   Q_OBJECT
   public:
   generalTab(QWidget* parent = NULL);

   private:
   QSpinBox*    m_intervalMin;
   QSpinBox*    m_intervalSec;
   QCheckBox*   m_playSound;
   QRadioButton* m_playDefault;
   QRadioButton* m_playFile;

   private slots:
   void intervalMinChanged(int newVal);
   void intervalSecChanged(int newVal);

   public slots:
   void save ();
   void load();
};
class configUi : public QDialog
{
   Q_OBJECT
   public:
   configUi (QWidget* parent = 0);

   private:
   QDialogButtonBox* m_stdBtn; 
   QTabWidget*       m_config;
   accountTab*       m_accountTab;
   generalTab*       m_generalTab;

   private slots:
   void save ();
   void saveAccessibility (QString text); 

   signals:
   void configUpdated();

};
#endif
