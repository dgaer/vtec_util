// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- // 
// JavaScript File
// Browser support: Internet Explorer, Netscape, Firefox, Chrome 
// Original Author: Douglas.Gaer@noaa.gov
// File Creation Date: 05/29/2008
// Date Last Modified: 05/07/2009
//
// Version control: 2.01
//
// Support Team:
//
// Contributors:    
// ----------------------------------------------------------- // 
// ------------- Program Description and Details ------------- // 
// ----------------------------------------------------------- // 
//
// Mouse over popup Zone function calls for decoded HLS
//
// ----------------------------------------------------------- // 

// Enable/Disable captions. Defaults to true.
var DisplayMouseOverCaptions = true;

// Setup the default caption message
var DefaultCaptionMessage = "NONE REPORTED IN THIS UPDATE";
var HasCaptionMessage = false;

// TODO: This should be false in the product version
// var DisplayDefaultCaption = false;
var DisplayDefaultCaption = true;

var CaptionMessage = DefaultCaptionMessage;

// Caption setup and working variables
var CaptionDOM = (document.getElementById) ? true : false;
var CaptionFireFox = (!document.all && CaptionDOM || window.opera) ? true: false;
var CaptionIE5 = ((navigator.userAgent.indexOf("MSIE")>-1) && CaptionDOM) ? true : false;
var CaptionIE4 = (document.all && !CaptionDOM) ? true : false;
var CaptionNoDynamic = (!CaptionFireFox && !CaptionIE4 && !CaptionIE5 && !CaptionDOM) ? true : false;
var CaptionFollowMouse= true;	
var CaptionHeight = "auto";
var CaptionWidth= "550px";
var CaptionOffsetX= 20;
var CaptionOffsetY= 12; 
var CaptionFontFamily= "Verdana, arial, helvetica, sans-serif";
var CaptionFontSize= "8pt";
var CaptionFontColor= "#000000";
var CaptionBgColor= "#FFFFFF"; 
var CaptionBorderColor= "#000080";
var CaptionTitleColor = "#000000";
var CaptionTitleFontSize = "8pt";
var CaptionTitleFontColor = "#FFFFFF"
var CaptionBorderWidth= 3;
var CaptionBorderStyle= "ridge";
var CaptionPadding= 4;
var Captiontool;
var Captioncss;
var CaptionTimeout1, CaptionTimeout2;
var CaptionIsOn = false;
var CaptionMouseX = 0;
var CaptionMouseY = 0;
var CaptionDebugText = "";

// TODO: Always turn off debugging in release version
var CaptionDebug = true;
// var CaptionDebug = false;

function CaptionPosition(evt) {
  if(!CaptionFollowMouse) {
    standardbody=(document.compatMode=="CSS1Compat")? document.documentElement : document.body
    CaptionMouseX = (CaptionFireFox)? evt.pageX: window.event.clientX + standardbody.scrollLeft;
    CaptionMouseY = (CaptionFireFox)? evt.pageY: window.event.clientY + standardbody.scrollTop;
  }

  var tpWd = (CaptionIE4||CaptionIE5)? Captiontool.clientWidth: Captiontool.offsetWidth;
  var tpHt = (CaptionIE4||CaptionIE5)? Captiontool.clientHeight: Captiontool.offsetHeight;
  var winWd = (CaptionFireFox)? window.innerWidth-20+window.pageXOffset: standardbody.clientWidth+standardbody.scrollLeft;
  var winHt = (CaptionFireFox)? window.innerHeight-20+window.pageYOffset: standardbody.clientHeight+standardbody.scrollTop;

  if((CaptionMouseX+CaptionOffsetX+tpWd)>winWd) {
    Captioncss.left = CaptionMouseX-(tpWd+CaptionOffsetX) + "px";
  }
  else {
    Captioncss.left = CaptionMouseX+CaptionOffsetX + "px";
  }

  if((CaptionMouseY+CaptionOffsetY+tpHt) > winHt) { 
    Captioncss.top = winHt-(tpHt+CaptionOffsetY) + "px";
  }
  else {
    Captioncss.top = CaptionMouseY+CaptionOffsetY + "px";
  }

  if(!CaptionFollowMouse) {
    CaptionTimeout1=setTimeout("Captioncss.visibility='visible'",100);
  }
}

function CaptionTrackMouse(evt) {
  standardbody=(document.compatMode=="CSS1Compat")? document.documentElement : document.body 
  CaptionMouseX = (CaptionFireFox)? evt.pageX: window.event.clientX + standardbody.scrollLeft;
  CaptionMouseY = (CaptionFireFox)? evt.pageY: window.event.clientY + standardbody.scrollTop;
  if(CaptionIsOn) CaptionPosition(evt);
}

