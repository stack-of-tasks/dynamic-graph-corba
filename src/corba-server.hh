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

#ifndef DYNAMIC_GRAPH_CORBA_CORBA_SERVER_HH
# define DYNAMIC_GRAPH_CORBA_CORBA_SERVER_HH
# include <dynamic-graph/entity.h>
# include <dynamic-graph/signal-ptr.h>
# include <dynamic-graph/signal-time-dependent.h>
# include <dynamic-graph/linear-algebra.h>

# include <hpp/corba/template/server.hh>

# include "corba-signal.hh"

namespace dg = ::dynamicgraph;

namespace dynamicgraph
{
  namespace corba
  {
    namespace impl
    {
      class CorbaSignal;
    }
  }
}

struct tsData
{
  double ts;
  std::vector<double> data;
};

struct dataStack
{
  dataStack ()
    : index (0),
      turnaround (false)
  {}

  void inc ()
  {
    ++index;
    if (index == stackOfTsData.size ())
      {
	index = 0;
	turnaround = true;
      }
  }

  std::vector<tsData> stackOfTsData;
  unsigned int index;
  bool turnaround;
};

typedef std::map<std::string, dynamicGraph::NotifyCallback_var> CallbackList;

class CorbaServer : public dg::Entity
{
public:
  typedef hpp::corba::Server<dg::corba::impl::CorbaSignal> corbaServer_t;
  typedef std::vector<dataStack>::size_type size_type;
  static const std::string CLASS_NAME;

  CorbaServer (const std::string& name);
  virtual ~CorbaServer ();

  virtual const std::string& getClassName () const
  {
    return CLASS_NAME;
  }

  void notifyClient (const std::string& name);
  void notifyClients ();
  void displayClientList (std::ostream& os);

protected:
  int& OneInternalStep(int& dummy, int);
  double& synchroValue (double& aValue, int time);

private:
  friend class dg::corba::impl::CorbaSignal;

  /// \brief Signal existing only to store data.
  dg::SignalTimeDependent<double, int> synchroSOUT;
  dg::SignalTimeDependent<int, int> OneInternalStepS;

  /// \brief Real state position values.
  dg::SignalPtr<dg::Vector,int> jointPositionSIN;

  /// \brief Callback registration.
  CallbackList callbackList;

  std::vector< dg::SignalTimeDependent< dg::Vector,int >* > vectorSOUT;
  std::vector< dg::SignalPtr< dg::Vector,int >* > vectorSIN;
  std::vector< dataStack > vectorSINStored;
  std::map<std::string, size_type> vectorName2Rank;

  corbaServer_t* server_;
};

#endif //! DYNAMIC_GRAPH_CORBA_CORBA_SERVER_HH
