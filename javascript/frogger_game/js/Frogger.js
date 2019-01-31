/// <reference path="GameObj.js" />
/// <reference path="Game.js" />

"use strict";

// #region Frogger

var Frogger = {};

Frogger._init_level = 1; // starting level
Frogger._last_level = 4;
Frogger._enable_grayscale = true; // turn on grayscale effects

Frogger.init = function (canvas) {
  // initialization fuction. "canvas" should be an HTML 5 "canvas" element
  Frogger._my_canvas = new MyCanvas(canvas);
  Frogger._load_images();
  Frogger._keydown_event = null;
  // more objects are created in "_load_images()" and "_load_images_done()"
};

Frogger._load_images = function () {
  // load "char-boy.png", "char-cat-girl.png", ... into __images
  Frogger._images = {};

  Frogger._app_state = {}; // _app_state creation
  Frogger._app_state.state = "loading images";
  Frogger._app_state.param = {
    num_images: -1,
    total_images: 0
  };

  // Notice that "num_images" start out 1 value behind.
  // This is compensated at the very end of this function.

  // The goal is to ensure that the load can never prematurely complete
  // until all of this function is executed. You don't want to just
  // load 1 image, and then say, oh I have 1 out of 1 image, I am done.

  // show a "loading" message (user might not see it in real use)
  Frogger._my_canvas.set_background("white", "black");
  Frogger._my_canvas.draw_text_centered("36px Impact", "yellow",
      "black", "Loading Images...", this._my_canvas.canvas.height / 2);

  Frogger._load_image("car.png", "car");
  Frogger._load_image("char-boy.png", "boy");
  Frogger._load_image("char-cat-girl.png", "girl");
  Frogger._load_image("dino.png", "dino");
  Frogger._load_image("dragon.png", "dragon");
  Frogger._load_image("enemy-bug.png", "bug");
  Frogger._load_image("explosion1.png", "explosion1");
  Frogger._load_image("explosion2.png", "explosion2");
  Frogger._load_image("explosion3.png", "explosion3");
  Frogger._load_image("fireball.png", "fireball");
  Frogger._load_image("Gem_Blue.png", "gem_blue");
  Frogger._load_image("Gem_Green.png", "gem_green");
  Frogger._load_image("Gem_Orange.png", "gem_orange");
  Frogger._load_image("grass-block.png", "grass");
  Frogger._load_image("house.png", "house");
  Frogger._load_image("Selector.png", "selector");
  Frogger._load_image("Star.png", "star");
  Frogger._load_image("stone-block.png", "stone");
  Frogger._load_image("ufo.png", "ufo");
  Frogger._load_image("water-block.png", "water");

  var app_state = Frogger._app_state;
  app_state.param.num_images++;
  if (app_state.param.num_images >= app_state.param.total_images)
    // console.log("Image loading done");
    // this is one of two places where this can happen
    Frogger._load_images_done();
};

Frogger._load_image = function (file_name, key_name) {
  // Create an image object out of the given "file_name"
  // and stuff that image inside this._images["key_name"]
  var app_state = Frogger._app_state;
  app_state.param.total_images++;

  Frogger._images[key_name] = new Image();
  Frogger._images[key_name].src = "./images/" + file_name;
  Frogger._images[key_name].onload = function () {
    app_state.param.num_images++;
    if (app_state.param.num_images >= app_state.param.total_images)
      // console.log("Image loading done");
      // this is one of two places where this can happen
      Frogger._load_images_done();
  };
};

Frogger._load_images_done = function () {
  // finish loading images, now more objects can be constructed
  Frogger._char_selection = new CharSelection(Frogger._my_canvas, Frogger._images);
  Frogger._game = new Game(Frogger._my_canvas, Frogger._images);

  // finished loading images, the next state is "character selection"
  Frogger._app_state.state = "character selection";
  Frogger._app_state.param = null;

  // assign images for the characters selection process
  Frogger._char_selection.render();
};

