EXAMPLE_MOD_DIR := $(USERMOD_DIR)

# Add all C files to SRC_USERMOD.
SRC_USERMOD += $(EXAMPLE_MOD_DIR)/aquestalk.c

# We can add our module folder to include paths if needed
# This is not actually needed in this example.
#CFLAGS_USERMOD += -I$(USERMODULES_DIR)
CFLAGS_USERMOD += -I$(EXAMPLE_MOD_DIR)
CFLAGS_USERMOD += -l$(EXAMPLE_MOD_DIR)/aquestalk-esp32/src/esp32/libaquestalk.a
CEXAMPLE_MOD_DIR := $(USERMOD_DIR)
