#!/bin/csh
# ----------------------------------------------------------- 
# UNIX Shell Script File Name: WWA_trigger_noarch.csh
# Operating System: RHEL 4.0
# Shell Used: C shell
# Produced By: Douglas.Gaer@noaa.gov
# File Creation Date: 02/25/2008
# Date Last Modified: 08/19/2008
# ----------------------------------------------------------- 
# ------------- Program Description and Details ------------- 
# ----------------------------------------------------------- 
#
# Script file used to SCP a single copy of current product to 
# /data/WWtrigger DIR on the LDM server. 
#
# ----------------------------------------------------------- 

set HOMEdir="/data/local/web/vtec_util"
set BINdir="${HOMEdir}/scripts"

${BINdir}/WWA_table_trigger.csh $1

exit
# ----------------------------------------------------------- 
# ******************************* 
# ********* End of File ********* 
# ******************************* 
