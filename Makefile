
CURRENT_DIR = $(shell pwd)

DL_DIR = $(CURRENT_DIR)/dl
SRC_DIR = $(CURRENT_DIR)/src
U8G2_DIR = $(CURRENT_DIR)/u8g2

# u8g2 lcd library
U8G2_VER = 2.29.11
U8G2_URL = https://github.com/olikraus/u8g2/archive/refs/tags/$(U8G2_VER).tar.gz
U8G2_FILE = u8g2-$(U8G2_VER).tar.gz
U8G2_TGZ = $(DL_DIR)/$(U8G2_FILE)

PATCHFILES := $(sort $(wildcard patches/*.patch ))

PATCH_CMD = \
  for f in $(PATCHFILES); do\
      echo $$f ":"; \
      patch -b -p1 < $$f || exit 1; \
  done

.PHONY: all
all: .stamp_src
	make -f rect.mk $@

.PHONY: clean
clean:
	make -f rect.mk $@
	-rm -rf $(U8G2_DIR)
	-rm .stamp_src

$(U8G2_TGZ):
	mkdir -p $(DL_DIR)
	wget $(U8G2_URL) -O $(U8G2_TGZ)

.stamp_src: $(U8G2_TGZ)
	mkdir -p $(U8G2_DIR)
	tar -C $(U8G2_DIR) -zxf $(U8G2_TGZ) --strip-components 1
	$(PATCH_CMD)
	touch $@

