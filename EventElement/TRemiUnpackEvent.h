//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		This class defines the interface object between the Unpack and the Calculate step
//----------------------------------------------------------------

#ifndef TRemiEVENT_H
#define TRemiEVENT_H

#include "../Core/TRemiIncludeFunctions.h"
#include "../Core/TRemiEventElement.h"
#include "../Core/Point/TRemiDetectorHit.h"

#include "Rtypes.h"

#include <vector>


/**
 * \brief Defines the Event produced by the Unpack step
 */
class TRemiUnpackEvent : public TRemiEventElement
{
	public:
		TRemiUnpackEvent() ;
		TRemiUnpackEvent(const char* name) ;
		virtual ~TRemiUnpackEvent() ;
		/**
		* Method called by the event owner (analysis step) to clear the
		* event element.
		*/
		void Clear(Option_t *t="");  
		
		void CheckSizes(Int_t numElec, Int_t numIon);
		
		void AddIon(const TRemiDetectorHit &IonHit);
		void AddElectron(const TRemiDetectorHit &ElectronHit);
		
		std::vector<TRemiDetectorHit>	fiIonHits;	///< Stores ion detector hits
		std::vector<TRemiDetectorHit>	fiElectronHits;	///< Stores electron detector hits
		Float_t	fiDelay;	///< Delay value
		Float_t	fiIntensity;	///< Intensity value
		Int_t	fiMinibunchIndex;	///< Minibunch index in FEL pulse train
		
	private:
	
	ClassDef(TRemiUnpackEvent, 3);
};
#endif //TRemiEVENT_H




//----------------------------END OF GO4 SOURCE FILE ---------------------

