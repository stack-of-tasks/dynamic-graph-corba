/*
  Copyright 2011, Florent Lamiraux, CNRS
  
  This file is part of dynamic-graph-corba.
  dynamic-graph-corba is free software: you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  as published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.
  
  dynamic-graph-corba is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Lesser Public License for more details.  You should have
  received a copy of the GNU Lesser General Public License along with
  dynamic-graph-corba. If not, see <http://www.gnu.org/licenses/>.
*/

#include "interpreter.impl.hh"

namespace dynamicgraph {
  namespace corba {
    namespace impl {
    
      char* Interpreter::python(const char* command)
      {
	std::string value = interpreter_.python(command);
	char* result = new char[value.length()+1];
	strcmp(result, value.c_str());
	return result;
      }

    } // namespace impl
  } // namespace corba
} // namespace  dynamicgraph
#endif //DYNAMIC_GRAPH_CORBA_INTERPRETER_IMPL_HH
