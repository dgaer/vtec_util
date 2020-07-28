#!/bin/csh
echo "Rebuidling the WWA trigger database on WWW"

/data/local/web/vtec_util/scripts/WWA_trigger.csh MIACEMMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIACFWMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIACWFMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIAFFAMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAFFSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAFFWMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAFLSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAFLWMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIAHLSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAHWOMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAMWSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIANOWMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIANPWMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIAPNSMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIARFWMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIARVSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIASMWMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIASPSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIASVRMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIASVSMFL
/data/local/web/vtec_util/scripts/WWA_trigger.csh MIATORMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIAWCNMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIAWSWMFL
/data/local/web/vtec_util/scripts/WWA_trigger_noarch.csh MIAMWWMFL

echo "Rebuild complete"
exit
