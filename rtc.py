import os,time

os.system("cd ../ceu-arduino && make CEU_DEFS=-DCEU_PM_MIN CEU_SRC=libraries/driver-rtc/examples/rtc_simple.ceu ARD_BOARD=uno ARD_PORT=/dev/ttyACM0")