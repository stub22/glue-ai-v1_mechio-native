#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/888527952/MotionFrameReceiver.o \
	${OBJECTDIR}/_ext/888527952/MotionFrameSender.o \
	${OBJECTDIR}/_ext/888527952/RobotProxy.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.motion.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.motion.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.motion.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/_ext/888527952/MotionFrameReceiver.o: ../../../src/motion/MotionFrameReceiver.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888527952
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888527952/MotionFrameReceiver.o ../../../src/motion/MotionFrameReceiver.cpp

${OBJECTDIR}/_ext/888527952/MotionFrameSender.o: ../../../src/motion/MotionFrameSender.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888527952
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888527952/MotionFrameSender.o ../../../src/motion/MotionFrameSender.cpp

${OBJECTDIR}/_ext/888527952/RobotProxy.o: ../../../src/motion/RobotProxy.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/888527952
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/888527952/RobotProxy.o ../../../src/motion/RobotProxy.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.motion.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
