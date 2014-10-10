######################################################################
# Automatically generated by qmake (2.01a) Fri Oct 22 14:18:31 2010
######################################################################

TEMPLATE = app
TARGET = emailNotifier
DEPENDPATH += . ../Ui ../emailReader
INCLUDEPATH += ../emailReader
QMAKE_CXXFLAGS += -pedantic -Wall -Wextra -Wno-long-long

# Input
HEADERS += ./notifierApp.h \
           ../Ui/config.h \
           ../Ui/configUi.h \
           ../Ui/emailNotifierUi.h \
           ../emailReader/emailAccount.h \
           ../emailReader/emailNotifier.h \
           ../emailReader/error.h \
           ../emailReader/Imap.h \
           ../emailReader/Pop3.h \
           ../emailReader/Socket.h
SOURCES += main.cpp \
           ./notifierApp.cpp \
           ../Ui/config.cpp \
           ../Ui/configUi.cpp \
           ../Ui/emailNotifierUi.cpp \
           ../emailReader/emailAccount.cpp \
           ../emailReader/emailNotifier.cpp \
           ../emailReader/error.cpp \
           ../emailReader/Imap.cpp \
           ../emailReader/Pop3.cpp \
           ../emailReader/Socket.cpp
QT += gui xml
LIBS += -lssl -lcrypto -lrt
