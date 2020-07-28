

if "%1" == "" goto EXIT_ERROR

set PROGRAM=..\vtec_util
set CWA=%1
set ICONdir=../../icons
set OUTPUTdir=..\%CWA%

set DATAdir=..\..\products\HLS1

mkdir %CWA%
cd %CWA%

echo %OUTPUTdir%

%PROGRAM% --cwa=%CWA% --config-file=%CWA%.cfg --icon-dir=%ICONdir% --data-dir=%DATAdir% --output-dir=%OUTPUTdir% --log-file-clear=%CWA%.log --warning-table --google-maps %2 %3 %4 %5 %6 %7 %8 %9

cd ..

goto EXIT

:EXIT_ERROR
echo You must supply a CWA
goto EXIT

:EXIT


