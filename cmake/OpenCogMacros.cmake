#
# OpenCogMacros.cmake
#
# Provide commands to generate bindings for AtomTypes in various
# languages (C++, scheme, python, OCaml, ...)
# XXX FIXME: This file is mis-named; unfortunately, many project use it!
#
# -----------------
# Multiple macros are provided. These are:
#
# Generate C++ headers and files.
# OPENCOG_GEN_CPP_ATOMTYPES(
#        SCRIPT_FILE
#        CPP_HEADER_FILE
#        CPP_DEFINITIONS_FILE
#        CPP_INHERITANCE_FILE)
#
# Generate Scheme bindings:
# OPENCOG_GEN_SCM_ATOMTYPES(SCRIPT_FILE SCM_FILE)
#
# Generate Python bindings:
# OPENCOG_GEN_PYTHON_ATOMTYPES(SCRIPT_FILE PYTHON_FILE)
#
# -----------------
# Generate Atom Types bindings for three languages at once:
# C++, scheme and python. This is deprecated; it's best to
# use the above to make individual calls for each language.
#
# Macro example call:
# OPENCOG_ADD_ATOM_TYPES(
#        SCRIPT_FILE
#        CPP_HEADER_FILE
#        CPP_DEFINITIONS_FILE
#        CPP_INHERITANCE_FILE
#        SCM_FILE
#        PYTHON_FILE)
#

IF(EXISTS ${CMAKE_SOURCE_DIR}/cmake/OpenCogAtomTypes.cmake)
	SET(OC_CMAKE_PATH ${CMAKE_SOURCE_DIR})
ELSE()
	SET(OC_CMAKE_PATH ${ATOMSPACE_DATA_DIR})
ENDIF()
SET(OC_ATOM_TYPES_PATH ${OC_CMAKE_PATH}/cmake/OpenCogAtomTypes.cmake)

# =========================================================
# Generate C++ headers and files.
#
MACRO(OPENCOG_GEN_CPP_ATOMTYPES SCRIPT_FILE
      HEADER_FILE DEFINITIONS_FILE INHERITANCE_FILE)

	ADD_CUSTOM_COMMAND (
		COMMAND "${CMAKE_COMMAND}"
			-DBUILD_CPP="yes"
			-DOC_CMAKE_PATH=\"${OC_CMAKE_PATH}\"
			-DSCRIPT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}\"
			-DHEADER_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${HEADER_FILE}\"
			-DDEFINITIONS_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${DEFINITIONS_FILE}\"
			-DINHERITANCE_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${INHERITANCE_FILE}\"
			-P "${OC_ATOM_TYPES_PATH}"
		OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_FILE}"
		        "${CMAKE_CURRENT_BINARY_DIR}/${DEFINITIONS_FILE}"
		        "${CMAKE_CURRENT_BINARY_DIR}/${INHERITANCE_FILE}"
		DEPENDS "${SCRIPT_FILE}" "${OC_ATOM_TYPES_PATH}"
		# Set working directory of atom types generator to current binary dir.
		# It will effectively set CMAKE_BINARY_DIR, CMAKE_SOURCE_DIR,
		# CMAKE_CURRENT_BINARY_DIR and CMAKE_CURRENT_SOURCE_DIR of the
		# OpenCogAtomTypes.cmake to the CMAKE_CURRENT_BINARY_DIR
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating C++ bindings for Atom Types."
	)
ENDMACRO()

# =========================================================
# Generate Scheme bindings.
#
MACRO(OPENCOG_GEN_SCM_ATOMTYPES SCRIPT_FILE SCM_FILE)

	ADD_CUSTOM_COMMAND (
		COMMAND "${CMAKE_COMMAND}"
			-DBUILD_SCM="yes"
			-DOC_CMAKE_PATH=\"${OC_CMAKE_PATH}\"
			-DSCRIPT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}\"
			-DSCM_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${SCM_FILE}\"
			-P "${OC_ATOM_TYPES_PATH}"
		OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/${SCM_FILE}"
		DEPENDS "${SCRIPT_FILE}" "${OC_ATOM_TYPES_PATH}"
		# Set working directory of atom types generator to current binary dir.
		# It will effectively set CMAKE_BINARY_DIR, CMAKE_SOURCE_DIR,
		# CMAKE_CURRENT_BINARY_DIR and CMAKE_CURRENT_SOURCE_DIR of the
		# OpenCogAtomTypes.cmake to the CMAKE_CURRENT_BINARY_DIR
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating Scheme bindings for Atom Types."
	)
