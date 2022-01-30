
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
    velocity_ = filter_velocity();
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

  double Encoder::filter_velocity() const
  {
    static const double alpha = 0.8;
    double vel_k1 = 2 * M_PI *(value_k1_ - value_k0_) / cpr_ * rate_;
    double val_filtered = alpha * vel_k1 + (1 - alpha) * velocity_;
    return val_filtered;
  }

  int Encoder::get_direction() const
  {
    int result;

    if (value_k1_ - value_k0_ > 0)
    {
      result = 1;
    } else if (value_k1_ == value_k0_)
    {
      result = 0;
    } else
    {
      result = -1;
    }

  return result;
  }

  void Encoder::set_initial_value(long const val)
  {
    value_k0_ = val;
    value_k1_ = val;
  }
} // namespace encoder
