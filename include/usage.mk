
define bar
$(shell printf "#%.0s" {1..59})
endef

define title
$(shell printf "# %-55s #" "")
$(shell printf "# %-55s #" "Compile i.MX6 Kernel & U-Boot Source Tool")
$(shell printf "# %-55s #" "")
endef

define usage_info

$(call bar)
$(call title)
$(call bar)

Compile Kernel: 

  $$ make compile_kernel

Compile Kernel and Build Modules for RootFS: 

  $$ make build_kernel

Compile U-Boot: 

  $$ make compile_uboot

Install Kernel Modules: 

  $$ make install_kernel_modules

endef

export usage_info

usage help: 
	@echo "$${usage_info}"

.DEFAULT_GOAL:=usage

default: usage;

