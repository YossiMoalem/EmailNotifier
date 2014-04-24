# Project Needs a name ofcourse
project(plasma-email-notifier)


# Find the required Libaries
find_package(KDE4 REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
include(KDE4Defaults
      ${QT_USE_FILE})
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   ./Ui
   ./emailReader
   )
 

# We add our source code here
set(email_notifier_SRCS 
         ./plasma-email-notifier.cpp 
          Ui/config.cpp
          Ui/configUi.cpp
          Ui/emailNotifierUi.cpp
          emailReader/emailAccount.cpp
          emailReader/emailNotifier.cpp
          emailReader/error.cpp
          emailReader/Imap.cpp
          emailReader/Pop3.cpp
          emailReader/emailChecker.cpp
          emailReader/Socket.cpp)
#Set here all headers that use Q_OBJECT
#set(email_notifier_HDRS Pop3.h)

#invoke MOC
qt4_wrap_cpp(email_notifier_MOC_HDRS ${email_notifier_HDRS})

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_email_notifier ${email_notifier_SRCS} 
   ${HDRS} ${SRCS} ${UI_H} ${MOC} ${QM} ${QRC})

set (FILES ${email_notifier_SRCS}
     ${email_notifier_MOC_HDRS})
ADD_EXECUTABLE(plasma-email-notifier ${FILES})
target_link_libraries(plasma-email-notifier 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} 
                      ${QT_LIBRARIES}  
                      ${QT_QTXML_LIBRARY}
                      ${QT_QTNETWORK_LIBRARY}
                      ssl crypto rt)



install(TARGETS plasma-email-notifier
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-emaili-notifier.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
