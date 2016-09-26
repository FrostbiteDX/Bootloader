PROJECT_NAME := STM32Bootloader

export OUTPUT_FILENAME
#MAKEFILE_NAME := $(CURDIR)/$(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
MAKEFILE_NAME := $(MAKEFILE_LIST)
MAKEFILE_DIR := $(dir $(MAKEFILE_NAME) )

MK := mkdir
RM := rm -rf

#echo suspend
ifeq ("$(VERBOSE)","1")
NO_ECHO :=
else
NO_ECHO := @
endif

# Toolchain commands
CPP             := 'g++'

#source common to all targets
CPP_SOURCE_FILES += \
$(abspath main.cpp) \
$(abspath stm32loader.cpp) \
$(abspath linuxcomport.cpp) \

#includes common to all targets
INC_PATHS  = -I$(abspath .)

OBJECT_DIRECTORY = _build
LISTING_DIRECTORY = $(OBJECT_DIRECTORY)
OUTPUT_BINARY_DIRECTORY = $(OBJECT_DIRECTORY)

# Sorting removes duplicates
BUILD_DIRECTORIES := $(sort $(OBJECT_DIRECTORY) $(OUTPUT_BINARY_DIRECTORY) $(LISTING_DIRECTORY) )

LDFLAGS += -lm -lstdc++ -lsupc++

#default target - first one defined
default: clean STM32Bootloader

#building all targets
all: clean
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e cleanobj
	$(NO_ECHO)$(MAKE) -f $(MAKEFILE_NAME) -C $(MAKEFILE_DIR) -e STM32Bootloader

#target for printing all targets
help:
	@echo following targets are available:
	@echo 	STM32Bootloader

CPP_SOURCE_FILE_NAMES = $(notdir $(CPP_SOURCE_FILES))
CPP_PATHS = $(call remduplicates, $(dir $(CPP_SOURCE_FILES) ) )
CPP_OBJECTS = $(addprefix $(OBJECT_DIRECTORY)/, $(CPP_SOURCE_FILE_NAMES:.cpp=.o) )

vpath %.cpp $(CPP_PATHS)

OBJECTS = $(CPP_OBJECTS)

STM32Bootloader: OUTPUT_FILENAME := STM32Bootloader

STM32Bootloader: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).o
	$(NO_ECHO)$(CPP) $(OBJECTS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).exe

## Create build directories
$(BUILD_DIRECTORIES):
	echo $(MAKEFILE_NAME)
	$(MK) $@

# Create objects from CPP SRC files
$(OBJECT_DIRECTORY)/%.o: %.cpp
	@echo Compiling file: $(notdir $<)
	$(NO_ECHO)$(CPP) $(CPPFLAGS) $(DEFINES) $(INC_PATHS) -c -o $@ $<

# Link
$(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).o: $(BUILD_DIRECTORIES) $(OBJECTS)
	@echo Linking target: $(OUTPUT_FILENAME).o
	$(NO_ECHO)$(CPP) $(LDFLAGS) $(OBJECTS) -lm -o $(OUTPUT_BINARY_DIRECTORY)/$(OUTPUT_FILENAME).exe

clean:
	$(RM) $(BUILD_DIRECTORIES)

cleanobj:
	$(RM) $(BUILD_DIRECTORIES)/*.o
