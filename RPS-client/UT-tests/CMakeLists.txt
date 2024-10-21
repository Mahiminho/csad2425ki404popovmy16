cmake_minimum_required(VERSION 3.16)
project(MyQtTests)

# Find the Qt components (adjust to the version you are using)
find_package(Qt5 REQUIRED COMPONENTS Core Test Widgets SerialPort Xml)

# Add the test sources
set(TEST_SOURCES
    UT-connection-tester.cpp
)

# Create the test executable
add_executable(MyQtTestRunner ${TEST_SOURCES})

# Link the test executable with your main project libraries and Qt Test
target_link_libraries(MyQtTestRunner PUBLIC Qt5::Core Qt5::Test Qt5::Widgets  Qt5::SerialPort Qt5::Xml Connection-Tester-Lib)

# Enable testing and add test to CTest
enable_testing()
add_test(NAME MyQtTests COMMAND MyQtTestRunner)
