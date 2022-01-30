#pragma once

namespace encoder
{

class Encoder
{
  public:
    Encoder (int cpr, int rate)
      : cpr_ {cpr}
      , rate_ {rate}
    {
    }

    int get_cpr() const { return cpr_; };
    int get_rate() const { return rate_; };
    long get_previous_value() const { return value_k0_; };
    long get_current_value() const { return value_k1_; };
    double get_postion() const { return position_; };
    double get_velocity() const { return velocity_; };

    void update(long const val);
    void print_values() const;
    double get_normalized_position() const;
    int get_direction() const;
    void set_initial_value(long const val);

  private:
    int cpr_;     // counts per revolution
    int rate_;    // Hz
    long value_k0_;
    long value_k1_;
    double position_;
    double velocity_;

    double filter_velocity() const;
};

} // namespace encoder
