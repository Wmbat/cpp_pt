#include "details/settings.hpp"
#include "materials/dielectric.hpp"
#include "materials/diffuse.hpp"
#include "materials/diffuse_light.hpp"
#include "materials/metallic.hpp"
#include "math/details.hpp"
#include "math/vec.hpp"
#include "renderables/triangle.hpp"
#include "scene.hpp"

#include <iostream>
#include <memory>

void random_sphere_scene(const render_settings& settings);
void cornell_box_scene(const render_settings& settings);

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
   render_settings settings{};

   random_sphere_scene(settings);
   // cornell_box_scene(settings);

   return 0;
}

void random_sphere_scene(const render_settings& settings)
{
   vec eye{13, 2, 3.0};
   vec look_at{0.0, 0.0, 0.0};

   // clang-format off
   camera::create_info info
   {
      .eye = eye,
      .look_at = look_at,
      .up = {0.0, 1.0, 0.0},
      .vertical_fov = 20,
      .aspect_ratio = (double)settings.window_width / settings.window_height,
      .aperture = 0.1,
      .focus_distance = 10
   };
   // clang-format on

   camera cam{info};

   scene scene{};

   scene.set_environment_colour({135 / 256.0, 206 / 256.0, 235 / 256.0});

   for (int a = -11; a < 11; ++a)
   {
      for (int b = -11; b < 11; ++b)
      {
         const double random_value = random_double();
         vec center{a + 0.9 * random_double(), 0.2, b + 0.9 * random_double()};
         if ((center - vec{4, 0.2, 0}).length() > 0.9)
         {
            if (random_value < 0.7)
            {
               scene.add_sphere(sphere{.center = center, .radius = 0.2},
                  std::make_unique<diffuse>(colour{}, random_vec() * random_vec()));
            }
            else if (random_value < 0.9)
            {
               scene.add_sphere(sphere{.center = center, .radius = 0.2},
                  std::make_unique<metallic>(
                     colour{}, random_vec() * random_vec(), random_double(0.0, 0.5)));
            }
            else
            {
               scene.add_sphere(sphere{.center = {-1.0, 0.0, -1.0}, .radius = 0.5},
                  std::make_unique<dielectric>(colour{1.0, 1.0, 1.0}, 1.5));
            }
         }
      }
   }

   // clang-format off
   scene.add_sphere(
         sphere{ .center{ -1000.0, 1000.0, 100.0 }, .radius = 100 },
         std::make_unique<diffuse_light>(colour{ 20.0, 20.0, 20.0 })
   );

   scene.add_sphere(
         sphere{ .center = {0.0, -1000.0, 0.0}, .radius = 1000 },
         std::make_unique<diffuse>(colour{}, colour{0.5, 0.5, 0.5})
   );

   scene.add_sphere(
         sphere { .center = {0.0, 1.0, 0.0}, .radius = 1 },
         std::make_unique<diffuse>(colour{}, colour{0.4, 0.2, 0.1})
   );
   scene.add_sphere(
         sphere { .center = {-4.0, 1.0, 0.0}, .radius = 1 },
         std::make_unique<metallic>(colour{}, colour{0.7, 0.6, 0.5}, 0.0)
   );
   scene.add_sphere(
         sphere { .center = {4.0, 1.0, 0.0}, .radius = 1 },
         std::make_unique<dielectric>(colour{1.0, 1.0, 1.0}, 1.5)
   );
   // clang-format on

   auto const img = scene.render(cam, settings);

   img.write();
}

void cornell_box_scene(const render_settings& settings)
{
   vec eye{0, 2, 3};
   vec look_at{0, 0, 0};

   // clang-format off
   camera::create_info info
   {
      .eye = eye,
      .look_at = look_at,
      .up = {0.0, 1.0, 0.0},
      .vertical_fov = 90,
      .aspect_ratio = (double)settings.window_width / settings.window_height,
      .aperture = 0.0,
      .focus_distance = 10
   };
   // clang-format on

   camera cam{info};

   scene scene{};
   scene.set_environment_colour({});

   // clang-format off
   scene.add_sphere(
         sphere{ .center{ -250.0, 250.0, 0.0 }, .radius = 100 },
         std::make_unique<diffuse_light>(colour{ 10.0, 10.0, 10.0 })
   );

   scene.add_sphere(
         sphere{ .center = {0.0, -1000.0, 0.0}, .radius = 1000 },
         std::make_unique<diffuse>(colour{}, colour{0.1, 0.3, 0.6})
   );

   scene.add_sphere(
         sphere { .center = {0.0, 0.5, 0.0}, .radius = 0.5 },
         std::make_unique<diffuse>(colour{}, colour{0.6, 0.3, 0.1})
   );

   // clang-format on
   scene.add_triangle(vec(0.5, 0.5, 1), vec(0.5, 0.5, 0), vec(1.5, 0.5, 1),
      std::make_unique<diffuse>(colour{}, colour{.3, .6, .1}));

   auto const img = scene.render(cam, settings);
   img.write();
};
