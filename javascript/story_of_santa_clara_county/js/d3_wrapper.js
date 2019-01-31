/// <reference path="d3.js" />
/// <reference path="data.js" />
/// <reference path="data_formatting.js" />

//************* LineGraph Class ******************************

/** Creates a line graph object. */
var LineGraph = function (svg_id) {
  this.svg_id = svg_id;
  this.hover_text_id = svg_id + "_text";

  // scale objects - create in the "plot_x_axis()" and "plot_y_axis()"
  this.x_scale = null; this.y_scale = null;

  // labels - set in the "plot_x_label()" and "plot_y_label()"
  this.x_label = ""; this.y_label = "";
  
  // the area for plotting
  this.top_left = { x: 100, y: 20 };
  this.lower_right = { x: 650, y: 450 };

  // control size of the circle in the plot
  this.circle_size = 6;
};


/** Remove existing stuff */
LineGraph.prototype.clear = function () {
  d3.select(this.svg_id).selectAll('g').remove();
  d3.select(this.svg_id).selectAll('text').remove();
  return this;
};


/** Create the x_scale object and plot x-axis */
LineGraph.prototype.plot_x_axis = function (x, negative_margin, positive_margin) {
  // create this.x_scale
  var min = d3.min(x) - negative_margin,
      max = d3.max(x) + positive_margin;
  this.x_scale = d3.scale.linear().domain([min, max])
                   .range([this.top_left.x, this.lower_right.x]);

  // draw vertical grid lines
  var x_axis = d3.svg.axis().scale(this.x_scale).orient('bottom')
                 .tickSize(-1 * (this.lower_right.y - this.top_left.y)).tickFormat("");
  d3.select(this.svg_id).append('g').call(x_axis).attr({
    'transform': 'translate(0, ' + (this.lower_right.y) + ')',
    'class': 'axis_grid_line'
  });

  // draw x-axis
  x_axis = d3.svg.axis().scale(this.x_scale).orient('bottom')
                     .tickFormat(d3.format("d"));
  d3.select(this.svg_id).append('g').call(x_axis).attr({
    'transform': 'translate(0,' + (this.lower_right.y) + ')',
    'class': 'axis'
  });

  return this;
};


/** Create the y_scale object and plot y-axis.
 * @param {Array.<Array.<number>>} y_array 
 */
LineGraph.prototype.plot_y_axis = function (y_array, negative_margin, positive_margin, use_log_scale) {
  // the y_array is a 2D array
  // y_array[0] is a set of points
  // the scale needs to be based on the maximum and minimum of all data in y_array
  
  // get min, max - initially based on y_array[0]
  var min = d3.min(y_array[0]),
      max = d3.max(y_array[0]);

  // expand upon the above min, max
  for (var i = 1; i < y_array.length; i++) {
    var new_min = d3.min(y_array[i]),
        new_max = d3.max(y_array[i]);
    if (new_min < min) min = new_min;
    if (new_max > max) max = new_max;
  }
  
  // add in margin
  min -= negative_margin;
  max += positive_margin;

  // create this.y_scale
  if (use_log_scale === true)
    this.y_scale = d3.scale.log().domain([min, max])
                   .range([this.lower_right.y, this.top_left.y]);
  else
    this.y_scale = d3.scale.linear().domain([min, max])
                     .range([this.lower_right.y, this.top_left.y]); 

  // draw horizontal grid lines
  var y_axis = d3.svg.axis().scale(this.y_scale).orient('left')
                 .tickSize(-1 * (this.lower_right.x - this.top_left.x)).tickFormat("");
  d3.select(this.svg_id).append('g').call(y_axis).attr({
    'transform': 'translate(' + (this.top_left.x) + ', 0)',
    'class': 'axis_grid_line'
  });

  // draw y-axis
  y_axis = d3.svg.axis().scale(this.y_scale).orient('left');
  d3.select(this.svg_id).append('g').call(y_axis).attr({
    'transform': 'translate(' + (this.top_left.x) + ', 0)',
    'class': 'axis'
  });

  return this;
};


