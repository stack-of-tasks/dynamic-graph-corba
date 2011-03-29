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

#include <boost/bind.hpp>

#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>

#include "corba-signal.impl.hh"

#include "corba-server.hh"

CorbaServer::CorbaServer (const std::string& name)
  : Entity (name)
  ,synchroSOUT (boost::bind (&CorbaServer::synchroValue, this, _1, _2),
		OneInternalStepS,
		"CorbaServer("+name+")::output(int)::synchro")
  ,OneInternalStepS (boost::bind (&CorbaServer::OneInternalStep,this,_1,_2) ,
		     jointPositionSIN,
		     "CorbaServer("+name+")::output(int)::onestepinternal")
  ,jointPositionSIN (NULL,"sotPatternGenerator("+name+")::input(vector)::position")
  ,server_ ()
{
  dgDEBUGIN(15);
  signalRegistration(jointPositionSIN << OneInternalStepS << synchroSOUT);
  dgDEBUGOUT(15);
  
  int argc = 1;
  char* argv[1] = {"undefined"};
  server_ = new corbaServer_t (argc, argv, true);
  server_->implementation ().setEntity (this);
  server_->startCorbaServer
    ("sot", "context", "signal", "");
}

CorbaServer::~CorbaServer ()
{
  dgDEBUGIN(15);
  dgDEBUGOUT(15);
}

int&
CorbaServer::OneInternalStep(int &dummy, int)
{
  return dummy;
}


double&
CorbaServer::synchroValue (double &aValue, int time)
{
  dgDEBUGIN(15);
  dgDEBUG(15)
    << "vectorSINStored.size: "
    << vectorSINStored.size() << " " << std::endl;
  OneInternalStepS (time);
  for(unsigned int i = 0;
      i < vectorSINStored.size ();
      i++)
    {
      dataStack & aDS=vectorSINStored[i];
      int aDSsize = aDS.stackOfTsData.size();
      dgDEBUG(15) << "aDSsize: " << aDSsize << " " << i << std::endl;
      if (aDSsize!=0)
	{
	  dgDEBUG(15) << vectorSIN[i] << std::endl;
	  dg::SignalPtr<ml::Vector,int>& signal = *vectorSIN[i];
	  const ml::Vector& data = signal.accessCopy();
	  
	  dgDEBUG(15) << "aDS.index: " << aDS.index << std::endl;
	  
	  tsData & aTsData = aDS.stackOfTsData[aDS.index];
	  
	  dgDEBUG(15) << "aTsData.data.size(): " << aTsData.data.size() <<std::endl;
	  if (aTsData.data.size()!=data.size())
	    aTsData.data.resize(data.size());
	  
	  for(unsigned int j=0;j<data.size();j++)
	    aTsData.data[j] = data(j);
	  
	  aTsData.ts = time *0.005;
	  
	  dgDEBUG(15) << "aTsData.ts: " << aTsData.ts
		      << " aTsData.size():" << aTsData.data.size()
		      << std::endl;
	  aDS.inc();
	}
    }
  dgDEBUGOUT(15);
  return aValue;
}

void
CorbaServer::notifyClients ()
{
  dgDEBUGIN(5);
  for( CallbackList::iterator iter=callbackList.begin();
       callbackList.end()!=iter;++iter )
    {
      dgDEBUG(15) << "Client." <<std::endl;
      dynamicGraph::NotifyCallback_ptr listen = (iter->second);
      dgDEBUG(15) << "Client %p=" << listen<<std::endl;
      listen->notifyClient();
    }
  dgDEBUGOUT(5);
}

void
CorbaServer::notifyClient (const std::string & name)
{
  CallbackList::iterator iter=callbackList.find(name);
  if( iter==callbackList.end() )
    {
      using dynamicgraph::dgERRORFLOW;
      dgERROR << "Client <" << name << "> not registred yet." << std::endl;
      return;
    }
  iter->second->notifyClient();
}


void
CorbaServer::displayClientList (std::ostream & os)
{
  dgDEBUGIN(5);
  for( CallbackList::iterator iter=callbackList.begin();
       callbackList.end()!=iter;++iter )
    {
      os << " - " << iter->first << std::endl;
    }
  dgDEBUGOUT(5);
}

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN (CorbaServer, "CorbaServer");
