// SDL_Test.cpp: Definiert den Einstiegspunkt für die Anwendung.
//

#include "Project_SDL1.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <random>
#include <string>
#include <cmath>

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
interacting_object::interacting_object(const std::set<std::string> properties)
    : properties_{properties}
{
}

rendered_object::rendered_object(
    const std::string &file_path,
    SDL_Surface *window_surface_ptr,
    int x_pos, int y_pos,
    std::set<std::string> properties)
    : interacting_object{properties},
      window_surface_ptr_{window_surface_ptr},
      image_ptr_{load_surface_for(file_path, window_surface_ptr)},
      x_pos_{x_pos},
      y_pos_{y_pos}
{
}

rendered_object::~rendered_object()
{
  SDL_FreeSurface(image_ptr_);
}

void rendered_object::draw(SDL_Surface *window_surface_ptr)
{
  SDL_Rect rect = SDL_Rect{this->x_pos_, this->y_pos_, TEXTURE_SIZE, TEXTURE_SIZE};
  auto blitRes = SDL_BlitScaled(this->image_ptr_, NULL, window_surface_ptr, &rect);

  if (blitRes != 0)
  {
    std::runtime_error("Couldn't draw texture on rectangle");
  }
}

moving_object::moving_object(
    const std::string &file_path,
    SDL_Surface *window_surface_ptr,
    int x_pos, int y_pos,
    int x_vel, int y_vel,
    std::set<std::string> properties)
    : rendered_object(file_path, window_surface_ptr, x_pos, y_pos, properties),
      x_vel_{x_vel},
      y_vel_(y_vel)
{
}

void moving_object::move_towards(const int x, const int y)
{
  int relative_x = x - x_pos_;
  int relative_y = y - y_pos_;

  double hyp = std::sqrt(relative_x * relative_x + relative_y * relative_y);

  if (hyp != 0)
  {
    this->x_pos_ = std::clamp((int)(this->x_pos_ + (double)this->x_vel_ * (((double)relative_x / hyp))), 0, WINDOW_WIDTH - TEXTURE_SIZE);
    this->y_pos_ = std::clamp((int)(this->y_pos_ + (double)this->y_vel_ * (((double)relative_y / hyp))), 0, WINDOW_HEIGHT - TEXTURE_SIZE);
  }
}

int moving_object::distance(moving_object &object) const
{
  int dist_x = object.get_x_pos() - this->x_pos_;
  int dist_y = object.get_y_pos() - this->y_pos_;

  return (int)(std::sqrt(dist_x * dist_x + dist_y * dist_y));
}

playable_character::playable_character(const std::string &file_path,
                                       SDL_Surface *window_surface_ptr,
                                       int x_pos, int y_pos,
                                       int x_vel, int y_vel,
                                       std::set<std::string> properties)
    : moving_object(file_path, window_surface_ptr, x_pos, y_pos, x_vel, y_vel, properties)
{
}

void playable_character::move()
{
  SDL_Event event;
  int next_x = this->x_pos_, next_y = this->y_pos_;

  if (SDL_PollEvent(&event))
  {
    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_q:
        next_x -= x_vel_;
        break;
      case SDLK_s:
        next_y += y_vel_;
        break;
      case SDLK_d:
        next_x += x_vel_;
        break;
      case SDLK_z:
        next_y -= y_vel_;
        break;
      }
    }
  }

  this->x_pos_ = std::clamp(next_x, 0, WINDOW_WIDTH - TEXTURE_SIZE);
  this->y_pos_ = std::clamp(next_y, 0, WINDOW_HEIGHT - TEXTURE_SIZE);
}

animal::animal(const std::string &file_path,
               SDL_Surface *window_surface_ptr,
               int x_pos, int y_pos,
               int x_vel, int y_vel,
               std::set<std::string> properties)
    : moving_object{file_path, window_surface_ptr, x_pos, y_pos, x_vel, y_vel, properties}
{
}

