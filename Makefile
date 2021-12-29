
CURRENT_DIR = $(shell pwd)

DL_DIR = $(CURRENT_DIR)/dl
SRC_DIR = $(CURRENT_DIR)/src

# u8g2 lcd library
U8G2_VER = 2.29.11
U8G2_URL = https://github.com/olikraus/u8g2/archive/refs/tags/$(U8G2_VER).tar.gz
U8G2_FILE = u8g2-$(U8G2_VER).tar.gz
U8G2_TGZ = $(DL_DIR)/$(U8G2_FILE)
U8G2_SRC = $(SRC_DIR)/u8g2

PATCHFILES := $(sort $(wildcard patches/*.patch ))

PATCH_CMD = \
  for f in $(PATCHFILES); do\
      echo $$f ":"; \
      patch -b -p1 < $$f || exit 1; \
  done

COPY_CMD = tar cf - -C files . | tar xf - -C $(SRC_DIR)

.PHONY: all
all: .stamp_src

.PHONY: clean
clean:
	-rm -rf $(U8G2_SRC)
	-rm .stamp_src

$(U8G2_TGZ):
	mkdir -p $(DL_DIR)
	wget $(U8G2_URL) -O $(U8G2_TGZ)

.stamp_src: $(U8G2_TGZ)
	mkdir -p $(U8G2_SRC)
	tar -C $(U8G2_SRC) -zxf $(U8G2_TGZ) --strip-components 1
	touch $@
