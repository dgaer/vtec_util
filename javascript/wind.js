
<!-- BEGIN BLOCK1 -->
<script language="javascript" type="text/javascript">
<!--

var HasAllZonesMessage = false;

var VTECSegmentText = new Array();
VTECSegmentText[0] = Array('\
THIS IS A WINDS TEST IN SEG 1.');
VTECSegmentText[1] = Array('\
THIS IS A WINDS TEST IN SEG 2.');
VTECSegmentText[2] = Array('\
THIS IS A WINDS TEST IN SEG 3.');

var VTECZoneListSegment = new Array();
VTECZoneListSegment[0] = new Array("FLZ168", "FLZ172", "FLZ173", "FLZ174");
VTECZoneListSegment[1] = new Array("FLZ063", "FLZ066", "FLZ067", "FLZ068", "FLZ070", "FLZ071", "FLZ072", "FLZ073", "FLZ074");
VTECZoneListSegment[2] = new Array("AMZ610", "AMZ630", "AMZ650", "AMZ651", "AMZ670", "AMZ671", "GMZ656", "GMZ657", "GMZ676");

function VTECGetText(zonenumber) { 
  HasCaptionMessage = false;
  CaptionMessage = DefaultCaptionMessage;

  var text = "";
  var numzonelists = VTECZoneListSegment.length;

  var i, j;
  for(i = 0; i < numzonelists; i++) {
     var n = VTECZoneListSegment[i].length;
     for(j = 0; j < n; j++) {
      if(VTECZoneListSegment[i][j] == zonenumber) {
        HasCaptionMessage = true;
	text += VTECSegmentText[i];
	break;
      }
    }
  }

  if(HasCaptionMessage) CaptionMessage = text;

}

// -->
</script>

<!-- END BLOCK1 -->

<!-- BEGIN BLOCK2 -->
<script language="javascript" type="text/javascript">
<!--
var AllZonesCaptionMessage = '\
SUITER IS BECOMING AN INCREASINGLY SERIOUS THREAT TO SOUTH FLORIDA.\n\
HOWEVER...IT IS STILL TOO EARLY TO FOCUS ON EXACT WIND AND SURGE \n\
FORECAST VALUES FOR SPECIFIC LOCATIONS. WHEN MAKING DECISIONS...DO \n\
NOT FOCUS ON THE EXACT FORECAST TRACK. A GENERAL CONCERN SHOULD BE \n\
FOR THE POSSIBILITY OF AT LEAST EXTENSIVE DAMAGE SOMEWHERE WITHIN\n\
SOUTH FLORIDA MAINLY BASED ON WINDS BUT SURGE IS ALSO BECOMING AN \n\
INCREASING CONCERN. ALTHOUG NOT LIKELY AT THIS TIME...HEAVY AND \n\
FLOOD-PRODUCING RAINS ARE ALSO POSSIBLE ACROSS MANY PORTIONS OF \n\
SOUTH FLORIDA THIS WEEKEND, ALONG WITH THE POSSIBILITY OF ISOLATED \n\
TORNADOES IN RAIN BANDA WELL AHEAD OF THE STORM.\n\
\n\
';

DefaultCaptionMessage = AllZonesCaptionMessage;
HasAllZonesMessage = true;
// -->
</script>

<!-- END BLOCK2 -->

<!-- START OF OVERVIEW -->
<STRONG>
<P>
SITUATION OVERVIEW<BR>
EXPECTED CONDITIONS AND IMPACTS FOR ALL AREAS:<BR>
</P>
</STRONG>
<A NAME="ALLZONES" ID="ALLZONES"></A><FONT COLOR="BLACK">
<PRE>SUITER IS BECOMING AN INCREASINGLY SERIOUS THREAT TO SOUTH FLORIDA.
HOWEVER...IT IS STILL TOO EARLY TO FOCUS ON EXACT WIND AND SURGE 
FORECAST VALUES FOR SPECIFIC LOCATIONS. WHEN MAKING DECISIONS...DO 
NOT FOCUS ON THE EXACT FORECAST TRACK. A GENERAL CONCERN SHOULD BE 
FOR THE POSSIBILITY OF AT LEAST EXTENSIVE DAMAGE SOMEWHERE WITHIN
SOUTH FLORIDA MAINLY BASED ON WINDS BUT SURGE IS ALSO BECOMING AN 
INCREASING CONCERN. ALTHOUG NOT LIKELY AT THIS TIME...HEAVY AND 
FLOOD-PRODUCING RAINS ARE ALSO POSSIBLE ACROSS MANY PORTIONS OF 
SOUTH FLORIDA THIS WEEKEND, ALONG WITH THE POSSIBILITY OF ISOLATED 
TORNADOES IN RAIN BANDA WELL AHEAD OF THE STORM.

