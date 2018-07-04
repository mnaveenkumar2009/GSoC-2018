import os,time

j = input()
if j==1 :
	os.system("cd ../ceu-arduino && make CEU_DEFS=-DCEU_PM_MIN CEU_SRC=libraries/driver-i2c/examples/master/master_sample.ceu ARD_BOARD=uno ARD_PORT=/dev/ttyACM0")
else :
	os.system("cd ../ceu-arduino && make CEU_DEFS=-DCEU_PM_MIN CEU_SRC=libraries/driver-i2c/examples/slave/slave_sample.ceu ARD_BOARD=mega ARD_PORT=/dev/ttyACM1")
