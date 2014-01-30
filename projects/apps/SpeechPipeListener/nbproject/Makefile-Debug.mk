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
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=-Wl,-rpath,../../libs/org.mechio.native.messaging/dist/Release/GNU-Linux-x86 -L../../libs/org.mechio.native.messaging/dist/Release/GNU-Linux-x86 -lorg.mechio.native.messaging -Wl,-rpath,../../libs/org.mechio.native.speech/dist/Debug/GNU-Linux-x86 -L../../libs/org.mechio.native.speech/dist/Debug/GNU-Linux-x86 -lorg.mechio.native.speech -Wl,-rpath,../../libs/org.mechio.native.common/dist/Release/GNU-Linux-x86 -L../../libs/org.mechio.native.common/dist/Release/GNU-Linux-x86 -lorg.mechio.native.common -lboost_thread -lavrocpp -lboost_system

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener: ../../libs/org.mechio.native.messaging/dist/Release/GNU-Linux-x86/liborg.mechio.native.messaging.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener: ../../libs/org.mechio.native.speech/dist/Debug/GNU-Linux-x86/liborg.mechio.native.speech.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener: ../../libs/org.mechio.native.common/dist/Release/GNU-Linux-x86/liborg.mechio.native.common.so

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../../libs/org.mechio.native.messaging && ${MAKE}  -f Makefile CONF=Release
	cd ../../libs/org.mechio.native.speech && ${MAKE}  -f Makefile CONF=Debug
	cd ../../libs/org.mechio.native.common && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/speechpipelistener

# Subprojects
.clean-subprojects:
	cd ../../libs/org.mechio.native.messaging && ${MAKE}  -f Makefile CONF=Release clean
	cd ../../libs/org.mechio.native.speech && ${MAKE}  -f Makefile CONF=Debug clean
	cd ../../libs/org.mechio.native.common && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
