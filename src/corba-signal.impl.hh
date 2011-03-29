// Copyright 2011, Thomas Moulard, JRL, CNRS/AIST.
//
// This file is part of dynamic-graph-corba.
// dynamic-graph-corba is free software: you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation, either version 3 of
// the License, or (at your option) any later version.
//
// dynamic-graph-corba is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// dynamic-graph-corba. If not, see <http://www.gnu.org/licenses/>.

#ifndef DYNAMIC_GRAPH_CORBA_CORBA_SIGNAL_IMPL_HH
# define DYNAMIC_GRAPH_CORBA_CORBA_SIGNAL_IMPL_HH
# include <omniORB4/CORBA.h>

# include <jrl/mal/boost.hh>
# include <dynamic-graph/entity.h>
# include <dynamic-graph/signal-ptr.h>
# include <dynamic-graph/signal-time-dependent.h>

# include <hpp/corba/template/server.hh>

# include "corba-signal.hh"


namespace ml = maal::boost;

namespace dg = ::dynamicgraph;

class CorbaServer;

namespace dynamicgraph
{
  namespace corba
  {
    namespace impl
    {
      class CorbaSignal : public virtual POA_dynamicGraph::CorbaSignal
      {
      public:
	typedef std::map<std::string,dynamicGraph::NotifyCallback_var> CallbackList;

	CorbaSignal ();
	virtual ~CorbaSignal ();

	void setEntity (CorbaServer* entity)
	{
	  entity_ = entity;
	}

	/* --- CORBA implementation --- */
	void readVector (const char * cmd,
			 dynamicGraph::DoubleSeq_out value)
	  throw (CORBA::SystemException);

	long int createOutputVectorSignal (const char * signalNameCorba)
	  throw (CORBA::SystemException);
	long int createInputVectorSignal (const char * signalNameCorba)
	  throw (CORBA::SystemException);

	void readInputVectorSignal (long int signalRank,
				    dynamicGraph::DoubleSeq_out value)
	  throw (CORBA::SystemException);
	void readSeqInputVectorSignal (const dynamicGraph::SeqOfRank& signalRanks,
				       dynamicGraph::SeqOfDoubleSeq_out values)
	  throw (CORBA::SystemException);
	void writeOutputVectorSignal (long int signalRank,
				      const dynamicGraph::DoubleSeq& value)
	  throw (CORBA::SystemException);

	/* Callback registration */
	void registerClient (dynamicGraph::NotifyCallback_ptr client,
			     const char * clientName)
	  throw (CORBA::SystemException);
	void unregisterClient (const char * clientName)
	  throw (CORBA::SystemException);

      private:
	CorbaServer* entity_;
      };

    } // namespace impl
  } // namespace corba
} // namespace  dynamicgraph

#endif //DYNAMIC_GRAPH_CORBA_CORBA_SIGNAL_IMPL_HH
