#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/preamp.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/preamp.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/DAC.c src/EventHandler.c src/ISR_LOW.c src/InputSelect.c src/Preamp.c src/PreampController.c src/VolumeControl.c src/cs3318.c src/cs4398.c src/cs8416.c src/eeprom.c src/infrared.c src/lcd.c src/mcp23s08.c src/menu.c src/terminal.c src/ISR_HIGH.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/DAC.p1 ${OBJECTDIR}/src/EventHandler.p1 ${OBJECTDIR}/src/ISR_LOW.p1 ${OBJECTDIR}/src/InputSelect.p1 ${OBJECTDIR}/src/Preamp.p1 ${OBJECTDIR}/src/PreampController.p1 ${OBJECTDIR}/src/VolumeControl.p1 ${OBJECTDIR}/src/cs3318.p1 ${OBJECTDIR}/src/cs4398.p1 ${OBJECTDIR}/src/cs8416.p1 ${OBJECTDIR}/src/eeprom.p1 ${OBJECTDIR}/src/infrared.p1 ${OBJECTDIR}/src/lcd.p1 ${OBJECTDIR}/src/mcp23s08.p1 ${OBJECTDIR}/src/menu.p1 ${OBJECTDIR}/src/terminal.p1 ${OBJECTDIR}/src/ISR_HIGH.p1
POSSIBLE_DEPFILES=${OBJECTDIR}/src/DAC.p1.d ${OBJECTDIR}/src/EventHandler.p1.d ${OBJECTDIR}/src/ISR_LOW.p1.d ${OBJECTDIR}/src/InputSelect.p1.d ${OBJECTDIR}/src/Preamp.p1.d ${OBJECTDIR}/src/PreampController.p1.d ${OBJECTDIR}/src/VolumeControl.p1.d ${OBJECTDIR}/src/cs3318.p1.d ${OBJECTDIR}/src/cs4398.p1.d ${OBJECTDIR}/src/cs8416.p1.d ${OBJECTDIR}/src/eeprom.p1.d ${OBJECTDIR}/src/infrared.p1.d ${OBJECTDIR}/src/lcd.p1.d ${OBJECTDIR}/src/mcp23s08.p1.d ${OBJECTDIR}/src/menu.p1.d ${OBJECTDIR}/src/terminal.p1.d ${OBJECTDIR}/src/ISR_HIGH.p1.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/DAC.p1 ${OBJECTDIR}/src/EventHandler.p1 ${OBJECTDIR}/src/ISR_LOW.p1 ${OBJECTDIR}/src/InputSelect.p1 ${OBJECTDIR}/src/Preamp.p1 ${OBJECTDIR}/src/PreampController.p1 ${OBJECTDIR}/src/VolumeControl.p1 ${OBJECTDIR}/src/cs3318.p1 ${OBJECTDIR}/src/cs4398.p1 ${OBJECTDIR}/src/cs8416.p1 ${OBJECTDIR}/src/eeprom.p1 ${OBJECTDIR}/src/infrared.p1 ${OBJECTDIR}/src/lcd.p1 ${OBJECTDIR}/src/mcp23s08.p1 ${OBJECTDIR}/src/menu.p1 ${OBJECTDIR}/src/terminal.p1 ${OBJECTDIR}/src/ISR_HIGH.p1

