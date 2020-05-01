#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# G-DOS MASTER MAKEFILE
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#
# This makefile contains nearly the entire build system for G-DOS.
# It automatically finds all sources and handles them appropriately.
# The only other makefiles in the entire project are located in the
# platform target directory. The platform target must implement:
#    "config.mk"   | platform specific variable definitions
#    "post.mk"     | platform specific make targets
#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

# Read in the build system config file
export
ifneq ("$(wildcard $(PWD)/.config)","")
include $(PWD)/.config
endif
include $(PWD)/src/platform/$(ARCH)/$(PLATFORM)/config.mk

# Error out if the required fields arent specified
ifneq ("$(MAKECMDGOALS)","clean")
ifndef ARCH
$(error "[!] ARCH is not set. Either pass it as an environment variable or use the config script.")
endif
ifndef PLATFORM
$(error "[!] PLATFORM is not set. Either pass it as an environment variable or use the config script.")
endif
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Define directories to use

BASEDIR := $(PWD)
BINDIR := $(BASEDIR)/bin
SRCDIR := $(BASEDIR)/src
OBJDIR := $(BASEDIR)/obj
INCDIR := $(BASEDIR)/include

BINARY_NAME := $(BINDIR)/cyclamen

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Append the default flags to the ones supplied by the target

CCFLAGS := $(CCFLAGS) -I $(SRCDIR)/platform/$(ARCH)/$(PLATFORM) -I $(SRCDIR)/cpu/$(ARCH)/include -I $(SRCDIR)/include
ASFLAGS := $(ASFLAGS) -I $(SRCDIR)/cpu/$(ARCH)/include
# Append the debug flag to the CFLAGS if debug was passed in the config
ifeq ("$(DEBUG)","true")
CCFLAGS := $(CCFLAGS) -g
endif

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Find sources & generate object targets

# Search source directories for any sources to build
SOURCES := $(filter-out $(SRCDIR)/cpu/% $(SRCDIR)/platform/%, $(shell find $(SRCDIR) -name '*.c')) $(shell find $(SRCDIR)/cpu/$(ARCH) -maxdepth 1 -name '*.c') $(shell find $(SRCDIR)/platform/$(ARCH)/$(PLATFORM) -maxdepth 1 -name '*.c')
SOURCES_ASM := $(filter-out $(SRCDIR)/cpu/% $(SRCDIR)/platform/%, $(shell find $(SRCDIR) -name '*.S')) $(shell find $(SRCDIR)/cpu/$(ARCH) -maxdepth 1 -name '*.S') $(shell find $(SRCDIR)/platform/$(ARCH)/$(PLATFORM) -maxdepth 1 -name '*.S')

# Create object lists from each source list
OBJECTS_ASM := $(patsubst $(BASEDIR)/%.S, $(BINDIR)/%.o, $(SOURCES_ASM))
OBJECTS := $(patsubst $(BASEDIR)/%.c, $(BINDIR)/%.o, $(SOURCES))

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Build rules

.PHONY: all
all:
	@echo "[DEP] Setting up directories"
	@mkdir -p $(BINDIR)
	@echo "[CCFLAGS] = $(CCFLAGS)"
	@$(MAKE) deps

.PHONY: deps
deps: $(OBJECTS) $(OBJECTS_ASM)
	@$(MAKE) $(BINARY_NAME)

.SECONDEXPANSION :
$(OBJECTS): $$(patsubst $$(BINDIR)%.o, $$(BASEDIR)%.c, $$@)
	@echo "[CC] -c $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.c))) -o $(shell realpath -m --relative-to=$(PWD) $(@:%.o=%.S))"
	@mkdir -p $(dir $@)
	@$(CC) $(CCFLAGS) -o $(BASEDIR)/$(notdir $(@:%.o=%.s)) $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.c))
	@mv $(BASEDIR)/$(notdir $(@:%.o=%.s)) $(dir $@)$(notdir $(@:%.o=%.S))
	@$(AS) $(ASFLAGS) -o $@ $(@:%.o=%.S)

$(OBJECTS_ASM): $$(patsubst $$(BINDIR)%.o, $$(BASEDIR)%.S, $$@)
	@echo "[AS] $(shell realpath -m --relative-to=$(PWD) $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.S))) -o $@"
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) -o $@ $(patsubst $(BINDIR)%, $(BASEDIR)%, $(@:%.o=%.S))

$(BINARY_NAME): $(OBJECTS) $(OBJECTS_ASM)
	@echo "[LD] Creating final binary"
	@$(LD) -o $@.bin $(shell find $(BINDIR) -name '*.o') $(LDFLAGS)

#=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
# Generic static targets

.PHONY: clean
clean:
	rm -rf $(OBJDIR)/*
	rm -rf $(BINDIR)/*
	rm -f $(BINARY_NAME)