std::shared_ptr<moving_object> moving_object::find_closest_object(std::vector<std::shared_ptr<moving_object>> objects, std::string object_type) const
{
  int closest_object_idx = -1;
  int closest_object_dist = (int)INFINITY;

  for (int i = 0; i < objects.size(); i++)
  {
    if ((objects[i]->has_property(object_type) || object_type.empty()) && (this != objects[i].get()))
    {
      int dist = this->distance(*objects[i].get());

      if (dist < closest_object_dist)
      {
        closest_object_dist = dist;
        closest_object_idx = i;
      }
    }
  }

  return closest_object_idx != -1 ? objects[closest_object_idx] : NULL;
}

sheep::sheep(const std::string &file_path,
             SDL_Surface *window_surface_ptr,
             int x_pos, int y_pos,
             int x_vel, int y_vel,
             std::set<std::string> properties)
    : animal{file_path, window_surface_ptr, x_pos, y_pos, x_vel, y_vel, properties}
{
  if (std::rand() % 100 < 50)
  {
    this->properties_.insert("male");
  }
  else
  {
    this->properties_.insert("female");
  }
}

// implement functions that are purely virtual in base class
void sheep::move()
{
  if (this->has_property("fleeing"))
  {
    return;
  }

  this->x_pos_ += this->x_vel_;
  this->y_pos_ += this->y_vel_;

  if (x_pos_ > WINDOW_WIDTH - TEXTURE_SIZE || x_pos_ < 0)
  {
    x_vel_ = -x_vel_;
  }
  if (y_pos_ > WINDOW_HEIGHT - TEXTURE_SIZE || y_pos_ < 0)
  {
    y_vel_ = -y_vel_;
  }
};

void sheep::interact(interacting_object &object)
{
  if (object.has_property("sheep"))
  {
    bool can_reproduce =
        (!this->has_property("male") != !object.has_property("male")) && !this->has_property("infertile") && !object.has_property("infertile");

    if (can_reproduce)
    {
      if (this->has_property("female"))
      {
        this->insert_property("reproduced");
      }
      else
      {
        object.insert_property("reproduced");
      }
    }
  }
}

wolf::wolf(const std::string &file_path,
           SDL_Surface *window_surface_ptr,
           int x_pos, int y_pos,
           int x_vel, int y_vel,
           int life,
           std::set<std::string> properties)
    : animal{file_path, window_surface_ptr, x_pos, y_pos, x_vel, y_vel, properties},
      life_{life}
{
}

// implement functions that are purely virtual in base class
void wolf::move()
{
  if (this->has_property("hunting"))
  {
    this->reduce_life(1);
    if (this->get_life() < 1)
    {
      this->insert_property("dead");
    }
  }
} // todo: Animals move around, but in a different
  // fashion depending on which type of animal

void wolf::interact(interacting_object &object)
{
  if (object.has_property("sheep"))
  {
    object.insert_property("dead");
    this->increase_life(200);
  }
}

dog::dog(const std::string &file_path,
         SDL_Surface *window_surface_ptr,
         std::shared_ptr<moving_object> target_object,
         int target_dist,
         int x_vel, int y_vel,
         std::set<std::string> properties)
    : animal{file_path, window_surface_ptr,
             target_dist, 0,
             x_vel, y_vel, properties},
      target_dist_{target_dist},
      target_object_{target_object}
{
}

void dog::move()
{
  this->x_pos_ = target_dist_ * std::cos(this->x_vel_ * (SDL_GetTicks() / 200.0)) + this->target_object_->get_x_pos();
  this->y_pos_ = target_dist_ * std::sin(this->y_vel_ * (SDL_GetTicks() / 200.0)) + this->target_object_->get_y_pos();
}

void dog::interact(interacting_object &object)
{
}

/* Ground */
ground::ground(SDL_Surface *window_surface_ptr) : window_surface_ptr_{window_surface_ptr}
{
}

void ground::add_object(std::shared_ptr<moving_object> a)
{
  this->objects_.push_back(a);
}