# Source Files
SOURCEFILES=src/DAC.c src/EventHandler.c src/ISR_LOW.c src/InputSelect.c src/Preamp.c src/PreampController.c src/VolumeControl.c src/cs3318.c src/cs4398.c src/cs8416.c src/eeprom.c src/infrared.c src/lcd.c src/mcp23s08.c src/menu.c src/terminal.c src/ISR_HIGH.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/preamp.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F4620
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/DAC.p1: src/DAC.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/DAC.p1.d 
	@${RM} ${OBJECTDIR}/src/DAC.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/DAC.p1 src/DAC.c 
	@-${MV} ${OBJECTDIR}/src/DAC.d ${OBJECTDIR}/src/DAC.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/DAC.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/EventHandler.p1: src/EventHandler.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/EventHandler.p1.d 
	@${RM} ${OBJECTDIR}/src/EventHandler.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/EventHandler.p1 src/EventHandler.c 
	@-${MV} ${OBJECTDIR}/src/EventHandler.d ${OBJECTDIR}/src/EventHandler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/EventHandler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ISR_LOW.p1: src/ISR_LOW.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ISR_LOW.p1.d 
	@${RM} ${OBJECTDIR}/src/ISR_LOW.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/ISR_LOW.p1 src/ISR_LOW.c 
	@-${MV} ${OBJECTDIR}/src/ISR_LOW.d ${OBJECTDIR}/src/ISR_LOW.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ISR_LOW.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/InputSelect.p1: src/InputSelect.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/InputSelect.p1.d 
	@${RM} ${OBJECTDIR}/src/InputSelect.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/InputSelect.p1 src/InputSelect.c 
	@-${MV} ${OBJECTDIR}/src/InputSelect.d ${OBJECTDIR}/src/InputSelect.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/InputSelect.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/Preamp.p1: src/Preamp.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/Preamp.p1.d 
	@${RM} ${OBJECTDIR}/src/Preamp.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/Preamp.p1 src/Preamp.c 
	@-${MV} ${OBJECTDIR}/src/Preamp.d ${OBJECTDIR}/src/Preamp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/Preamp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/PreampController.p1: src/PreampController.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/PreampController.p1.d 
	@${RM} ${OBJECTDIR}/src/PreampController.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/PreampController.p1 src/PreampController.c 
	@-${MV} ${OBJECTDIR}/src/PreampController.d ${OBJECTDIR}/src/PreampController.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/PreampController.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/VolumeControl.p1: src/VolumeControl.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/VolumeControl.p1.d 
	@${RM} ${OBJECTDIR}/src/VolumeControl.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/VolumeControl.p1 src/VolumeControl.c 
	@-${MV} ${OBJECTDIR}/src/VolumeControl.d ${OBJECTDIR}/src/VolumeControl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/VolumeControl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/cs3318.p1: src/cs3318.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs3318.p1.d 
	@${RM} ${OBJECTDIR}/src/cs3318.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/cs3318.p1 src/cs3318.c 
	@-${MV} ${OBJECTDIR}/src/cs3318.d ${OBJECTDIR}/src/cs3318.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cs3318.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/cs4398.p1: src/cs4398.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs4398.p1.d 
	@${RM} ${OBJECTDIR}/src/cs4398.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/cs4398.p1 src/cs4398.c 
	@-${MV} ${OBJECTDIR}/src/cs4398.d ${OBJECTDIR}/src/cs4398.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cs4398.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/cs8416.p1: src/cs8416.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs8416.p1.d 
	@${RM} ${OBJECTDIR}/src/cs8416.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/cs8416.p1 src/cs8416.c 
	@-${MV} ${OBJECTDIR}/src/cs8416.d ${OBJECTDIR}/src/cs8416.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cs8416.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eeprom.p1: src/eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/eeprom.p1 src/eeprom.c 
	@-${MV} ${OBJECTDIR}/src/eeprom.d ${OBJECTDIR}/src/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/infrared.p1: src/infrared.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/infrared.p1.d 
	@${RM} ${OBJECTDIR}/src/infrared.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/infrared.p1 src/infrared.c 
	@-${MV} ${OBJECTDIR}/src/infrared.d ${OBJECTDIR}/src/infrared.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/infrared.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/lcd.p1: src/lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/lcd.p1.d 
	@${RM} ${OBJECTDIR}/src/lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/lcd.p1 src/lcd.c 
	@-${MV} ${OBJECTDIR}/src/lcd.d ${OBJECTDIR}/src/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/mcp23s08.p1: src/mcp23s08.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mcp23s08.p1.d 
	@${RM} ${OBJECTDIR}/src/mcp23s08.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/mcp23s08.p1 src/mcp23s08.c 
	@-${MV} ${OBJECTDIR}/src/mcp23s08.d ${OBJECTDIR}/src/mcp23s08.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/mcp23s08.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/menu.p1: src/menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/menu.p1.d 
	@${RM} ${OBJECTDIR}/src/menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/menu.p1 src/menu.c 
	@-${MV} ${OBJECTDIR}/src/menu.d ${OBJECTDIR}/src/menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/terminal.p1: src/terminal.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/terminal.p1.d 
	@${RM} ${OBJECTDIR}/src/terminal.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/terminal.p1 src/terminal.c 
	@-${MV} ${OBJECTDIR}/src/terminal.d ${OBJECTDIR}/src/terminal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/terminal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ISR_HIGH.p1: src/ISR_HIGH.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ISR_HIGH.p1.d 
	@${RM} ${OBJECTDIR}/src/ISR_HIGH.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1  -mdebugger=none   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/ISR_HIGH.p1 src/ISR_HIGH.c 
	@-${MV} ${OBJECTDIR}/src/ISR_HIGH.d ${OBJECTDIR}/src/ISR_HIGH.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ISR_HIGH.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