/** Plot x and y arrays. */
LineGraph.prototype.plot_xy = function (x, y, class_name) {
  // self pointer is needed for D3 callbacks
  var self = this;
  
  // create the 'data' object that will be binded to the tags
  var data = [];
  for (var i = 0; i < x.length; i++)
    data.push({ x: x[i], y: y[i] });
  
  // plot the line
  //    #1. create line() generator object
  var generator = d3.svg.line()
                        .x(function (d) { return self.x_scale(d.x); })
                        .y(function (d) { return self.y_scale(d.y); });

  //    #2. use the line() generator object to create <path> tag
  d3.select(this.svg_id).append('g').append('path').datum(data)
                   .attr({
                     'd': generator, 'class': class_name + "_line",
                     'fill': 'none',
                   });

  // function for displaying the hover text
  var display_hover_text = function (x, y) {
    // x location of the text box:
    var x_pix = self.x_scale(x);

    // render the text box
    var text_tag = d3.select(self.svg_id).select(self.hover_text_id);

    text_tag.text("(" + x + ", " + y.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",") + ")")
      // Alternative rendering:
      //  .html('<tspan dx="0" dy="0">' + self.x_label + ": " + x + "</tspan>"
      //  + '<tspan dx="0" dy="1.2em">' + self.y_label + ": " + y + "</tspan>")
      .attr({
        "display": null,
        "x": x_pix,
        "y": self.y_scale(y) - 10
      });

    // the alignment of the text depends on the <text> location
    if (x_pix < self.top_left.x + 100)
      // <text> near left side of graph, so apply a 'start' align
      text_tag.style('text-anchor', 'start');
    else if (x_pix > self.lower_right.x - 100)
      text_tag.style('text-anchor', 'end');
    else
      text_tag.style('text-anchor', 'middle');

  }
      
  if (this.circle_size > 0) {
    // plot points as <circle> tags 
    var circle_tags = d3.select(this.svg_id).append('g').selectAll('circle')
                        .data(data).enter().append('circle');
    circle_tags.attr({
      'cx': function (d) { return self.x_scale(d.x); },
      'cy': function (d) { return self.y_scale(d.y); },
      'r': self.circle_size,
      'class': class_name
    });

    // add mouse over event
    circle_tags.on('mouseenter', function (d, i) {
      display_hover_text(d.x, d.y);
      d3.select(this).attr('class', class_name + "_highlighted");
    })
    circle_tags.on('mouseout', function (d, i) {
      // hide the hover text on 'mouseout'
      d3.select(self.svg_id).select(self.hover_text_id)
        .attr({ "display": "none" });

      d3.select(this).attr('class', class_name);
    });

    // add a <text> for displaying hover text
    d3.select(this.svg_id).append('text').attr({
      'display': 'none',
      'id': this.hover_text_id.substr(1)
    });
  }

  return this;
};


/** Plot x label. */
LineGraph.prototype.plot_x_label = function (text, dx, dy) {
  this.x_label = text;

  d3.select(this.svg_id).append('text')
    .text(text)
    .attr({
      "class": "axis_text",
      "transform": "translate(" + ((this.top_left.x + this.lower_right.x) / 2 + dx)
                  + ", " + (this.lower_right.y + dy) + ")"
    });

  return this;
};


/** Plot y label. */
LineGraph.prototype.plot_y_label = function (text, dx, dy) {
  this.y_label = text;

  d3.select(this.svg_id).append('text')
    .text(text)
    .attr({
      "class": "axis_text",
      "transform": "translate(" + (this.top_left.x + dx) + ", "
                      + ((this.top_left.y + this.lower_right.y) / 2 + dy)
                      + ") rotate(-90)"
    });

  return this;
};


/** Plot label. 
 * @param {boolean} absolute_coord
*/
LineGraph.prototype.plot_label = function (text, x, y, class_name, absolute_coord) {

  if (absolute_coord) {
    // do not apply scale functions to (x, y)
    var text_tag = d3.select(this.svg_id).append('text')
                    .text(text)
                    .attr({
                      "x": x, "y": y,
                      "class": class_name
                    });
  }
  else {
    // apply scale functions to (x, y)
    var text_tag = d3.select(this.svg_id).append('text')
                    .text(text)
                    .attr({
                      "x": this.x_scale(x), "y": this.y_scale(y),
                      "class": class_name
                    });
  }  
  
  return this;
};


/** Draw a line. */
LineGraph.prototype.plot_line = function (x1, y1, x2, y2, class_name) {
  d3.select(this.svg_id).append('g').append('line')
                        .attr({
                          "x1": x1, "y1": y1, "x2": x2, "y2": y2,
                          "class": class_name
                        });
};



//************* AreaGraph Class ****************************** 

