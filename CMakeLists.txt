cmake_minimum_required(VERSION 3.5)

project(SchoolManagementSystem VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)


set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SchoolManagementSystem
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        adminer.h adminer.cpp adminer.ui
        reguser.h reguser.cpp reguser.ui
        Entity.h Entity.cpp


        displayer.h displayer.cpp displayer.ui

        schedulesetter.h schedulesetter.cpp schedulesetter.ui
        courseeditor.h courseeditor.cpp courseeditor.ui

        changestudentgroup.h changestudentgroup.cpp changestudentgroup.ui
        groupeditor.h groupeditor.cpp groupeditor.ui
        addgroup.h addgroup.cpp addgroup.ui
        recyclebin.h recyclebin.cpp recyclebin.ui

        addstudent.h addstudent.cpp addstudent.ui
        sessionhandler.h sessionhandler.cpp sessionhandler.ui
        sessionactivation.h sessionactivation.cpp sessionactivation.ui
        addcourse.h addcourse.cpp addcourse.ui
        searchteacher.h searchteacher.cpp searchteacher.ui
        Icons.qrc





    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SchoolManagementSystem APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SchoolManagementSystem SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SchoolManagementSystem
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(SchoolManagementSystem PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.SchoolManagementSystem)
endif()
set_target_properties(SchoolManagementSystem PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS SchoolManagementSystem
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(SchoolManagementSystem)
endif()
