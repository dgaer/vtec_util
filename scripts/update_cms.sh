#!/bin/bash
# ----------------------------------------------------------- 
# UNIX Shell Script File Name: update.sh
# Operating System: RHEL 4.0
# Shell Used: BASH shell
# Produced By: Douglas.Gaer@noaa.gov
# File Creation Date: 05/20/2008
# Date Last Modified: 01/15/2009
# ----------------------------------------------------------- 
# ------------- Program Description and Details ------------- 
# ----------------------------------------------------------- 
#
# Update the WWA table using the new Web directories on CMS
#
# ----------------------------------------------------------- 

trap ExitProc SIGHUP SIGINT SIGTERM SIGQUIT SIGABRT

function ExitProc() {
    echo "$PROGRAMname has been signaled to terminate"
    echo "Running clean-up and exit proceedure"
    rm -f $LOCKfile 
    echo "Exiting..."
    exit 1
}

export TZ=UTC

# Script varaibles
CWA="MFL"
RUNdir="/home/homepage/hazards"
BINdir="$RUNdir/bin"
LOGdir="$RUNdir/logs"
VARdir="$RUNdir/var"
ETCdir="$RUNdir/etc"
SPOOLdir="/tmp/vtec_util"
WEBdir="/www/apps-data/localwfo/mfl/hazards/wwa"
SASWEBdir="/www/html/intranet/SAS/hazards/wwa"
ICONdir="/images/mfl/hazards/icons"

# NOTE: If this process runs on LDAD you must change the following variables
#RUNdir="/data/ldad/localhazards"
#WEBdir="/data/ldad/web/apps-data/mfl/hazards/wwa"
#SASWEBdir="/data/ldad/localhazards/SAS"

# Process locking variables
PROGRAMname="$0"
LOCKfile="$VARdir/CWA.lck"
MINold="5"

source $BINdir/process_lock.sh

# Script's main thread starts here
# -----------------------------------------------------------
user=`whoami`
if [ $user != homepage ]
    then
    echo "You must be homepage to execute this script "
    echo "Exiting..."
    exit
fi

mkdir -p $ETCdir
mkdir -p $LOGdir
mkdir -p $VARdir
mkdir -p $WEBdir
mkdir -p $SASWEBdir
mkdir -p $ICONdir
mkdir -p $SPOOLdir
 
datetime=`date`
echo "Generating the Watch, Warning, and Advisory table for $CWA"
echo "Logging output to $LOGdir/$CWA.log"

LockFileCheck $MINold

CreateLockFile

echo "Purging polygon warning Web pages older than 3 days"
find $WEBdir/*$CWA*.txt -mtime +3 -type f -print | xargs rm -f

# Set the data directory here
# Try the WWA trigger DIR first
DATAdir="/data/WWtriggers"
input="--data-dir=$DATAdir"

if [ ! -e "$DATAdir" ]
    then 
    echo "WWA Trigger $DATAdir input data DIR does not exist"
    echo "Will try to use the local LDM database"
    DATAdir="/data/$CWAshort"
    input="--data-dir=$DATAdir"
fi

if [ ! -e "$DATAdir" ]
    then 
    echo "LDM $DATAdir input data DIR does not exist"
    echo "Will try to download from the SRH Website"
    input="--get-web-products"
fi

# Set all your program options here
OPTS="--html-include=.txt --warning-table --google-maps --shortfused-table --no-dynamic-zones --drop-routine-products"
CMDLineOPTS="$1 $2 $3 $4 $5 $6 $7 $8 $9"

# Cron job wants to force Web products because AWIPS is down for a build
if [ "$1" == "--get-web-products" ]
    then
    input="--get-web-products"
    CMDLineOPTS="$2 $3 $4 $5 $6 $7 $8 $9"
fi

cd $ETCdir
$BINdir/vtec_util --config-file="$ETCdir/$CWA.cfg" --log-file-clear="$LOGdir/$CWA.log" --icon-dir="$ICONdir" --output-dir="$WEBdir" $input $OPTS $CMDLineOPTS
# Program exits with 0 if successful or 1 if any errors occur 
if [ "$?" != "0" ]
    then
    echo "Error generating the table"
    echo "No Web pages will be uploaded to the $CWA Website"
    RemoveLockFile
    exit
fi

# Set all your SAS program options here
OPTS="--warning-table --google-maps --no-dynamic-zones --drop-routine-products --key-table-in-page --ver-table-in-page --add-sas-refresh-tag=60"
CMDLineOPTS="$1 $2 $3 $4 $5 $6 $7 $8 $9"

# Cron job wants to force Web products because AWIPS is down for a build
if [ "$1" == "--get-web-products" ]
    then
    input="--get-web-products"
    CMDLineOPTS="$2 $3 $4 $5 $6 $7 $8 $9"
fi

cd $ETCdir
$BINdir/vtec_util --config-file="$ETCdir/$CWA-sas.cfg" --log-file-clear="$LOGdir/$CWA-sas.log" --icon-dir="$ICONdir" --output-dir="$SASWEBdir" $input $OPTS $CMDLineOPTS
# Program exits with 0 if successful or 1 if any errors occur 
if [ "$?" != "0" ]
    then
    echo "Error generating the SAS table"
    RemoveLockFile
    exit
fi

RemoveLockFile

echo "$CWA Web table script complete."
echo "Exiting script..."
exit
# ----------------------------------------------------------- 
# ******************************* 
# ********* End of File ********* 
# ******************************* 