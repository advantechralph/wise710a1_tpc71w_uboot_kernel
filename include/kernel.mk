
.PHONY: build_kernel
build_kernel: compile_kernel install_kernel_modules

.PHONY: compile_kernel
compile_kernel: fetch_kernel $(builddir)/.compile_kernel

$(builddir)/.compile_kernel: $(builddir)/.compile_kernel_config \
                            $(builddir)/.compile_kernel_module_prepare \
                            $(builddir)/.compile_kernel_zImage \
                            $(builddir)/.compile_kernel_modules \
                            $(builddir)/.compile_kernel_dtb

$(builddir)/.compile_kernel_config: 
	@cd $(kernelsrc)/arch/arm/configs && cp -a $(kernelconfig) imx_v7_adv_defconfig
	@make -C $(kernelsrc) O=$(kernelbuild) imx_v7_adv_defconfig
	@touch $@

$(builddir)/.compile_kernel_module_prepare: 
	@make -C $(kernelsrc) O=$(kernelbuild) modules_prepare
	@touch $@

$(builddir)/.compile_kernel_zImage: 
	@make -C $(kernelsrc) O=$(kernelbuild) zImage
	@ln -sf $(kernelbuild)/arch/arm/boot/zImage $(builddir)/zImage

$(builddir)/.compile_kernel_modules: 
	@make -C $(kernelsrc) O=$(kernelbuild) modules

$(builddir)/.compile_kernel_dtb: 
	@make -C $(kernelsrc) O=$(kernelbuild) $(modelname).dtb
	@ln -sf $(kernelbuild)/arch/arm/boot/dts/$(modelname).dtb $(builddir)/$(modelname).dtb

.PHONY: install_kernel_modules
install_kernel_modules: $(builddir)/.install_kernel_modules

$(builddir)/.install_kernel_modules: 
	@mkdir -p $(rootfs)
	@make -C $(kernelsrc) O=$(kernelbuild) INSTALL_MOD_PATH=$(rootfs) modules_install

