/// <reference path="Game.js" />
/// <reference path="Frogger.js" />

"use strict";

// #region class GameObj

var GameObj = function (my_canvas, image, map) {
  this._my_canvas = my_canvas;
  this._image = image;
  this._map = map;

  // coordinate locations
  this._x = 0; // so if board is 11 squares wide, _x is 0 to 10
  this._y = 0;
  this._x_draw = 0; // draw the image at this pixel location
  this._y_draw = 0;

  // movement control
  this._is_moving = false;
  this._speed = 0.001; // grid move per milli-second
  this._dx = 0; this._dy = 0; // direction of movement
  this._time = 0; // previous micro-movement timestamp
  this._dest_x = 0; this._dest_y = 0; // destination location

  this._enable = false; // enable flag

  this._y_draw_offset = null; // option to modify _y_draw computation
  this._x_draw_offset = null;

  // collision coordinates
  this._x1_coll = 0; this._y1_coll = 0;
  this._x2_coll = 0; this._y2_coll = 0;
  this._x1_coll_offset = 0;
  this._y1_coll_offset = 0;
  this._coll_box_width = 0;
  this._coll_box_height = 0;
  this._collided = false;
};

GameObj.prototype.render = function () {
  if (!this._enable) return;
  this._my_canvas.context.drawImage(this._image, this._x_draw, this._y_draw);
};

GameObj.prototype.start_move = function (time, dx, dy) {
  // start a move; "time" is the current time
  // the "dx" is not the same as "_dx"
  // the dx, dy are total moves; the _dx, _dy are move ratios

  // ignore this request if dx and dy are zero
  if (dx === 0 && dy === 0) return;
  this._is_moving = true;
  this._enable = true;

  this._time = time;
  this._dest_x = this._x + dx; // store the destination
  this._dest_y = this._y + dy;

  // figure out the movement ratios _dx and _dy
  // first the easy cases - also to avoid division by zero
  if (dx === 0) {
    this._dx = 0;
    if (dy > 0) this._dy = 1;
    else this._dy = -1;
  }
  else if (dy === 0) {
    this._dy = 0;
    if (dx > 0) this._dx = 1;
    else this._dx = -1;
  }
  else {
    // code only gets here if dx != 0 AND dy != 0
    var angle = Math.atan(dy / dx);
    this._dx = 1 * Math.cos(angle);
    this._dy = 1 * Math.sin(angle);
  }
};

GameObj.prototype.set_location = function (x, y) {
  // update _x, _y, _x_draw, _y_draw
  this._x = x;
  this._y = y;

  // pixel locations, corresponding to the board coordinate
  var x_pix = this._map.to_x_pix(x);
  var y_pix = this._map.to_y_pix(y);

  // compute the location used for drawing
  if (this._x_draw_offset === null)
    this._x_draw = x_pix - this._image.width / 2;
  else
    this._x_draw = x_pix - this._x_draw_offset;

  if (this._y_draw_offset === null)
    this._y_draw = y_pix - this._image.height / 2;
  else
    this._y_draw = y_pix - this._y_draw_offset;

  // compute the collision related coordinates
  this._x1_coll = x_pix - this._x1_coll_offset;
  this._y1_coll = y_pix - this._y1_coll_offset;
  this._x2_coll = this._x1_coll + this._coll_box_width;
  this._y2_coll = this._y1_coll + this._coll_box_height;
};

GameObj.prototype.update = function (time) {
  if (!this._enable) return;
  if (!this._is_moving) return;

  var time_diff = time - this._time;
  if (time_diff === 0) return;

  this._time = time;

  // figure out distance to move, compute new location
  var dist = time_diff * this._speed;
  var x = this._x + dist * this._dx;
  var y = this._y + dist * this._dy;

  // limit location so there's no overshoot
  if (this._dx > 0 && x > this._dest_x) x = this._dest_x;
  if (this._dx < 0 && x < this._dest_x) x = this._dest_x;
  if (this._dy > 0 && y > this._dest_y) y = this._dest_y;
  if (this._dy < 0 && y < this._dest_y) y = this._dest_y;

  this.set_location(x, y);

  // detect the end of movement here
  if (x === this._dest_x && y === this._dest_y)
    this._is_moving = false;
};

GameObj.prototype.set_enable = function (enable) {
  this._enable = enable;
};

GameObj.prototype.set_collided = function (collided) {
  this._collided = collided;
};

/** 
 * Put this unit between two x coordinates x1 and x2. Assume x2 > x1.
 */
