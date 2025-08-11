.PHONY: all
all: morse

.PHONY: clean
clean:
	$(RM) morse

README: morse.1
	mandoc $< | col -b > $@
