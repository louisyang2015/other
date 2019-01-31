/// <reference path="GameObj.js" />
/// <reference path="Frogger.js" />

"use strict";

// #region class Game

var Game = function (my_canvas, images) {
  this._my_canvas = my_canvas;
  this._images = images;

  this._map = new Map(my_canvas, images);

  this._player = new Player(my_canvas, images, this._map);
  this._enemies_controller = new EnemiesController();

  this._temp_effects = new TempEffects(my_canvas,
        [images.explosion1, images.explosion2, images.explosion3],
        this._map);

  this._win = false;
  this._lost = false;
};

/** Initialize a game object given the player character and the 
  * difficulty level. This should be called for every new game.
  * @param {string} player_str - "boy", "girl", "dino", "ufo"
  * @param {number} level
  */
Game.prototype.init = function (player_str, level) {

  this._map.init();
  this._level = level;

  // the player gets "configured" only on the first level
  if (level === Frogger._init_level)
    this._player.config(player_str);
  this._player.init_new_level();

  var enemies = this._enemies_controller.init(level, this._my_canvas,
                                       this._images, this._map);
  this._bugs = enemies[0];
  this._cars = enemies[1];
  this._dragons = enemies[2];
  this._fireballs = enemies[3];

  this._temp_effects.set_text_styles(player_str);

  this._win = false;
  this._lost = false;
};

/**
 * Extra animation related work that is required for the very first frame.
 */
Game.prototype.first_frame = function (time) {
  var i, j;
  for (i = 0; i < this._bugs.length; i++)
    for (j = 0; j < this._bugs[i].length; j++)
      this._bugs[i][j].start_move_to_right(time);

  for (i = 0; i < this._cars.length; i++)
    for (j = 0; j < this._cars[i].length; j++)
      this._cars[i][j].start_move_to_right(time);

  for (i = 0; i < this._dragons.length; i++)
    for (j = 0; j < this._dragons[i].length; j++)
      this._dragons[i][j].start_move_to_right(time);
};

Game.prototype.render = function (time) {
  // update position of all units
  this._update(time);

  this._check_collisions(time);

  // check of end of game conditions - reached home, fell into water
  // or HP reached zero
  var game_over_msg = null;
  var game_over_color = null;
  if (this._map.is_at_home(this._player._x, this._player._y)) {
    this._win = true;
    if (this._level >= Frogger._last_level)
      game_over_msg = "You have won the game!";
    else
      game_over_msg = "You have finished level " + String(this._level) + "!";
    game_over_color = "#00BF32"; // Green
  }
  else if (this._map.is_on_water(this._player._x, this._player._y)
           && this._player._flyer === false) {
    this._lost = true;
    game_over_msg = "You Drowned!";
    game_over_color = "#2C17B1"; // Blue
  }
  else if (this._player._hp === 0) {
    this._lost = true;
    game_over_msg = "You Died!";
    game_over_color = "#EA3A8D"; // Magenta
  }

  // spawn more enemies
  this._enemies_controller.spawn(time);

  // render to screen
  this._draw_all(game_over_msg, game_over_color);
};

Game.prototype._update = function (time) {
  // update player
  this._player.update(time);

  // update bug, car, dragon movements
  var i, j;
  for (i = 0; i < this._bugs.length; i++)
    for (j = 0; j < this._bugs[i].length; j++)
      this._bugs[i][j].update(time);

  for (i = 0; i < this._cars.length; i++)
    for (j = 0; j < this._cars[i].length; j++)
      this._cars[i][j].update(time);

  for (i = 0; i < this._dragons.length; i++)
    for (j = 0; j < this._dragons[i].length; j++)
      this._dragons[i][j].update(time);

  // update dragon casting the fireball
  for (i = 0; i < this._dragons.length; i++)
    for (j = 0; j < this._dragons[i].length; j++)
      this._dragons[i][j].update_fireball(this._fireballs[i][j], time);

  // update any existing fireball
  for (i = 0; i < this._fireballs.length; i++)
    for (j = 0; j < this._fireballs[i].length; j++)
      this._fireballs[i][j].update(time);

  this._temp_effects.update(time);
};