else
${OBJECTDIR}/src/DAC.p1: src/DAC.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/DAC.p1.d 
	@${RM} ${OBJECTDIR}/src/DAC.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/DAC.p1 src/DAC.c 
	@-${MV} ${OBJECTDIR}/src/DAC.d ${OBJECTDIR}/src/DAC.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/DAC.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/EventHandler.p1: src/EventHandler.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/EventHandler.p1.d 
	@${RM} ${OBJECTDIR}/src/EventHandler.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/EventHandler.p1 src/EventHandler.c 
	@-${MV} ${OBJECTDIR}/src/EventHandler.d ${OBJECTDIR}/src/EventHandler.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/EventHandler.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ISR_LOW.p1: src/ISR_LOW.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ISR_LOW.p1.d 
	@${RM} ${OBJECTDIR}/src/ISR_LOW.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/ISR_LOW.p1 src/ISR_LOW.c 
	@-${MV} ${OBJECTDIR}/src/ISR_LOW.d ${OBJECTDIR}/src/ISR_LOW.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ISR_LOW.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/InputSelect.p1: src/InputSelect.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/InputSelect.p1.d 
	@${RM} ${OBJECTDIR}/src/InputSelect.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/InputSelect.p1 src/InputSelect.c 
	@-${MV} ${OBJECTDIR}/src/InputSelect.d ${OBJECTDIR}/src/InputSelect.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/InputSelect.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/Preamp.p1: src/Preamp.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/Preamp.p1.d 
	@${RM} ${OBJECTDIR}/src/Preamp.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/Preamp.p1 src/Preamp.c 
	@-${MV} ${OBJECTDIR}/src/Preamp.d ${OBJECTDIR}/src/Preamp.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/Preamp.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/PreampController.p1: src/PreampController.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/PreampController.p1.d 
	@${RM} ${OBJECTDIR}/src/PreampController.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/PreampController.p1 src/PreampController.c 
	@-${MV} ${OBJECTDIR}/src/PreampController.d ${OBJECTDIR}/src/PreampController.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/PreampController.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/VolumeControl.p1: src/VolumeControl.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/VolumeControl.p1.d 
	@${RM} ${OBJECTDIR}/src/VolumeControl.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/VolumeControl.p1 src/VolumeControl.c 
	@-${MV} ${OBJECTDIR}/src/VolumeControl.d ${OBJECTDIR}/src/VolumeControl.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/VolumeControl.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/cs3318.p1: src/cs3318.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs3318.p1.d 
	@${RM} ${OBJECTDIR}/src/cs3318.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/cs3318.p1 src/cs3318.c 
	@-${MV} ${OBJECTDIR}/src/cs3318.d ${OBJECTDIR}/src/cs3318.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cs3318.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/cs4398.p1: src/cs4398.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs4398.p1.d 
	@${RM} ${OBJECTDIR}/src/cs4398.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/cs4398.p1 src/cs4398.c 
	@-${MV} ${OBJECTDIR}/src/cs4398.d ${OBJECTDIR}/src/cs4398.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cs4398.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/cs8416.p1: src/cs8416.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/cs8416.p1.d 
	@${RM} ${OBJECTDIR}/src/cs8416.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/cs8416.p1 src/cs8416.c 
	@-${MV} ${OBJECTDIR}/src/cs8416.d ${OBJECTDIR}/src/cs8416.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/cs8416.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/eeprom.p1: src/eeprom.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/eeprom.p1.d 
	@${RM} ${OBJECTDIR}/src/eeprom.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/eeprom.p1 src/eeprom.c 
	@-${MV} ${OBJECTDIR}/src/eeprom.d ${OBJECTDIR}/src/eeprom.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/eeprom.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/infrared.p1: src/infrared.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/infrared.p1.d 
	@${RM} ${OBJECTDIR}/src/infrared.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/infrared.p1 src/infrared.c 
	@-${MV} ${OBJECTDIR}/src/infrared.d ${OBJECTDIR}/src/infrared.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/infrared.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/lcd.p1: src/lcd.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/lcd.p1.d 
	@${RM} ${OBJECTDIR}/src/lcd.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/lcd.p1 src/lcd.c 
	@-${MV} ${OBJECTDIR}/src/lcd.d ${OBJECTDIR}/src/lcd.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/lcd.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/mcp23s08.p1: src/mcp23s08.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/mcp23s08.p1.d 
	@${RM} ${OBJECTDIR}/src/mcp23s08.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/mcp23s08.p1 src/mcp23s08.c 
	@-${MV} ${OBJECTDIR}/src/mcp23s08.d ${OBJECTDIR}/src/mcp23s08.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/mcp23s08.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/menu.p1: src/menu.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/menu.p1.d 
	@${RM} ${OBJECTDIR}/src/menu.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/menu.p1 src/menu.c 
	@-${MV} ${OBJECTDIR}/src/menu.d ${OBJECTDIR}/src/menu.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/menu.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/terminal.p1: src/terminal.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/terminal.p1.d 
	@${RM} ${OBJECTDIR}/src/terminal.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/terminal.p1 src/terminal.c 
	@-${MV} ${OBJECTDIR}/src/terminal.d ${OBJECTDIR}/src/terminal.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/terminal.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
