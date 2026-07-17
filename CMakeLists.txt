cmake_minimum_required(VERSION 3.16)

project(WDS_Projekt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools SerialPort)

set(TS_FILES
    WDS_Projekt_en.ts
    WDS_Projekt_de.ts
)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)



if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(WDS_Projekt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        topbar.h topbar.cpp
        simulatorarea.h simulatorarea.cpp
        animatedbutton.h animatedbutton.cpp
        mainpage.h mainpage.cpp
        datapage.h datapage.cpp
        controllermanager.h controllermanager.cpp
        virtualhorizon.h virtualhorizon.cpp
        bottombar.h bottombar.cpp
        flightdataarea.h flightdataarea.cpp
        toolbar.h toolbar.cpp
        flightmathmodel.h flightmathmodel.cpp
        entryoverlay.h entryoverlay.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET WDS_Projekt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
qt_add_translations(WDS_Projekt
    TS_FILES ${TS_FILES}
    RESOURCE_PREFIX "/translates"
)
 else()
    if(ANDROID)
        add_library(WDS_Projekt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(WDS_Projekt
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(WDS_Projekt PRIVATE 
Qt${QT_VERSION_MAJOR}::Widgets 
Qt${QT_VERSION_MAJOR}::SerialPort
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.WDS_Projekt)
endif()
set_target_properties(WDS_Projekt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS WDS_Projekt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_add_resources(WDS_Projekt "img_resources"
    PREFIX "/grafika"
    FILES "las.png"
)
qt_add_resources(WDS_Projekt "img_resources2"
    PREFIX "/grafika"
    FILES "niebo.png"
)
qt_add_resources(WDS_Projekt "img_resources3"
    PREFIX "/grafika"
    FILES "plane.png"
)

qt_add_resources(WDS_Projekt "img_resources4"
    PREFIX "/grafika"
    FILES "smigla.png"
)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(WDS_Projekt)
endif()
