
function getDateString()
{
	// This will work until 2090 -- what do we do then?
	months= new Array("January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December");	
	days = new Array("Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday");
	
	today = new Date();
	return(days[today.getDay()] + ", " + months[today.getMonth()] + " " + today.getDate())
}

