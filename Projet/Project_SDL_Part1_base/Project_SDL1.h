// SDL_Test.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.

#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <vector>
#include <set>

// Defintions
constexpr double frame_rate = 60.0; // refresh rate
constexpr double frame_time = 1. / frame_rate;
constexpr unsigned frame_width = 1400; // Width of window in pixel
constexpr unsigned frame_height = 900; // Height of window in pixel
// Minimal distance of animals to the border
// of the screen
constexpr unsigned frame_boundary = 100;

// Helper function to initialize SDL
void init();

class interacting_object
{
protected:
  std::set<std::string> properties_;

public:
  interacting_object(const std::set<std::string> properties = std::set<std::string>());
  ~interacting_object(){};

  virtual void interact(interacting_object &object){};

  void insert_property(std::string property) { properties_.insert(property); };
  void remove_property(std::string property) { properties_.erase(property); };
  bool has_property(const std::string key) { return properties_.find(key) != properties_.end(); };
};

class rendered_object : public interacting_object
{
private:
  SDL_Surface *window_surface_ptr_;
  SDL_Surface *image_ptr_;

protected:
  int x_pos_;
  int y_pos_;

public:
  rendered_object(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      int x_pos = 0, int y_pos = 0,
      std::set<std::string> properties = std::set<std::string>());
  ~rendered_object();

  virtual void interact(interacting_object &object){};

  int get_x_pos() const { return x_pos_; };
  int get_y_pos() const { return y_pos_; };

  void draw(SDL_Surface *window_surface_ptr);
};

class moving_object : public rendered_object
{
protected:
  int x_vel_;
  int y_vel_;

public:
  moving_object(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      int x_pos = 0, int y_pos = 0,
      int x_vel = 1, int y_vel = 1,
      std::set<std::string> properties = std::set<std::string>());
  ~moving_object(){};

  void set_x_vel(int x_vel) { x_vel_ = x_vel; };
  void set_y_vel(int y_vel) { y_vel_ = y_vel; };

  int get_x_vel() const { return x_vel_; }
  int get_y_vel() const { return y_vel_; }

  virtual void interact(interacting_object &object){};
  virtual void move(){};

  std::shared_ptr<moving_object> find_closest_object(std::vector<std::shared_ptr<moving_object>> objects, std::string object_type = "") const;
  void move_towards(const int x, const int y);
  int distance(moving_object &object) const;
  // int step(); ??
};

class playable_character : public moving_object
{
public:
  playable_character(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      int x_pos = 0, int y_pos = 0,
      int x_vel = 1, int y_vel = 1,
      std::set<std::string> properties = std::set<std::string>({"player", "alive"}));
  ~playable_character(){};

  virtual void interact(interacting_object &object){};

  void move();
};

class animal : public moving_object
{
public:
  animal(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      int x_pos = 0, int y_pos = 0,
      int x_vel = 0, int y_vel = 0,
      std::set<std::string> properties = std::set<std::string>());
  ~animal(){};

  virtual void interact(interacting_object &object){};
  virtual void move(){};
};

class sheep : public animal
{
public:
  sheep(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      int x_pos = 0, int y_pos = 0,
      int x_vel = 1, int y_vel = 1,
      std::set<std::string> properties = std::set<std::string>({"sheep", "prey", "alive"}));
  ~sheep(){};

  void interact(interacting_object &object);

  void move();
};

// Insert here:
// class wolf, derived from animal
// Use only sheep at first. Once the application works
// for sheep you can add the wolves
class wolf : public animal
{
public:
  // todo
  // Ctor
  wolf(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      int x_pos = 0, int y_pos = 0,
      int x_vel = 1, int y_vel = 1,
      int life = 200,
      std::set<std::string> properties = std::set<std::string>({"wolf", "predator", "alive"}));
  // Dtor
  ~wolf(){};
  // implement functions that are purely virtual in base class
  int get_life() const { return life_; };
  void increase_life(int k) { life_ += k; };
  void reduce_life(int k) { life_ -= k; };

  void interact(interacting_object &object);
  void move();

private:
  int life_;
};

class dog : public animal
{
public:
  // todo
  // Ctor
  dog(
      const std::string &file_path,
      SDL_Surface *window_surface_ptr,
      std::shared_ptr<moving_object>,
      int target_dist = 128,
      int x_vel = 1, int y_vel = 1,
      std::set<std::string> properties = std::set<std::string>({"dog", "alive"}));
  // Dtor
  ~dog(){};
  // implement functions that are purely virtual in base class
  void interact(interacting_object &object);
  void move();

private:
  std::shared_ptr<moving_object> target_object_;
  int target_dist_;
};

// The "ground" on which all the animals live (like the std::vector
// in the zoo example).
class ground
{
private:
  // Attention, NON-OWNING ptr, again to the screen
  SDL_Surface *window_surface_ptr_;

  // Some attribute to store all the wolves and sheep
  // here
  std::vector<std::shared_ptr<moving_object>> objects_;

public:
  ground(SDL_Surface *window_surface_ptr);           // todo: Ctor
  ~ground();                                         // todo: Dtor, again for clean up (if necessary)
  void add_object(std::shared_ptr<moving_object> a); // todo: Add an animal
  void update();                                     // todo: "refresh the screen": Move animals and draw them
  // Possibly other methods, depends on your implementation
};

// The application class, which is in charge of generating the window
class application
{
private:
  // The following are OWNING ptrs
  SDL_Window *window_ptr_;
  SDL_Surface *window_surface_ptr_;
  SDL_Event window_event_;

  // Other attributes here, for example an instance of ground
  unsigned n_sheep_;
  unsigned n_wolf_;
  ground ground_;

public:
  application(unsigned n_sheep, unsigned n_wolf); // Ctor
  ~application();                                 // dtor

  int loop(unsigned period); // main loop of the application.
                             // this ensures that the screen is updated
                             // at the correct rate.
                             // See SDL_GetTicks() and SDL_Delay() to enforce a
                             // duration the application should terminate after
                             // 'period' seconds
};