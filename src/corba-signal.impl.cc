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
#include <sot/core/matrix-homogeneous.hh>
using dynamicgraph::sot::MatrixHomogeneous;
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
	    SignalBase<int>& sigA =
	      PoolStorage::getInstance()->getSignal (sigName);
	    Signal<dynamicgraph::Vector,int>& signal =
	      dynamic_cast<Signal<dynamicgraph::Vector, int>&> (sigA);

	    const dynamicgraph::Vector& data = signal.accessCopy ();
	    dynamicGraph::DoubleSeq_var resCorba (new dynamicGraph::DoubleSeq);
	    resCorba->length (data.size ());
	    for (int i = 0; i  <data.size (); ++i)
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
      void
      CorbaSignal::readMatrix (const char* signalNameCorba,
			       dynamicGraph::SeqOfDoubleSeq_out value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	std::string buffer (signalNameCorba);
	std::istringstream sigName (buffer);

	try
	  {
	    SignalBase<int>& sigA =
	      PoolStorage::getInstance()->getSignal (sigName);
	    Signal<dynamicgraph::Matrix,int>& signal =
	      dynamic_cast<Signal<dynamicgraph::Matrix, int>&> (sigA);

	    const dynamicgraph::Matrix& data = signal.accessCopy ();
            std::cout << data;
	    dynamicGraph::SeqOfDoubleSeq_var
              resCorba (new dynamicGraph::SeqOfDoubleSeq);

	    resCorba->length (data.rows());
	    for (int i = 0; i  <data.rows() ; ++i)
              {
                resCorba[i].length(data.cols());
                for (int j = 0; j  <data.cols (); ++j)
                  resCorba[i][j]=data(i,j);
              }
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
		       <<" is not of type Matrix." << std::endl;
	  }
	catch( ... )
	  { dgDEBUG(1) << "Signal "<<buffer
		       <<" is not define of not of type Matrix." << std::endl; }

	dgDEBUGOUT(15);
      }
      void
      CorbaSignal::readMatrixHomogeneous (const char* signalNameCorba,
			       dynamicGraph::SeqOfDoubleSeq_out value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	std::string buffer (signalNameCorba);
	std::istringstream sigName (buffer);

	try
	  {
	    SignalBase<int>& sigA =
	      PoolStorage::getInstance()->getSignal (sigName);
	    Signal<MatrixHomogeneous,int>& signal =
	      dynamic_cast<Signal<MatrixHomogeneous, int>&> (sigA);

	    const MatrixHomogeneous& data = signal.accessCopy ();
            std::cout << data;
	    dynamicGraph::SeqOfDoubleSeq_var
              resCorba (new dynamicGraph::SeqOfDoubleSeq);

	    resCorba->length (data.rows());
	    for (int i = 0; i  <data.rows() ; ++i)
              {
                resCorba[i].length(data.cols());
                for (int j = 0; j  <data.cols (); ++j)
                  resCorba[i][j]=data(i,j);
              }
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
		       <<" is not of type Matrix." << std::endl;
	  }
	catch( ... )
	  { dgDEBUG(1) << "Signal "<<buffer
		       <<" is not define of not of type Matrix." << std::endl; }

	dgDEBUGOUT(15);
      }


      ::CORBA::Long
      CorbaSignal::createOutputVectorSignal(const char* signalNameCorba)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return 0;

	std::string signalName(signalNameCorba);

	// Does the signal exist?
	typedef dg::SignalTimeDependent< dynamicgraph::Vector,int > sig_t;
	std::vector<sig_t*>::iterator its =
	  std::find_if(entity_->vectorSOUT.begin(), entity_->vectorSOUT.end(),
		       boost::bind(std::equal_to<std::string>(),
				   boost::bind(&sig_t::getName, _1),
				   boost::ref(signalName))
		       );

	// If the signal already exists, return it.
	if( its!=entity_->vectorSOUT.end() ) {
	  ::CORBA::Long i =
	    (::CORBA::Long)std::distance(entity_->vectorSOUT.begin(), its);
	  return i;
	}

	dgDEBUG(15) << "Create output signal: <" << signalName <<">."<< std::endl;
	dg::SignalTimeDependent< dynamicgraph::Vector,int > * newSignal
	  = new dg::SignalTimeDependent< dynamicgraph::Vector,int >
	  ( signalName );
	newSignal->setDependencyType( TimeDependency<int>::BOOL_DEPENDENT );

	entity_->vectorSOUT.push_back (newSignal);
	entity_->signalRegistration (*newSignal);

	dgDEBUGOUT(15);
	/* Return the rank of the new signal in the std::vector. */
	return( (::CORBA::Long)entity_->vectorSOUT.size()-1 );
      }

      ::CORBA::Long CorbaSignal::
      createInputVectorSignal( const char * signalNameCorba )
	throw(CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return 0;

	std::string signalName(signalNameCorba);

	// Does the signal exist?
	typedef dg::SignalPtr< dynamicgraph::Vector,int > sig_t;
	std::vector<sig_t*>::iterator its =
	  std::find_if(entity_->vectorSIN.begin(), entity_->vectorSIN.end(),
		       boost::bind(std::equal_to<std::string>(),
				   boost::bind(&sig_t::getName, _1),
				   boost::ref(signalName))
		       );

	// If the signal already exists, return it.
	if( its!=entity_->vectorSIN.end() ) {
	  ::CORBA::Long i =
	    (::CORBA::Long) std::distance(entity_->vectorSIN.begin(), its);
	  return i;
	}

	dgDEBUG(15) << "Create input signal: <" << signalName <<">."<< std::endl;
	dg::SignalPtr< dynamicgraph::Vector,int > * newSignal
	  = new dg::SignalPtr< dynamicgraph::Vector,int >
	  ( NULL,signalName );

	entity_->vectorSIN.push_back( newSignal );
	entity_->signalRegistration( *newSignal );

	dataStack EmptyStack;
	entity_->vectorSINStored.push_back(EmptyStack);

	dgDEBUGOUT(15);

	entity_->vectorName2Rank[signalName] = entity_->vectorSIN.size()-1;
	/* Return the rank of the new signal in the std::vector. */
	return( (::CORBA::Long)entity_->vectorSIN.size()-1 );
      }

      void
      CorbaSignal::readInputVectorSignal (::CORBA::Long signalRankCorba,
					  dynamicGraph::DoubleSeq_out value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	int signalRank = signalRankCorba;
	dg::SignalPtr<dynamicgraph::Vector,int>& signal = *entity_->vectorSIN[signalRank];

	const dynamicgraph::Vector& data = signal.accessCopy();
	dynamicGraph::DoubleSeq_var resCorba( new dynamicGraph::DoubleSeq );
	CorbaServer::size_type lsizeOfSTD = 0;
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
	    resCorba->length( (::CORBA::ULong)((data.size()+1)*lsizeOfSTD) );

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
	    SignalPtr<dynamicgraph::Vector,int>& signal = *entity_->vectorSIN[signalRank];

	    const dynamicgraph::Vector& data = signal.accessCopy();
	    dynamicGraph::DoubleSeq  resCorba = aSDS[liRank];

	    resCorba.length( data.size() );
	    for( unsigned int i=0;i<data.size();++i )
	      { resCorba[i]=data(i); }

	  }
	values=aSDS._retn();

	dgDEBUGOUT(15);
      }

      void
      CorbaSignal::writeOutputVectorSignal (::CORBA::Long signalRankCorba,
					    const dynamicGraph::DoubleSeq& value)
	throw (CORBA::SystemException)
      {
	dgDEBUGIN(15);

	if (!entity_)
	  return;

	int signalRank = signalRankCorba;

	Signal<dynamicgraph::Vector,int>& signal = *entity_->vectorSOUT[signalRank];

	dynamicgraph::Vector data( value.length() );
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
