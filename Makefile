ifndef GO4SYS
GO4SYS = $(shell go4-config --go4sys)
endif

include $(GO4SYS)/Makefile.config

# Switching Debug mode on, If you want to have optimized code instead of debug option, comment the following line
DOOPTIMIZATION = false

# Enter additional libraries needed for this project
USER_LIB_DEP += $(ROOTLIBPATH)/libFFTW.$(DllSuf)

# Needed for ROOT's MathMore library
ANAL_LIB_DEP += $(ROOTLIBPATH)/libMathMore.$(DllSuf)
LDFLAGS      += $(LIBS_FULLSET) -lMathMore

## normally should be like this for every module, but can be specific

ifdef GO4PACKAGE
EXUSERSRC_DIR = trunk_new
else
EXUSERSRC_DIR = .
endif

EXUSERSRC_LINKDEF_DIR   = $(EXUSERSRC_DIR)/Core/LinkDef
EXUSERSRC_LINKDEF       = $(EXUSERSRC_LINKDEF_DIR)/Go4UserAnalysisLinkDef.$(HedSuf)
EXUSERSRC_LINKDEF_OPEN  = $(EXUSERSRC_LINKDEF_DIR)/LinkDef.Opening
EXUSERSRC_LINKDEF_CLOSE = $(EXUSERSRC_LINKDEF_DIR)/LinkDef.Closing
EXUSERSRC_LIBNAME       = $(GO4_USERLIBNAME)

## must be similar for every module

EXUSERSRC_DICT = $(EXUSERSRC_DIR)/$(DICT_PREFIX)Remi
EXUSERSRC_DH   = $(EXUSERSRC_DICT).$(HedSuf)
EXUSERSRC_DS   = $(EXUSERSRC_DICT).$(SrcSuf)
EXUSERSRC_DO   = $(EXUSERSRC_DICT).$(ObjSuf)

EXUSERSRC_H    = $(filter-out $(EXUSERSRC_DH) $(EXUSERSRC_LINKDEF), $(wildcard $(EXUSERSRC_DIR)/*.$(HedSuf) ) $(wildcard $(EXUSERSRC_DIR)/*/*.$(HedSuf) ) $(wildcard $(EXUSERSRC_DIR)/*/*/*.$(HedSuf) ) $(wildcard $(EXUSERSRC_DIR)/*/*/*/*.$(HedSuf) )  )
EXUSERSRC_S    = $(filter-out $(EXUSERSRC_DS), $(wildcard $(EXUSERSRC_DIR)/*.$(SrcSuf)) $(wildcard $(EXUSERSRC_DIR)/*/*.$(SrcSuf)) $(wildcard $(EXUSERSRC_DIR)/*/*/*.$(SrcSuf)) $(wildcard $(EXUSERSRC_DIR)/*/*/*/*.$(SrcSuf)) )
EXUSERSRC_O    = $(EXUSERSRC_S:.$(SrcSuf)=.$(ObjSuf))

EXUSERSRC_DEP  = $(EXUSERSRC_O:.$(ObjSuf)=.$(DepSuf))
EXUSERSRC_DDEP = $(EXUSERSRC_DO:.$(ObjSuf)=.$(DepSuf))

EXUSERSRC_LIB  = $(EXUSERSRC_DIR)/$(EXUSERSRC_LIBNAME).$(DllSuf)

# used in the main Makefile

EXAMPDEPENDENCS += $(EXUSERSRC_DEP) $(EXUSERSRC_DDEP)

ifdef DOPACKAGE
DISTRFILES += $(EXUSERSRC_S) $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)
#DISTRFILES += $(EXUSERSRC_DIR)/Readme.txt
#DISTRFILES += $(EXUSERSRC_DIR)/befoil50.scf $(EXUSERSRC_DIR)/tafoil50.scf
endif

##### produce list of .conf files

CONF_FILE_EXT = conf
CONF_EXPL_EXT = $(CONF_FILE_EXT).example

CONF_EXPLS = $(wildcard c*.$(CONF_EXPL_EXT))
CONF_FILES = $(CONF_EXPLS:.$(CONF_EXPL_EXT)=.$(CONF_FILE_EXT))

##### local rules #####

all:: $(EXUSERSRC_LIB) $(CONF_FILES)

$(EXUSERSRC_LIB): $(EXUSERSRC_O) $(EXUSERSRC_DO) $(ANAL_LIB_DEP) $(USER_LIB_DEP) $(EXUSERSRC_H)
	@$(MakeLibrary) $(EXUSERSRC_LIBNAME) "$(EXUSERSRC_O) $(EXUSERSRC_DO)" $(EXUSERSRC_DIR) $(EXUSERSRC_LINKDEF) "$(ANAL_LIB_DEP)" "$(USER_LIB_DEP)" $(EXUSERSRC_DS) "$(EXUSERSRC_H)"

$(EXUSERSRC_DS): $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)
	@$(ROOTCINTGO4) $(EXUSERSRC_LIB) $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF)

$(EXUSERSRC_LINKDEF): $(EXUSERSRC_H) $(EXUSERSRC_LINKDEF_OPEN) $(EXUSERSRC_LINKDEF_CLOSE)
	@echo "Generating LinkDef file $@ ..."
	@cp $(EXUSERSRC_LINKDEF_OPEN) $@
	@/bin/echo -e "$(strip $(foreach FILE,$(EXUSERSRC_H),\n#pragma link C++ defined_in \"$(FILE)\";))" >> $@
	@cat $(EXUSERSRC_LINKDEF_CLOSE) >> $@

%.$(CONF_FILE_EXT): %.$(CONF_EXPL_EXT)
	@cp -v -n $< $@

clean-docs::
	@rm -rf doc/latex/*
	@rm -rf doc/html/*

clean-bin::
	@rm -f $(EXUSERSRC_O) $(EXUSERSRC_DO) $(EXUSERSRC_LINKDEF)
	@rm -f $(EXUSERSRC_DEP) $(EXUSERSRC_DDEP) $(EXUSERSRC_DS) $(EXUSERSRC_DH)

clean:: clean-bin clean-docs
	@$(CleanLib) $(EXUSERSRC_LIBNAME) $(EXUSERSRC_DIR)

docs::
	@doxygen
	@cd doc/latex; make
	@ln -fs html/index.html  doc/
	@ln -fs latex/refman.pdf doc/

include $(GO4SYS)/Makefile.rules
