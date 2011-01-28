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

#include <dynamic-graph/entity.h>
#include <dynamic-graph/signal-ptr.h>
#include <hpp/corba/template/server.hh>

#include "dynamic-graph/corba/api.hh"

namespace dynamicgraph {
  namespace corba {
    namespace impl {
      class Interpreter;
    } // namespace impl
    typedef hpp::corba::Server<impl::Interpreter> Server;
    class DYNAMIC_GRAPH_CORBA_DLLAPI Interpreter : public Entity
    {
    public:
      /// \brief Constructor by name
      ///
      /// \param inName name of the entity as registered in the pool.
      Interpreter(const std::string& inName);

      /// \brief Start corba server
      ///
      /// \param contextId, contextKind, objectId, objectKind identification of
      /// the server in corba name server. Can be understood as a filename in a
      /// directory: contextId.contextKind/objectId.objectKind.
      void startCorbaServer(const std::string& contextId,
			    const std::string& contextKind,
			    const std::string& objectId,
			    const std::string& objectKind);

      virtual const std::string& getClassName () const {
	return CLASS_NAME;
      }

      /// \brief Process pending Corba requests
      ///
      /// \param inLoop whether the function should return after processing
      ///        pending requests or loop and wait for forthcoming requests.
      int processRequest(bool inLoop);
    private:
      static const std::string CLASS_NAME;
      Server* server_;
    };
  } // namespace corba
} // namespace dynamicgraph
#endif // DYNAMIC_GRAPH_CORBA_INTERPRETER_HH
