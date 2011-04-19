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

#include <dynamic-graph/debug.h>
#include <dynamic-graph/factory.h>
#include <dynamic-graph/pool.h>
#include "corba-signal.impl.hh"

#include "corba-server.hh"

namespace dynamicgraph
{
  namespace corba
  {
    namespace impl
    {
      CorbaSignal::CorbaSignal ()
	: POA_dynamicGraph::CorbaSignal (),
	  entity_ (0)
      {}

      CorbaSignal::~CorbaSignal ()
      {}

      void
      CorbaSignal::readVector (const char* signalNameCorba,
			       dynamicGraph::DoubleSeq_out value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	std::string buffer (signalNameCorba);
	std::istringstream sigName (buffer);

	try
	  {
	    SignalBase<int>& sigA = g_pool.getSignal (sigName);
	    Signal<ml::Vector,int>& signal =
	      dynamic_cast<Signal<ml::Vector, int>&> (sigA);

	    const ml::Vector& data = signal.accessCopy ();
	    dynamicGraph::DoubleSeq_var resCorba (new dynamicGraph::DoubleSeq);
	    resCorba->length (data.size ());
	    for (unsigned int i = 0; i  <data.size (); ++i)
	      resCorba[i]=data(i);
	    value = resCorba._retn ();
	  }
	catch (ExceptionFactory& e)
	  {
	    dgDEBUG(1) << "Catch exception " << e << std::endl;
	    dgDEBUG(1) << "Signal "<<buffer<<" does not exist." << std::endl;
	  }
	catch (std::bad_cast& e)
	  {
	    dgDEBUG(1) << "STD::bad_cast: signal "<<buffer
		       <<" is not of type Vector." << std::endl;
	  }
	catch( ... )
	  { dgDEBUG(1) << "Signal "<<buffer
		       <<" is not define of not of type Vector." << std::endl; }

	dgDEBUGOUT(15);
      }


      long int
      CorbaSignal::createOutputVectorSignal(const char* signalNameCorba)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	std::string signalName(signalNameCorba);

	// Does the signal exist?
	typedef dg::SignalTimeDependent< ml::Vector,int > sig_t;
	std::vector<sig_t*>::iterator its =
	  std::find_if(entity_->vectorSOUT.begin(), entity_->vectorSOUT.end(),
		       boost::bind(std::equal_to<std::string>(),
				   boost::bind(&sig_t::getName, _1),
				   boost::ref(signalName))
		       );

	// If the signal already exists, return it.
	if( its!=entity_->vectorSOUT.end() ) {
	  long i = std::distance(entity_->vectorSOUT.begin(), its);
	  return i;
	}

	dgDEBUG(15) << "Create output signal: <" << signalName <<">."<< std::endl;
	dg::SignalTimeDependent< ml::Vector,int > * newSignal
	  = new dg::SignalTimeDependent< ml::Vector,int >
	  ( signalName );
	newSignal->setDependencyType( TimeDependency<int>::BOOL_DEPENDENT );

	entity_->vectorSOUT.push_back (newSignal);
	entity_->signalRegistration (*newSignal);

	dgDEBUGOUT(15);
	/* Return the rank of the new signal in the std::vector. */
	return( entity_->vectorSOUT.size()-1 );
      }

      long int CorbaSignal::
      createInputVectorSignal( const char * signalNameCorba )
	throw(CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	std::string signalName(signalNameCorba);

	// Does the signal exist?
	typedef dg::SignalPtr< ml::Vector,int > sig_t;
	std::vector<sig_t*>::iterator its =
	  std::find_if(entity_->vectorSIN.begin(), entity_->vectorSIN.end(),
		       boost::bind(std::equal_to<std::string>(),
				   boost::bind(&sig_t::getName, _1),
				   boost::ref(signalName))
		       );

	// If the signal already exists, return it.
	if( its!=entity_->vectorSIN.end() ) {
	  long i = std::distance(entity_->vectorSIN.begin(), its);
	  return i;
	}

	dgDEBUG(15) << "Create input signal: <" << signalName <<">."<< std::endl;
	dg::SignalPtr< ml::Vector,int > * newSignal
	  = new dg::SignalPtr< ml::Vector,int >
	  ( NULL,signalName );

	entity_->vectorSIN.push_back( newSignal );
	entity_->signalRegistration( *newSignal );

	dataStack EmptyStack;
	entity_->vectorSINStored.push_back(EmptyStack);

	dgDEBUGOUT(15);

	entity_->vectorName2Rank[signalName] = entity_->vectorSIN.size()-1;
	/* Return the rank of the new signal in the std::vector. */
	return( entity_->vectorSIN.size()-1 );
      }