Frogger.keydown_handler = function (e) {
  // "character selection" state
  if (Frogger._app_state.state === "character selection") {
    Frogger._char_selection.keydown_handler(e);

    if (Frogger._char_selection.done) {
      Frogger._app_state.state = "first frame";
      Frogger._app_state.param = null;
      Frogger._game.init(Frogger._char_selection.get_player(), Frogger._init_level);
      Frogger.render(null);
    }
  }

    // "game running" state
  else if (Frogger._app_state.state === "game running") {
    Frogger._keydown_event = e; // delay keyboard input to render() frame
  }

    // "level completed" state
  else if (Frogger._app_state.state === "level completed") {
    if (e.keyCode === 13) { // enter key

      var level = Frogger._game._level + 1;

      if (level <= Frogger._last_level) {
        // continue onto the new level
        Frogger._app_state.state = "first frame";
        Frogger._app_state.param = null;
        Frogger._game.init(Frogger._char_selection.get_player(), level);
        Frogger.render(null);
      }
      else {
        // player has finished the game, go back to the character selection screen
        Frogger._app_state.state = "character selection";
        Frogger._app_state.param = null;
        Frogger._char_selection.reset();
        Frogger._char_selection.render();
      }
      
    }
  }

    // "game lost" state
  else if (Frogger._app_state.state === "game lost") {
    if (e.keyCode === 13) { // enter key
      Frogger._app_state.state = "character selection";
      Frogger._app_state.param = null;
      Frogger._char_selection.reset();
      Frogger._char_selection.render();
    }
  }
};

Frogger.render = function (time) {
  // "first frame" state
  if (Frogger._app_state.state === "first frame") {
    requestAnimationFrame(Frogger.render);

    if (time !== null) {
      Frogger._game.first_frame(time);
      Frogger._app_state.state = "game running";
      Frogger._old_time_stamp = time;
    }
  }

    // "game running" state
  else if (Frogger._app_state.state === "game running") {

    // handle game over / end of level situations
    if(Frogger._game._win) {
      Frogger._app_state.state = "level completed";
    }
    else if (Frogger._game._lost) {
      Frogger._app_state.state = "game lost";
    }

      // handle "game running" situation
    else {
      
      requestAnimationFrame(Frogger.render);

      if (time !== null) {

        if (time - Frogger._old_time_stamp > 0.01) {
          // process keyboard input here
          if (Frogger._keydown_event !== null) {

            // before handling the keyboard event, need to filter out repeated 
            // pressing of the same key
            var repeat_key_press = false;
            if (Frogger._prev_keydown_code === Frogger._keydown_event.keyCode) {
              if (time - Frogger._prev_keydown_time < 100)
                repeat_key_press = true;
            }

            if (!repeat_key_press) {
              Frogger._game.keydown_handler(Frogger._keydown_event, time);
              Frogger._prev_keydown_code = Frogger._keydown_event.keyCode;
              Frogger._prev_keydown_time = time;
            }
          }

          Frogger._game.render(time);

          Frogger._old_time_stamp = time;
        }
      }

      Frogger._keydown_event = null;
    }
  }
};

/**
 * Returns random numbers, min to max, max included
 */
Frogger._random_int = function (min, max) {
  return Math.floor(Math.random() * (max - min + 1)) + min;
};

// #endregion


// #region class Canvas 

var MyCanvas = function (canvas) {
  // "canvas" is a canvas element from HTML
  this.canvas = canvas;
  this.context = canvas.getContext("2d");
};

/** convenience method for writing text onto canvas */
MyCanvas.prototype.draw_text = function (font,
         fill, stroke, text, x, y) {
  // convenience method for writing centered text onto canvas
  this.context.save();
  this.context.font = font;
  this.context.textAlign = "center";
  this.context.fillStyle = fill;
  this.context.strokeStyle = stroke;
  this.context.lineWidth = 1;
  this.context.fillText(text, x, y);
  this.context.strokeText(text, x, y);
  this.context.restore();
};

/** convenience method for writing centered text onto canvas */
MyCanvas.prototype.draw_text_centered = function (font,
        fill, stroke, text, y) {
  this.draw_text(font, fill, stroke, text, this.canvas.width / 2, y);
};

/** Turns a section of the image into grayscale
  * @param {Array} rect_list - an array of rectangles, each rectangle item is 
  *     encoded as (x, y, width, height)
  * @param {boolean} change_alpha - if false, alpha is left at 255
  */
