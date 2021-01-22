include $(PWD)/cross.mk

O=$(realpath $(PWD)/../../build/kernel_build)
K=$(realpath $(PWD)/../../build/kernel_src)

obj-m=gpio_test.o

all: 
#	@make -C $(K) M=$(PWD) O=$(O) modules
	@make -C $(O) M=$(PWD) 

clean: 
	@rm -rf *.ko *.cmd *.o *.mod.* Module.symvers modules.order .*.cmd .tmp_versions

test: 
	@echo $(PWD)
