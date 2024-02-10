# Change openwrt path accordingly
OPENWRT_PATH ?= ~/Documentos/charlinhos/openwrt/
export STAGING_DIR = $(OPENWRT_PATH)/staging_dir

TOOLCHAIN_PATH ?= $(STAGING_DIR)/toolchain-mipsel_24kc_gcc-11.2.0_musl
ROOTFS_PATH ?= $(STAGING_DIR)/staging_dir/target-mipsel_24kc_musl

CC := $(TOOLCHAIN_PATH)/bin/mipsel-openwrt-linux-gcc-11.2.0

C_SOURCE = $(wildcard src/*.c)
USR_INC_PATH = ./inc

LIB_DIR := $(ROOTFS_PATH)/usr/lib
INC_DIR := $(ROOTFS_PATH)/usr/include

CFLAGS :=  -Wall -Wextra -Werror -I$(INC_DIR) -I$(USR_INC_PATH) -I$(INCLUDE_PATH) -Iinclude 
LDFLAGS := -L$(LIB_DIR)

OUTPUT_DIR := ./build

OUTPUT := JIGTest
BUILD_LOG := build.log

all: $(OUTPUT_DIR) $(OUTPUT_DIR)/$(OUTPUT)

$(OUTPUT_DIR): 
	mkdir -p $(OUTPUT_DIR) 

$(OUTPUT_DIR)/$(OUTPUT): $(C_SOURCE)
	$(CC) $(CFLAGS) -o $@ $(C_SOURCE) $(LDFLAGS) 2> $(OUTPUT_DIR)/$(BUILD_LOG)

clean:
	rm -rf $(OUTPUT_DIR)

viewlog:
	cat $(OUTPUT_DIR)/$(BUILD_LOG)