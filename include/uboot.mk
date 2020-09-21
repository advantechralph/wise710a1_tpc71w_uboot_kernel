
.PHONY: compile_uboot
compile_uboot: fetch_uboot $(builddir)/.compile_uboot

$(builddir)/.compile_uboot: $(builddir)/.compile_uboot_config \
                            $(builddir)/.compile_uboot_all \
                            $(builddir)/.compile_uboot_imx

$(builddir)/.compile_uboot_config: 
	@make -C $(ubootsrc) O=$(ubootbuild) $(ubootconfig)
	@touch $@

$(builddir)/.compile_uboot_all: 
	@make -C $(ubootsrc) O=$(ubootbuild) V=1

$(builddir)/.compile_uboot_imx: 
	@make -C $(ubootsrc) O=$(ubootbuild) u-boot.imx


