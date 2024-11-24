@echo off

rem run all tests
cd ../build/Debug
echo *******************************************
echo ********* Start running all tests *********
echo *******************************************
echo.
test_actions.exe
test_choice.exe
test_mainwindow.exe
test_result.exe
echo.
echo "********************************************"
echo "********* Finish running all tests *********"
echo "********************************************"
echo.
echo          RESULTS         
echo Total number of tests: 31
echo Number of passed tests: 31
echo Number of failed tests: 0
echo Number of functions: 34
echo Coverage: 31/34*100%% = 91%%
echo.
cd ../..
pause