${OBJECTDIR}/src/ISR_HIGH.p1: src/ISR_HIGH.c  nbproject/Makefile-${CND_CONF}.mk 
	@${MKDIR} "${OBJECTDIR}/src" 
	@${RM} ${OBJECTDIR}/src/ISR_HIGH.p1.d 
	@${RM} ${OBJECTDIR}/src/ISR_HIGH.p1 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -DXPRJ_default=$(CND_CONF)  -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits $(COMPARISON_BUILD)  -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     -o ${OBJECTDIR}/src/ISR_HIGH.p1 src/ISR_HIGH.c 
	@-${MV} ${OBJECTDIR}/src/ISR_HIGH.d ${OBJECTDIR}/src/ISR_HIGH.p1.d 
	@${FIXDEPS} ${OBJECTDIR}/src/ISR_HIGH.p1.d $(SILENT) -rsi ${MP_CC_DIR}../  
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/preamp.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/preamp.${IMAGE_TYPE}.map  -D__DEBUG=1  -mdebugger=none  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto        $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/preamp.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	@${RM} ${DISTDIR}/preamp.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/preamp.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/preamp.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -fno-short-double -fno-short-float -memi=wordwrite -mrom=default,-FF00-FFFF -O0 -fasmfile -maddrqual=ignore -xassembler-with-cpp -mwarn=-3 -Wa,-a -msummary=-psect,-class,+mem,-hex,-file  -ginhx32 -Wl,--data-init -mno-keep-startup -mdownload -mdefault-config-bits -std=c99 -gdwarf-3 -mstack=hybrid:auto:auto:auto     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/preamp.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}     
	
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
