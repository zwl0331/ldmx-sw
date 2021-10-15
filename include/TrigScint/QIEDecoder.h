#ifndef TRIGSCINT_QIEDECODER_H
#define TRIGSCINT_QIEDECODER_H

//#include <boost/archive/binary_oarchive.hpp>
#include <iostream>
#include <fstream>
//#include "TMath.h"

                                             
#include "Framework/Configure/Parameters.h"  // Needed to import parameters from configuration file
#include "Framework/Event.h"
#include "Framework/EventProcessor.h"  //Needed to declare processor
#include "TrigScint/Event/TrigScintQIEDigis.h"
#include "TrigScint/Event/QIEStream.h"

namespace trigscint {


enum TrigScintSection {
  UPSTREAM_TAGGER = 1,
  UPSTREAM_TARGET,
  DOWNSTREAM_TARGET,
  NUM_SECTIONS
};

  class QIEDecoder : public framework::Producer{
 public:
  QIEDecoder(const std::string &name, framework::Process &process)
      : Producer(name, process) {}

  /**
   * Default destructor, closes up boost archive and input stream
   */
	~QIEDecoder() = default;

	/**
	 * Configure our converter based off the configuration parameters
	 * decoded from the passed python script
	 */
	//	void configure(const framework::config::Parameters& ps);
	virtual void configure(framework::config::Parameters &ps);

  virtual void produce(framework::Event &event);

  virtual void onFileOpen();

  virtual void onFileClose();

  virtual void onProcessStart();

  virtual void onProcessEnd();

  private:

	
	/// the channel mapping
	
	
	std::string channelMapFileName_;
	std::ifstream channelMapFile_;
	std::map< int, int> channelMap_;

	// input collection and pass name 
	std::string inputCollection_;
	std::string outputCollection_;
	std::string inputPassName_;
	
	// verbosity for very specific printouts that don't play well with logger format
	bool verbose_{false};

	// number of channels in the pad 
	int nChannels_{50};

  }; //encoder

}//namespace trigscint 
                                                                                                 

#endif // TRIGSCINT_QIEDECODER_H