Game.prototype._check_collisions = function (time) {
  var bugs = this._bugs; var cars = this._cars;
  var dragons = this._dragons; var fireballs = this._fireballs;
  var player = this._player;

  // bug collisions
  var i, j;
  for (i = 0; i < bugs.length; i++)
    for (j = 0; j < bugs[i].length; j++) {
      if (bugs[i][j]._enable === true) {

        player.check_collision_bug(bugs[i][j], time, this._temp_effects);
      }
    }

  // car collisions
  for (i = 0; i < cars.length; i++)
    for (j = 0; j < cars[i].length; j++) {
      if (cars[i][j]._enable === true) {

        player.check_collision_car(cars[i][j], time, this._temp_effects);
      }
    }

  // dragon collisions
  for (i = 0; i < dragons.length; i++)
    for (j = 0; j < dragons[i].length; j++) {
      if (dragons[i][j]._enable === true) {

        player.check_collision_dragon(dragons[i][j], time, this._temp_effects);
      }
    }

  // fireball collisions
  for (i = 0; i < fireballs.length; i++)
    for (j = 0; j < fireballs[i].length; j++) {
      if (fireballs[i][j]._enable === true) {

        player.check_collision_fireball(fireballs[i][j], time, this._temp_effects);
      }
    }
};

Game.prototype._draw_all = function (game_over_msg, game_over_color) {
  this._my_canvas.set_background("white", "white");

  this._map.render();

  // render boy / girl
  if (this._player._image === this._images.boy
        || this._player._image === this._images.girl)
    this._player.render();

  // render bugs and cars
  var i, j;
  for (i = 0; i < this._bugs.length; i++)
    for (j = 0; j < this._bugs[i].length; j++)
      this._bugs[i][j].render();

  for (i = 0; i < this._cars.length; i++)
    for (j = 0; j < this._cars[i].length; j++)
      this._cars[i][j].render();

  // render dinosaur / UFO
  if (this._player._image === this._images.dino
        || this._player._image === this._images.ufo)
    this._player.render();

  // render fireball
  for (i = 0; i < this._fireballs.length; i++)
    for (j = 0; j < this._fireballs[i].length; j++)
      this._fireballs[i][j].render();

  // render dragon
  for (i = 0; i < this._dragons.length; i++)
    for (j = 0; j < this._dragons[i].length; j++)
      this._dragons[i][j].render();

  // render temporary effects
  this._temp_effects.render();

  // render health and difficulty level
  var hp = "Health: " + String(this._player._hp);
  var level = "Level: " + String(this._level);
  var canvas_width = this._my_canvas.canvas.width;

  this._my_canvas.draw_text("30px Impact", "#539700", "black", hp,
      canvas_width * 0.15, 40);
  this._my_canvas.draw_text("30px Impact", "#FF9200", "black", level,
      canvas_width * 0.85, 40);

  // render game over messages
  if (game_over_msg !== null) {
    var canvas_height = this._my_canvas.canvas.height;

    this._my_canvas.grayscale_filter([[0, 0, canvas_width, canvas_height]], true);

    this._my_canvas.draw_text_centered("70px Comic Sans MS", game_over_color, "black",
          game_over_msg, this._my_canvas.canvas.height * 0.35);
    this._my_canvas.draw_text_centered("40px Arial", game_over_color, "black",
          "Press ENTER to continue...", this._my_canvas.canvas.height * 0.75);
  }
};

Game.prototype.keydown_handler = function (e, time) {
  this._player.keydown_handler(e, time);
};

// #endregion


// #region class EnemiesController

var EnemiesController = function () {
};

/** Enemy creation function.
  * @returns {Array} [bus, cars, dragons, fireballs]  */
