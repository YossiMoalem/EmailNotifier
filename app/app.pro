######################################################################
# Automatically generated by qmake (2.01a) Fri Oct 22 14:18:31 2010
######################################################################

TEMPLATE = app
TARGET = emailNotifier
DEPENDPATH += . common
INCLUDEPATH += .

# Input
HEADERS += ../common/config.h \
           ../common/configUi.h \
           ../common/emailAccount.h \
           ../common/emailNotifyer.h \
           ../common/emailNotifyerUi.h \
           ../common/error.h \
           ../common/Imap.h \
           ../common/notifyerApp.h \
           ../common/Pop3.h \
           ../common/emailChecker.h \
           ../common/Socket.h
SOURCES += main.cpp \
           ../common/config.cpp \
           ../common/configUi.cpp \
           ../common/emailAccount.cpp \
           ../common/emailNotifyer.cpp \
           ../common/emailNotifyerUi.cpp \
           ../common/error.cpp \
           ../common/Imap.cpp \
           ../common/notifyerApp.cpp \
           ../common/Pop3.cpp \
           ../common/emailChecker.cpp \
           ../common/Socket.cpp
QT += widgets xml
LIBS += -lssl -lcrypto -lrt
