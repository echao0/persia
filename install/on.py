#import RPi.GPIO as GPIO
import time
import os

# Configure the PIN # 8
#GPIO.setmode(GPIO.BOARD)
#GPIO.setup(8, GPIO.OUT)
#GPIO.setwarnings(False)

# Blink Interval 
blink_interval = .5 #Time interval in Seconds

os.system('/etc/init.d/apache2 restartls')
os.system('/etc/init.d/mysql restart')

# Blinker Loop
while True:
# GPIO.output(8, True)
 time.sleep(blink_interval)
# GPIO.output(8, False)
# time.sleep(blink_interval)

# Release Resources
#GPIO.cleanup()
