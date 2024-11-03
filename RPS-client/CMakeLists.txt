cmake_minimum_required(VERSION 3.16)

project(RPS-CLIENT VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/forms")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTORCC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/img")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS SerialPort)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialPort)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Xml)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Test REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        inc/mainwindow.h
        forms/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RPS-CLIENT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET RPS-CLIENT APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(RPS-CLIENT SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(RPS-CLIENT
            ${PROJECT_SOURCES}
            inc/actions.h src/actions.cpp forms/actions.ui
            inc/choice.h src/choice.cpp forms/choice.ui
            img/imgs.qrc
            inc/result.h src/result.cpp forms/result.ui
        )
    endif()
endif()

target_link_libraries(RPS-CLIENT PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RPS-CLIENT PUBLIC Qt${QT_VERSION_MAJOR}::SerialPort)
target_link_libraries(RPS-CLIENT PUBLIC Qt${QT_VERSION_MAJOR}::Xml)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.RPS-CLIENT)
endif()
set_target_properties(RPS-CLIENT PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS RPS-CLIENT
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(RPS-CLIENT)
endif()



set(TEST_TARGET UT-tests)

# Add test sources
add_executable(${TEST_TARGET}
    UT-tests/UT-app-tester.cpp
)

target_sources(${TEST_TARGET} PUBLIC
    inc/actions.h src/actions.cpp forms/actions.ui
    inc/choice.h src/choice.cpp forms/choice.ui
    inc/result.h src/result.cpp forms/result.ui
    inc/mainwindow.h src/mainwindow.cpp forms/mainwindow.ui
)

# Link the Qt5::Test library to the test executable
target_link_libraries(${TEST_TARGET} PUBLIC Qt5::Test Qt5::Core Qt5::Widgets Qt5::SerialPort Qt5::Xml)

# Enable testing
enable_testing()

# Define a test
add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})
