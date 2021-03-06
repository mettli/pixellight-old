#*********************************************************#
#*  File: CMakeLists.txt                                 *
#*
#*  Copyright (C) 2002-2013 The PixelLight Team (http://www.pixellight.org/)
#*
#*  This file is part of PixelLight.
#*
#*  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
#*  and associated documentation files (the "Software"), to deal in the Software without
#*  restriction, including without limitation the rights to use, copy, modify, merge, publish,
#*  distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
#*  Software is furnished to do so, subject to the following conditions:
#*
#*  The above copyright notice and this permission notice shall be included in all copies or
#*  substantial portions of the Software.
#*
#*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
#*  BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#*  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
#*  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#*  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#*********************************************************#


##################################################
## Project
##################################################
define_project(PL3dsMaxSceneExport_2011)

##################################################
## Prerequisites
##################################################

# Create 3dsMax directory
file(MAKE_DIRECTORY ${PL_BIN_DIR}/Tools/3dsMax)

##################################################
## Source files
##################################################
add_sources(
	../PL3dsMaxSceneExport_2008/src/PL3dsMaxSceneExport.cpp
	../PL3dsMaxSceneExport_2008/src/PLLog.cpp
	../PL3dsMaxSceneExport_2008/src/PLScene.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneCamera.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneCell.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneContainer.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneEnumProc.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneExportDialogs.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneExportOptions.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneHelper.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneLight.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMaterial.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMaterialFX.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMaterialColor.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMaterialStandard.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMaterialBakeShell.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMaterialSRShaderLighting.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneMesh.cpp
	../PL3dsMaxSceneExport_2008/src/PLScenePLMesh.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneNode.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneObject.cpp
	../PL3dsMaxSceneExport_2008/src/PLScenePortal.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneAntiPortal.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneCellPortal.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneSpline.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneTexture.cpp
	../PL3dsMaxSceneExport_2008/src/PLSceneUnknown.cpp
	../PL3dsMaxSceneExport_2008/src/PLTextFile.cpp
	../PL3dsMaxSceneExport_2008/src/PLToolbar.cpp
	../PL3dsMaxSceneExport_2008/src/PLTools.cpp
)
if(WIN32)
	##################################################
	## Win32
	##################################################
	add_sources(
		../PL3dsMaxSceneExport_2008/src/../PL3dsMaxSceneExport.rc
	)
endif()

##################################################
## Include directories
##################################################
add_include_directories(
	${CMAKE_BINARY_DIR}
	../PL3dsMaxSceneExport_2008/include/
	${CMAKE_SOURCE_DIR}/Base/PLCore/include/
	${CMAKE_SOURCE_DIR}/Base/PLMesh/include/
	${MAX2011_INCLUDE_DIRS}
)

##################################################
## Additional libraries
##################################################
add_libs(
	PLCoreStat
	${WIN32_ADVAPI32_LIB}
	${WIN32_SHELL32_LIB}
	${WIN32_USER32_LIB}
	${WIN32_COMCTL32_LIB}
	${WIN32_COMDLG32_LIB}
	${MAX2011_LIBRARIES}
)

##################################################
## Preprocessor definitions
##################################################
add_compile_defs(
	${WIN32_COMPILE_DEFS}
	PLCORE_STATIC
	_SECURE_SCL=0
)
remove_compile_defs(
	UNICODE										# Use unicode
	_UNICODE									# Use unicode
)

##################################################
## Compiler flags
##################################################
add_compile_flags(
	${WIN32_COMPILE_FLAGS}
	/EHsc										# Exceptions
	/GR											# Enable RTTI
)
remove_compile_flags(
	/GR-										# Disable RTTI
)

##################################################
## Linker flags
##################################################
add_linker_flags(
	${WIN32_LINKER_FLAGS}
	/NODEFAULTLIB:"libcp.lib"					# Ignore libraries
	/NODEFAULTLIB:"libci.lib"					# Ignore libraries
	/NODEFAULTLIB:"msvcirt.lib"					# Ignore libraries
	/NODEFAULTLIB:"libcmt.lib"					# Ignore libraries
	/NODEFAULTLIB:"libcmtd.lib"					# Ignore libraries
	/NODEFAULTLIB:"msvcrtd.lib"					# Ignore libraries
	/NODEFAULTLIB:"LIBC.lib"					# Ignore libraries
)

##################################################
## Build
##################################################
build_library(${CMAKETOOLS_CURRENT_TARGET} SHARED)

##################################################
## Dependencies
##################################################
add_dependencies(${CMAKETOOLS_CURRENT_TARGET}	External-Max2011 PLCoreStat)
add_dependencies(Tools                       	${CMAKETOOLS_CURRENT_TARGET})

##################################################
## Post-Build
##################################################

# Dynamic library
add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKETOOLS_CURRENT_OUTPUT_DIR}/${CMAKETOOLS_CURRENT_SHARED_LIB} "${PL_BIN_DIR}/Tools/3dsMax/${CMAKETOOLS_CURRENT_TARGET}.dle"
)

# FX shader
add_custom_command(TARGET ${CMAKETOOLS_CURRENT_TARGET}
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/Bin/Tools/3dsMax/PixelLight_SRShaderLighting.fx "${PL_BIN_DIR}/Tools/3dsMax"
)

##################################################
## Install
##################################################

# Library
install(FILES ${PL_BIN_DIR}/Tools/3dsMax/${CMAKETOOLS_CURRENT_PROJECT}.dle
	DESTINATION ${PL_INSTALL_ROOT}/Tools/3dsMax			COMPONENT SDK
)

# FX shader
install(FILES ${PL_BIN_DIR}/Tools/3dsMax/PixelLight_SRShaderLighting.fx
	DESTINATION ${PL_INSTALL_ROOT}/Tools/3dsMax			COMPONENT SDK
)
