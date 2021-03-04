
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

  Build Kernel and U-Boot: 

    $$ make modelname=tpc71wn21pa build

  Compile Kernel: 

    $$ make  modelname=tpc71wn21pa compile_kernel

  Compile Kernel and Build Modules for RootFS: 

    $$ make  modelname=tpc71wn21pa build_kernel

  Compile U-Boot: 

    $$ make  modelname=tpc71wn21pa compile_uboot

  Install Kernel Modules: 

    $$ make  modelname=tpc71wn21pa install_kernel_modules

  Supported Model Name: 

    $(shell echo $(shell find models -mindepth 1 -maxdepth 1 -name "*.mk" -printf "%P\n" | sed -e "s/\.mk *$$//g") | sed "s/ /, /g")

  Check information: 

    ex: 
      $$ make info modelname=tpc71wn21pa

endef

export usage_info

usage help: 
	@echo "$${usage_info}"

.DEFAULT_GOAL:=usage

default: usage;