/** Creates an area graph object. */
var AreaGraph = function (svg_id) {
  this.svg_id = svg_id;
  this.hover_text_id = svg_id + "_text";

  // the area for plotting
  this.top_left = { x: 100, y: 20 };
  this.lower_right = { x: 650, y: 450 };

  // scale objects - created in "create_scale_objects()"
  this.x_scale = null, this.y_scale = null;  

  // labels - set in the "plot_x_label()" and "plot_y_label()"
  this.x_label = ""; this.y_label = "";

  // control size of the circle in the plot
  this.circle_size = 6;
};


/** Remove existing stuff */
AreaGraph.prototype.clear = function () {
  d3.select(this.svg_id).selectAll('g').remove();
  d3.select(this.svg_id).selectAll('text').remove();
  return this;
};


/** Create scale objects */
AreaGraph.prototype.create_scale_objects = function (data) {
  // find the min and max in data[][].x
  // currently assumes that all rows have the same "x", so just going over one row
  // to find min and max is sufficient.
  var min = data[0][0].x,
      max = data[0][0].x;

  for (var i = 0; i < data[0].length; i++) {
    if (data[0][i].x < min) min = data[0][i].x;
    if (data[0][i].x > max) max = data[0][i].x;
  }

  // create this.x_scale
  this.x_scale = d3.scale.linear().domain([min, max])
                   .range([this.top_left.x, this.lower_right.x]);

  // create this.y_scale
  this.y_scale = d3.scale.linear().domain([0, 1])
                         .range([this.lower_right.y, this.top_left.y]);
};


/** Plot x-axis and y-axis. */
AreaGraph.prototype.plot_axes = function () {
  // draw vertical grid lines
  var x_axis = d3.svg.axis().scale(this.x_scale).orient('bottom')
                 .tickSize(-1 * (this.lower_right.y - this.top_left.y)).tickFormat("");
  d3.select(this.svg_id).append('g').call(x_axis).attr({
    'transform': 'translate(0, ' + (this.lower_right.y) + ')',
    'class': 'axis_grid_line'
  });

  // draw x-axis
  x_axis = d3.svg.axis().scale(this.x_scale).orient('bottom')
                     .tickFormat(d3.format("d"));
  d3.select(this.svg_id).append('g').call(x_axis).attr({
    'transform': 'translate(0,' + (this.lower_right.y) + ')',
    'class': 'axis'
  });

  return this;
};


/** Plot x label. */
AreaGraph.prototype.plot_x_label = function (text, dx, dy) {
  this.x_label = text;

  d3.select(this.svg_id).append('text')
    .text(text)
    .attr({
      "class": "axis_text",
      "transform": "translate(" + ((this.top_left.x + this.lower_right.x) / 2 + dx)
                  + ", " + (this.lower_right.y + dy) + ")"
    });

  return this;
};


/** Plot y label. */
AreaGraph.prototype.plot_y_label = function (text, dx, dy) {
  this.y_label = text;

  d3.select(this.svg_id).append('text')
    .text(text)
    .attr({
      "class": "axis_text",
      "transform": "translate(" + (this.top_left.x + dx) + ", "
                      + ((this.top_left.y + this.lower_right.y) / 2 + dy)
                      + ") rotate(-90)"
    });

  return this;
};


