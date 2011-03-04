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

      void Interpreter::python(const char* inCommand, CORBA::String_out res,
                               CORBA::String_out out,CORBA::String_out  err)
      {
        std::string pyres, pyout, pyerr;
        interpreter_.python(inCommand, pyres, pyout, pyerr);
        res = pyres.c_str();
        out = pyout.c_str();
        err = pyerr.c_str();
        return;
      }
      Interpreter::~Interpreter()
      {
      }
} // namespace impl
  } // namespace corba
} // namespace  dynamicgraph