ENDMACRO()

# =========================================================
# Generate Python bindings.
#
MACRO(OPENCOG_GEN_PYTHON_ATOMTYPES SCRIPT_FILE PYTHON_FILE)

	ADD_CUSTOM_COMMAND (
		COMMAND "${CMAKE_COMMAND}"
			-DBUILD_PYTHON="yes"
			-DOC_CMAKE_PATH=\"${OC_CMAKE_PATH}\"
			-DSCRIPT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}\"
			-DPYTHON_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_FILE}\"
			-P "${OC_ATOM_TYPES_PATH}"
		OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_FILE}"
		DEPENDS "${SCRIPT_FILE}" "${OC_ATOM_TYPES_PATH}"
		# Set working directory of atom types generator to current binary dir.
		# It will effectively set CMAKE_BINARY_DIR, CMAKE_SOURCE_DIR,
		# CMAKE_CURRENT_BINARY_DIR and CMAKE_CURRENT_SOURCE_DIR of the
		# OpenCogAtomTypes.cmake to the CMAKE_CURRENT_BINARY_DIR
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating Python bindings for Atom Types."
	)
ENDMACRO()

# =========================================================
# Deprecated, do not use in new code.
#

MACRO(OPENCOG_ADD_ATOM_TYPES SCRIPT_FILE
      HEADER_FILE DEFINITIONS_FILE INHERITANCE_FILE SCM_FILE PYTHON_FILE)

# We want to be able to say just this:
#
#	OPENCOG_GEN_SCM_ATOMTYPES(${SCRIPT_FILE} ${SCM_FILE})
#	OPENCOG_GEN_PYTHON_ATOMTYPES(${SCRIPT_FILE} ${PYTHON_FILE})
#	OPENCOG_GEN_CPP_ATOMTYPES(${SCRIPT_FILE}
#		${HEADER_FILE} ${DEFINITIONS_FILE} ${INHERITANCE_FILE})
#
# but we can't, because missing dependencies in assorted projects
# would cause the scheme and python files to not be generated.
# No amount of ADD_CUSTOM_TARGET here seeems able to fix this,
# So we do the below instead.

	ADD_CUSTOM_COMMAND (
		COMMAND "${CMAKE_COMMAND}"
			-DBUILD_CSP=yes
			-DOC_CMAKE_PATH=\"${OC_CMAKE_PATH}\"
			-DSCRIPT_FILE=\"${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT_FILE}\"
			-DHEADER_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${HEADER_FILE}\"
			-DDEFINITIONS_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${DEFINITIONS_FILE}\"
			-DINHERITANCE_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${INHERITANCE_FILE}\"
			-DSCM_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${SCM_FILE}\"
			-DPYTHON_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_FILE}\"
			-P "${OC_ATOM_TYPES_PATH}"
		OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/${HEADER_FILE}"
		        "${CMAKE_CURRENT_BINARY_DIR}/${DEFINITIONS_FILE}"
		        "${CMAKE_CURRENT_BINARY_DIR}/${INHERITANCE_FILE}"
		        "${CMAKE_CURRENT_BINARY_DIR}/${SCM_FILE}"
		        "${CMAKE_CURRENT_BINARY_DIR}/${PYTHON_FILE}"
		DEPENDS "${SCRIPT_FILE}" "${OC_ATOM_TYPES_PATH}"
		# Set working directory of atom types generator to current binary dir.
		# It will effectively set CMAKE_BINARY_DIR, CMAKE_SOURCE_DIR,
		# CMAKE_CURRENT_BINARY_DIR and CMAKE_CURRENT_SOURCE_DIR of the
		# OpenCogAtomTypes.cmake to the CMAKE_CURRENT_BINARY_DIR
		WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		COMMENT "Generating opencog types"
	)
ENDMACRO()

# =========================================================