/** Plot data, which is an array of arrays. */
AreaGraph.prototype.plot_data = function (data, class_names) {
  // data[0] is one series. It's an array of data objects.
  // data[0][0] is an object that contains "x", "y", and "y_percent"
  // class_names is an array. class_name[0] will be applied to data[0].

  // self pointer is needed for D3 callbacks
  var self = this;

  // draw the area plots
  var area_plot_g_tag = d3.select(this.svg_id).append('g');

  for (var i = 0; i < data.length; i++) {
    // #1. create area() generator
    var generator = d3.svg.area().y0(function (d) { return self.y_scale(d.y_percent0); })
                                 .x(function (d) { return self.x_scale(d.x); })
                                 .y1(function (d) { return self.y_scale(d.y_percent1); });

    // #2. create <path> tag
    area_plot_g_tag.append('path').datum(data[i])
                   .attr({
                     "d": generator, "class": class_names[i] + "_area"
                   });
  }
  

  


  /*
  // plot the line
  //    #1. create line() generator object
  var generator = d3.svg.line()
                        .x(function (d) { return self.x_scale(d.x); })
                        .y(function (d) { return self.y_scale(d.y); });

  //    #2. use the line() generator object to create <path> tag
  d3.select(this.svg_id).append('g').append('path').datum(data)
                   .attr({
                     'd': generator, 'class': class_name + "_line",
                     'fill': 'none',
                   });

  // function for displaying the hover text
  var display_hover_text = function (x, y) {
    // x location of the text box:
    var x_pix = self.x_scale(x);

    // render the text box
    var text_tag = d3.select(self.svg_id).select(self.hover_text_id);

    text_tag.text("(" + x + ", " + y.toString().replace(/\B(?=(\d{3})+(?!\d))/g, ",") + ")")
       // Alternative rendering:
       //   .html('<tspan dx="0" dy="0">' + self.x_label + ": " + x + "</tspan>"
       //     + '<tspan dx="0" dy="1.2em">' + self.y_label + ": " + y + "</tspan>")
      .attr({
        "display": null,
        "x": x_pix,
        "y": self.y_scale(y) - 10
      });

    // the alignment of the text depends on the <text> location
    if (x_pix < self.top_left.x + 100)
      // <text> near left side of graph, so apply a 'start' align
      text_tag.style('text-anchor', 'start');
    else if (x_pix > self.lower_right.x - 100)
      text_tag.style('text-anchor', 'end');
    else
      text_tag.style('text-anchor', 'middle');

  }

  if (this.circle_size > 0) {
    // plot points as <circle> tags 
    var circle_tags = d3.select(this.svg_id).append('g').selectAll('circle')
                        .data(data).enter().append('circle');
    circle_tags.attr({
      'cx': function (d) { return self.x_scale(d.x); },
      'cy': function (d) { return self.y_scale(d.y); },
      'r': self.circle_size,
      'class': class_name
    });

    // add mouse over event
    circle_tags.on('mouseenter', function (d, i) {
      display_hover_text(d.x, d.y);
      d3.select(this).attr('class', class_name + "_highlighted");
    })
    circle_tags.on('mouseout', function (d, i) {
      // hide the hover text on 'mouseout'
      d3.select(self.svg_id).select(self.hover_text_id)
        .attr({ "display": "none" });

      d3.select(this).attr('class', class_name);
    });

    // add a <text> for displaying hover text
    d3.select(this.svg_id).append('text').attr({
      'display': 'none',
      'id': this.hover_text_id.substr(1)
    });
  }
  */
  return this;
};


/** Plot label. */
AreaGraph.prototype.plot_label = function (text, x, y, class_name) {
  var text_tag = d3.select(this.svg_id).append('text')
                    .text(text)
                    .attr({
                      "x": this.x_scale(x), "y": this.y_scale(y),
                      "class": class_name
                    });

  return this;
};


/** Plot x and y arrays. */
AreaGraph.prototype.plot_xy = function (x, y, class_name) {
  // self pointer is needed for D3 callbacks
  var self = this;

  // create the 'data' object that will be binded to the tags
  var data = [];
  for (var i = 0; i < x.length; i++)
    data.push({ x: x[i], y: y[i] });

  // plot the line
  //    #1. create line() generator object
  var generator = d3.svg.line()
                        .x(function (d) { return self.x_scale(d.x); })
                        .y(function (d) { return self.y_scale(d.y); });

  //    #2. use the line() generator object to create <path> tag
  d3.select(this.svg_id).append('g').append('path').datum(data)
                   .attr({
                     'd': generator, 'class': class_name + "_line",
                     'fill': 'none',
                   });

  // function for displaying the hover text
  var display_hover_text = function (x, y) {
    // x location of the text box:
    var x_pix = self.x_scale(x);

    // convert the y to a percentage string
    var y_str = (y * 100).toString();
    if (y_str.length > 5) y_str = y_str.substring(0, 5);
    y_str += '%';

    // render the text box
    var text_tag = d3.select(self.svg_id).select(self.hover_text_id);

    text_tag.text("(" + x + ", " + y_str + ")")
      .attr({
        "display": null,
        "x": x_pix,
        "y": self.y_scale(y) - 10
      });

    // the alignment of the text depends on the <text> location
    if (x_pix < self.top_left.x + 100)
      // <text> near left side of graph, so apply a 'start' align
      text_tag.style('text-anchor', 'start');
    else if (x_pix > self.lower_right.x - 100)
      text_tag.style('text-anchor', 'end');
    else
      text_tag.style('text-anchor', 'middle');

  }

  if (this.circle_size > 0) {
    // plot points as <circle> tags 
    var circle_tags = d3.select(this.svg_id).append('g').selectAll('circle')
                        .data(data).enter().append('circle');
    circle_tags.attr({
      'cx': function (d) { return self.x_scale(d.x); },
      'cy': function (d) { return self.y_scale(d.y); },
      'r': self.circle_size,
      'class': class_name
    });

    // add mouse over event
    circle_tags.on('mouseenter', function (d, i) {
      display_hover_text(d.x, d.y);
      d3.select(this).attr('class', class_name + "_highlighted");
    })
    circle_tags.on('mouseout', function (d, i) {
      // hide the hover text on 'mouseout'
      d3.select(self.svg_id).select(self.hover_text_id)
        .attr({ "display": "none" });

      d3.select(this).attr('class', class_name);
    });

    // add a <text> for displaying hover text
    d3.select(this.svg_id).append('text').attr({
      'display': 'none',
      'id': this.hover_text_id.substr(1)
    });
  }

  return this;
};


