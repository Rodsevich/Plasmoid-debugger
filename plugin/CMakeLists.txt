add_definitions(-DTRANSLATION_DOMAIN=\"plasma_applet_org.kde.pruebas\")
option (USE_QT5 "Use Qt5 instead of Qt4" ON)

#find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS Network)
find_package(KF5Archive)

set(SRCS
	filecalendarplugin.cpp
	prueba.cpp
	filecalendar.cpp
	incidence.cpp
	calendartodo.cpp
	calendarevent.cpp
)
add_definitions(-DKDE_DEFAULT_DEBUG_AREA=1209)
ecm_qt_declare_logging_category(SRCS HEADER debug_p.h IDENTIFIER LOG_PLASMA CATEGORY_NAME org.kde.plasma DEFAULT_SEVERITY Debug)
ecm_qt_declare_logging_category(SRCS HEADER debug_prueba.h IDENTIFIER LOG_PRUEBA CATEGORY_NAME log_prueba DEFAULT_SEVERITY Debug)

add_library(filecalendarplugin SHARED ${SRCS})

target_link_libraries(filecalendarplugin
                        Qt5::Quick
			Qt5::Svg
			KF5::CalendarCore
			KF5::Package
			KF5::Plasma
			KF5::Archive
                     )

install(FILES qmldir DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/filecalendarplugin)
install(TARGETS filecalendarplugin DESTINATION ${QML_INSTALL_DIR}/org/kde/plasma/private/filecalendarplugin)
