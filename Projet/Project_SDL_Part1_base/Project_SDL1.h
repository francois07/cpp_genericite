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

class animal
{
private:
  SDL_Surface *window_surface_ptr_; // ptr to the surface on which we want the
                                    // animal to be drawn, also non-owning
  SDL_Surface *image_ptr_;          // The texture of the sheep (the loaded image), use
                                    // load_surface_for
  // todo: Attribute(s) to define its position
protected:
  int x_pos_;
  int y_pos_;

  int x_vel_;
  int y_vel_;

public:
  animal(const std::string &file_path, SDL_Surface *window_surface_ptr);
  // todo: The constructor has to load the sdl_surface that corresponds to the
  // texture
  ~animal(); // todo: Use the destructor to release memory and "clean up
             // behind you"

  void draw(SDL_Surface *window_surface_ptr); // todo: Draw the animal on the screen <-> window_surface_ptr.
                                              // Note that this function is not virtual, it does not depend
                                              // on the static type of the instance

  virtual void move(){}; // todo: Animals move around, but in a different
                         // fashion depending on which type of animal
  int get_x_pos() const { return x_pos_; };
  int get_y_pos() const { return y_pos_; };
};

// Insert here:
// class sheep, derived from animal
class sheep : public animal
{
public:
  // todo
  // Ctor
  sheep(const std::string &file_path, SDL_Surface *window_surface_ptr);
  // Dtor
  ~sheep();
  // implement functions that are purely virtual in base class
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
  wolf(const std::string &file_path, SDL_Surface *window_surface_ptr);
  // Dtor
  ~wolf();
  // implement functions that are purely virtual in base class
  void move();

  int get_target_x() const { return target_x_; };
  int get_target_y() const { return target_y_; };

  void set_target_x(int x) { this->target_x_ = x; };
  void set_target_y(int y) { this->target_y_ = y; };

  int find_closest_sheep(std::vector<std::shared_ptr<animal>> animals) const;

private:
  int target_x_;
  int target_y_;
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
  std::vector<std::shared_ptr<animal>> animals_;

public:
  ground(SDL_Surface *window_surface_ptr);    // todo: Ctor
  ~ground();                                  // todo: Dtor, again for clean up (if necessary)
  void add_animal(std::shared_ptr<animal> a); // todo: Add an animal
  void update();                              // todo: "refresh the screen": Move animals and draw them
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