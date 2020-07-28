#######################
#### Start of File ####
#######################
# --------------------------------------------------------------- 
# Makefile Contents: Makefile for example program
# C/C++ Compiler Used: gcc version 3.4.4
# --------------------------------------------------------------- 
# Define a name for the executable
PROJECT = vtec_util

# For 32-bit builds
#64BITCFG = 0

# Default to 64-bit build
64BITCFG = 1

# Default to no debugging
FINAL = 1

# GXD/GXS/GXT library path
GCODE_LIB_DIR = ../3plibs/datareel

# Setup OpenSSL library path if using downloaded version
#OPENSSL_LIB_DIR = ../../../openssl
#OPENSSL_INC_PATHS = $(OPENSSL_LIB_DIR)$(PATHSEP)include
#OPENSSL_DEFMACS = -DOPENSSL_THREADS -D_REENTRANT -DDSO_DLFCN -DHAVE_DLFCN_H \
#-DL_ENDIAN -DOPENSSL_BN_ASM_PART_WORDS -DOPENSSL_IA32_SSE2 \
#-DSHA1_ASM -DMD5_ASM -DRMD160_ASM -DAES_ASM 
#OPENSSL_RELEASE_FLAGS = 
#OPENSSL_DEBUG_FLAGS = 
#OPENSSL_LINKER_FLAGS =
#SSL_LIB=$(OPENSSL_LIB_DIR)$(PATHSEP)lib$(PATHSEP)libcrypto.a
#CRYPTO_LIB=$(OPENSSL_LIB_DIR)$(PATHSEP)lib$(PATHSEP)libssl.a

include linux.env

# Setup additional paths for includes and source code
APP_PATH = ..

ADD_INC_PATHS = -I../../include -I../../src 

# Compile the files and build the executable
# ===============================================================
all:    $(PROJECT)

include ../project.mak

$(PROJECT):	$(OBJECTS)
	$(CPP) $(COMPILE_FLAGS) $(OBJECTS) $(LINK_LIBRARIES) \
	$(OUTPUT) $(PROJECT) $(LINKER_FLAGS)
# ===============================================================
# Set to your installation directory
install:
	mkdir -p ../bin/linux
	cp vtec_util ../bin/linux/vtec_util

# Remove object files and the executable after running make 
# ===============================================================
clean:
	echo Removing all OBJECT files from working directory...
	rm -f *.o 

	echo Removing EXECUTABLE file from working directory...
	rm -f $(PROJECT)

	echo Removing all test LOG files from working directory...
	rm -f *.log 

	echo Removing all test OUT files from working directory...
	rm -f *.out 

	echo Removing all test EDS files from working directory...
	rm -f *.eds 

	echo Removing all test DATABASE files from working directory...
	rm -f *.gxd 

	echo Removing all test INDEX files from working directory...
	rm -f *.btx 
	rm -f *.gix

	echo Removing all test InfoHog files from working directory...
	rm -f *.ihd 
	rm -f *.ihx 

	echo Removing DEBUG file...
	rm -f *.expand
	rm -f *.sibling
	rm -f *.initvals
	rm -f *.unshare
	rm -f *.vregs
	rm -f *.into_cfglayout
	rm -f *.jump
	rm -f *.reginfo
	rm -f *.outof_cfglayout
	rm -f *.split1
	rm -f *.dfinit
	rm -f *.mode_sw
	rm -f *.asmcons
	rm -f *.subregs_of_mode_init
	rm -f *.ira
	rm -f *.subregs_of_mode_finish
	rm -f *.split2
	rm -f *.pro_and_epilogue
	rm -f *.stack
	rm -f *.alignments
	rm -f *.mach
	rm -f *.barriers
	rm -f *.eh_ranges
	rm -f *.shorten
	rm -f *.dfinish
	rm -f *.final
	rm -f *.eh

# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################
