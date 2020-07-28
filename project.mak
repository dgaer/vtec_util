# Include file used for all makefiles

# Setup additional paths for includes and source code
include ../3plibs/datareel/env/glibdeps.mak

PROJECT_DEP = $(APP_PATH)$(PATHSEP)m_vtec.h \
$(APP_PATH)$(PATHSEP)m_vtec.cpp \
$(APP_PATH)$(PATHSEP)m_ugc.h \
$(APP_PATH)$(PATHSEP)m_ugc.cpp \
$(APP_PATH)$(PATHSEP)m_prod.h \
$(APP_PATH)$(PATHSEP)m_prod.cpp \
$(APP_PATH)$(PATHSEP)m_util.h \
$(APP_PATH)$(PATHSEP)m_util.cpp \
$(APP_PATH)$(PATHSEP)m_config.h \
$(APP_PATH)$(PATHSEP)m_config.cpp \
$(APP_PATH)$(PATHSEP)m_tables.h \
$(APP_PATH)$(PATHSEP)m_tables.cpp \
$(APP_PATH)$(PATHSEP)m_time.h \
$(APP_PATH)$(PATHSEP)m_time.cpp \
$(APP_PATH)$(PATHSEP)m_poly.h \
$(APP_PATH)$(PATHSEP)m_poly.cpp \
$(APP_PATH)$(PATHSEP)m_google.h \
$(APP_PATH)$(PATHSEP)m_google.cpp \
$(APP_PATH)$(PATHSEP)m_warning_table.h \
$(APP_PATH)$(PATHSEP)m_warning_table.cpp \
$(APP_PATH)$(PATHSEP)m_help.h \
$(APP_PATH)$(PATHSEP)m_help.cpp \
$(APP_PATH)$(PATHSEP)m_html.h \
$(APP_PATH)$(PATHSEP)m_html.cpp \
$(APP_PATH)$(PATHSEP)m_php.h \
$(APP_PATH)$(PATHSEP)m_php.cpp \
$(APP_PATH)$(PATHSEP)m_xml.h \
$(APP_PATH)$(PATHSEP)m_rss.h \
$(APP_PATH)$(PATHSEP)m_xml.cpp \
$(APP_PATH)$(PATHSEP)m_rss.cpp \
$(APP_PATH)$(PATHSEP)m_java.h \
$(APP_PATH)$(PATHSEP)m_java.cpp \
$(APP_PATH)$(PATHSEP)m_javascript.h \
$(APP_PATH)$(PATHSEP)m_javascript.cpp \
$(APP_PATH)$(PATHSEP)m_read_product.h \
$(APP_PATH)$(PATHSEP)m_read_product.cpp

# ===============================================================

# Compile the files and build the executable
# ===============================================================
all:	$(PROJECT)$(EXE_EXT)

include ../3plibs/datareel/env/glibobjs.mak

$(PROJECT)$(OBJ_EXT):	$(APP_PATH)$(PATHSEP)$(PROJECT).cpp $(PROJECT_DEP)
	$(CPP) $(COMPILE_ONLY) $(COMPILE_FLAGS) $(APP_PATH)$(PATHSEP)$(PROJECT).cpp

# Make the executable
OBJECTS = $(PROJECT)$(OBJ_EXT) gxssl$(OBJ_EXT) \
asprint$(OBJ_EXT) \
bstreei$(OBJ_EXT) \
btcache$(OBJ_EXT) \
btnode$(OBJ_EXT) \
btstack$(OBJ_EXT) \
cdate$(OBJ_EXT) \
dbasekey$(OBJ_EXT) \
dbfcache$(OBJ_EXT) \
dbugmgr$(OBJ_EXT) \
devcache$(OBJ_EXT) \
dfileb$(OBJ_EXT) \
ehandler$(OBJ_EXT) \
fstring$(OBJ_EXT) \
futils$(OBJ_EXT) \
gpersist$(OBJ_EXT) \
gthreadt$(OBJ_EXT) \
gxbtree$(OBJ_EXT) \
gxcond$(OBJ_EXT) \
gxconfig$(OBJ_EXT) \
gxcrc32$(OBJ_EXT) \
gxdatagm$(OBJ_EXT) \
gxdbase$(OBJ_EXT) \
gxderror$(OBJ_EXT) \
gxdfp64$(OBJ_EXT) \
gxdfptr$(OBJ_EXT) \
gxdlcode$(OBJ_EXT) \
gxdstats$(OBJ_EXT) \
gxfloat$(OBJ_EXT) \
gxint16$(OBJ_EXT) \
gxint32$(OBJ_EXT) \
gxint64$(OBJ_EXT) \
gxip32$(OBJ_EXT) \
gxlistb$(OBJ_EXT) \
gxmac48$(OBJ_EXT) \
gxmutex$(OBJ_EXT) \
gxrdbdef$(OBJ_EXT) \
gxrdbhdr$(OBJ_EXT) \
gxrdbms$(OBJ_EXT) \
gxrdbsql$(OBJ_EXT) \
gxsrss$(OBJ_EXT) \
gxsxml$(OBJ_EXT) \
gxscomm$(OBJ_EXT) \
gxsema$(OBJ_EXT) \
gxsftp$(OBJ_EXT) \
gxshtml$(OBJ_EXT) \
gxshttp$(OBJ_EXT) \
gxshttpc$(OBJ_EXT) \
gxsmtp$(OBJ_EXT) \
gxsocket$(OBJ_EXT) \
gxsping$(OBJ_EXT) \
gxspop3$(OBJ_EXT) \
gxstream$(OBJ_EXT) \
gxsurl$(OBJ_EXT) \
gxsutils$(OBJ_EXT) \
gxs_b64$(OBJ_EXT) \
gxtelnet$(OBJ_EXT) \
gxthread$(OBJ_EXT) \
gxuint16$(OBJ_EXT) \
gxuint32$(OBJ_EXT) \
gxuint64$(OBJ_EXT) \
htmldrv$(OBJ_EXT) \
leaktest$(OBJ_EXT) \
logfile$(OBJ_EXT) \
memblock$(OBJ_EXT) \
membuf$(OBJ_EXT) \
ostrbase$(OBJ_EXT) \
pod$(OBJ_EXT) \
pscript$(OBJ_EXT) \
scomserv$(OBJ_EXT) \
stdafx$(OBJ_EXT) \
strutil$(OBJ_EXT) \
systime$(OBJ_EXT) \
terminal$(OBJ_EXT) \
thelpers$(OBJ_EXT) \
thrapiw$(OBJ_EXT) \
thrpool$(OBJ_EXT) \
ustring$(OBJ_EXT) \
wserror$(OBJ_EXT)
# ===============================================================