function CaptionInit() {
  if(CaptionNoDynamic) return;
  Captiontool = (CaptionIE4)? document.all['CaptionDiv']: (CaptionIE5||CaptionFireFox)? document.getElementById('CaptionDiv'): null;
  Captioncss = Captiontool.style;
  if(CaptionIE4 || CaptionIE5 || CaptionFireFox) {
    Captioncss.width = CaptionWidth;
    Captioncss.height = CaptionHeight;
    Captioncss.fontFamily = CaptionFontFamily;
    Captioncss.fontSize = CaptionFontSize;
    Captioncss.color = CaptionFontColor;
    Captioncss.backgroundColor = CaptionBgColor;
    Captioncss.borderColor = CaptionBorderColor;
    Captioncss.borderWidth = CaptionBorderWidth;
    Captioncss.padding = CaptionPadding;
    Captioncss.borderStyle = CaptionBorderStyle;
  }
  if(Captiontool && CaptionFollowMouse) {
    document.onmousemove = CaptionTrackMouse;
  }
}

function CaptionOn(evt, num, title, zonenumber, zonename) {
  // Mouse over caption calls	
  // JS mouseover, tag: onmouseover="CaptionOn(evt, 1, title, 'FLZ074', 'METRO MIAMI DADE');"
  // JS mouseout, tag: onmouseout="CaptionOff();"
  // 508 compliant caption calls, required for non-mouse users 
  // JS onfocus, tag: onfocus="CaptionOn(evt, 1, title, 'FLZ074', 'METRO MIAMI DADE');"
  // JS onblur, tag, onblur="CaptionOff();"
	
  if(!DisplayMouseOverCaptions) return;  
  if(!Captiontool) return;

  VTECGetText(zonenumber);
  if(!HasCaptionMessage) { 
    CaptionMessage = DefaultCaptionMessage; 
    if(!DisplayDefaultCaption) return;
  }
  if(CaptionTimeout1) clearTimeout(CaptionTimeout1);	
  if(CaptionTimeout2) clearTimeout(CaptionTimeout2);
  CaptionIsOn = true;

  var fonttag = 'font-family:' + CaptionFontFamily + '; font-size:' + CaptionFontSize + '; color:' + CaptionFontColor + ';'; 
  var fontstart = '<span style="' + fonttag + '">'; 
  var fontend = '</span>';

  var fonttagtitle = 'font-family:' + CaptionFontFamily + '; font-size:' + CaptionTitleFontSize + '; color:' + CaptionTitleFontColor + ';'; 
  var fontstarttitle = '<span style="' + fonttagtitle + '">'; 
  var fontendtitle = '</span>';

  if(CaptionIE4 || CaptionIE5 || CaptionFireFox) {
    // Short and long titles	
    // var mytitle = fontstarttitle + '<strong>' + zonename + '</strong>' + fontendtitle; 	
    mytitle = fontstarttitle + title + ' STATEMENT AS EXPECTED ACCORDING TO CURRENT FORECAST<br />NOTE: When accounting for uncertainty the graphics depict potential impact.<br />' + zonenumber + ' <strong>' + zonename + '</strong>' + fontendtitle;
    if(CaptionDebug) {	
       CaptionDebugText = '<br />' + ' mouseX = ' + CaptionMouseX + ' ' + ' mouseY = ' + CaptionMouseY + ' </br />';
     if(CaptionFireFox) {
        CaptionDebugText = '<br />' + CaptionDebugText + 'Browser is FireFox <br />';  
      } 
      if(CaptionIE4) {
        CaptionDebugText = '<br />' + CaptionDebugText + 'Browser is IE4 <br />';  
      } 
      if(CaptionIE5) {
        CaptionDebugText = '<br />' + CaptionDebugText + 'Browser is IE5 of higher <br />';  
      } 

    }
    var titletable = '<table border="0" width="' + CaptionWidth + '"><tr><td bgcolor="' + CaptionTitleColor + '">' + mytitle + '</td></tr></table>';	
    var Caption =  '<table border="0" width="' + CaptionWidth + '"><tr><td>' + fontstart + '<pre>' + CaptionMessage + '</pre>' + fontend;
    Caption = Caption + '</td></tr></table>';
    if(CaptionDebug) {	
      Caption = Caption + '<strong><pre>' + CaptionDebugText + '</strong></pre>\n';
    }
    Captioncss.backgroundColor = CaptionBgColor;
    Captiontool.innerHTML = titletable + Caption;
  }

  if(!CaptionFollowMouse) {
    CaptionPosition(evt);
  }
  else {
    CaptionTimeout1 = setTimeout("Captioncss.visibility='visible'",100);
  }
}

function CaptionOff() {
  // JS Mouse out function, tag: onmouseout="CaptionOff()"
  if(!Captiontool) return;
  CaptionTimeout2 = setTimeout("Captioncss.visibility='hidden'", 100);
  CaptionIsOn = false;
}

function CaptionLoadEvent(func) {
  var oldonload = window.onload;
  if (typeof window.onload != 'function') {
    window.onload = func;
  }
  else {
    window.onload = function() {
     if (oldonload) {
       oldonload();
     }
    func();
   }
  }
}

// Start the caption code
if (CaptionNoDynamic) { event = "nope" }
CaptionLoadEvent(CaptionInit);
document.write('<div id="CaptionDiv" style="position:absolute; visibility:hidden; z-index:100"></div>');

// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
