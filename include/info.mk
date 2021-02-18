define info_str

modelname=$(modelname)
kernelconfig=$(kernelconfig)
ubootconfig=$(ubootconfig)

endef

export info_str

info: 
	@echo "$${info_str}"

.PHONY: models
models: 
	@echo $$(find models -mindepth 1 -maxdepth 1 -name "*.mk" -printf "%P\n" | sed -e "s/\.mk *$$//g")