      void
      CorbaSignal::readInputVectorSignal (long int signalRankCorba,
					  dynamicGraph::DoubleSeq_out value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	int signalRank = signalRankCorba;
	dg::SignalPtr<ml::Vector,int>& signal = *entity_->vectorSIN[signalRank];

	const ml::Vector& data = signal.accessCopy();
	dynamicGraph::DoubleSeq_var resCorba( new dynamicGraph::DoubleSeq );
	unsigned int lsizeOfSTD = 0;
	dataStack &aDS = entity_->vectorSINStored[signalRank];

	if (!aDS.turnaround)
	  lsizeOfSTD=aDS.index;
	else
	  lsizeOfSTD=aDS.stackOfTsData.size();

	if (lsizeOfSTD==0)
	  {
	    resCorba->length( data.size() );
	    for( unsigned int i=0;i<data.size();++i ){ resCorba[i]=data(i); }
	  }
	else
	  {
	    resCorba->length( (data.size()+1)*lsizeOfSTD );

	    unsigned int lindex = 0;

	    for(unsigned int j=0;j<lsizeOfSTD;j++)
	      {
		resCorba[lindex++]=aDS.stackOfTsData[j].ts;
		for( unsigned int i=0;i<data.size();++i )
		  { resCorba[lindex++]=aDS.stackOfTsData[j].data[i]; }
	      }
	  }
	value=resCorba._retn();

	dgDEBUGOUT(15);
      }

      void
      CorbaSignal::readSeqInputVectorSignal
      (const dynamicGraph::SeqOfRank& signalRanks,
       dynamicGraph::SeqOfDoubleSeq_out values)
	throw(CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	dynamicGraph::SeqOfDoubleSeq_var aSDS = new dynamicGraph::SeqOfDoubleSeq;

	aSDS->length (signalRanks.length ());

	for (unsigned int liRank=0;
	     liRank < signalRanks.length();
	     ++liRank)
	  {
	    int signalRank = signalRanks[liRank] ;
	    SignalPtr<ml::Vector,int>& signal = *entity_->vectorSIN[signalRank];

	    const ml::Vector& data = signal.accessCopy();
	    dynamicGraph::DoubleSeq  resCorba = aSDS[liRank];

	    resCorba.length( data.size() );
	    for( unsigned int i=0;i<data.size();++i )
	      { resCorba[i]=data(i); }

	  }
	values=aSDS._retn();

	dgDEBUGOUT(15);
      }

      void
      CorbaSignal::writeOutputVectorSignal (long int signalRankCorba,
					    const dynamicGraph::DoubleSeq& value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	int signalRank = signalRankCorba;

	Signal<ml::Vector,int>& signal = *entity_->vectorSOUT[signalRank];

	ml::Vector data( value.length() );
	for( unsigned int i=0;i<value.length();++i ){ data(i)=value[i]; }
	signal = data;

	dgDEBUGOUT(15);
      }

      void
      CorbaSignal::registerClient (dynamicGraph::NotifyCallback_ptr client,
				   const char* clientName)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	std::string name(clientName);
	CallbackList::iterator iter=entity_->callbackList.find(name);
	if( iter!=entity_->callbackList.end() )
	  {
	    dgDEBUG(1)
	      << "Client "
	      << clientName << " already defined. Rebind." << std::endl;
	  }

	entity_->callbackList[name] = dynamicGraph::NotifyCallback::_duplicate(client);
	dgDEBUGOUT(15);
      }

      void CorbaSignal::unregisterClient(const char * clientName)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	std::string name(clientName);
	CallbackList::iterator iter=entity_->callbackList.find(name);
	if(iter == entity_->callbackList.end())
	  {
	    dgDEBUG(1)
	      << "Client " << clientName
	      << " not bound yet. Cannot unregister." << std::endl;
	  }
	entity_->callbackList.erase(name);

	dgDEBUGOUT(15);
      }

    } // namespace impl
  } // namespace corba
} // namespace  dynamicgraph
