.PHONY: distclean
distclean: 
	@rm -rf $(builddir)

.PHONY: clean
clean: 
	@rm -rf $(builddir)/*_build 
	@rm -rf $(builddir)/*.dtb
	@rm -rf $(builddir)/zImage
	@rm -rf $(builddir)/rootfs
	@rm -rf $(builddir)/.compile*



