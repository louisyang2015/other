// TODO - modify API url as needed
var api_url = "https://f0qj73614m.execute-api.us-west-1.amazonaws.com/beta/query-rds-db"

function Year_tb_keyup_handler(e) {
  // user pressing "enter" in the "year" textbox will start the search process
  if (e.keyCode == 13) search_button_click_handler();
}

function Ratings_cb_change_handler() {
  // user changing the ratings method will start the search process
  search_button_click_handler();
}

function Strategy_cb_change_handler() {
  // user changing the genre selection will start the search process
  search_button_click_handler();
}

/**
 * Event handler for the "search" button.
 */
function search_button_click_handler() {
  // gather user input into a JSON object called "search_request"
  search_request = compose_search_request();
  if (search_request == null) return;

  // make the API call
  http_request = new XMLHttpRequest();
  http_request.open('POST', api_url, true);  
  http_request.send(JSON.stringify(search_request));

  http_request.onreadystatechange = function () {
    if (http_request.readyState === XMLHttpRequest.DONE) {
      if (http_request.status === 200) {
        var result = JSON.parse(http_request.responseText);

        if (typeof result.error === "undefined") {
          render_search_results(result.query_results, search_request.sort_by); // no error case
        }
        else {
          render_error(result.error); // error case
        }

      } else {
        render_error('There was a problem with the request.');
      }
    }
  };

}


/**
 * Return a search request (object) based on user input.
 */
function compose_search_request() {

  var year = [0, 0];
  var textboxes = [StartYear_textbox, EndYear_textbox];

  var i = 0;
  for (i = 0; i < 2; i++) {
    // check that the year is a number > 1000 and < 9999
    year[i] = parseInt(textboxes[i].value, 10);

    if (isNaN(year[i])) {
      alert("The year \"" + textboxes[i].value + "\" needs to be a number.");
      return null;
    }

    if (year[i] < 1000 || year[i] > 9999) {
      alert("The year needs to between 1000 and 9999.");
      return null;
    }  
  }

  // swap start year and end year if necessary
  if (year[0] > year[1]) {
    var temp = year[0];
    year[0] = year[1];
    year[1] = temp;
  }

  return {
    "genre_id": Strategy_cb.value,
    "start_year": year[0],
    "end_year": year[1],
    "sort_by": Ratings_cb.value
  };
}


/**
 * Renders the "error" string to "output_div" tag.
 * @param {string} error
 */
function render_error(error) {  
  // remove existing children tags
  output_div.innerHTML = '';

  output_div.textContent = error;
}


/**
 * Renders the results (Object[]) to "output_div" tag.
 * @param {Object[]} results
* @param {string} sort_by
 */
function render_search_results(results, sort_by) {
  // remove existing children tags
  output_div.innerHTML = '';

  // handle the case where there are no results
  if (results.length == 0) {
    output_div.textContent = "Nothing found";
    return;
  }

  // create a <table> tag
  var table_tag = output_div.appendChild(create_tag("table"));

  var i = 0;
  for (i = 0; i < results.length; i++) {
    // for each results[i], create a <tr> tag
    var tr_tag = table_tag.appendChild(create_tag("tr"));

    // add to the <tr>
    // score, (title, url), release_date
    tr_tag.appendChild(create_tag_with_text("td", results[i][0], class_name="center"));
    tr_tag.appendChild(create_tag_with_link("td", results[i][1], results[i][2]));
    tr_tag.appendChild(create_tag_with_text("td", results[i][3], class_name = "center"));
  }
}


/**
 * Same as document.createElement(tag_name)
 * @param {string} tag_name
 */
function create_tag(tag_name) {
  return document.createElement(tag_name);
}


/**
 * Returns HTML Element representing <tag_name>text</tag_name>.
 * @param {string} tag_name
 * @param {string} text
 */
function create_tag_with_text(tag_name, text, class_name = null) {
  var tag = document.createElement(tag_name);
  tag.textContent = text;

  if (class_name != null)
    tag.className = class_name;

  return tag;
}


/**
 * Returns HTML Element representing <tag_name><a href=url>text</a></tag_name>.
 * @param {string} tag_name
 * @param {string} text
 * @param {string} url
 */
function create_tag_with_link(tag_name, text, url) {
  var tag = document.createElement(tag_name);
  var a = document.createElement("a");
  a.href = url;
  a.target = "_blank";
  a.textContent = text;
  tag.appendChild(a);

  return tag;
}