</PRE>
</FONT>
<HR>
<!-- END OF OVERVIEW -->
<STRONG>
<P>
...WINDS...<BR>
EXPECTED CONDITIONS AND IMPACTS FOR THE FOLLOWING AREAS:<BR>
</P>
</STRONG>
<HR>
<STRONG>
<A NAME="FLZ168" ID="FLZ168"></A><A NAME="FLZ172" ID="FLZ172"></A><A NAME="FLZ173" ID="FLZ173"></A><A NAME="FLZ174" ID="FLZ174"></A>COASTAL PALM BEACH, COASTAL BROWARD, COASTAL MIAMI DADE, FAR SOUTH MIAMI DADE
</STRONG>
<FONT COLOR="BLACK">
<PRE>
THIS IS A WINDS TEST IN SEG 1.
</PRE>
</FONT>
<HR>
<STRONG>
<A NAME="FLZ063" ID="FLZ063"></A><A NAME="FLZ066" ID="FLZ066"></A><A NAME="FLZ067" ID="FLZ067"></A><A NAME="FLZ068" ID="FLZ068"></A><A NAME="FLZ070" ID="FLZ070"></A><A NAME="FLZ071" ID="FLZ071"></A><A NAME="FLZ072" ID="FLZ072"></A><A NAME="FLZ073" ID="FLZ073"></A><A NAME="FLZ074" ID="FLZ074"></A>GLADES, HENDRY, INLAND PALM BEACH, METRO PALM BEACH, INLAND COLLIER, INLAND BROWARD, METRO BROWARD, INLAND MIAMI DADE, METRO MIAMI DADE
</STRONG>
<FONT COLOR="BLACK">
<PRE>
THIS IS A WINDS TEST IN SEG 2.
</PRE>
</FONT>
<HR>
<STRONG>
<A NAME="AMZ610" ID="AMZ610"></A><A NAME="AMZ630" ID="AMZ630"></A><A NAME="AMZ650" ID="AMZ650"></A><A NAME="AMZ651" ID="AMZ651"></A><A NAME="AMZ670" ID="AMZ670"></A><A NAME="AMZ671" ID="AMZ671"></A><A NAME="GMZ656" ID="GMZ656"></A><A NAME="GMZ657" ID="GMZ657"></A><A NAME="GMZ676" ID="GMZ676"></A>LAKE OKEECHOBEE, BISCAYNE BAY, COASTAL WATERS FROM JUPITER INLET TO DEERFIELD BEACH, FL OUT 20 NM, COASTAL WATERS FROM DEERFIELD BEACH TO OCEAN REEF, FL OUT 20 NM, WATERS FROM JUPITER INLET TO DEERFIELD BEACH, FL EXTENDING FROM 20 NM TO 60 NM, WATERS FROM DEERFIELD BEACH TO OCEAN REEF, FL EXTENDING FROM 20 NM TO THE TERRITORIAL WATERS OF THE BAHAMAS, COASTAL WATERS FROM CHOKOLOSKEE TO BONITA BEACH, FL OUT 20 NM, COASTAL WATERS FROM EAST CAPE SABLE TO CHOKOLOSKEE, FL OUT 20 NM, GULF WATERS FROM CHOKOLOSKEE TO BONITA BEACH, FL EXTENDING FROM  20 TO 60 NM
</STRONG>
<FONT COLOR="BLACK">
<PRE>
THIS IS A WINDS TEST IN SEG 3.
</PRE>
</FONT>
