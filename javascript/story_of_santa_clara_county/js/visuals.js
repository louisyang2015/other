/// <reference path="d3.js" />
/// <reference path="data.js" />
/// <reference path="d3_wrapper.js" />

var svg_id = '#chart01';
var line_graph = new LineGraph(svg_id);
var area_graph = new AreaGraph(svg_id);
var event_graph = new EventGraph(svg_id);


/** Renders the Santa Clara Population Growth visual. */
function plot_sc_population(plot_events) {
  line_graph.circle_size = 6;
  line_graph.top_left = { x: 120, y: 20 };
  line_graph.lower_right = { x: 670, y: 440 };

  line_graph.clear()
            .plot_x_label("Year", 0, 50)
            .plot_y_label("Population", -90, 0)
            .plot_x_axis(sc_population.x, 10, 10)
            .plot_y_axis([sc_population.y], 0, 100000)
            .plot_xy(sc_population.x, sc_population.y, "sc_pop");

  // plot historical events
  if (plot_events) {
    event_graph.set_scales(line_graph.x_scale, line_graph.y_scale);
    event_graph.plot_events(events.santa_clara);
  }
}

/** Renders the Santa Clara vs USA Population Growth visual. */
function plot_sc_vs_usa_growth(plot_events) {
  line_graph.circle_size = 6;
  line_graph.top_left = { x: 110, y: 20 };
  line_graph.lower_right = { x: 670, y: 440 };

  line_graph.clear()
            .plot_x_label("Year", 0, 50)
            .plot_y_label("Growth since 1860 (%)", -70, 0)
            .plot_x_axis(sc_population_growth.x, 10, 10)
            .plot_y_axis([sc_population_growth.y], 0, 0)
            .plot_label("Santa Clara", 1940, 9000, "sc_pop_text")
            .plot_label("County", 1940, 8000, "sc_pop_text")
            .plot_label("USA", 1980, 1500, "usa_pop_text")
            .plot_xy(sc_population_growth.x, sc_population_growth.y, "sc_pop")
            .plot_xy(usa_population_growth.x, usa_population_growth.y, "usa_pop");

  // plot historical events
  if (plot_events) {
    event_graph.set_scales(line_graph.x_scale, line_graph.y_scale);
    event_graph.plot_events(events.santa_clara_vs_usa);
  }
}


/** Renders the company stock value visual. */
function plot_stock(stock_symbol, use_log_scale, add_sp500) {
  var data = stock_data[stock_symbol];
  line_graph.circle_size = 0;
  line_graph.top_left = { x: 85, y: 20 };
  line_graph.lower_right = { x: 670, y: 440 };

  line_graph.clear()
            .plot_x_label("Year", 0, 50)
            .plot_y_label("Stock Price ($)", -50, 0)
            .plot_x_axis(data.x, 1, 1);
            

  if (add_sp500) {
    // need to graph the S&P 500 data

    // extract the S&P 500 stock price to plot from "sp500_stock"
    var sp500_extract = [];

    // the extraction starts at index "start", and it is scaled by "ratio"
    var start = sp500_stock.length - data.y.length;
    var ratio =  data.y[0] / sp500_stock[start];
    for (var i = 0; i < data.y.length; i++)
      sp500_extract.push(sp500_stock[start + i] * ratio);
    
    // plot the y-axis and data
    line_graph.plot_y_axis([data.y, sp500_extract], 0, 0, use_log_scale)
              .plot_xy(data.x, data.y, "stock_price")
              .plot_xy(data.x, sp500_extract, "sp500_price");

    // add a legend
    line_graph.plot_line(100, 50, 140, 50, "sp500_legend_line");
    line_graph.plot_label("S&P 500", 150, 58, "sp500_legend_text", true);
  }
  else {
    // standard case
    line_graph.plot_y_axis([data.y], 0, 0, use_log_scale)
              .plot_xy(data.x, data.y, "stock_price");
  }
}


/** Renders the county distribution of bay area population. */
function plot_county_distribution() {
  area_graph.top_left = { x: 50, y: 20 };
  area_graph.lower_right = { x: 670, y: 440 };
  area_graph.create_scale_objects(nine_county_population);

  var class_names = ["sonoma", "solano", "santa_clara", "san_mateo",
                    "san_francisco", "napa", "marin", "contra_costa",
                    "alameda"];

  area_graph.clear()
            .plot_x_label("Year", 0, 50)
            .plot_y_label("Population Distribution", -15, 0)
            .plot_data(nine_county_population, class_names)
            .plot_axes()
            .plot_label("San Francisco", 1880, 0.4, "AreaGraph_label_light")
            .plot_label("Alameda", 1920, 0.85, "AreaGraph_label_light")
            .plot_label("Santa Clara", 1970, 0.20, "AreaGraph_label_light");
}


/** Renders the ethnicity distribution of bay area population. */
function plot_ethnic_distribution() {
  area_graph.top_left = { x: 50, y: 20 };
  area_graph.lower_right = { x: 670, y: 440 };
  area_graph.create_scale_objects(ethnicity_data);

  var class_names = ["asian", "other", "black", "white"];

  area_graph.clear()
            .plot_x_label("Year", 0, 50)
            .plot_y_label("Population Distribution", -15, 0)
            .plot_data(ethnicity_data, class_names)
            .plot_axes();
}


/** Renders the Asian percentage data for Santa Clara county and USA. */
function plot_sc_vs_usa_asian_distribution(show_label) {
  if (show_label) {
    area_graph.plot_label("Santa Clara", 1980, 0.24, "sc_asian_text")
              .plot_label("County Asian", 1980, 0.17, "sc_asian_text")
              .plot_label("USA Asian", 2000, 0.07, "usa_asian_text");
  }
  
  area_graph.plot_xy(ethnic_data_years, sc_asian_percentage, "sc_asian")
            .plot_xy(ethnic_data_years, usa_asian_percentage, "usa_asian");
}


/** Add "Asian" and "White + Hispanic" to the ethnicity graph. */
function add_text_labels_to_ethnicity_graph() {
  area_graph.plot_label("Asian", 2000, 0.10, "AreaGraph_label_asian")
            .plot_label("White + Hispanic", 1970, 0.50, "AreaGraph_label_white");
}


/** Add historical events to the Asian Immigration graph. */
function plot_asian_immigration_events() {
  event_graph.set_scales(area_graph.x_scale, area_graph.y_scale);
  event_graph.plot_events(events.asian_immigration);
}


/** Add historical events to the stock price graph. */
function plot_stock_events(symbol) {
  event_graph.set_scales(line_graph.x_scale, line_graph.y_scale);
  event_graph.plot_events(events.stocks[symbol]);
}