EnemiesController.prototype.init = function (level, my_canvas, images, map) {
  // returns [bus, cars, dragons, fireballs]
  // bugs[0] is an array of bugs that appear on the same row on the map
  // bugs will also be available as this._bugs

  this._level = level;
  this._max_x = map._num_col - 1;
  this._square_width = map._square_width;

  var bugs = []; var cars = [];
  var dragons = []; var fireballs = [];

  var difficulty = this._create_difficulty(level);

  // ground_enemies, max_ground_enemies, ground_speed, 
  var i, j, enemy, enemies, type, num, speed, spawn_space;
  for (i = 0; i < difficulty.ground_enemies.length; i++) {

    // currently on row "i+1"
    enemies = [];
    type = difficulty.ground_enemies[i];
    num = difficulty.max_ground_enemies[i];
    speed = difficulty.ground_speed[i];

    if (type !== null) {

      // add to "enemies"
      for (j = 0; j < num; j++) {
        var image;
        if (type === "bug") image = images.bug;
        else if (type === "car") image = images.car;
        enemy = new Enemy(my_canvas, image, map, speed);
        enemy.set_location(0, i + 1);
        enemies.push(enemy);
      }

      // spawn enemies on map
      spawn_space = this._max_x / (num - 1);

      for (j = 0; j < num - 1; j++) {
        enemies[j].spawn_between(j * spawn_space, (j + 1) * spawn_space, i + 1);
      }

      // add to bugs or cars
      if (type === "bug") bugs.push(enemies);
      else if (type === "car") cars.push(enemies);
    }
  }

  // air_enemies, max_air_enemies, air_speed
  for (i = 0; i < difficulty.air_enemies.length; i++) {

    // currently on row "i+1"
    enemies = [];
    var fireball_row = [];
    type = difficulty.air_enemies[i];
    num = difficulty.max_air_enemies[i];
    speed = difficulty.air_speed[i];

    if (type !== null) {

      // add to "enemies"
      for (j = 0; j < num; j++) {
        enemy = new Dragon(my_canvas, images.dragon, map, speed);
        enemy.set_location(0, i + 1);
        enemies.push(enemy);

        //every dragon has its own fireball
        var fireball = new Enemy(my_canvas, images.fireball,
                                 map, speed * 1.4);
        fireball.set_location(0, i + 1);
        fireball_row.push(fireball);
      }

      // spawn enemies on map
      spawn_space = this._max_x / (num - 1);

      for (j = 0; j < num - 1; j++) {
        enemies[j].spawn_between(j * spawn_space, (j + 1) * spawn_space, i + 1);
      }

      dragons.push(enemies);
      fireballs.push(fireball_row);
    }
  }

  // XXX_respawn_distances
  this._bug_respawn_distances = [];
  this._car_respawn_distances = [];
  this._dragon_respawn_distances = [];

  for (i = 0; i < bugs.length; i++)
    this._bug_respawn_distances.push(null);

  for (i = 0; i < cars.length; i++)
    this._car_respawn_distances.push(null);

  for (i = 0; i < dragons.length; i++)
    this._dragon_respawn_distances.push(null);

  this._bugs = bugs; this._cars = cars;
  this._dragons = dragons; this._fireballs = fireballs;

  return [bugs, cars, dragons, fireballs];
};

/**
 * Returns a difficulty object, given an integer level.
 */
