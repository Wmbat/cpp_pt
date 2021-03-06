#pragma once

#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <sstream>

class progress_keeper
{
public:
   explicit progress_keeper(size_t sample_count_in) noexcept : sample_count(sample_count_in) {}

   void update(size_t samples_done) noexcept
   {
      const double progress =
         static_cast<double>(samples_done) / static_cast<double>(sample_count) * 100;
      if (progress >= last_progress + minimum_progress)
      {
         std::cout << std::fixed << std::setprecision(2) << progress << "% (" << samples_done
                   << " / " << sample_count << ")\n"
                   << std::flush;

         last_progress = progress;
      }
   }

private:
   size_t sample_count{};
   const double minimum_progress{5.0};
   double last_progress{};
};
