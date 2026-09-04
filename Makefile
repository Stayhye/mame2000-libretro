DEBUG=0
DISABLE_ERROR_LOGGING = 1
#set later according to platform
IS_X86 = 0
TARGET_NAME = mame2000

CORE_DIR = .
CFLAGS += -fsigned-char

ifeq (,$(findstring msvc,$(platform)))
GCC_DEFINES := -Wno-sign-compare -Wunused -Wpointer-arith -Waggregate-return -Wshadow
endif

ifneq ($(SANITIZER),)
   CFLAGS   := -fsanitize=$(SANITIZER) $(CFLAGS)
   CXXFLAGS := -fsanitize=$(SANITIZER) $(CXXFLAGS)
   LDFLAGS  := -fsanitize=$(SANITIZER) $(LDFLAGS)
endif

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

SPACE :=
SPACE := $(SPACE) $(SPACE)
BACKSLASH :=
BACKSLASH := \$(BACKSLASH)
filter_out1 = $(filter-out $(firstword $1),$1)
filter_out2 = $(call filter_out1,$(call filter_out1,$1))
unixpath = $(subst \,/,$1)
unixcygpath = /$(subst :,,$(call unixpath,$1))

ifeq ($(platform),)
platform = unix
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   platform = osx
else ifneq ($(findstring win,$(shell uname -a)),)
   platform = win
endif
endif

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
	EXE_EXT = .exe
	system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
	system_platform = osx
ifeq ($(shell uname -p),powerpc)
	arch = ppc
else
	arch = intel
endif
else ifneq ($(findstring MINGW,$(shell uname -a)),)
	system_platform = win
endif
  
# PS2
ifeq ($(platform), ps2)
   TARGET := $(TARGET_NAME)_libretro_$(platform).a
   CC = mips64r5900el-ps2-elf-gcc
   CXX = mips64r5900el-ps2-elf-g++
   AR = mips64r5900el-ps2-elf-ar
   CFLAGS += -O3 -march=r5900 -mtune=r5900 -G0 -ffast-math -fomit-frame-pointer -DPS2 -DABGR1555 -fno-expensive-optimizations
   CXXFLAGS += -O3 -march=r5900 -mtune=r5900 -G0 -ffast-math -fomit-frame-pointer -DPS2 -DABGR1555 -fno-expensive-optimizations
   STATIC_LINKING=1
   PLATFORM_DEFINES := -DPS2 -DVIDEO_ABGR1555 -DIOAPI_NO_64

# Windows
else
   TARGET := $(TARGET_NAME)_libretro.dll
   CC ?= gcc
   SHARED := -shared -static-libgcc -static-libstdc++ -s -Wl,--version-script=link.T
   CFLAGS += -D__WIN32__ -D__WIN32_LIBRETRO__ -Wno-missing-field-initializers
   IS_X86 = 1
endif

ifeq ($(IS_X86), 1)
X86_DEFINES = -Dasm="__asm__ __volatile__"
CFLAGS +=  $(X86_DEFINES)
endif

ifeq ($(ARM), 1)
   CFLAGS += -fsigned-char
#   USE_CYCLONE = 1
#   USE_DRZ80 = 1
endif

ifeq ($(DEBUG), 1)
CFLAGS += -O0 -g
else
CFLAGS += -O2 -DNDEBUG
endif

# compiler, linker and utilities
RM = rm -f
LIBS = -lm
OBJ = obj

ifeq ($(HAVE_RZLIB),)
ZLIB_INCLUDE := -Isrc/zlib
endif

INCFLAGS :=

# include the various .mak files
include Makefile.common

DEFS += $(fpic) $(PLATFORM_DEFINES) $(ZLIB_INCLUDE) $(GCC_DEFINES) $(INCFLAGS) $(INCFLAGS_PLATFORM)

ifneq (,$(findstring msvc,$(platform)))
ifeq ($(DEBUG),1)
DEFS += -MTd
else
DEFS += -MT
endif
endif

# combine the various definitions to one
CDEFS +=  $(ENDIANNESS_DEFINES) $(DEFS) $(COREDEFS) $(CPUDEFS) $(SOUNDDEFS)

OBJECTS := $(SOURCES_C:.c=.o) $(SOURCES_ASM:.s=.o)

OBJOUT   = -o 
LINKOUT  = -o 

ifneq (,$(findstring msvc,$(platform)))
	OBJOUT = -Fo
	LINKOUT = -out:
	LD = link.exe
else
	LD = $(CC)
endif

all: $(TARGET)

$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(LD) $(SHARED) $(LDFLAGS) $(OBJECTS) $(LIBS) $(LINKOUT)$@
endif
    
%.o: %.c
	$(CC) $(CDEFS) $(CFLAGS) -c $< $(OBJOUT)$@

%.o: %.s
	$(CC) $(CDEFS) $(CFLAGS) -c $< $(OBJOUT)$@

%.o: %.S
	$(CC) $(CDEFS) $(CFLAGS) -c $< $(OBJOUT)$@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
