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

#ifndef DYNAMIC_GRAPH_CORBA_INTERPRETER_HH
# define DYNAMIC_GRAPH_CORBA_INTERPRETER_HH

#include <hpp/corba/template/server.hh>

#include "dynamic-graph/corba/api.hh"

namespace dynamicgraph {
  namespace python {
    class Interpreter;
  }
  namespace corba {
    namespace impl {
      class Interpreter;
    } // namespace impl
    typedef hpp::corba::Server<impl::Interpreter> Server;
    class DYNAMIC_GRAPH_CORBA_DLLAPI Interpreter
    {
    public:
      /// \brief Constructor
      Interpreter();

      /// \brief Start corba server
      ///
      /// \param contextId, contextKind, objectId, objectKind identification of
      /// the server in corba name server. Can be understood as a filename in a
      /// directory: contextId.contextKind/objectId.objectKind.
      void startCorbaServer(const std::string& contextId,
			    const std::string& contextKind,
			    const std::string& objectId,
			    const std::string& objectKind);

      /// \brief Process pending Corba requests
      ///
      /// \param inLoop whether the function should return after processing
      ///        pending requests or loop and wait for forthcoming requests.
      int processRequest(bool inLoop);

      /// \brief Locally run python command in interpreter
      std::string python(const std::string& inCommand);

      /// \brief Return a reference to the local python interpreter
      python::Interpreter& local();

    private:
      Server* server_;
    };
  } // namespace corba
} // namespace dynamicgraph
#endif // DYNAMIC_GRAPH_CORBA_INTERPRETER_HH