GameObj.prototype.spawn_between = function (x1, x2, y) {
  // translate this unit's width into coordinate equivalent
  var x_width = this._image.width / this._map._square_width;

  // determine a suitable "x" between x1 and x2, taking into account x_width
  var x = (x1 + x2) / 2; // default "x" - worst case
  // (x2 - x1 < x_width) means there is not enough space to put this unit
  if (x2 - x1 > x_width) {
    var avail_space = x2 - x1 - x_width;
    x = x1 + Math.random() * avail_space + x_width / 2;
  }
  this.set_location(x, y);
  this.set_enable(true);
  this.set_collided(false);
};

// #endregion


// #region class Player inherits from GameObj

var Player = function (my_canvas, images, map) {
  GameObj.call(this, my_canvas, images.boy, map);
  this._images = images;

  // starting location
  this._x_start = Math.floor(map._num_col / 2);
  // _y_start is _y_max

  // movement limit
  this._x_max = map._num_col - 1;
  this._y_max = map._num_stone_rows + 1 + 0.6;

  this._enable = true;
};

Player.prototype = Object.create(GameObj.prototype);
Player.prototype.constructor = Player;

/** Reset the object settings based on the "player". */
Player.prototype.config = function (player) {
  
  if (player === "boy") {
    this._image = this._images.boy;
    this._y_draw_offset = 83;
    this._x_draw_offset = null;
    this._speed = 0.0015;
    this._x1_coll_offset = 26;
    this._y1_coll_offset = 66;
    this._coll_box_width = 52;
    this._coll_box_height = 56;
    this._eats_bugs = false;
    this._flyer = false;
    this._breaks_cars = false;
    this._hp = 100;
    this._hp_max = 100;
  }
  else if (player === "girl") {
    this._image = this._images.girl;
    this._y_draw_offset = 83;
    this._x_draw_offset = null;
    this._speed = 0.0015;
    this._x1_coll_offset = 26;
    this._y1_coll_offset = 66;
    this._coll_box_width = 52;
    this._coll_box_height = 56;
    this._eats_bugs = false;
    this._flyer = false;
    this._breaks_cars = false;
    this._hp = 100;
    this._hp_max = 100;
  }
  else if (player === "dino") {
    this._image = this._images.dino;
    this._y_draw_offset = 206;
    this._x_draw_offset = 81;
    this._speed = 0.0005;
    this._x1_coll_offset = 56;
    this._y1_coll_offset = 191;
    this._coll_box_width = 112;
    this._coll_box_height = 181;
    this._eats_bugs = true;
    this._flyer = false;
    this._breaks_cars = true;
    this._hp = 600;
    this._hp_max = 620;
  }
  else if (player === "ufo") {
    this._image = this._images.ufo;
    this._y_draw_offset = null;
    this._x_draw_offset = null;
    this._speed = 0.003;
    this._x1_coll_offset = 85;
    this._y1_coll_offset = 21;
    this._coll_box_width = 170;
    this._coll_box_height = 62;
    this._eats_bugs = false;
    this._flyer = true;
    this._breaks_cars = false;
    this._hp = 150;
    this._hp_max = 150;
  }
};

Player.prototype.init_new_level = function () {
  this.set_location(this._x_start, this._y_max);
  this._dx = 0; this._dy = 0;
  this._is_moving = false;
};

Player.prototype.keydown_handler = function (e, time) {
  if (time === null) return;

  // determine the distance travel; the goal is that a 
  // single key press should be 1 second of travel
  var dist = 1 * this._speed / 0.001;

  // determine the destination x, y
  var x = this._x; var y = this._y;

  if (e.keyCode === 37) { // left arrow
    x -= dist;
  }
  else if (e.keyCode === 38) { // up arrow
    y -= dist;
  }
  else if (e.keyCode === 39) { // right arrow
    x += dist;
  }
  else if (e.keyCode === 40) { // down arrow
    y += dist;
  }

  // movement limitation
  if (x < 0) x = 0;
  else if (x > this._x_max) x = this._x_max;
  if (y < 0) y = 0;
  else if (y > this._y_max) y = this._y_max;

  // compute dx and dy
  var dx = x - this._x;
  var dy = y - this._y;

  this.start_move(time, dx, dy);
};

/** Check the player's collision box against the one inside game_obj.
  * Returns true or false. */
Player.prototype._check_collision = function (game_obj) {

  if (this._x1_coll < game_obj._x2_coll &&
      game_obj._x1_coll < this._x2_coll &&
      this._y1_coll < game_obj._y2_coll &&
      game_obj._y1_coll < this._y2_coll) {

    game_obj.set_collided(true);
    return true;
  }

  else
    return false;
};

/** Check for collision between player and the given bug object. */
Player.prototype.check_collision_bug = function (bug, time,
      temp_effects) {

  if (this._flyer) return;
  if (bug._enable === false) return;
  if (bug._collided) return;

  if (this._check_collision(bug)) {
    if (this._eats_bugs) {
      bug.set_enable(false);
      temp_effects.add_text(time, "Yum!", this._x, this._y - 0.2);
      this._modify_hp(10);
    }
    else {
      temp_effects.add_text(time, "Ouch!", this._x, this._y - 0.2);
      this._modify_hp(-20);
    }
  }
};

