cmake_minimum_required(VERSION 3.5)

project(CmakeCBT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/v_noabi")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/v_noabi")
include_directories("${CMAKE_CURRENT_LIST_DIR}/bin")
include_directories("${CMAKE_CURRENT_LIST_DIR}/lib")
include_directories("${CMAKE_CURRENT_LIST_DIR}/share")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(mongocxx_DIR "${CMAKE_CURRENT_LIST_DIR}/share/mongocxx")
set(bsoncxx_DIR "${CMAKE_CURRENT_LIST_DIR}/share/bsoncxx")

find_package(mongocxx CONFIG REQUIRED)

set(Headers
    startwindow.h
    studentwindow.h
    dbcontroller.h
    model.h
    errors.h
    adminwindow.h
    displaywidgets.h
    mytable.h
)

set(Sources
    startwindow.cpp
    studentwindow.cpp
    dbcontroller.cpp
    model.cpp
    adminwindow.cpp
    errors.cpp
    mytable.cpp
    displaywidgets.cpp
)

set(UI
    studentwindow.ui
    adminwindow.ui
    startwindow.ui
)

set(PROJECT_SOURCES
    main.cpp
    ${Headers}
    ${Sources}
    ${UI}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CmakeCBT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CmakeCBT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CmakeCBT
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CmakeCBT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CmakeCBT PRIVATE mongo::mongocxx_shared)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.untitled)
endif()
set_target_properties(CmakeCBT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CmakeCBT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CmakeCBT)
endif()


#add_subdirectory(tests)
#enable_testing()

#set(TestPROJECT_SOURCES
#    ${Tests}
#    ${Headers}
#    ${Sources}
#    ${UI}
#)

#add_executable(
#  TestCBT
#  ${TestPROJECT_SOURCES}
#)
#target_link_libraries(
#  TestCBT
#  GTest::gtest_main
#)

#include(GoogleTest)
#gtest_discover_tests(TestCBT)




#target_link_libraries(TestCmakeCBT PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
#target_link_libraries(TestCmakeCBT PRIVATE mongo::mongocxx_shared)
#target_link_libraries (TestCmakeCBT PRIVATE gtest gmock)


