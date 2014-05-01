#include	"Diffract_INCs.h"//s.Jim:E_Diffract:E_HOLZObj.c#include	"ST_StereoMacros.c"#include	"UT_VectorMacros.c"#include	"SF_MathLib.h"#include	"HOLZObj.h"#include		"COMPLEX.h"#include	"D_Handles.h"void HOLZObj::MyCalculate(void)	{	long 	hmax,kmax,lmax,			h,k,l,			thehMax,thekMax,thelMax,			i;	long	x,y,z;	long	theDotP;				double 	zoneAxisLength,reciprocalLatticeSize,			dx,dy,dz,theD,zoneParameter;				double  intens,factor,RofEwaldSph,R;		double 	chx,chy,chz,			ckx,cky,ckz,			clx,cly,clz;				double 	hZoneAxis,kZoneAxis,lZoneAxis;		SpotInfoPtr	theSpots,thisSpot,theSpot;		double 	xForm[3][3],			xFormMatrix[3][3];				double 	maxIntensity,xD,yD,zD;		double 	*theUs,*theVs,*theWs,*theZs,*theBs;		double		dH,dK,dL;				short 	atom_Count;	long	plotCrystal;	OSErr	iErr;	double	diameter,electronMassRation;	Ptr		*theHandle;	dcomplex cAb,sF;	double bigK,ratio,Uo;	electronMassRation =   1. + ( (double)energy / 511.);				ResetCurvedLabels();	if(!SetScatteringPosAIntensity(setCrystal,&theUs,&theVs,&theWs,&theZs,&theBs,&atom_Count))return;				data_Count		=	0;	data_Size 		= 	sizeof(SpotInfo);					if(lowOrder <= 0)		lowOrder = 1;		deltaCell.a 		= 	0;	deltaCell.b 		=	0;	deltaCell.c			= 	0;	deltaCell.alpha 	= 	0;	deltaCell.beta 		= 	0;	deltaCell.gamma 	= 	0;	delVoltage 			= 	0;			RofEwaldSph 		= 	1. / wavelength; 	plotCrystal 		= setCrystal->idNum;	gamma						=	0;	thisSpot 				= &(((SpotInfoPtr)*theDataHandle)[data_Count++]);		thisSpot->intensity 	= (short double)1.0;	thisSpot->x				= 0.0;	thisSpot->y 			= 0.0;	thisSpot->z 			= 0.0;	thisSpot->angle 		= 0.0;	thisSpot->h 			= (short)theZoneAxis->h;	thisSpot->k 			= (short)theZoneAxis->k;	thisSpot->l 			= (short)theZoneAxis->l;	thisSpot->direction 	= true;	thisSpot->theCrystal 	= setCrystal;	thisSpot->flags 		= plotCrystal;		hZoneAxis 		= (double)theZoneAxis->h;	kZoneAxis 		= (double)theZoneAxis->k;	lZoneAxis		= (double)theZoneAxis->l;	/*Added for v2.0*/		GetOrientation(xFormMatrix);//	InvertMatrix(xFormMatrix);				zoneParameter 		= 	setCrystal->theUnitCell.a;	if(setCrystal->theUnitCell.b < zoneParameter) 		zoneParameter 	= 	setCrystal->theUnitCell.b;	if(setCrystal->theUnitCell.c < zoneParameter)		zoneParameter  	= 	setCrystal->theUnitCell.c ;		zoneAxisLength 	= 1./theZoneAxis->Periodicity(highOrder,cameraConstant/wavelength,wavelength,&diameter);	reciprocalLatticeSize = (diameter/(2 * cameraConstant)) + (2 * convergenceAngle); 		hmax 	= SizeofReciprocalLattice(1.,0.,0.,reciprocalLatticeSize,setCrystal);	kmax 	= SizeofReciprocalLattice(0.,1.,0.,reciprocalLatticeSize,setCrystal);	lmax 	= SizeofReciprocalLattice(0.,0.,1.,reciprocalLatticeSize,setCrystal);		if(setCrystal->lattice_Type == HEXAGONAL || setCrystal->lattice_Type == TRIGONAL){		hmax *= 2;		kmax *= 2;	}		thehMax = -hmax;	thekMax = -kmax;	thelMax = -lmax;				xD = (double)theZoneAxis->h;	yD = (double)theZoneAxis->k;	zD = (double)theZoneAxis->l;			setCrystal->		Get001XForm(xForm);	MatrixMultiply		(xForm,xFormMatrix,xForm); 	ConverttoReciprocal	(xForm,(double)setCrystal->volume,true);	zeroIntensity = (double)Cabs(TotalStructureFactor(0,0,0,theUs,theVs,theWs,atom_Count,theZs,theBs,setCrystal->volume,1,setCrystal,&cAb));	chx 				= xForm[0][0];	chy 				= xForm[1][0];	chz 				= xForm[2][0];	ckx 				= xForm[0][1];	cky 				= xForm[1][1];	ckz 				= xForm[2][1];	clx 				= xForm[0][2];	cly 				= xForm[1][2];	clz 				= xForm[2][2];		ratio 		= (.006648352) * (1 + (1.9569341e-3 * energy));// this converts Vg to Ug, Vg is the fourier components of the 	//crystal potential given in eV, Ug is the Dynamical Structure factor	Uo  		= ratio * zeroIntensity;	bigK  		= sqrt( (1/(wavelength * wavelength)) + Uo); // this calculates the momentum vector	//K inside the crystal.  sF.r is the mean inner potential Uo for this crystal			maxIntensity = 0;	/*MFTemp*/D_HLock((Handle)theDataHandle);	theSpots = (SpotInfoPtr)*theDataHandle;	x = thehMax * hZoneAxis;	theHandle = setCrystal->AtomicScatteringSet(electronMassRation,theBeamFlag);	theDotP = 1;	for(h = thehMax ; h <= hmax  && gNoBreak ;h++){					y = thekMax * kZoneAxis;				for(k = thekMax; k <= kmax && gNoBreak ; k++){						z = lZoneAxis * thelMax;			for(l =  thelMax; l <= lmax && gNoBreak ; l++){																		//{					dx = chx * h + ckx * k + clx * l;					dy = chy * h + cky * k + cly * l;					dz = chz * h + ckz * k + clz * l;					if(dz < 0.0) 						dz = -dz;										theD 	= sqrt(dx * dx  + dy * dy);					R 		= fabs((2. * RofEwaldSph * dz));					R 		-= (dz * dz);					factor 	= -((sqrt(R) - theD)  / convergenceAngle);																		//}									NEWSTART:  				z += lZoneAxis;			}			y +=  kZoneAxis;		}		x += hZoneAxis;	}	KillPtr				((Ptr)theUs);	if(adjustEnergy){		gamma /= 2 * bigK;	}	setCrystal->ClearAtomicScatSet(theHandle);	thisSpot 		= 	&(((SpotInfoPtr)*theDataHandle)[1]);	for(i = 1;i <= data_Count - 1;thisSpot++,i++){			thisSpot->intensity /= maxIntensity;	}	thisSpot 		= 	&(((SpotInfoPtr)*theDataHandle)[1]);		for(i = 1;i < data_Count;thisSpot++,i++){			if(thisSpot->intensity < .05){				theSpot = thisSpot;				for(k = i;k < data_Count - 1;theSpot++,k++){					*theSpot = theSpot[1];									}				i--;				thisSpot--;			}	}	/*MFTemp*/D_HUnlock			((Handle)theDataHandle);	if(data_Length >  data_Count){		D_RecoverMemory		(theDataHandle,data_Size,(long)data_Count);	}		numSpots = data_Length 	= 	data_Count;	/*Temp*/KillHandle			((Handle*)&deltaData); 	deltaData		= 	(SpotInfoHandle)theDataHandle;	iErr			= 	D_HandToHand((Handle*)&deltaData);	return;}