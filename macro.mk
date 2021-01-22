currdir=$(shell pwd)
builddir=$(currdir)/build
# modelname=imx6dl-wise710-a1
# modelname=imx6dl-tpc71w-n10pa
modelname=imx6q-tpc71w-n21pa
###############################################
#              Kernel Macros                  #
###############################################
rootfs=$(builddir)/rootfs
kernelbuild=$(builddir)/kernel_build
ifeq (modelname,imx6dl-wise710-a1)
kernelconfig=imx_v7_adv_defconfig-wise710a2
else
kernelconfig=imx_v7_adv_defconfig-tpc71w
endif
kernelgiturl=https://github.com/advantechralph/wise710a1_tpc71w_uboot_kernel.git
kernelgitbranch=kernel_src
kernelsrc=$(builddir)/kernel_src
###############################################
#              U-Boot Macros                  #
###############################################
ubootgiturl=$(kernelgiturl)
ubootgitbranch=uboot_src
ubootsrc=$(builddir)/uboot_src
ubootbuild=$(builddir)/uboot_build
ifeq ($(modelname),imx6dl-wise710-a1)
ubootconfig=mx6dlwise710a1_2G_defconfig
endif
ifeq ($(modelname),imx6dl-tpc71w-n10pa)
ubootconfig=mx6tpc71wn10pa_defconfig
endif
ifeq ($(modelname),imx6q-tpc71w-n21pa)
ubootconfig=mx6tpc71wn21pa_defconfig
endif

