/**
 * @file EcalTrigPrimDigiProducer.h
 * @brief Class that performs emulation of the EcalTriggerPrimitives
 * @author Jeremiah Mans, University of Minnesota
 * @author Omar Moreno, SLAC National Accelerator Laboratory
 * @author Cameron Bravo, SLAC National Accelerator Laboratory
 */

#ifndef EVENTPROC_ECALTRIGPRIMDIGIPRODUCER_H_
#define EVENTPROC_ECALTRIGPRIMDIGIPRODUCER_H_INC


//----------------//
//   C++ StdLib   //
//----------------//
#include "Framework/EventProcessor.h"

namespace ldmx {

    /**
     * @class EcalRecProducer
     * @brief Performs basic ECal reconstruction
     *
     * Reconstruction is done from the EcalDigi samples.
     * Some hard-coded parameters are used for position and energy calculation.
     */
    class EcalTrigPrimDigiProducer : public Producer {
    public:
	/** 
	 * Constructor
	 */
	EcalTrigPrimDigiProducer(const std::string& name, Process& process);

	/**
	 * Grabs configure parameters from the python config file.
	 *
	 * Parameter        Default
	 * inputDigiCollName     EcalDigis
	 * inputDigiPassName     "" <-- blank means take any pass if only one collection exists
	 */
	virtual void configure(Parameters&);


	/**
	 * Produce EcalHits and put them into the event bus using the
	 * EcalDigis as input.
	 */
	virtual void produce(Event& event);
	
    };
}

      
#endif // EVENTPROC_ECALTRIGPRIMDIGIPRODUCER_H_INC
