ACCELEROMETER = apps/Accelerometer
ADC = apps/Adc
CAMERA_CAPTURE = apps/CameraCapture
CAMERA_DISPLAY = apps/CameraDisplay
COMPASS = apps/Compass
FACE_DETECTOR = apps/FaceDetector
GPIO = apps/Gpio
GYROSCOPE = apps/Gyroscope
ROBOT_MOTION_TEST = apps/RobotMotionTest
SENSOR_COMMAND = apps/SensorCommand
SPEECH_CLIENT = apps/SpeechClient
SPEECH_PIPE_LISTENER = apps/SpeechPipeListener
TEST_SENSORS = apps/TestSensors

LIB_COMMON = libs/org.mechio.native.common
LIB_I2C_USB = libs/org.mechio.native.i2cusb
LIB_MESSAGING = libs/org.mechio.native.messaging
LIB_MOTION = libs/org.mechio.native.motion
LIB_SPEECH = libs/org.mechio.native.speech
LIB_VISION = libs/org.mechio.native.vision
LIB_VISION_CAMERA = libs/org.mechio.native.vision.camera
LIB_VISION_DISPLAY = libs/org.mechio.native.vision.display
LIB_VISION_PROC_FACE_DETECT = libs/org.mechio.native.vision.proc.facedetect

MY_APPS = $(ACCELEROMETER) $(ADC) $(CAMERA_CAPTURE) $(CAMERA_DISPLAY) $(COMPASS) $(FACE_DETECTOR) $(GPIO) $(GYROSCOPE) $(ROBOT_MOTION_TEST) $(SENSOR_COMMAND) $(SPEECH_CLIENT) $(SPEECH_PIPE_LISTENER) $(TEST_SENSORS)
MY_LIBS = $(LIB_COMMON) $(LIB_I2C_USB) $(LIB_MESSAGING) $(LIB_MOTION) $(LIB_SPEECH) $(LIB_VISION) $(LIB_VISION_CAMERA) $(LIB_VISION_DISPLAY) $(LIB_VISION_PROC_FACE_DETECT)
MY_TARGETS = $(MY_LIBS) $(MY_APPS)

.PHONY: all $(MY_TARGETS)
all: $(MY_TARGETS)

$(MY_TARGETS):
	@$(MAKE) CONF=Release -C $@;
