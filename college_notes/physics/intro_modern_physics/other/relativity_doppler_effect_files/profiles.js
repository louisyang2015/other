
//
// profile functions, for making pages customizable
//



function switch_profile(newprofile) {
	setCookie ("profile", newprofile, "1000");
	document.location.reload(true);
}

function switch_profile_norefresh(newprofile) {
	setCookie ("profile", newprofile, "1000");
}

function refresh_profile() {
	setCookie ("profile", current_profile(), "1000");
}

function current_profile() {
	if (getCookie("profile") != "undefined")
	{
		return getCookie("profile");
	} else {
		switch_profile("bw");
		return getCookie("profile");
	}
}



//
// returns the color the page is, based on the set cookie
// returns white if no cookie exists (because "bw" is the default)
//

function color_pick () {
	if (current_profile() == null)
		return ("#ffffff");
	else if (current_profile() == "bw")
		return ("#ffffff");
	else if (current_profile() == "antique")
		return ("#f4ecd9");
	else if (current_profile() == "blue")
		return ("#eeeeff");
	else if (current_profile() == "gray")
		return ("#eeeeee");
	else if (current_profile() == "white")
		return ("#ffffff");
	else if (current_profile() == "low")
		return ("#f9e2e3");
	else
		return ("#ffffff");

}

//
// builds the location of the graphic.. for example
// "graphics/profiles/antique_top.gif"
// where 'top' is the type value passed to the function
// and 'antique' is pulled from the cookies
// defaults to 'bw', if the cookie doesn't exist
//


function graphic_pick (type) {

	if (current_profile() != null)
		x = "http://img.sparknotes.com/graphics/" + current_profile() + "_" + type + ".gif";
	else
		x = "http://img.sparknotes.com/graphics/bw_" + type + ".gif";
	return (x);

}

refresh_profile();