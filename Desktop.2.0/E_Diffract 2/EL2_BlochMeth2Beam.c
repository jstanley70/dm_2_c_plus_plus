#include	"Diffract_INCs.h"#include 	"nrutil.h"#define		FREE_MATRIX free_matrix#define		MATRIX		matrixdouble DiffractObject::Single2BeamWaveDynamic(short *nout,dcomplex *sF,SpotInfoDPtr theSpotsSt){	double		matrixX[3][3];	double		*intensities,inten;	static short j = 0;	rreal		**theUgReal,**theUgIm;	short		nBeams = 2,pBeams;	short		start = 0,totalBeams = 2;	short		oldSpotLoc;	long		maxBeams;	oldSpotLoc = spotLoc;	spotLoc = 1;	if(!SetUp2BeamUgh((Handle*)&theUgReal,(Handle*)&theUgIm,&nBeams,&pBeams,theSpotsSt,totalBeams)) {*nout = 0; StopWatch();return 0;}	maxBeams = nBeams + 10;	*nout =  2;		delugeFlag 		= false;	calcIntensities = 0L;	Identity_Matrix(matrixX);	intensities =  BlochMethod2Beam(theUgReal,theUgIm,matrixX,*nout,nBeams,pBeams,sF,theSpotsSt);	inten = intensities[1];	if(intensities == 0L){		FREE_MATRIX(theUgReal,1,maxBeams,1,maxBeams);		FREE_MATRIX(theUgIm,1,maxBeams,1,maxBeams);		*nout = 0;		return 0;	}	KillPtr(intensities);	FREE_MATRIX(theUgReal,1,maxBeams,1,maxBeams);	FREE_MATRIX(theUgIm,1,maxBeams,1,maxBeams);	newPictReq = true;	spotLoc = oldSpotLoc;	return inten;}double* DiffractObject::BlochMethod2Beam(rreal **theUgReal,rreal **theUgIm,double matrixD[3][3],short nout,short nbeams,short pBeams,dcomplex *sF,SpotInfoDPtr theSpotsSt)//matrix is the matrix required to tilt the reciprocal lattice to mimic the beam tilt{	double  		bigK;	SpotInfoDPtr 	theSpots;			rreal			*eigenValR,*eigenValIm,**eigenVecR,**eigenVecIm;	rreal			*thePtr3,*thePtr2;	long			j,i,k;	double			*intensities,ratio;		double			Uo;	short			maxBeams;	short			nBeams;	dcomplex		**a,*b,*thePtr;			short			totalBeams,start;	pBeams = pBeams;		matrixD[0][0] = matrixD[0][0];	start = 0;	totalBeams = 2;	maxBeams 	= nbeams + 10;	nBeams		=	nbeams;	theSpots 	= (SpotInfoDPtr)theSpotsSt;		switch(gCurrentObj->theBeamFlag){		case 1:		default:		ratio 	= (.006648352) * (1 + (1.9569341e-3 * energy));		break;		case 2:		ratio 	= 2.818e-5 * ( wavelength *  wavelength);		break;	}	//crystal potential given in eV, Ug is the Dynamical Structure factor	Uo  		= ratio * theSpots->sF.r;	bigK  		= sqrt( (1/(wavelength * wavelength)) + Uo); // this calculates the momentum vector	//K inside the crystal.  sF.r is the mean inner potential Uo for this crystal	thePtr2 = eigenValR 	= (rreal*)D_NewPtr((maxBeams + 2) * sizeof(rreal));	thePtr3 = eigenValIm 	= (rreal*)D_NewPtr((maxBeams + 2) * sizeof(rreal));	//thePtr4 = eigenVecR 	= (float*)D_NewPtr(totalSize * sizeof(float));	//thePtr5 = eigenVecIm 	= (float*)D_NewPtr(totalSize * sizeof(float));	//thePtr1 = a 			= (dcomplex*)D_NewPtr(totalSize * sizeof(dcomplex));	thePtr  = b 			= (dcomplex*)D_NewPtr((maxBeams + 2) * sizeof(dcomplex));	a = dcommatrix(1,maxBeams,1,maxBeams);	eigenVecIm 	= MATRIX(1,maxBeams,1,maxBeams);	eigenVecR	= MATRIX(1,maxBeams,1,maxBeams);	for(i = 0; i < maxBeams + 2;i++,thePtr2++,thePtr3++,thePtr++){		*thePtr2 = *thePtr3 = 0;		thePtr->r = 0;		thePtr->i = 0;	}	for(i = 1; i < maxBeams;i++){		for(k = 1; k < maxBeams;k++){			a[i][k].r = 0;			a[i][k].i = 0;			eigenVecIm[i][k] = 0;			eigenVecR[i][k] = 0;		}	}				//reverts 000 beam to orginal notation		theSpots++;	theUgReal[1][1] = 0.0;	theUgIm[1][1] 	= absorpMean * Uo;//(ratio * theSpots->sF.r) is Uo mean scattering potential	for( i = 1;i < nBeams;theSpots++,i++){			theUgReal[i][i] = -2 * bigK * theSpots->y ;		theUgIm[i][i] = absorpMean * Uo;	}	//Calculates the bethe Perturbation potential			//calculation and number of beams available but not used, normally this is 0,but will not be if the bethe perturbations		//are not zero.	EisPack				(nbeams,nbeams,theUgReal,theUgIm,eigenValR,eigenValIm,eigenVecR,eigenVecIm,&i);	if(i == 0){		for(j = 1; j <= nbeams;j++){			for(i = 1; i <= nbeams;i++){				a[i][j].r = eigenVecR[i][j];				a[i][j].i = eigenVecIm[i][j];			}		}				C_InverseRow		( nbeams, nbeams,a,b, &i);		intensities	= 0L;		if(i == 100)intensities = (double*)GetBeamIntensities(bigK,eigenVecR,eigenVecIm,eigenValR,eigenValIm,b,nbeams,nout,sF,(short)spotLoc - start);	}		KillPtr(eigenValR);	KillPtr(eigenValIm);	KillPtr(b);		FREE_MATRIX(eigenVecR,1,maxBeams,1,maxBeams);	FREE_MATRIX(eigenVecIm,1,maxBeams,1,maxBeams);	FREE_MATRIX((rreal**)a,1,maxBeams,1,maxBeams);	return intensities;}