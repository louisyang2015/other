/// <reference path="data.js" />

// ********** Helper functions ************************************

/** Combine an 'x' array and a 'y' array into a 'new_row' for 'full_array'. */
function add_row(full_array, one_row_x, one_row_y) {
  var new_row = [];

  for (var i = 0; i < one_row_x.length; i++)
    new_row.push({ "x": one_row_x[i], "y": one_row_y[i] });

  full_array.push(new_row);
}

/** Go through 'full_array' and add a 'y_percentage' value to each object. */
function add_percentages(full_array) {
  for (var j = 0; j < full_array[0].length; j++) {

    // first find the total for the current column 'j'
    var total = 0;

    for (var i = 0; i < full_array.length; i++)
      // we are currently on row 'i', column 'j'
      total += full_array[i][j].y;

    // add 'y_percentage' for all objects in column 'j'
    var percent_so_far = 0;
    for (var i = 0; i < full_array.length; i++) {
      full_array[i][j]["y_percent0"] = percent_so_far;
      percent_so_far += full_array[i][j].y / total;
      full_array[i][j]["y_percent1"] = percent_so_far;
    }
  }
}

// ************ Data Formatting of Population Growth ********************
var sc_population_growth = {}
sc_population_growth.x = sc_population.x;
sc_population_growth.y = [];

var usa_population_growth = {}
usa_population_growth.x = sc_population.x;
usa_population_growth.y = [];

for (var i = 0; i < sc_population.x.length; i++) {
  sc_population_growth.y.push(Math.floor(
    sc_population.y[i] / sc_population.y[0] * 100.0));
  usa_population_growth.y.push(Math.floor(
    usa_population.y[i] / usa_population.y[0] * 100.0));
}


// ************ Data Formatting of nine_county_population ***************

// collect sc_population, alameda_population, etc into a data structure
// suitable for D3.js area() generator

var nine_county_population = [];

add_row(nine_county_population, sc_population.x, sonoma_population);
add_row(nine_county_population, sc_population.x, solano_population);
add_row(nine_county_population, sc_population.x, sc_population.y);
add_row(nine_county_population, sc_population.x, san_mateo_population);
add_row(nine_county_population, sc_population.x, sf_population);
add_row(nine_county_population, sc_population.x, napa_population);
add_row(nine_county_population, sc_population.x, marin_population);
add_row(nine_county_population, sc_population.x, contra_costa_population);
add_row(nine_county_population, sc_population.x, alameda_population);

add_percentages(nine_county_population);

// nine_county_population[0] is the population of Santa Clara county
// each item has ".x" for the year, ".y" for the population,
// ".y_percent1" for the overall percentage of the Bay Area population,
// ".y_percent0" for use in graphing lower bound of area graph.

// nine_county_population[8] is the population of San Francisco county


// ************ Data Formatting of ethnicity_data ***************
var ethnicity_data = [];

add_row(ethnicity_data, ethnic_data_years, sc_asian_population);
add_row(ethnicity_data, ethnic_data_years, sc_other_population);
add_row(ethnicity_data, ethnic_data_years, sc_black_population);
add_row(ethnicity_data, ethnic_data_years, sc_white_population);

add_percentages(ethnicity_data);

// at this point, the structure of "ethnicity_data" is similar to 
// "nine_county_population"

// extract percentage Asian from "ethnicity_data"
var sc_asian_percentage = [];

for (var i = 0; i < ethnicity_data[0].length; i++)
  // Since Asian is the first array, then the "y_percent0"
  // should be zero. For other ethnicity, you need to use
  // y_percent1 - y_percent0.
  sc_asian_percentage.push(ethnicity_data[0][i].y_percent1)

