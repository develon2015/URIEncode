encode: $(shell find . -regex '.*\.c')
	gcc $^ -o $@

.PHONY: install
install: encode
	cp encode /usr/local/bin

