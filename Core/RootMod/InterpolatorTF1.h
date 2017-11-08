#ifndef INTERPOLATORTF1_H
#define INTERPOLATORTF1_H

#include "Math/Interpolator.h"
#include "TString.h"
#include "TF1.h"
#include "Math/WrappedTF1.h"

/*
For the inheritance from ROOT::Math::Interpolator to work
$(ROOTLIBPATH)/libMathMore.so
has to be added to USER_LIB_DEP or ANAL_LIB_DEP in the Makefile.
As well as
$(LIBS_FULLSET) -lMathMore
to LDFLAGS.
*/

class InterpolatorTF1 : public ROOT::Math::Interpolator
{
	public:
		InterpolatorTF1(unsigned int ndata, ROOT::Math::Interpolation::Type type) :
			ROOT::Math::Interpolator(ndata, type) {};

		Double_t Eval (Double_t * x, Double_t * p) const
		{
			return this->ROOT::Math::Interpolator::Eval(*x);
		}

		TF1 GetTF1(Double_t xmin, Double_t xmax, TString name="interpolated")
		{
			f = TF1(name, this, &InterpolatorTF1::Eval, xmin, xmax, 0);
			return f;
		}

		ROOT::Math::WrappedTF1 GetWrappedTF1(Double_t xmin, Double_t xmax, TString name="interpolated")
		{
			f = GetTF1(xmin, xmax, name);
			return ROOT::Math::WrappedTF1(f);
		}

	private:
		TF1 f;

	ClassDef(InterpolatorTF1, 1);
};

#endif

