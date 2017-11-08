//----------------------------------------------------------------
//		GENERiC Analysis routine for reaction microscopes
//		v. 2.1
//
//		Parameter class for the momentum calculation
//----------------------------------------------------------------

#ifndef SPARMOM_H
#define SPARMOM_H

#include "TGo4Parameter.h"

/**
 * \brief Parameter class for momentum calculation and calibration
 */
class TRemiMomentumParameter : public TGo4Parameter 
{

	public:
		TRemiMomentumParameter() : TGo4Parameter("RemiMomentumParameter")	{ Init(); }
		TRemiMomentumParameter(const char* name) : TGo4Parameter(name)	{ Init(); }
		virtual ~TRemiMomentumParameter()	{}
		
		Float_t scaleX;	///< scaling factor for X-position
		Float_t scaleY;	///< scaling factor for Y-position
		Float_t scaleT;	///< scaling factor for time-of-flight
		Float_t shiftX;	///< [mm] shift in position space in x-direction
		Float_t shiftY;	///< [mm] shift in position space in y-direction
		Float_t shiftT;	///< [ns] allows for fine tuning of the time offset
		Float_t lincorshiftX; ///< [mm/ns] linear correction for x-position
		Float_t lincorshiftY; ///< [mm/ns] linear correction for y-position
		Float_t scalePx; ///< scaling factor for x momentum
		Float_t scalePy; ///< scaling factor for y momentum
		Float_t scalePz; ///< scaling factor for z momentum
		Float_t rotatePhi;	///< Rotate final momentum by azimuthal angle phi
		Float_t shiftVx; ///< [a.u.] shift in velocity space in x-direction
		Float_t shiftVy; ///< [a.u.] shift in velocity space in y-direction
		Float_t scaleEn; ///< scaling factor for the particle energy
		Float_t MagneticField;	///< [Gs] applied magnetic field. Use correct sign! 
		Float_t u; ///< [V] acceleration voltage.
		Float_t accel; ///< [m] acceleration length
		Float_t drift; ///< [m] drift length
		Float_t NewtonPo; ///< [a.u.] Momentum value to initialise Newton method
		Float_t NewtonEpsilon; ///< accuracy factor, Newton will stop when target function smaller than t*epsilon
		Int_t NewtonMaxI; ///< maximum number of Newton iterations
		
	private:
		/** \brief Setting default values */
		void Init()
		{
			lincorshiftX = 0;
			lincorshiftY = 0;
			scalePx=1.;
			scalePy=1.;
			scalePz=1.;
			rotatePhi=0.;
			scaleX=1.;
			scaleY=1.;
			scaleT = 1.;
			shiftT = 0.;
			shiftX = 0.;
			shiftY = 0.;
			shiftVx = 0.;
			shiftVy = 0.;
			scaleEn = 1.;
			MagneticField = 0.;
			NewtonPo = 2.0;
			NewtonEpsilon = 0.0001;
			NewtonMaxI= 20;
			u=20;
			accel=.11;
			drift=0.;
		}
		
	ClassDef(TRemiMomentumParameter, 1);
};

#endif //SPARPI_H






//----------------------------END OF GO4 SOURCE FILE ---------------------


