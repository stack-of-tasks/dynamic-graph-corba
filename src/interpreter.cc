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
#include <dynamic-graph/command-setter.h>
#include <dynamic-graph/command-getter.h>

#include "dynamic-graph/corba/interpreter.hh"
#include "interpreter.impl.hh"

namespace dynamicgraph {
  namespace corba {
    Interpreter::Interpreter()
    {
      int argc = 1;
      char* argv[1] = {"undefined"};
      server_ = new Server(argc, argv, true);
    }

    void Interpreter::startCorbaServer(const std::string& contextId,
				       const std::string& contextKind,
				       const std::string& objectId,
				       const std::string& objectKind)
    {
      if (server_->startCorbaServer(contextId, contextKind,
				    objectId, objectKind) != 0) {
	throw ExceptionAbstract(ExceptionAbstract::ABSTRACT,
				"failed to start corba server.\n"
				"Are you sure the name server is running?");
      }
    }

    int Interpreter::processRequest(bool inLoop)
    {
      return server_->processRequest(inLoop);
    }
    void Interpreter::python(const std::string& inCommand, CORBA::String_out res,
                             CORBA::String_out out, CORBA::String_out err)
    {
      server_->implementation().python(inCommand.c_str(), res, out, err);
      return;
    }

    python::Interpreter& Interpreter::local()
    {
      return server_->implementation().interpreter_;
    }

  } // namespace corba
} // namespace dynamicgraph
