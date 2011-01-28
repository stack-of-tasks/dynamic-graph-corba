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
  namespace command {
    namespace interpreter {
      class StartCorbaServer : public Command
      {
      public:
	virtual ~StartCorbaServer() {}
	StartCorbaServer(corba::Interpreter& entity,
			 const std::string& docstring) :
	  Command(entity, boost::assign::list_of(Value::STRING)(Value::STRING)
		  (Value::STRING)(Value::STRING), docstring)
	{
	}

	virtual Value doExecute()
	{
	  corba::Interpreter& ci = static_cast<corba::Interpreter&>(owner());
	  std::vector<Value> values = getParameterValues();
	  std::string contextId = (std::string) values[0].value();
	  std::string contextKind = (std::string) values[1].value();
	  std::string objectId = (std::string) values[2].value();
	  std::string objectKind = (std::string) values[3].value();
	  ci.startCorbaServer(contextId, contextKind, objectId, objectKind);
	  return Value();
	}
      }; // class StartCorbaServer
    } // namespace interpreter
  } // namespace command

  namespace corba {
    DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(Interpreter, "CorbaInterpreter");

    Interpreter::Interpreter(const std::string& inName) : Entity(inName)
    {
      int argc = 1;
      char* argv[1] = {"undefined"};
      server_ = new Server(argc, argv, true);

      std::string docstring =
	"    \n"
	"    Start the corba server\n"
	"    \n"
	"      Input:\n"
	"        - 4 string (contextId, contextKind, objectId, objectKind) defining\n"
	"          the name of the server in the name server.\n"
	"          This name can be understood as a filename in a directory\n"
	"    \n";
	addCommand("startCorbaServer",
		   new command::interpreter::StartCorbaServer
		   (*this, docstring));
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
  } // namespace corba
} // namespace dynamicgraph
