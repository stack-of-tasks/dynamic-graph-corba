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

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/cpack.cmake)
INCLUDE(cmake/header.cmake)

SET(PROJECT_NAME dynamic-graph-corba)
SET(PROJECT_DESCRIPTION "Corba server and client for dynamic-graph")
SET(PROJECT_URL "http://github.com/jrl-umi3218/dynamic-graph-corba")

SET(CUSTOM_HEADER_DIR dynamic-graph/corba)
SET(${PROJECT_NAME}_HEADERS
  include/dynamic-graph/corba/interpreter.hh
  include/dynamic-graph/corba/api.hh
)

SET(CXX_DISABLE_WERROR 1)
SETUP_PROJECT()

# Trigger dependency to dynamic-graph-python
ADD_REQUIRED_DEPENDENCY("sot-core >= 2.5")
ADD_REQUIRED_DEPENDENCY("dynamic-graph-python >= 2.5")
ADD_REQUIRED_DEPENDENCY("hpp-template-corba >= 1.1")

# Add dependency toward dynamic-graph-corba library in pkg-config file.
PKG_CONFIG_APPEND_LIBS("dynamic-graph-corba")

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(share)

SETUP_PROJECT_FINALIZE()
SETUP_PROJECT_CPACK()
