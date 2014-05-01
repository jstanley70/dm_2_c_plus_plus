//rces.Jim:Utilities/Libs:Utilities Headers:SF_MathLib.h#ifndef	_SF_LIB	#define	_SF_LIB#include "COMPLEX.h"dcomplex	StructureFactor(double h,double k,double l,double *theUs,double *theVs,							double *theWs,long num,							double *theZs,double *theBs,double volume,double dspace);dcomplex	StructureFactorDW(	double h,double k,double l,double *theUs,double *theVs,								double *theWs,long num,								double *theZs,double *theBs,double volume,double dspace);								dcomplex  TotalStructureFactor2D(double h,double k,double *theUs,double *theVs,long num,double *theZs,double *theBs,double volume,double dspace,Crystal *thisCrystal,dcomplex *abC);			dcomplex	StructureFactorDW_2D(double h,double k,double *theUs,double *theVs,			long num,double *theZs,double *theBs,double volume,double dspace);dcomplex	StructureFactor_2D (double h,double k,double *theUs,double *theVs,long num,double *theZs,double *theBs,double volume,double dspace);void		GetDWFactor			(AtomPtr theAtom);Boolean 	SetScatteringPosAIntensity			(Crystal *crystal,double **theUs,double **theVs,double **theWs,double **theZs,double **theBs,short *atom_Count);double	StructureFactorOld(double h,double k,double l,double* theUs,double* theVs,double* theWs,long num,double* theZs,double volume);double 	CalcExpDenZA(short baseCrystal,Crystal* theCrystal[4],PPT_Info* the_PPT_Info[3],Boolean *plotFlags,double *targetZ,double *targetAtWt);dcomplex  TotalStructureFactor(double h,double k,double l,double *theUs,double *theVs,			double *theWs,long num,double *theZs,double *theBs,double volume,double dspace,Crystal *thisCrystal,dcomplex *abC);dcomplex ConvertStructureFactor(dcomplex sF,short strctFlag,double volume);#endif