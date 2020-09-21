CC_PATH=/opt/fsl-imx-x11/4.1.15-2.0.0/sysroots/x86_64-pokysdk-linux/usr
CROSS_COMPILE=arm-poky-linux-gnueabi-
CC=$(CROSS_COMPILE)gcc
ARCH=arm
PATH:=$(CC_PATH)/bin:$(CC_PATH)/bin/arm-poky-linux-gnueabi:$(PATH)
SHELL=/bin/bash
SYSROOT=/opt/fsl-imx-x11/4.1.15-2.0.0/sysroots/cortexa9hf-neon-poky-linux-gnueabi
CFLAGS=-march=armv7-a -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a9 --sysroot=$(SYSROOT)
KCFLAGS=--sysroot=$(SYSROOT)

export CC_PATH ARCH CROSS_COMPILE PATH CC SHELL KCFLAGS

