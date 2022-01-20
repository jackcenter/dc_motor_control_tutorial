import RPi.GPIO as GPIO


# TODO: check pins are appropriate for each input
# TOOD: add documentation

class Motor:
    def __init__(self, in1_pin, in2_pin, pwm_pin, stby_pin, direction):
  
        self.in1_pin = in1_pin
        self.in2_pin = in2_pin
        self.pwm_pin = pwm_pin
        self.stby_pin = stby_pin
        self.direction = direction

        self.pwm_duty_cycle = 0
        self.pwm_period = 1000
        self.pwm_instance = None


    @property
    def direction(self):
        return self._direction


    @direction.setter
    def direction(self, value):
        if value not in {-1, 1}:
            raise ValueError("direction must by either 1 or -1")
        self._direction = value


    def init(self):

        print("Initializing motor.")

        # General setup
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)

        # GPIO pin setup
        GPIO.setup(self.in1_pin, GPIO.OUT)
        GPIO.setup(self.in2_pin, GPIO.OUT)
        GPIO.setup(self.pwm_pin, GPIO.OUT)
        GPIO.setup(self.stby_pin, GPIO.OUT)

        self.pwm_instance = GPIO.PWM(self.pwm_pin, self.pwm_period)


    def activate(self):

        print("Activating motor.")

        self.pwm_instance.start(0)
        GPIO.output(self.in1_pin, GPIO.LOW)
        GPIO.output(self.in2_pin, GPIO.LOW)
        GPIO.output(self.stby_pin, GPIO.HIGH)


    def deactivate(self):

        print("Deactivating motor.")

        GPIO.output(self.stby_pin, GPIO.HIGH)
        GPIO.output(self.in2_pin, GPIO.LOW)
        GPIO.output(self.in1_pin, GPIO.LOW)
        self.pwm_instance.stop()
        
    
    def drive(self, cmd):

        print("Drive command received: {}.".format(cmd))

        if not self.check_valid_drive_command(cmd):
            print("[Error]: invalid drive command passed to the motor. Drive command ignored.")
            return

        if (cmd*self.direction < 0):
            self.drive_clockwise(abs(cmd))

        elif (0 < cmd*self.direction):
            self.drive_counterclockwise(abs(cmd))
        
        else:
            self.stop()


    def stop(self):

        print("Stopping.")

        GPIO.output(self.in1_pin, GPIO.LOW)
        GPIO.output(self.in2_pin, GPIO.LOW)
        self.pwm_instance.ChangeDutyCycle(100)

    
    def brake(self):

        print("Braking.")

        GPIO.output(self.in1_pin, GPIO.HIGH)
        GPIO.output(self.in2_pin, GPIO.HIGH)
        self.pwm_instance.ChangeDutyCycle(0)


    def drive_counterclockwise(self, duty_cycle):

        print("Driving counterclockwise at {}%.".format(duty_cycle))

        if not self.check_valid_duty_cycle(duty_cycle):
            print("[Error]: invalid duty cycle passed to the motor. Drive command ignored.")
            return

        GPIO.output(self.in1_pin, GPIO.LOW)
        GPIO.output(self.in2_pin, GPIO.HIGH) 
        self.pwm_instance.ChangeDutyCycle(duty_cycle)


    def drive_clockwise(self, duty_cycle):

        print("Driving clockwise at {}%.".format(duty_cycle))

        if not self.check_valid_duty_cycle(duty_cycle):
            print("[Error]: invalid duty cycle passed to the motor. Drive command ignored.")
            return      

        GPIO.output(self.in1_pin, GPIO.HIGH)
        GPIO.output(self.in2_pin, GPIO.LOW) 
        self.pwm_instance.ChangeDutyCycle(duty_cycle)


    @staticmethod
    def check_valid_drive_command(cmd):

        result = True

        if cmd < -100 or 100 < cmd:
            print("[Warning]: a drive command of '{}' is invalid.".format(cmd))
            result = False

        return result           


    @staticmethod
    def check_valid_duty_cycle(duty_cycle):

        result = True

        if duty_cycle < 0 or 100 < duty_cycle:
            print("[Warning]: a duty cycle of '{}' is invalid.".format(duty_cycle))
            result = False

        return result

