.PHONY: bsp
bsp: clean $(builddir)/sources.tar.gz

$(builddir)/sources.tar.gz: 
	@rm -rf $(builddir)/sources*
	@git archive --format=tar --output=$(builddir)/sources.tar master
	@tar --exclude=.git -rf $(builddir)/sources.tar build/kernel_src 
	@tar --exclude=.git -rf $(builddir)/sources.tar build/uboot_src 
	@mkdir -p $(builddir)/sources
	@tar -C $(builddir)/sources -xpf $(builddir)/sources.tar
	@tar -C $(builddir) --numeric-owner -zcpf $(builddir)/sources.tar.gz sources
	@rm -rf $(builddir)/sources $(builddir)/sources.tar

