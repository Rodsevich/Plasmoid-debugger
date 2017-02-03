cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

set(KDE_INSTALL_USE_QT_SYS_PATHS ON CACHE BOOL "Install the plugin in the right directory")
message(STATUS "Largando cmake con KDE_INSTALL_USE_QT_SYS_PATHS en modo: ${KDE_INSTALL_USE_QT_SYS_PATHS}")

set(QT_MIN_VERSION "5.3.0")
set(KF5_MIN_VERSION "5.0.0")

find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_MODULE_DIR} ${ECM_KDE_MODULE_DIR})
set(CMAKE_BUILD_TYPE "Debug")

include(KDEInstallDirs)
include(KDECompilerSettings)
include(KDECMakeSettings)
#Para evitar Unknown CMake command "ecm_qt_declare_logging_category"
include(ECMQtDeclareLoggingCategory)

find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED NO_MODULE COMPONENTS
	Core
	Quick
	Svg
)
find_package(KF5 REQUIRED COMPONENTS
	CalendarCore
	Plasma
	Package
)

add_subdirectory( plugin )
