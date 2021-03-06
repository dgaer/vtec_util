#######################
#### Start of File ####
#######################
# --------------------------------------------------------------- 
# Makefile Contents: Makefile for the WX windows program
# C/C++ Compiler Used: Microsoft Visual C/C++ 6.0 SP6
# --------------------------------------------------------------- 
# Define a name for the executable
PROJECT = vtec_util
FINAL = 1
64BITCFG = 0

# Installation directory for the application and config files
INSTALL_DIR = ..\..\bin\win32

# Setup OpenSSL library path
OPENSSL_LIB_DIR = ../../../3plibs/openssl

# GXD/GXS/GXT library path
GCODE_LIB_DIR = ../../../3plibs/datareel

!include $(OPENSSL_LIB_DIR)\env\msvc.env
!include msvc.env

# Setup additional paths for includes and source code
APP_PATH = ..

ADD_INC_PATHS = -I../../include -I../../src -I../

# Compile the files and build the executable
# ===============================================================
all:	$(PROJECT).exe

!include ../project.mak

$(PROJECT).exe:	$(OBJECTS) $(RCS)
	$(LINKER) $(LINKER_FLAGS) \
	$(OBJECTS) $(RCS) $(LINK_LIBRARIES) $(GXLINK_LIB) /OUT:$@ 
# ===============================================================

# Install the binaries to the bin directory
# ===============================================================
install:
	@echo Installing $(PROJECT) binaries to the bin directory...
	@if exist $(PROJECT).exe copy $(PROJECT).exe $(INSTALL_DIR)
# ===============================================================

# Remove OBJS, debug files, and executable after running nmake 
# ===============================================================
clean:
	@echo Removing all .SBR files from working directory...
	if exist *.sbr del *.sbr  

	@echo Removing all .VCW files from working directory...
	if exist *.vcw del *.vcw 

	@echo Removing all .PDB files from working directory...
	if exist *.pdb del *.pdb 

	@echo Removing all .WSP files from working directory...
	if exist *.wsp del *.wsp 

	@echo Removing all .BSC files from working directory...
	if exist *.bsc del *.bsc 

	@echo Removing all .SBT files from working directory...
	if exist *.sbt del *.sbt 

	@echo Removing all .ILK files from working directory...
	if exist *.ilk del *.ilk 

	@echo Removing all .IDB files from working directory...
	if exist *.idb del *.idb 

	@echo Removing all .MDP files from working directory...
	if exist *.mdp del *.mdp 

	@echo Removing all .PCH files from working directory...
	if exist *.pch del *.pch 

	@echo Removing all .NCB files from working directory...
	if exist *.ncb del *.ncb 

	@echo Removing all .RES files from working directory...
	if exist *.res del *.res 

	@echo Removing all $(OBJ_EXT) files from working directory...
	if exist *.obj del *.obj

	@echo Removing the EXECUTABLE file from working directory
	if exist $(PROJECT).exe del $(PROJECT).exe 

	@echo Removing all database files from working directory
	if exist *.ihd del *.idh
	if exist *.ihx del *.idx
	if exist *.hsd del *.hsd
	if exist *.hsx del *.hsx
	if exist *.ds1 del *.ds1
	if exist *.log del *.log
# --------------------------------------------------------------- 
#####################
#### End of File ####
#####################
