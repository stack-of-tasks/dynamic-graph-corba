# Copyright 2011, Florent Lamiraux, CNRS
#
# This file is part of dynamic-graph-corba.
# dynamic-graph-corba is free software: you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# dynamic-graph-corba is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# dynamic-graph-corba. If not, see <http://www.gnu.org/licenses/>.

include(../cmake/idl.cmake)
include(../cmake/python.cmake)

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)
GENERATE_IDL_FILE(interpreter ${CMAKE_SOURCE_DIR}/share/idl/dynamic-graph)

SET(LIBRARY_NAME ${PROJECT_NAME})
ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  interpreter.cc
  interpreter.hh
  interpreterSK.cc)

PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} dynamic-graph-python)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} hpp-template-corba)

SET_TARGET_PROPERTIES(${LIBRARY_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} dynamic-graph-python)

INSTALL(TARGETS ${LIBRARY_NAME}
  DESTINATION lib)

DYNAMIC_GRAPH_PYTHON_MODULE("corba" ${LIBRARY_NAME} wrap)
