#!/bin/csh
# ----------------------------------------------------------- 
# UNIX Shell Script File Name: /data/local/web/vtec_util/WWA_table_trigger.csh
# Operating System: RHEL 4.0
# Shell Used: C shell
# Produced By: Douglas.Gaer@noaa.gov
# File Creation Date: 02/25/2008
# Date Last Modified: 08/19/2008
# ----------------------------------------------------------- 
# ------------- Program Description and Details ------------- 
# ----------------------------------------------------------- 
#
# Script file used to SCP products to the /data/WWtrigger DIR on the 
# LDM server. This script will request a product from the text database
# and send a copy to the LDM server. If the --arch argument is 
# supplied the current copy and and archive copy will be sent.
#
# Usage 1:
# WWA_table_trigger.csh PIL
#
# Usage 2:
# WWA_table_trigger.csh PIL --arch
#
# ----------------------------------------------------------- 

set CWA="MFL"
set HOMEdir="/data/local/web/vtec_util"
set SPOOLdir="${HOMEdir}/spool"
mkdir -p ${SPOOLdir}
cd ${SPOOLdir}

# Setup the product time stamp
set YYYY=`date +%Y`
set YY=`date +%y`
set MM=`date +%m`
set DD=`date +%d`
set HH=`date +%H`
set MI=`date +%M`
set SS=`date +%S`

set timestamp=${YY}${MM}${DD}${HH}${MI}${SS}
set currfile=`echo $1 | cut -c 4-9`
set archfile=${currfile}.${timestamp}

# Request the product from the TEXT database
textdb -r  ${1} > ${archfile}

# SCP the files to WWW server or LDAD
echo "Sending ${currfile}"
scp -o stricthostkeychecking=no ${archfile} ldm@www:/data/WWtriggers/${currfile}

if ( "$2" == "--arch" ) then 
    echo "Sending archive copy ${archfile}"
    scp -o stricthostkeychecking=no ${archfile} ldm@www:/data/WWtriggers/${archfile}
endif

# Cleanup the spool directory
/bin/rm -f ${archfile}

echo "FINISHED SCRIPT"

exit
# ----------------------------------------------------------- 
# ******************************* 
# ********* End of File ********* 
# ******************************* 
