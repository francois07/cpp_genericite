// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 640

#define TEXTURE_SIZE 64

void init()
{
  // Initialize SDL
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO) < 0)
    throw std::runtime_error("init():" + std::string(SDL_GetError()));

  // Initialize PNG loading
  int imgFlags = IMG_INIT_PNG;
  if (!(IMG_Init(imgFlags) & imgFlags))
    throw std::runtime_error("init(): SDL_image could not initialize! "
                             "SDL_image Error: " +
                             std::string(IMG_GetError()));
}

namespace
{
  // Defining a namespace without a name -> Anonymous workspace
  // Its purpose is to indicate to the compiler that everything
  // inside of it is UNIQUELY used within this source file.

  SDL_Surface *load_surface_for(const std::string &path,
                                SDL_Surface *window_surface_ptr)
  {
    // Helper function to load a png for a specific surface
    // See SDL_ConvertSurface
    auto path_char = path.c_str();
    auto *surface = IMG_Load(path_char);

    return surface;
  }

} // namespace

animal::animal(const std::string &file_path, SDL_Surface *window_surface_ptr)
    : image_ptr_{load_surface_for(file_path, window_surface_ptr)},
      window_surface_ptr_{window_surface_ptr},
      x_vel_{1},
      y_vel_{1}
{
  this->x_pos_ = std::rand() % (WINDOW_WIDTH - TEXTURE_SIZE);
  this->y_pos_ = std::rand() % (WINDOW_HEIGHT - TEXTURE_SIZE);
  this->draw(window_surface_ptr);
};
// todo: The constructor has to load the sdl_surface that corresponds to the
// texture
animal::~animal()
{
  SDL_FreeSurface(image_ptr_);
}; // todo: Use the destructor to release memory and "clean up
   // behind you"

void animal::draw(SDL_Surface *window_surface_ptr)
{
  SDL_Rect rect = SDL_Rect{this->x_pos_, this->y_pos_, TEXTURE_SIZE, TEXTURE_SIZE};
  auto blitRes = SDL_BlitScaled(this->image_ptr_, NULL, window_surface_ptr, &rect);

  if (blitRes != 0)
  {
    std::runtime_error("Couldn't draw texture on rectangle");
  }
}; // todo: Draw the animal on the screen <-> window_surface_ptr.
   // Note that this function is not virtual, it does not depend
   // on the static type of the instance

sheep::sheep(const std::string &file_path, SDL_Surface *window_surface_ptr) : animal{file_path, window_surface_ptr}
{
}
// Dtor
sheep::~sheep()
{
}
// implement functions that are purely virtual in base class
void sheep::move()
{
  this->x_pos_ += 2 * this->x_vel_;
  this->y_pos_ += 2 * this->y_vel_;

  if (x_pos_ > WINDOW_WIDTH - TEXTURE_SIZE || x_pos_ < 0)
  {
    x_vel_ = -x_vel_;
  }
  if (y_pos_ > WINDOW_HEIGHT - TEXTURE_SIZE || y_pos_ < 0)
  {
    y_vel_ = -y_vel_;
  }
}; // todo: Animals move around, but in a different
   // fashion depending on which type of animal

wolf::wolf(const std::string &file_path, SDL_Surface *window_surface_ptr) : animal{file_path, window_surface_ptr}
{
}
// Dtor
wolf::~wolf() {}
// implement functions that are purely virtual in base class
void wolf::move()
{
  this->x_pos_ += 2 * this->x_vel_;
  this->y_pos_ += 2 * this->y_vel_;

  if (x_pos_ > WINDOW_WIDTH - TEXTURE_SIZE || x_pos_ < 0)
  {
    x_vel_ = -x_vel_;
  }
  if (y_pos_ > WINDOW_HEIGHT - TEXTURE_SIZE || y_pos_ < 0)
  {
    y_vel_ = -y_vel_;
  }
}; // todo: Animals move around, but in a different
   // fashion depending on which type of animal

/* Ground */
ground::ground(SDL_Surface *window_surface_ptr) : window_surface_ptr_{window_surface_ptr}
{
}

void ground::add_animal(std::shared_ptr<animal> a)
{
  this->animals_.push_back(a);
  std::cout << this->animals_.size() << std::endl;
}

void ground::update()
{
  SDL_FillRect(this->window_surface_ptr_, NULL, 0x00FF00);
  for (auto a : this->animals_)
  {
    a->move();
    a->draw(window_surface_ptr_);
  }
}

ground::~ground()
{
}

/* Application */
application::application(unsigned n_sheep, unsigned n_wolf)
    : n_sheep_{n_sheep},
      n_wolf_{n_wolf},
      window_ptr_{SDL_CreateWindow("Projet C++", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN)},
      window_surface_ptr_{SDL_GetWindowSurface(this->window_ptr_)},
      ground_{this->window_surface_ptr_},
      window_event_{}
{
  for (int i = 0; i < n_sheep_; i++)
  {
    std::shared_ptr<animal> a_ptr = std::make_shared<sheep>("../media/sheep.png", this->window_surface_ptr_);

    this->ground_.add_animal(a_ptr);
  }
  for (int i = 0; i < n_wolf_; i++)
  {
    std::shared_ptr<animal> a_ptr = std::make_shared<wolf>("../media/wolf.png", this->window_surface_ptr_);

    this->ground_.add_animal(a_ptr);
  }
}

application::~application()
{
  SDL_DestroyWindow(this->window_ptr_);
}

int application::loop(unsigned period)
{
  int ticks = 0;
  while (ticks < (period)*1000)
  {
    ticks = SDL_GetTicks();

    this->ground_.update();
    SDL_UpdateWindowSurface(this->window_ptr_);

    SDL_Delay(10);
  }

  return 0;
}