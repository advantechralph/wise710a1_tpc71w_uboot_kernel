.PHONY: distclean
distclean: 
	@rm -rf $(builddir)

.PHONY: clean
clean: 
	@cd $(builddir) && \
		find . -mindepth 1 -maxdepth 1  -not -regex ".*_src" -exec rm -rf {} \;

