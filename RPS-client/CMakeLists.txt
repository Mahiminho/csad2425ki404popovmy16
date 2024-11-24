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
    inc/actions.h src/actions.cpp forms/actions.ui
    inc/choice.h src/choice.cpp forms/choice.ui
    inc/result.h src/result.cpp forms/result.ui
    img/imgs.qrc
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(RPS-CLIENT
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(RPS-CLIENT SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(RPS-CLIENT
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(RPS-CLIENT PUBLIC Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(RPS-CLIENT PUBLIC Qt${QT_VERSION_MAJOR}::SerialPort)
target_link_libraries(RPS-CLIENT PUBLIC Qt${QT_VERSION_MAJOR}::Xml)

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

# # # Test target
# set(TEST_TARGET UT-tests)

# add_executable(${TEST_TARGET}
#     UT-tests/test_actions.cpp
#     UT-tests/test_choice.cpp
#     UT-tests/test_mainwindow.cpp
#     UT-tests/test_result.cpp
# )

# target_include_directories(${TEST_TARGET} PRIVATE
#     ${CMAKE_SOURCE_DIR}/inc
#     ${CMAKE_SOURCE_DIR}/UT-tests
# )

# target_link_libraries(${TEST_TARGET} PUBLIC Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Xml)

# enable_testing()

# add_test(NAME ${TEST_TARGET} COMMAND ${TEST_TARGET})

# Test targets
set(TEST_SOURCES
    UT-tests/test_actions.cpp
    UT-tests/test_choice.cpp
    UT-tests/test_mainwindow.cpp
    UT-tests/test_result.cpp
)

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE}
        src/actions.cpp
        src/choice.cpp
        src/mainwindow.cpp
        src/result.cpp
        inc/actions.h
        inc/choice.h
        inc/mainwindow.h
        inc/result.h
        forms/actions.ui
        forms/choice.ui
        forms/mainwindow.ui
        forms/result.ui
    )
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/inc
        ${CMAKE_SOURCE_DIR}/UT-tests
    )
    target_link_libraries(${TEST_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::SerialPort Qt${QT_VERSION_MAJOR}::Xml)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()

enable_testing()