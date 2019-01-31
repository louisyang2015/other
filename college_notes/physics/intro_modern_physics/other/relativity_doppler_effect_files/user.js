
//
// Stores the Current Page for the user, for up to one month
//
                
function saveBookmark() {
        
        var today = new Date();
        var currenturl = window.location + "";
        var currenttitle = document.title;
        var identifier = currenturl.substring(7,9);

        currenttitle = currenttitle.substring(12, currenttitle.length);

        setCookie ("bookmark", currenturl, "1000");
        setCookie ("new_bookmark", currenttitle, "1000");
        
		if (identifier == "ww")
		{
			if (currenturl.indexOf(".dir") == -1)
			{
				setCookie ("last_guide_url", currenturl, "1000");
				setCookie ("last_guide_title", currenttitle, "1000");
			}
		} else if (identifier == "mb")
		{
		var boardid_start = currenturl.indexOf("b=");
		var boardid_end = currenturl.indexOf("&");
		if (boardid_end == -1) {
			boardid_end = currenturl.indexOf("#");
			if (boardid_end == -1) boardid_end = currenturl.length+1
		};
		if (boardid_start != -1)
			{
				currenturl = "http://mb.sparknotes.com/mb.mpl?" + currenturl.substring(boardid_start, boardid_end);
			} else {
				currenturl = null;
				currenttitle = null;
			}
		setCookie ("last_board_url", currenturl, "1000");
        setCookie ("last_board_title", currenttitle, "1000");
		}


}

                      
                 
//       
// Reloads the stored page
//
  
function loadBookmark() {
  
        var newurl = getCookie("bookmark");

        if (newurl != null && newurl != "") {
                window.top.location = newurl;
        }
        else {
                window.alert ("Redirect failed.");
        }
        
}
 


//
//  inserts a line of small text allowing the user to load
//  the bookmark
//

function insertTextBookmark() {

	var last_page = getCookie("bookmark");
	var title = getCookie("new_bookmark");
	
	show_bookmark = ((last_page != null) && (last_page != "") && (title != null) && (title != ""));

	if (show_bookmark) {
		document.write ("<font size=\"1\" face=\"Verdana, Arial, sans-serif\">"
		                + "Welcome back.  Still studying <A HREF=\"" 
                        + last_page
                        + "\" class=\"note_link\">" 
                        + title 
                        + "</A>?"
                        + "<br><br>"
                        + "</font>");
	}


}



//
//  inserts a line colored "color" of small text allowing the user to load
//  the bookmark
//

function printBookMarkUserBox(color) {

	var last_page = getCookie("bookmark");
	var title = getCookie("new_bookmark");
	
	show_bookmark = ((last_page != null) && (last_page != "") && (title != null) && (title != ""));

	if (show_bookmark) {
		document.write ("Still studying <A HREF=\"JavaScript:loadBookmark()\"><font color=" + color + ">" 
                        + title 
                        + "</font>"
                        + "</A>?");
	} else {
        document.write ("Remember: SparkNotes has Study Guides for <i>every</i> subject.");
    }


}


//
//  returns a link (colored "color") to the last visited Study Guide or null otherwise
//

function lastGuideVisited(color) {

	var url = getCookie("last_guide_url");
	var title = getCookie("last_guide_title");
	
	show_bookmark = ((url != null) && (url != "") && (title != null) && (title != ""));

	if (show_bookmark) {
		return "<A HREF=\"" + url + "\"><font color=" + color + ">" 
                        + title 
                        + "</font>"
                        + "</A>";
	} else {
        return null;
    }

}

//
//  returns a link (colored "color") to the last visited Message Board or null otherwise
//

function lastBoardVisited(color) {

	var url = getCookie("last_board_url");
	var title = getCookie("last_board_title");
	
	show_bookmark = ((url != null) && (url != "") && (title != null) && (title != ""));

	if (show_bookmark) {
		return "<A HREF=\"" + url + "\"><font color=" + color + ">" 
                        + title 
                        + "</font>"
                        + "</A>";
	} else {
        return null;
    }

}