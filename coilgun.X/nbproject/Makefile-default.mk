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
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=
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
SOURCEFILES_QUOTED_IF_SPACED=startup_samc20j16a.c system_samc20j16a.c main.c coilgun.c driver.c plib_clock.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/startup_samc20j16a.o ${OBJECTDIR}/system_samc20j16a.o ${OBJECTDIR}/main.o ${OBJECTDIR}/coilgun.o ${OBJECTDIR}/driver.o ${OBJECTDIR}/plib_clock.o
POSSIBLE_DEPFILES=${OBJECTDIR}/startup_samc20j16a.o.d ${OBJECTDIR}/system_samc20j16a.o.d ${OBJECTDIR}/main.o.d ${OBJECTDIR}/coilgun.o.d ${OBJECTDIR}/driver.o.d ${OBJECTDIR}/plib_clock.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/startup_samc20j16a.o ${OBJECTDIR}/system_samc20j16a.o ${OBJECTDIR}/main.o ${OBJECTDIR}/coilgun.o ${OBJECTDIR}/driver.o ${OBJECTDIR}/plib_clock.o

# Source Files
SOURCEFILES=startup_samc20j16a.c system_samc20j16a.c main.c coilgun.c driver.c plib_clock.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${DFP_DIR}/samc20/include"  -I "${CMSIS_DIR}/CMSIS/Core/Include"



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
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=SAMC20J16A
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
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/startup_samc20j16a.o: startup_samc20j16a.c  .generated_files/flags/default/dcd1ba65b0510132a978f4785388aafb9b30f049 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samc20j16a.o.d 
	@${RM} ${OBJECTDIR}/startup_samc20j16a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samc20j16a.o.d" -o ${OBJECTDIR}/startup_samc20j16a.o startup_samc20j16a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/system_samc20j16a.o: system_samc20j16a.c  .generated_files/flags/default/64a99dab2a49ded65415aa86657895e1dd0f08f3 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samc20j16a.o.d 
	@${RM} ${OBJECTDIR}/system_samc20j16a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samc20j16a.o.d" -o ${OBJECTDIR}/system_samc20j16a.o system_samc20j16a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/e45d480b6cbf4302b90af25893d905c048c6f4f5 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/coilgun.o: coilgun.c  .generated_files/flags/default/41e7b652d6f6d7eb551d97ea6c4ff3722cc002c7 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/coilgun.o.d 
	@${RM} ${OBJECTDIR}/coilgun.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/coilgun.o.d" -o ${OBJECTDIR}/coilgun.o coilgun.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/driver.o: driver.c  .generated_files/flags/default/d7b2c676689db201861813392a09909d1dc1988b .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/driver.o.d 
	@${RM} ${OBJECTDIR}/driver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/driver.o.d" -o ${OBJECTDIR}/driver.o driver.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/plib_clock.o: plib_clock.c  .generated_files/flags/default/64e637073edeeb273d10a3692ca1410291916bc0 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus -g -D__DEBUG  -gdwarf-2  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/plib_clock.o.d" -o ${OBJECTDIR}/plib_clock.o plib_clock.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
else
${OBJECTDIR}/startup_samc20j16a.o: startup_samc20j16a.c  .generated_files/flags/default/ae467e1409a4ca5e31a72f61b243d9f2d3f3b7ce .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/startup_samc20j16a.o.d 
	@${RM} ${OBJECTDIR}/startup_samc20j16a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/startup_samc20j16a.o.d" -o ${OBJECTDIR}/startup_samc20j16a.o startup_samc20j16a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/system_samc20j16a.o: system_samc20j16a.c  .generated_files/flags/default/229cafc6a516433199a22dd0845a93b2c9ce3ac5 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/system_samc20j16a.o.d 
	@${RM} ${OBJECTDIR}/system_samc20j16a.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/system_samc20j16a.o.d" -o ${OBJECTDIR}/system_samc20j16a.o system_samc20j16a.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/46fac9c640b68652b4792d144329d17397634387 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/main.o.d" -o ${OBJECTDIR}/main.o main.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/coilgun.o: coilgun.c  .generated_files/flags/default/861e6a5d07aff1d00af33d66331d79517d4e14de .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/coilgun.o.d 
	@${RM} ${OBJECTDIR}/coilgun.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/coilgun.o.d" -o ${OBJECTDIR}/coilgun.o coilgun.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/driver.o: driver.c  .generated_files/flags/default/3e015cb093a9adac4056dbb893f31e6ce4b95abe .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/driver.o.d 
	@${RM} ${OBJECTDIR}/driver.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/driver.o.d" -o ${OBJECTDIR}/driver.o driver.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
${OBJECTDIR}/plib_clock.o: plib_clock.c  .generated_files/flags/default/f9d4bb27845a0ac02f20f9972a5c35f19e7a2be3 .generated_files/flags/default/ac2ca9e3df0636ffa4b9d40c4622c146c19e8762
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -mcpu=cortex-m0plus  -x c -c -D__$(MP_PROCESSOR_OPTION)__  -mthumb ${PACK_COMMON_OPTIONS}  -Os -ffunction-sections -mlong-calls -Wall -MP -MMD -MF "${OBJECTDIR}/plib_clock.o.d" -o ${OBJECTDIR}/plib_clock.o plib_clock.c  -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD) 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    samc20j16a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m0plus   -gdwarf-2  -D__$(MP_PROCESSOR_OPTION)__   -T"samc20j16a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}\coilgun.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1 -Wl,--gc-sections  
	
	
	
	
	
	
else
${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   samc20j16a_flash.ld
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=cortex-m0plus  -D__$(MP_PROCESSOR_OPTION)__   -T"samc20j16a_flash.ld"  -mthumb --specs=nosys.specs -Wl,-Map="${DISTDIR}\coilgun.X.${IMAGE_TYPE}.map"  -o ${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}      -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION) -Wl,--gc-sections  
	
	${MP_CC_DIR}\\arm-none-eabi-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature "${DISTDIR}/coilgun.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/coilgun.X.${IMAGE_TYPE}.hex"
	
	
	
	
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

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
