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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/609706966/AvroQpidOutputStream.o \
	${OBJECTDIR}/_ext/609706966/MessageDrain.o \
	${OBJECTDIR}/_ext/609706966/MessagingProvider.o


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
LDLIBSOPTIONS=-lavrocpp -lqpidmessaging

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.messaging.${CND_DLIB_EXT}

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.messaging.${CND_DLIB_EXT}: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.messaging.${CND_DLIB_EXT} ${OBJECTFILES} ${LDLIBSOPTIONS} -shared -fPIC

${OBJECTDIR}/_ext/609706966/AvroQpidOutputStream.o: ../../../src/messaging/AvroQpidOutputStream.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/609706966
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/609706966/AvroQpidOutputStream.o ../../../src/messaging/AvroQpidOutputStream.cpp

${OBJECTDIR}/_ext/609706966/MessageDrain.o: ../../../src/messaging/MessageDrain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/609706966
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/609706966/MessageDrain.o ../../../src/messaging/MessageDrain.cpp

${OBJECTDIR}/_ext/609706966/MessagingProvider.o: ../../../src/messaging/MessagingProvider.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/609706966
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../../include -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/609706966/MessagingProvider.o ../../../src/messaging/MessagingProvider.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/liborg.mechio.native.messaging.${CND_DLIB_EXT}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