MyCanvas.prototype.grayscale_filter = function (rect_list, change_alpha) {
  
  if (Frogger._enable_grayscale === false) return;

  var data = this.context.getImageData(0, 0, this.canvas.width, this.canvas.height);

  for (var h = 0; h < rect_list.length; h++) {
    // (x, y, width, height) = rect_list[h]
    var x = rect_list[h][0]; var y = rect_list[h][1];
    var width = rect_list[h][2]; var height = rect_list[h][3];

    for (var row = y; row < y + height; row++) {
      var pixel_index = row * this.canvas.width + x;
      for (var i = 0; i < width; i++) {
        // change data[pixel_index] to gray scale
        var loc = pixel_index * 4;
        var gray = 0.03 * data.data[loc] + 0.59 * data.data[loc + 1]
                     + 0.11 * data.data[loc + 2];
        data.data[loc] = gray;
        data.data[loc + 1] = gray;
        data.data[loc + 2] = gray;
        if (change_alpha === true)
          data.data[loc + 3] = 255 - gray; // lower alpha = more transparent = more white
        else
          data.data[loc + 3] = 255;

        pixel_index++;
      }
    }
  }

  this.context.putImageData(data, 0, 0);
};

MyCanvas.prototype.set_background = function (fill, stroke) {
  this.context.save();
  this.context.fillStyle = fill;
  this.context.strokeStyle = stroke;
  this.context.fillRect(0, 0, this.canvas.width, this.canvas.height);
  this.context.strokeRect(0, 0, this.canvas.width, this.canvas.height);
  this.context.restore();
};

// #endregion Canvas


// #region class CharSelection

var CharSelection = function (my_canvas, images) {
  this._my_canvas = my_canvas;
  this._images = images;
  this._current_selection = 0;
};

CharSelection.prototype.reset = function () {
  this.done = false;
};

CharSelection.prototype.render = function () {
  this._my_canvas.set_background("black", "black");

  // character images and descriptions
  var char_images = [this._images.boy, this._images.girl,
    this._images.dino, this._images.ufo];

  var char_descriptions = ["Human Boy", "Human Girl",
    "Dinosaur - slow, very strong, gains HP from eating bugs",
    "UFO - flier, fast, ignore ground enemies"];

  // draw playable characters; 100x100 per character
  // some specs about where to draw the characters
  var space = (this._my_canvas.canvas.width * 0.7 - 400) / 3; // space between characters
  var height = Math.floor(this._my_canvas.canvas.height / 2) - 50; // the height at which characters are lined up

  // draw star above the currently selected character
  this._my_canvas.context.drawImage(
      this._images.star,
      this._my_canvas.canvas.width * 0.15 + this._current_selection * (100 + space),
      height - 150, 100, 100);

  // draw selector below the currently selected character
  this._my_canvas.context.drawImage(
      this._images.selector,
      this._my_canvas.canvas.width * 0.15 + this._current_selection * (100 + space) - 12,
      height - 70, 101 * 1.2, 171 * 1.2);

  // draw the characters; build up a rectangle list
  var rect_list = [];
  for (var i = 0; i < char_images.length; i++) {
    // the standard, calculated "x" location
    var x = Math.floor(this._my_canvas.canvas.width * 0.15 + i * (100 + space));

    // adjustment - move the dinosaur a little to the right
    if (i === 2) x = x + 10;

    this._my_canvas.context.drawImage(char_images[i], x, height, 100, 100);

    // if this character is not currently selected, add it to rect_list, to be gray out
    if (this._current_selection !== i)
      rect_list.push([x - 5, height - 5, 110, 110]);
  }

  // apply gray scale filter to the unselected characters
  this._my_canvas.grayscale_filter(rect_list, false);

  // describe the currently selected character
  this._my_canvas.draw_text_centered("30px Times New Roman", "yellow", "yellow",
    char_descriptions[this._current_selection], height + 180);
};

CharSelection.prototype.keydown_handler = function (e) {
  // keyboard input from the user, update the "_current_selection" variable

  // left or right arrow case
  if (e.keyCode === 37 || e.keyCode === 39) {
    if (e.keyCode === 37)
      // console.log("left arrow");
      this._current_selection--;
    else if (e.keyCode === 39)
      // console.log("right arrow");
      this._current_selection++;
    if (this._current_selection < 0) this._current_selection = 0;
    else if (this._current_selection > 3) this._current_selection = 3;

    this.render();
  }

    // ENTER key case
  else if (e.keyCode === 13) this.done = true;
};

/** Returns a string representing the player that got selected. */
CharSelection.prototype.get_player = function () {
  if (this._current_selection === 0) return "boy";
  else if (this._current_selection === 1) return "girl";
  else if (this._current_selection === 2) return "dino";
  else if (this._current_selection === 3) return "ufo";
};

// #endregion
