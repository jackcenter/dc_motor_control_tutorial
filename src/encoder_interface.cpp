
#include <cmath>
#include <stdio.h>    // printf()
#include "encoder_interface.h"


namespace encoder
{
  void Encoder::update(long const val)
  {
    // TODO: filter the velocity result
    // TODO: this assumes perfect rate holding, maybe input should include time
    value_k0_ = value_k1_;
    value_k1_ = val;
    position_ = 2 * M_PI * value_k1_ / cpr_ ;
    velocity_ = 2 * M_PI *(value_k1_ - value_k0_) / cpr_ * rate_;
  }

  void Encoder::print_values() const
  {
    double pos_n = get_normalized_position();
    pos_n *= (180 / M_PI);
    printf("Val:\t%li\tPosND:\t%f\tPos:\t%f\tVel:\t%f\n", value_k1_, pos_n, position_, velocity_);
  }

  double Encoder::get_normalized_position() const
  {
    double val = fmod(position_ + M_PI, 2 * M_PI);

    if (val < 0)
        val += 2 * M_PI;
    return val - M_PI;
  }
} // namespace encoder
