
.PHONY: fetch_info
fetch_info: 
	@echo "kernelgiturl=$(kernelgiturl)"
	@echo "kernelgitbranch=$(kernelgitbranch)"
	@echo "ubootgiturl=$(ubootgiturl)"
	@echo "ubootgitbranch=$(ubootgitbranch)"

define fetch_git_source_cmd
  git clone $(1) -b $(2) --depth 1 $(3)
endef

define fetch_rules
.PHONY: fetch_$(1)
fetch_$(1): $(builddir) $(builddir)/.fetch_$(1)

$(builddir)/.fetch_$(1): $($(1)src)

$($(1)src): 
	@$(call fetch_git_source_cmd,$($(1)giturl),$($(1)gitbranch),$$@)
endef

$(eval $(call fetch_rules,kernel))
$(eval $(call fetch_rules,uboot))