EnemiesController.prototype._create_difficulty = function (level) {
  // level design
  // level 1 - show off all enemies
  // level 2 - bugs
  // level 3 - cars
  // level 4 - dragons and cars

  // difficulty object:
  // ground_enemies - "bug", "car", or null - what ground enemy is in each row
  // air_enemies - "dragon", or null - what air enemy is in each row
  // max_enemies - how many enemies per row
  var difficulty = {};

  if (level === 1) {
    // enemy description: 
    difficulty.ground_enemies = ["car", "bug", "bug", "bug", null];
    difficulty.air_enemies = [null, null, null, null, "dragon"];
    // This means row #1 will be a car row, followed by 
    // 3 bug rows, followed by a dragon row

    // number of enemies (for a particular row)
    difficulty.max_ground_enemies = [2, 2, 2, 2, 0];
    difficulty.max_air_enemies = [0, 0, 0, 0, 2];

    // enemy speed (for a particular row)
    difficulty.ground_speed = [0.002, 0.001, 0.001, 0.001, null];
    difficulty.air_speed = [null, null, null, null, 0.003];
  }
  else if (level === 2) {
    difficulty.ground_enemies = ["bug", "bug", "bug", "bug", "bug"];
    difficulty.air_enemies = [null, null, null, null, null];
    difficulty.max_ground_enemies = [4, 4, 4, 4, 4];
    difficulty.max_air_enemies = [0, 0, 0, 0, 0];
    difficulty.ground_speed = [0.001, 0.001, 0.001, 0.001, 0.001];
    difficulty.air_speed = [null, null, null, null, null];
  }
  else if (level === 3) {
    difficulty.ground_enemies = ["car", "car", "car", "car", "car"];
    difficulty.air_enemies = [null, null, null, null, null];
    difficulty.max_ground_enemies = [3, 2, 3, 2, 3];
    difficulty.max_air_enemies = [0, 0, 0, 0, 0];
    difficulty.ground_speed = [0.002, 0.002, 0.002, 0.002, 0.002];
    difficulty.air_speed = [null, null, null, null, null];
  }
  else {
    // level 4+
    difficulty.ground_enemies = ["car", "car", "car", "car", "car"];
    difficulty.air_enemies = [null, "dragon", null, "dragon", null];
    difficulty.max_ground_enemies = [3, 3, 3, 3, 3];
    difficulty.max_air_enemies = [0, 3, 0, 3, 0];
    difficulty.ground_speed = [0.002, 0.002, 0.002, 0.002, 0.002];
    difficulty.air_speed = [null, 0.003, null, 0.003, null];
  }

  // shuffle the data - for example ["bug", "bug", "car"] 
  // can become ["bug", "car", bug"]
  var length = difficulty.ground_enemies.length;
  for (var i = 0; i < length; i++) {

    // swap "i" with another item in the array
    var j = Frogger._random_int(0, length - 1);

    if (i !== j) {
      this._swap(difficulty.ground_enemies, i, j);
      this._swap(difficulty.air_enemies, i, j);
      this._swap(difficulty.max_ground_enemies, i, j);
      this._swap(difficulty.max_air_enemies, i, j);
      this._swap(difficulty.ground_speed, i, j);
      this._swap(difficulty.air_speed, i, j);
    }
  }

  return difficulty;
};

/**
 * Swap two elements of an array.
 */
EnemiesController.prototype._swap = function (array, i, j) {
  var temp = array[i];
  array[i] = array[j];
  array[j] = temp;
};

/**
 * Make enemies appear and start the motion across the screen
 */
EnemiesController.prototype.spawn = function (time) {
  var i;
  for (i = 0; i < this._bugs.length; i++)
    this._spawn_unit(this._bugs[i], this._bug_respawn_distances, i, time);

  for (i = 0; i < this._cars.length; i++)
    this._spawn_unit(this._cars[i], this._car_respawn_distances, i, time);

  for (i = 0; i < this._dragons.length; i++)
    this._spawn_unit(this._dragons[i], this._dragon_respawn_distances, i, time);
};

/**
 * Go through an array of bug or car objects and spawn as necessary.
 * The "index" shows where to get the "respawn_distances".
 */
