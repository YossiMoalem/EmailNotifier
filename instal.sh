#! /bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "Please run this installer as root" 1>&2
   exit 1
fi

cp plasma_applet_email_notifier.so /usr/lib/kde4/
cp plasma_applet_email_notifier.desktop /usr/share/kde4/services/ 

echo "We now need to restart the plasma desktop."
echo "The best way to do it is to logout. However, we restart plasma now, without logging out"
read -p "Restart plasma (y/n) ? "
[ $REPLY == "y" ] &&  kquitapp  kquitapp plasma-desktop &&  kquitapp plasma-desktop  || echo "Faild to restart plasma. please logout"