void ground::update()
{
  SDL_FillRect(this->window_surface_ptr_, NULL, 0x00FF00);

  for (int i = 0; i < this->objects_.size(); i++)
  {
    std::shared_ptr<moving_object> a = this->objects_[i];

    if (a->has_property("dead"))
    {
      this->objects_.erase(this->objects_.begin() + i);
      continue;
    }

    if (a->has_property("reproduced"))
    {
      std::shared_ptr<moving_object> new_sheep = std::make_shared<sheep>("../media/sheep.png", this->window_surface_ptr_, std::rand() % WINDOW_WIDTH, std::rand() % WINDOW_HEIGHT);

      this->add_object(new_sheep);
      a->remove_property("reproduced");
      a->insert_property("infertile");
    }

    if (a->has_property("infertile"))
    {
      if (rand() % 10000 < 5)
      {
        a->remove_property("infertile");
      }
    }

    if (a->has_property("fleeing"))
    {
      a->set_x_vel(10);
      a->set_y_vel(10);
    }

    auto closest_object = a->find_closest_object(this->objects_);

    if (a->distance(*closest_object.get()) < TEXTURE_SIZE)
    {
      a->interact(*closest_object.get());
    }

    if (a->has_property("wolf"))
    {
      if (auto closest_prey = a->find_closest_object(this->objects_, "prey"))
      {
        a->move_towards(closest_prey->get_x_pos(), closest_prey->get_y_pos());
        a->insert_property("hunting");
      }
      if (auto closest_dog = a->find_closest_object(this->objects_, "dog"))
      {
        int dist = a->distance(*closest_dog.get());
        int dist_x = closest_dog->get_x_pos() - a->get_x_pos();
        int dist_y = closest_dog->get_y_pos() - a->get_y_pos();

        if (dist < TEXTURE_SIZE * 3)
        {
          a->move_towards(
              closest_dog->get_x_pos() > a->get_x_pos() ? 0 : WINDOW_WIDTH,
              closest_dog->get_y_pos() > a->get_y_pos() ? 0 : WINDOW_HEIGHT);
        }
      }
    }

    if (a->has_property("sheep"))
    {
      if (auto closest_predator = a->find_closest_object(this->objects_, "predator"))
      {
        if (a->distance(*closest_predator.get()) < TEXTURE_SIZE * 2)
        {
          if (!a->has_property("fleeing"))
          {
            a->insert_property("fleeing");
          }

          a->move_towards(
              closest_predator->get_x_pos() > a->get_x_pos() ? 0 : WINDOW_WIDTH,
              closest_predator->get_y_pos() > a->get_y_pos() ? 0 : WINDOW_HEIGHT);
        }
        else if (a->has_property("fleeing"))
        {
          a->remove_property("fleeing");
          a->set_x_vel(1);
          a->set_y_vel(1);
        }
      }
    }

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
    std::shared_ptr<moving_object> a_ptr = std::make_shared<sheep>("../media/sheep.png", this->window_surface_ptr_, std::rand() % (WINDOW_WIDTH - TEXTURE_SIZE), std::rand() % (WINDOW_HEIGHT - TEXTURE_SIZE));
    this->ground_.add_object(a_ptr);
  }
  for (int i = 0; i < n_wolf_; i++)
  {
    std::shared_ptr<moving_object> a_ptr = std::make_shared<wolf>("../media/wolf.png", this->window_surface_ptr_, std::rand() % (WINDOW_WIDTH - TEXTURE_SIZE), std::rand() % (WINDOW_HEIGHT - TEXTURE_SIZE), 2, 2);

    this->ground_.add_object(a_ptr);
  }

  std::shared_ptr<moving_object> player_ptr = std::make_shared<playable_character>("../media/shepherd.png", this->window_surface_ptr_, 50, 50, 10, 10);

  std::shared_ptr<moving_object> dog_ptr = std::make_shared<dog>("../media/dog.png", this->window_surface_ptr_, player_ptr, 64);

  this->ground_.add_object(player_ptr);
  this->ground_.add_object(dog_ptr);
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