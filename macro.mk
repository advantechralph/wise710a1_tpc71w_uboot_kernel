currdir=$(shell pwd)
builddir=$(currdir)/build
# imx6dl-wise710-a1, imx6dl-tpc71w-n10pa, imx6q-tpc71w-n21pa
# modelname=wise710a1
# modelname=wise710a1_2G
# modelname=tpc71wn10pa
modelname=tpc71wn21pa
###############################################
#              Kernel Macros                  #
###############################################
rootfs=$(builddir)/rootfs
kernelbuild=$(builddir)/kernel_build
# kernelconfig=imx_v7_adv_defconfig-wise710a2
kernelconfig=imx_v7_adv_defconfig-tpc71w
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
# ubootconfig=mx6dlwise710a1_2G_defconfig
# ubootconfig=mx6tpc71wn10pa_defconfig
ubootconfig=mx6tpc71wn21pa_defconfig

