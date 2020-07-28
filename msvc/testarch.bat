@echo off
	
rem Batch file used to run a CWA from an archive product DIR

if "%1" == "" goto EXIT_ERROR

set PROGRAM=..\vtec_util
set CWA=%1
set ICONdir=../../icons
set OUTPUTdir=..\%CWA%
rem set DATAdir=C:\tmp\vtec_util.arch
set DATAdir=..\..\products\MFL-08192008
remo set DATAdir=C:\tmp\arch

if not exist %CWA% mkdir %CWA%
cd %CWA%

@echo on

%PROGRAM% --cwa=%CWA% --config-file=%CWA%.cfg --icon-dir=%ICONdir% --data-dir=%DATAdir% --output-dir=%OUTPUTdir% --log-file-clear=%CWA%.log --warning-table --arch-mode=TEST --google-maps --no-dynamic-zones --dump-html-tables --ver-worksheets --worksheet-out-dir=%CWA%-worksheets --worksheet-print-cmd="a2ps" --force-worksheet-test --key-table-in-page  --ver-table-in-page %2 %3 %4 %5 %6 %7 %8 %9

@echo off

cd ..
goto EXIT

:EXIT_ERROR
echo You must supply a CWA
goto EXIT

:EXIT