EnemiesController.prototype._spawn_unit = function (game_objs,
      respawn_distances, index, time) {

  if (game_objs.length === 0) return;

  // disable enemy units thave have stopped
  var i;
  for (i = 0; i < game_objs.length; i++) {
    if (game_objs[i]._is_moving === false)
      game_objs[i].set_enable(false);
  }

  // is there a respawn distance? 
  var dist = respawn_distances[index];
  if (dist !== null) {
    // a respawn distance exist, check for opportunity to respawn
    // #1, a disabled object has to exist
    var disabled_index = -1;
    for (i = 0; i < game_objs.length; i++) {
      if (game_objs[i]._enable === false) {
        disabled_index = i;
        break;
      }
    }
    if (disabled_index === -1) return; // enemies already at maximum for this level

    // #2, the smallest _x has to be larger than x_start + dist

    // the start location (and end location) is off the map, so that the unit
    // doesn't just teleport into the map. This location is thus dependent on
    // the width of the unit

    var x_start = -0.5 - game_objs[i]._image.width / 2 / this._square_width;

    var smallest_x = 1000; // find left most unit's coordinate

    for (i = 0; i < game_objs.length; i++) {

      if (game_objs[i]._enable === true) {
        if (game_objs[i]._x < smallest_x)
          smallest_x = game_objs[i]._x;
      }
    }
    if (smallest_x <= x_start + dist) return; // not yet ready to spawn

    // codes gets here means both conditions are okay. spawn the unit
    var dx = -2 * x_start + this._max_x;
    var obj = game_objs[disabled_index];
    obj.set_location(x_start, obj._y);
    obj.start_move(time, dx, 0);
    obj.set_collided(false);

    // set the current respawn distance to null so a new one will be generated
    respawn_distances[index] = null;

    // in the case of the dragon, the fireball stats need to be reset
    if (game_objs[disabled_index]._image === Frogger._images.dragon)
      // this object is a dragon
      game_objs[disabled_index].reset_fireball();

    return;
  }
  else {
    // if no respawn distance, generate one
    respawn_distances[index] = this._compute_respawn_distance(game_objs);
  }

};

/**
 * Computes the re-spawn distane for the current row of objects. It's
 * assumed that all objects in the row are identical.
 */
EnemiesController.prototype._compute_respawn_distance = function (game_objs) {
  // average distance between enemies; hold one enemy in reserve to be 
  // spawned whenever the distance is sufficient, avoid starvation - 
  // meaning distance condition is satisfied but no unit is available
  // to be spawned. Starvation makes short distances un-implementable,
  // causing the traffic to lose its randomness.
  var avg_dist = this._max_x / (game_objs.length - 1);

  // default lower bound and upper bound
  var lower_bound = avg_dist * 0.5;
  var upper_bound = avg_dist * 1.5;

  // this lower_bound might be too low - check against enemy size
  if (lower_bound < (game_objs[0]._image.width / this._square_width)) {
    // lower bound must be enemy size, to prevent overlap
    lower_bound = game_objs[0]._image.width / this._square_width;

    upper_bound = lower_bound * 1.75; // alternative upper_bound
  }

  var dist = lower_bound + Math.random() * (upper_bound - lower_bound);
  return dist;
};

// #endregion


// #region class TempEffects

/**
 * Temporary effects.
 * @param {MyCanvas} my_canvas - wrapper around HTML canvas element
 * @param {Array} explode_images - images to use for explosions
 */
var TempEffects = function (my_canvas, explode_images, map) {
  this._my_canvas = my_canvas;
  this._explode_images = explode_images;
  this._map = map;

  // text settings
  this._stroke = null;
  this._fill = null;
  this._font = "50px Comic Sans MS";

  this._text_effects_buf = new CircularBuffer(5); // text effects buffer 
  this._exp_effects_buf = new CircularBuffer(5); // explosion effects buffer
};

/**
 * Sets text effects drawing parameters. This is dependent on the 
 * player character selection.
 * @param {string} player - "boy", "girl", "dino", or "ufo"
 */
