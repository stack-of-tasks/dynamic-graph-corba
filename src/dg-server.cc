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

#include <dynamic-graph/factory.h>
#include "dynamic-graph/corba/interpreter.hh"

int main()
{
  dynamicgraph::corba::Interpreter* interpreter = 
    static_cast<dynamicgraph::corba::Interpreter*>
    (dynamicgraph::g_factory.newEntity("CorbaInterpreter", "corbaInterpreter"));

  interpreter->startCorbaServer("dynamic_graph","test","python",
				"server");
  interpreter->processRequest(true);
}