//*********************** Event Graph Class *****************************

var EventGraph = function (svg_id) {
  this.svg_id = svg_id;
  this.hover_text_id = svg_id + "_text";

  // scale objects - these are assigned in "set_scales(...)"
  this.x_scale = null; this.y_scale = null;

  // HTML rendering options
  // colors
  // this.rect_color = '#91b6d4'; // light blue
  this.rect_color = 'rgba(70, 130, 180, 0.5)'; // steel blue
  this.rect_highlight_color = 'orange';

  this.text_class = "event_text";
};


EventGraph.prototype.set_scales = function (x_scale, y_scale) {
  this.x_scale = x_scale;
  this.y_scale = y_scale;
};


/** Plot an array of events.
 * @param {Array.<Object>} event_array
 */
EventGraph.prototype.plot_events = function (event_array) {
  
  var g_tag = d3.select(this.svg_id).append('g');

  for (var i = 0; i < event_array.length; i++)
    this.plot_event(g_tag, event_array[i]);
};


/** Plot one event. "event_obj" is an object from the global 
"data.js::events" collection. */
EventGraph.prototype.plot_event = function (g_tag, event_obj) {
  var self = this;

  // extract event coordinates:
  var x1 = this.x_scale(event_obj.start),
      y1 = this.y_scale(event_obj.y),
      x2 = this.x_scale(event_obj.end);

  // draw the event
  // create <rect> and <text> tags
  var rect_tag = g_tag.append('rect');
  var text_tag = g_tag.append('text');

  // set <text> attributes
  text_tag.attr({
    'x': x1 + (x2 - x1) / 2,
    'y': y1, 'class': this.text_class
  });
  text_tag.text(event_obj.name);

  // set <rect> attributes
  var bbox = text_tag[0][0].getBBox();
  rect_tag.attr({
    'x': x1, 'width': (x2 - x1),
    'fill': this.rect_color,
    'y': y1 - bbox.height - 5,
    'height': bbox.height + 17
  });

  // the <div> tag that will show the event information
  var div_tag = d3.select('#event_div')

  // function to render event_obj.html in a <div> tag
  function show_content() {
    rect_tag.attr('fill', self.rect_highlight_color);
    div_tag.html(event_obj.html);

    // need to change <div> to hidden so to get width, height
    div_tag.style('visibility', 'hidden');
    var div_width = div_tag[0][0].getBoundingClientRect().width;
    var div_height = div_tag[0][0].getBoundingClientRect().height;

    // calculate the <div> tag's "top" and "left"
    // get the location of the <svg> element
    var svg_tag = d3.select(self.svg_id)[0][0];
    var scroll_top = document.documentElement.scrollTop || document.body.scrollTop;
    var scroll_left = document.documentElement.scrollLeft || document.body.scrollLeft;
    var svg_top = svg_tag.getBoundingClientRect().top + scroll_top;
    var svg_left = svg_tag.getBoundingClientRect().left + scroll_left;

    // calculate "top" first
    var top = svg_top + y1 + event_obj.dy;
    if (event_obj.dy < 0) top -= div_height;

    var left = svg_left + x1 + event_obj.dx;
    // for now don't apply any more shift to "left" - so rely on above
    // and below modifiers, the "dy" modifier, primarily
    //if (event_obj.dx > 0) left += bbox.width;
    //else if (event_obj.dx < 0) left -= div_width;

    div_tag.style({
      "top": top + "px", "left": left + "px",
      "visibility": "visible"
    });
  }

  // function to hide event_obj.html
  function hide_content() {
    rect_tag.attr('fill', self.rect_color);
    div_tag.style('visibility', 'collapse');
  }

  // mouse events that will trigger show_content() and hide_content()
  rect_tag.on('mouseenter', show_content);
  text_tag.on('mouseenter', show_content);
  rect_tag.on('mouseleave', hide_content);
};

