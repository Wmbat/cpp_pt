#pragma once

#include <cpp_pt/math/details.hpp>
#include <cpp_pt/math/vec.hpp>
#include <cpp_pt/ray.hpp>

#include <cmath>

class camera
{
public:
   struct create_info
   {
      vec eye;
      vec look_at;
      vec up;
      double vertical_fov{0.0};
      double aspect_ratio{0.0};
      double aperture{0.0};
      double focus_distance{0.0};
   };

public:
   camera(const create_info& info) noexcept
   {
      const auto theta = to_radians(info.vertical_fov);
      const auto half_height = tan(theta / 2);
      const auto half_width = info.aspect_ratio * half_height;

      {
         const vec z = normalise(origin - info.look_at);
         const vec x = normalise(cross(info.up, z));
         const vec y = cross(z, x);

         axis = ortho_normal_basis(x, y, z);
      }

      lower_left_corner = origin - half_width * info.focus_distance * axis.x() -
         half_height * info.focus_distance * axis.y() - info.focus_distance * axis.z();
      horizontal = 2 * half_width * info.focus_distance * axis.x();
      vertical = 2 * half_height * info.focus_distance * axis.y();
   }

   [[nodiscard]] auto shoot_ray(double u, double v) const noexcept -> ray
   {
      const vec random_disk = lens_radius * random_in_unit_disk();
      const vec offset = axis.x() * random_disk.x() + axis.y() * random_disk.y();

      return ray(
         origin + offset, lower_left_corner + u * horizontal + v * vertical - origin - offset);
   }

private:
   const vec origin;
   vec horizontal;
   vec vertical;
   vec lower_left_corner;
   ortho_normal_basis axis;
   double lens_radius{};
};
