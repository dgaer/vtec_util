#!/bin/bash
# UNIX Shell Script
# Tested Operating System(s): RHEL 3, 4, 5
# Tested Run Level(s): 3, 5
# Shell Used: BASH shell
# Original Author(s): Douglas.Gaer@noaa.gov
# File Creation Date: 01/27/2009
# Date Last Modified: 01/23/2014
#
# Version control: 1.36
#
# Support Team:
#
# Contributors:
# ----------------------------------------------------------- 
# ------------- Program Description and Details ------------- 
# ----------------------------------------------------------- 
#
# HLS text product test formatter for Graphical HLS
#
# NOTE: This is a test program only. Do not distribute
#
# ----------------------------------------------------------- 
SHELL="/bin/bash"

PRODUCT="WRKHLS"
PRODUCTdir="."
WEBAPPSbin="../linux"
GHLSdir="."

if [ "$1" != "" ]; then PRODUCT="${1}"; fi;

if [ ! -e "${PRODUCT}" ]
    then 
    echo "ERROR - ${PRODUCT} does not exit"
    exit 1
fi

echo "Enter storm number. Like storm number 1 for 2014 would be 01"
read stormnum
echo "Enter name year. Like 2014 would be 14"
read stormyr
echo "STORM NUMBER AND YEAR: ${stormnum}${stormyr}"
echo "Enter storm NAME. Like Tropical Storm Wilma or Hurricane Wilma"
read STORMNAME
stormname=`echo ${STORMNAME} | tr [:lower:] [:upper:]`
echo "STORM NAME IS: $stormname"
    

# create files to hold text and open file handles
echo "<a href=\"javascript:ghlsPopUp('http://www.nhc.noaa.gov/storm_graphics/AT${stormnum}/AL${stormnum}${stormyr}W5.gif');\">Forecast Track</a>" > ${PRODUCTdir}/track.txt
echo "$stormname" > ${PRODUCTdir}/stormname.txt

dotext=""

${WEBAPPSbin}/vtec_util --read-product=${GHLSdir}/wind.cfg ${PRODUCTdir}/${PRODUCT}${dotext} > ${PRODUCTdir}/wind.txt
${WEBAPPSbin}/vtec_util --read-product=${GHLSdir}/surge.cfg ${PRODUCTdir}/${PRODUCT}${dotext} > ${PRODUCTdir}/surge.txt
${WEBAPPSbin}/vtec_util --read-product=${GHLSdir}/flood.cfg ${PRODUCTdir}/${PRODUCT}${dotext} > ${PRODUCTdir}/flood.txt
${WEBAPPSbin}/vtec_util --read-product=${GHLSdir}/tornado.cfg ${PRODUCTdir}/${PRODUCT}${dotext} > ${PRODUCTdir}/tornado.txt
${WEBAPPSbin}/vtec_util --read-product=${GHLSdir}/marine.cfg ${PRODUCTdir}/${PRODUCT}${dotext} > ${PRODUCTdir}/marine.txt

mkdir -p webfiles

cp -fp wind.txt webfiles/wind.html
cp -fp surge.txt webfiles/surge.html
cp -fp flood.txt webfiles/flood.html
cp -fp tornado.txt webfiles/tornado.html
cp -fp marine.txt webfiles/marine.html

# Trigger the Web side PHP script to display the ACTIVE GHLS logo
date +%s > ${PRODUCTdir}/ghls_active.txt
echo ${stormnum} >> ${PRODUCTdir}/ghls_active.txt
echo ${stormname} >> ${PRODUCTdir}/ghls_active.txt
echo ${stormyr} >> ${PRODUCTdir}/ghls_active.txt

echo "Exiting"
exit 0
# ----------------------------------------------------------- 
# ******************************* 
# ********* End of File ********* 
# ******************************* 
