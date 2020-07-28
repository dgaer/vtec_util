#!/bin/bash
# ----------------------------------------------------------- 
# UNIX Shell Script File Name: update.sh
# Operating System: RHEL 4.0
# Shell Used: BASH shell
# Produced By: Douglas.Gaer@noaa.gov
# File Creation Date: 05/20/2008
# Date Last Modified: 08/18/2008
# ----------------------------------------------------------- 
# ------------- Program Description and Details ------------- 
# ----------------------------------------------------------- 
#
# Update the prototype WWA table
#
# ----------------------------------------------------------- 

trap ExitProc SIGHUP SIGINT SIGTERM SIGQUIT SIGABRT

function ExitProc() {
    echo "$PROGRAMname has been signaled to terminate"
    echo "Running clean-up and exit proceedure"
    rm -f $LOCKfile 
    rm -f $VARdir/$CWA-slxfer.cmd
    echo "Exiting..."
    exit 1
}

export TZ=UTC

# Script varaibles
CWA="MFL"
HOMEdir="/home/homepage/hazards"
BINdir="$HOMEdir/bin"
LOGdir="$HOMEdir/logs"
VARdir="$HOMEdir/var"
ETCdir="$HOMEdir/etc"
SPOOLdir="/tmp/vtec_util"
WEBdir="/www/html/mfl/hazards/wwa"
SASWEBdir="/www/html/intranet/SAS/hazards/wwa"
ICONdir="/mfl/images/mfl/hazards"

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

datetime=`date`
echo "Generating the Watch, Warning, and Advisory table for $CWA"
echo "Logging output to $LOGdir/$CWA.log"

LockFileCheck $MINold

CreateLockFile

echo "Purging polygon warning Web pages older than 3 days"
find $WEBdir/*$CWA*.php -mtime +3 -type f -print | xargs rm -f

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
OPTS="--php --warning-table --google-maps --shortfused-table --no-dynamic-zones --drop-routine-products"
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

# TODO: Remove the FTP section when we swithc to rsync
# Generate Commands to FTP
cd $WEBdir
echo "Updating the $CWA Web Site via FTP file transfer"
cat /dev/null > $VARdir/$CWA-slxfer.cmd
username=`cat /home/homepage/.martha2-u`
passwd=`cat /home/homepage/.martha2-p`
echo "user $username $passwd" >> $VARdir/$CWA-slxfer.cmd
echo "cd hazards" >> $VARdir/$CWA-slxfer.cmd
echo "cd wwa" >> $VARdir/$CWA-slxfer.cmd
echo "bin" >> $VARdir/$CWA-slxfer.cmd
echo "prompt" >> $VARdir/$CWA-slxfer.cmd

ftpfiles=`ls -1rat *$CWA*`
for i in $ftpfiles
  do
  echo "put $i" >> $VARdir/$CWA-slxfer.cmd
done
echo "bye" >> $VARdir/$CWA-slxfer.cmd

ftp -i -v -d -n martha2.srh.noaa.gov <$VARdir/$CWA-slxfer.cmd > $LOGdir/$CWA-ftp.log 2>&1
cat $LOGdir/$CWA-ftp.log | grep -i "226 File receive OK" > /dev/null 2>&1
if [ "$?" != "0" ]
    then
    echo "Error FTP file transfer failed for $CWA"
fi
rm -f $VARdir/$CWA-slxfer.cmd

RemoveLockFile

echo "$CWA Web table script complete."
echo "Exiting script..."
exit
# ----------------------------------------------------------- 
# ******************************* 
# ********* End of File ********* 
# ******************************* 
