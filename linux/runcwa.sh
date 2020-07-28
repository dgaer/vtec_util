#!/bin/bash

export TZ=UTC

if [ "$1" == "" ] 
    then
    echo "You must supply a CWA"
    exit
fi

PROGRAM="../vtec_util"
CWA="$1"
ICONdir="../../../icons"
OUTPUTdir="html"
DATAdir="/data/ldm/$CWA"
input="--data-dir=$DATAdir"

if [ ! -e "$DATAdir" ]
    then 
    echo "$DATAdir input data DIR does not exist"
    echo "Will try to download from the SRH Website"
    input="--get-web-products"
fi

# Force Web updated for testing
input="--get-web-products"

# Test the different options here
#OPTS="--warning-table --google-maps --dump-zone-table --match-dynamic-names --verbose --debug --arch-mode" 
#OPTS="--warning-table --dump-zone-table --match-dynamic-names --verbose --debug" 
OPTS="--warning-table --no-dynamic-zones  --verbose --debug" 

##OPTS="--warning-table --google-maps --dump-zone-table --match-dynamic-names --verbose --debug" 
##OPTS="--warning-table --google-maps --verbose --debug  --no-dynamic-zones"
##OPTS="--warning-table --google-maps --verbose --debug --arch-mode"

# Save testing time by using previously downloaded Web products
##input="--data-dir=/tmp/vtec_util"

mkdir -p $CWA
cd $CWA

$PROGRAM --cwa=$CWA --config-file=$CWA.cfg --icon-dir=$ICONdir --output-dir=$OUTPUTdir --log-file-clear=$CWA.log $input $OPTS $2 $3 $4 $5 $6 $7 $8 $9


