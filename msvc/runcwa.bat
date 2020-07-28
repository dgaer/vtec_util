@echo off

if "%1" == "" goto EXIT_ERROR

set PROGRAM=..\vtec_util
set CWA=%1
set ICONdir=../../icons
set OUTPUTdir=..\%CWA%

mkdir %CWA%
cd %CWA%

@echo on

%PROGRAM% --cwa=%CWA% --config-file=%CWA%.cfg --icon-dir=%ICONdir% --get-web-products --output-dir=%OUTPUTdir% --log-file-clear=%CWA%.log --warning-table --google-maps --dump-zone-table --match-dynamic-names --verbose %2 %3 %4 %5 %6 %7 %8 %9

@echo off

cd ..
goto EXIT

:EXIT_ERROR
echo You must supply a CWA
goto EXIT

:EXIT


