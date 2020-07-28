<?php
// ------------------------------- //
// -------- Start of File -------- //
// ------------------------------- //
// ----------------------------------------------------------- //
// PHP Source Code File
// PHP Interpreter Version: 5.1.6
// Original Author: Douglas.Gaer@noaa.gov
// File Creation Date: 01/14/2009
// Date Last Modified: 01/15/2009
//
// Version control: 1.01
//
// Support Team: 
//
// Contributors:
// ----------------------------------------------------------- //
// ------------- Program Description and Details ------------- //
// ----------------------------------------------------------- //
/*
CMS page used to request a MAP file from a sites APPS-DATA
directory.
*/
// ----------------------------------------------------------- //
// SCRIPT CONTENT BEGINS BELOW.
?>

<?php
// Set up this page's environment for CMS and non-CMS sites
if(!$PageSiteID) $PageSiteID = "mfl";
if(!$PageTitle) $PageTitle = "Polygon Warning MAP";
if(!$IncludePath) $IncludePath = "/www/html/" . $PageSiteID . "/includes";
if(!$AppsDataPath) $AppsDataPath = "/www/apps-data/localwfo/" . $PageSiteID . "/hazards/wwa";

// Read PHP super globals passed to us from a hyperlink or browser
$page_req = $_GET['n'];
$title_req = $_GET['title'];

// Validate the variables passed to this script 
if($title_req) $PageTitle = $title_req;
if($page_req) $mycontentpage = $AppsDataPath . "/" . $page_req . ".txt";

$has_headers = 1; // Check for all the PHP include files 
if(!file_exists("$IncludePath/main_header.php")) $has_headers = 0;
if(!file_exists("$IncludePath/main_left.php")) $has_headers = 0;
if(!file_exists("$IncludePath/main_footer.php")) $has_headers = 0;

// Include the header and left menu
if($has_headers) include("$IncludePath/main_header.php");
if($has_headers) include("$IncludePath/main_left.php");

if(!$mycontentpage) {
  echo "<p><h1>No page request made</h1></p>\n";
}
else {
  if(!file_exists("$mycontentpage")) {
    echo "<p><h1>No valid page requested</h1></p>\n";
  }
  else {
    include("$mycontentpage");
  }
}

// Include the page footer
if($has_headers) include("$IncludePath/main_footer.php");
?>

<?php
// SCRIPT CONTENT ENDS ABOVE.
// ----------------------------------------------------------- //
// ------------------------------- //
// --------- End of File --------- //
// ------------------------------- //
?>