/** Check for collision between player and the given car object. */
Player.prototype.check_collision_car = function (car, time,
      temp_effects) {
  
  if (this._flyer) return;
  if (car._enable === false) return;
  if (car._collided) return;

  if (this._check_collision(car)) {
    if (this._breaks_cars) {
      car.set_enable(false);
      temp_effects.add_explosion(time, car._x, car._y);
      temp_effects.add_text(time, "Ouch!", this._x, this._y - 0.2);
      this._modify_hp(-25);
    }
    else {
      temp_effects.add_text(time, "Ouch!", this._x, this._y - 0.2);
      this._modify_hp(-50);
    }
  }
};

/** Check for collision between player and the given dragon object. */
Player.prototype.check_collision_dragon = function (dragon, time,
      temp_effects) {
  
  if (this._flyer === false) return;
  if (dragon._enable === false) return;
  if (dragon._collided) return;

  if (this._check_collision(dragon)) {
    temp_effects.add_text(time, "Ouch!", this._x, this._y - 0.2);
    this._modify_hp(-50);
  }
};

/**
 * Check for collision between player and the given fireball object.
 */
Player.prototype.check_collision_fireball = function (fireball, time,
      temp_effects) {
  
  if (fireball._enable === false) return;
  if (fireball._collided) return;

  if (this._check_collision(fireball)) {
    temp_effects.add_explosion(time, fireball._x, fireball._y);
    temp_effects.add_text(time, "Ouch!", this._x + 0.4, this._y - 0.4);
    fireball.set_enable(false);
    this._modify_hp(-50);
  }
};

/** Change the HP by the given "hp_change". */
Player.prototype._modify_hp = function (hp_change) {
  this._hp += hp_change;
  if (this._hp > this._hp_max) this._hp = this._hp_max;
  else if (this._hp < 0) this._hp = 0;
};

// #endregion


// #region class Enemy inherits from GameObj

var Enemy = function (my_canvas, image, map, speed) {
  GameObj.call(this, my_canvas, image, map);
  this._speed = speed;

  // collision box spec
  if (image === Frogger._images.bug) {
    this._x1_coll_offset = 41;
    this._y1_coll_offset = 30;
    this._coll_box_width = 75;
    this._coll_box_height = 52;
  }
  else if (image === Frogger._images.car) {
    this._x1_coll_offset = 85;
    this._y1_coll_offset = 17;
    this._coll_box_width = 170;
    this._coll_box_height = 43;
  }
  else if (image === Frogger._images.fireball) {
    this._x1_coll_offset = 9;
    this._y1_coll_offset = 24;
    this._coll_box_width = 44;
    this._coll_box_height = 48;
  }
};

Enemy.prototype = Object.create(GameObj.prototype);
Enemy.prototype.constructor = Enemy;

/**
 * call "start_move" to move the bug to the right side of the map
 */
Enemy.prototype.start_move_to_right = function (time) {
  if (this._enable === false) return;
  var final_x = this._map._num_col - 0.5
                + this._image.width / 2 / this._map._square_width;
  var dx = final_x - this._x;
  if (dx > 0) this.start_move(time, dx, 0);
};

// #endregion


// #region class Dragon inherits from Enemy

var Dragon = function (my_canvas, image, map, speed) {
  Enemy.call(this, my_canvas, image, map, speed);

  this.reset_fireball();

  // collision spec
  this._x1_coll_offset = 47;
  this._y1_coll_offset = 30;
  this._coll_box_width = 128;
  this._coll_box_height = 62;
};

Dragon.prototype = Object.create(Enemy.prototype);
Dragon.prototype.constructor = Dragon;

/**
 * Resets fireball related variables.
 */
Dragon.prototype.reset_fireball = function () {
  // the dragon will launch fireball when it's x value is "fireball_x"
  var max_x = this._map._num_col - 1;
  this._fireball_x = Math.random() * max_x / 2;
  this._fireballs_left = 1;
};

/**
 * Configures an Enemy object to appear in front
 * of the dragon.
 */
Dragon.prototype.update_fireball = function (fireball, time) {
  if (this._enable === false) return;

  // fireball is a disabled Enemy object
  if (this._fireballs_left >= 0
    && this._x >= this._fireball_x) {
    fireball.set_location(this._x + 1.2, this._y);
    fireball.set_enable(true);
    fireball.set_collided(false);
    fireball.start_move_to_right(time);
    this._fireballs_left--;
  }
};

// #endregion

