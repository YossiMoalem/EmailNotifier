# Project Needs a name ofcourse
project(plasma-email-notifier)
 
# Find the required Libaries
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
 
add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${KDE4_INCLUDES}
   )
 
# We add our source code here
set(email_notifier_SRCS 
         ./plasma-email-notifier.cpp 
          common/config.cpp
          common/configUi.cpp
          common/emailAccount.cpp
          common/emailChecker.cpp
          common/emailNotifier.cpp
          common/emailNotifierUi.cpp
          common/error.cpp
          common/Imap.cpp
          common/Pop3.cpp
          common/Socket.cpp)
set(email_notifier_MOC_HDRS Pop3.h)

qt4_wrap_cpp(plasma-email-notifier ${email_notifier_MOC_HDRS})

# Now make sure all files get to the right place
kde4_add_plugin(plasma_applet_email_notifier ${email_notifier_SRCS} 
   ${HDRS} ${SRCS} ${UI_H} ${MOC} ${QM} ${QRC})

target_link_libraries(plasma_applet_email_notifier 
                      ${KDE4_PLASMA_LIBS} ${KDE4_KDEUI_LIBS} 
                      ${QT_LIBRARIES}  
                      ${QT_QTXML_LIBRARY}
                      ${QT_QTNETWORK_LIBRARY}
                      ssl crypto rt)

 
install(TARGETS plasma_applet_email_notifier
        DESTINATION ${PLUGIN_INSTALL_DIR})
 
install(FILES plasma-applet-email_notifier.desktop
        DESTINATION ${SERVICES_INSTALL_DIR})
