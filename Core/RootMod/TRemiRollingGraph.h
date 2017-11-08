#ifndef TREMIROLLINGGRAPH_H
#define TREMIROLLINGGRAPH_H

#include "TGraphErrors.h"
#include "Rtypes.h"

/** \brief Graphs that renew themselves iteratively to monitor a value
*
* As old points are to be deleted at a certain point, this has the appearance of a rolling counter.
*/
class TRemiRollingGraph : public TGraphErrors
{
	public :
		TRemiRollingGraph();
		TRemiRollingGraph(Int_t NumPoints, Int_t UpdateInterval = 1);
		
		virtual ~TRemiRollingGraph();
		
		virtual void	Fill (Double_t value, Double_t xerror=-1, Double_t yerror=-1);
		void	Clear ();
		
		Int_t	GetNumPoints () const	{ return mNumPoints; }	///< Returns number of displayed points
		Int_t	GetUpdateInterval () const	{ return mUpdateInterval; }	///< Returns interval of entries when a new bin is opened
		
	protected :
		void	NextBin ();
		
		Int_t	mNumPoints;	///< Nominal size of the graph, i.e. number of points to display
		Int_t	mUpdateInterval;	///< Number of Fill() functions to call before a new average is started
		Long64_t	mCounter;	///< No. of values filled in total
		Double_t	mSum;	///< Summed total value in the current Bin
		Double_t	mSumAveraged;	///< Averaged summed total value in the current Bin
	
	ClassDef(TRemiRollingGraph, 1);
};

#endif