TempEffects.prototype.set_text_styles = function (player) {
  if (player === "boy") {
    this._fill = "#3415B0"; // blue
    this._stroke = "#FFCF00"; // yellow
  }
  else if (player === "girl") {
    this._fill = "#D30068"; // magenta
    this._stroke = "#92EC00"; // green
  }
  else if (player === "dino") {
    this._fill = "#FFDF00"; // yellow
    this._stroke = "#539700"; // dark green
  }
  else if (player === "ufo") {
    this._fill = "#FFFA00"; // yellow
    this._stroke = "#44036F"; // dark purple
  }
};

/** Add explosion at location x,y. */
TempEffects.prototype.add_explosion = function (time, x, y) {

  // set up "obj" and "add_needed" flags
  var obj;
  var add_needed = false;
  if (this._exp_effects_buf.is_full())
    obj = this._exp_effects_buf.modify_oldest();
  else {
    add_needed = true;
    obj = {};
  }

  // choose an image for the explosion
  var img_index = Frogger._random_int(0, this._explode_images.length - 1);

  // configure "obj"
  obj.content = this._explode_images[img_index];
  obj.x = this._map.to_x_pix(x) - obj.content.width / 2;
  obj.y = this._map.to_y_pix(y) - obj.content.height / 2;
  obj.time = time;
  obj.enable = true;

  if (add_needed) this._exp_effects_buf.add(obj);
};

/** Add text at location x,y. */
TempEffects.prototype.add_text = function (time, text, x, y) {

  // set up "obj" and "add_needed" flags
  var obj;
  var add_needed = false;
  if (this._text_effects_buf.is_full())
    obj = this._text_effects_buf.modify_oldest();
  else {
    add_needed = true;
    obj = {};
  }

  // configure "obj"
  obj.x = this._map.to_x_pix(x);
  obj.y = this._map.to_y_pix(y);
  obj.content = text;
  obj.time = time;
  obj.enable = true;

  if (add_needed) this._text_effects_buf.add(obj);
};

/** Disable all existing effects. */
TempEffects.prototype.clear = function () {
  var i;
  for (i = 0; i < this._exp_effects_buf.length() ; i++)
    this._exp_effects_buf.get(i).enable = false;

  for (i = 0; i < this._text_effects_buf.length() ; i++)
    this._text_effects_buf.get(i).enable = false;
};

/** Disable all effects that are out date. */
TempEffects.prototype.update = function (time) {
  var i, j, time_diff;

  var buffers = [this._exp_effects_buf, this._text_effects_buf];

  for (i = 0; i < buffers.length; i++) {

    var buf = buffers[i];
    for (j = 0; j < buf.length() ; j++) {

      if (buf.get(j).enable) {

        time_diff = time - buf.get(j).time;
        if (time_diff > 1500) {
          buf.get(j).enable = false;
        }
      }
    }
  }

};

/** Draw all effects onto the _my_canvas. */
TempEffects.prototype.render = function () {
  // draw explosion objects
  var buf = this._exp_effects_buf;

  var i, obj;
  for (i = 0; i < buf.length() ; i++) {
    obj = buf.get(i);

    if (obj.enable)
      this._my_canvas.context.drawImage(obj.content, obj.x, obj.y);
  }

  // draw text objects
  buf = this._text_effects_buf;
  for (i = 0; i < buf.length() ; i++) {
    obj = buf.get(i);

    if (obj.enable)
      this._my_canvas.draw_text(this._font, this._fill, this._stroke,
        obj.content, obj.x, obj.y);
  }
};

// #endregion


// #region class CircularBuffer

/**
 * A circular buffer to store generic content.
 * @param {number} max_size - maximum size of the internal storage buffer
 */
var CircularBuffer = function (max_size) {
  this._MAX_SIZE = max_size;
  this._content = [];
  this._oldest = 0; // 0, 1, ... max_size-1, then 0 again
  // new objects are added to _content[_oldest]
};

CircularBuffer.prototype.length = function () {
  return this._content.length;
};

CircularBuffer.prototype.is_full = function () {
  if (this._content.length >= this._MAX_SIZE) return true;
  else return false;
};

