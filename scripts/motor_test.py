from time import sleep
from motor import Motor


def main():

    in1_pin = 27
    in2_pin = 22
    pwm_pin = 18
    stby_pin = 17
    direction = 1

    print_header()

    print("Starting tests.\n")

    print("-- Test 1: activate motor --")
    test_motor = Motor(in1_pin, in2_pin, pwm_pin, stby_pin, direction)
    test_motor.init()
    test_motor.activate()
    print()
    sleep(1)

    print("-- Test 2: driving motor counterclockwise --")
    test_motor.drive(20)
    print()
    sleep(1)

    test_motor.drive(40)
    print()
    sleep(1)

    print("-- Test 3: stopping motor --")
    test_motor.stop()
    print()
    sleep(1)

    print("-- Test 4: driving motor clockwise --")
    test_motor.drive(-20)
    print()
    sleep(1)

    test_motor.drive(-40)
    print()
    sleep(1)

    print("-- Test 5: braking motor --")
    test_motor.brake()
    print()
    sleep(1)

    print("-- Test 6: passing invalid commands --")
    test_motor.drive(120)
    print()
    sleep(1)

    test_motor.drive(-120)
    print()
    sleep(1)

    print("-- Test 7: deactivate motor --")
    test_motor.deactivate()
    print()
    sleep(1)

    print("Test complete.")


def print_header():
    print()
    print("==============================================")
    print("==============================================")
    print("            Functional Motor Test:            ")
    print("              RPi 4 to TB6612FNG              ")
    print("                                              ")
    print("             Author: Jack Center              ")
    print("==============================================")
    print("==============================================")
    print()


if __name__ == "__main__":
    main()
