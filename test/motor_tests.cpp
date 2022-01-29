#include "motor_interface.h"
#include <gtest/gtest.h>
#include <pigpio.h>

// TODO: add checks for status_
// TODO: figure out how to stop this from connecting to hardware

TEST(TestMotor, drive_commands)
{
  int const in1_pin = 27;
  int const in2_pin = 22;
  int const pwm_pin = 18;
  int const stby_pin = 17;
  int const diretion = 1;

  motor::Motor test_motor = motor::Motor(in1_pin, in2_pin, pwm_pin, stby_pin, diretion);

  // should fail, and result in no change
  test_motor.drive(50);		
  ASSERT_EQ(test_motor.get_duty_cycle(), 0);
  ASSERT_EQ(test_motor.get_current_direction(), 0);

	// should fail, and result in no change
  test_motor.init();
	test_motor.drive(50);		
	ASSERT_EQ(test_motor.get_duty_cycle(), 0);
	ASSERT_EQ(test_motor.get_current_direction(), 0);

	// should fail, and result in no change
  test_motor.activate();
	test_motor.drive(-101);
	ASSERT_EQ(test_motor.get_duty_cycle(), 0);
	ASSERT_EQ(test_motor.get_current_direction(), 0);

	// should result in a clockwise running motor.
	test_motor.drive(-100);	
	ASSERT_EQ(test_motor.get_duty_cycle(), 100);
	ASSERT_EQ(test_motor.get_current_direction(), -1);
	
	// should result in a stopped motor.
	test_motor.drive(0);						
	ASSERT_EQ(test_motor.get_duty_cycle(), 0);
	ASSERT_EQ(test_motor.get_current_direction(), 0);

	// should result in a counterclockwise running motor.
	test_motor.drive(100);					// should succeed
	ASSERT_EQ(test_motor.get_duty_cycle(), 100);
	ASSERT_EQ(test_motor.get_current_direction(), 1);

	// should fail, and result in no change
	test_motor.drive(101);					
	ASSERT_EQ(test_motor.get_duty_cycle(), 100);
	ASSERT_EQ(test_motor.get_current_direction(), 1);

// TODO: can remove this once simulated hardware is used
  gpioTerminate();    
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