/**Adds an object to the circular buffer.*/
CircularBuffer.prototype.add = function (obj) {
  var content = this._content;

  if (content.length < this._MAX_SIZE)
    // the content array is allowed to grow
    content.push(obj);

  else {
    // the content array has reach its max size
    content[this._oldest] = obj;
    this._oldest++;
    if (this._oldest >= this._MAX_SIZE) this._oldest = 0;
  }
};

/** Returns the object at "index". Returns a null if no object exist. */
CircularBuffer.prototype.get = function (index) {
  var real_index = this._oldest + index;
  if (real_index >= this._MAX_SIZE) real_index -= this._MAX_SIZE;

  if (real_index >= this._content.length) return null; // no object at "index"
  else return this._content[real_index];
};

/** Returns the oldest object for modification. Advance the 
  * oldest pointer. */
CircularBuffer.prototype.modify_oldest = function () {
  var obj = this._content[this._oldest];

  this._oldest++;
  if (this._oldest >= this._MAX_SIZE) this._oldest = 0;

  return obj;
};

// #endregion


// #region class Map

/** Represents the game map. */
var Map = function (my_canvas, images) {
  this._my_canvas = my_canvas;
  this._images = images;

  // map settings:
  this._num_col = 11; // width of the map in squares
  this._num_stone_rows = 5;  // height of the stone part of the map
  this._square_width = 100;  // 100px wide map square
  this._square_height = 80;  // 80px tall square
  this._header_space = 50; // 50 px of white space above the water tiles

  this._home_square = 3;

};

/** Setup map for a new game */
Map.prototype.init = function () {
  // randomize this._home_square
  this._home_square = 3 + Frogger._random_int(0, this._num_col - 4);
  // _home_square can be 3, 4, ... 
};

/** Converts logical coordinate "x" to pixel coordinate. */
Map.prototype.to_x_pix = function (x) {
  return this._square_width / 2 + x * this._square_width;
};

/** Converts logical coordinate "y" to pixel coordinate. */
Map.prototype.to_y_pix = function (y) {
  return this._square_height / 2 + y * this._square_height + this._header_space;
  // the 50 is the white space on the top of "water-block.png"
};

/** Draws the map onto the my_canvas */
Map.prototype.render = function () {

  var num_col = this._num_col;
  var num_stone_rows = this._num_stone_rows;
  var square_width = this._square_width;
  var square_height = this._square_height;

  var row = 0; // the row number we are currently drawing
  var col = 0;

  // row #0 is ocean, with one "_home_square"  
  var x = 0, y = 0;
  for (col = 0; col < num_col; col++) {
    x = col * square_width;
    if (col === this._home_square) {
      this._my_canvas.context.drawImage(this._images.grass, x, 0);
      this._my_canvas.context.drawImage(this._images.house, x + 12, 35);
    }
    else
      this._my_canvas.context.drawImage(this._images.water, x, 0);
  }

  // some rows of stone
  for (row = 1; row < num_stone_rows + 1; row++) {
    y = row * square_height;

    for (col = 0; col < num_col; col++) {
      x = col * square_width;
      this._my_canvas.context.drawImage(this._images.stone, x, y);
    }
  }

  // one last row of grass
  row = num_stone_rows + 1;
  y = row * square_height;

  for (col = 0; col < num_col; col++) {
    x = col * square_width;
    this._my_canvas.context.drawImage(this._images.grass, x, y);
  }
};

/** Returns true if (x,y) is at the home tile. */
Map.prototype.is_at_home = function (x, y) {
  if (y < 0.25 && x > this._home_square - 0.5
      && x < this._home_square + 0.5)
    return true;
  else
    return false;
};

/** Returns true if (x,y) is on a water tile. */
Map.prototype.is_on_water = function (x, y) {
  if (y < 0.5) {
    // check the water tiles to the left of the _home_square
    if (x < this._home_square - 0.5) return true;

    // check the water tiles to the right of the _home_square
    if (x > this._home_square + 0.5) return true;
  }
  else
    return false;
};

// #endregion
