#include	"Diffract_INCs.h"#include	"ST_StereoMacros.c"#include	"UT_VectorMacros.c"#include	"PCVNumerical.h"#include	"RulerButton.h"#include	"AtomicScatter.h"#include	"SF_MathLib.h"#include 	"TextTableObj.h"  #include 	"mathRecipies.h"#include 	"nrutil.h"#include "UextBoxFree.h"#include "SADObj.h"short SADObj::PlotStreaks(long thePtX,long thePtY,float devParam,short crystal,float intensity,double grayGain){	double minimum,delZ;	PointInfoPtr theIo;	long		totalPoints;	long		pointNo;	Rect		oldOval;	Rect		theOval;	double	intensityRange;	PointInfoPtr	baseIo;	long		theIdent,maxX = 0;	double 	aintens;	float  *theIntensiyM,*ptrIntens,*ptrIntens2;	float  delZ_devParam;	short i,k;	long  count;	short numTimes;			if(!fullCalcPPt)return 0;			totalPoints = streakSize[crystal];	if(totalPoints < 10) return 0 ;	theIntensiyM = (float*)D_NewPtr(sizeof(float) * 6241);	if(theIntensiyM == 0L) return 0;	ptrIntens = theIntensiyM;	for(i = 0; i < 6241; i++,ptrIntens++)*ptrIntens = 0;	SetRect(&oldOval,-1,-1,1,1);	PenNormal();		D_HLock(streaks[crystal]);	baseIo = theIo = *(streaks[crystal]);	theIdent = 	crystal + 1;	theIo = baseIo;			intensityRange = 1. / (double)dyRange;	minimum =  .005;	if(autoSlice)delZ = .5 / scaleFactor;	else delZ = voxelThickness;	numTimes = 0;	RENEW: delZ_devParam = delZ + devParam;	pointNo = 0;	maxX = 0;	count = 0;	while(pointNo < totalPoints){		if(fabs(theIo->z - devParam) > delZ){pointNo++;theIo++; continue;}		aintens = theIo->intensity;		if(greyFlag || fabs(aintens) >= minimum) {			short	theX2,theY2;						theX2	=  (short)Round(scaleFactor  *  theIo->y);			theY2	=  (short)Round(scaleFactor  *  theIo->x);			if(fabs((double)theX2) > maxX) maxX = fabs((double)theX2);			if(autoSlice){				if(fabs((double)theY2) > maxX) maxX = fabs((double)theY2);			}			if(fabs((double)theY2) > 39 || fabs((double)theX2) > 39){pointNo++;theIo++;continue;}			theY2 += 39;			theX2 += 39;			ptrIntens2 = &theIntensiyM[(theX2 * 79) + theY2];			if(*ptrIntens2 < aintens){					*ptrIntens2 = aintens;					count++;			}		}		pointNo++;theIo++;	}	if(autoSlice && count < (maxX * maxX * .9) && numTimes < 3){delZ *= 1.5;numTimes++;goto RENEW;}	ptrIntens = theIntensiyM;	for(i = 0; i < 6241;i++,ptrIntens++){		*ptrIntens *= intensity;	}	ptrIntens = theIntensiyM;	grayGain *= 3;	for(i = -39; i <= 39;i++){		for(k = -39;k <= 39;k++,ptrIntens++){			double	theValue;			short l,m;			theOval = oldOval;			aintens = *ptrIntens;			if(aintens < 0.00001 ) continue;						theOval = oldOval;			if(greyFlag){				theValue = Round(grayGain * (pow((double) aintens,intensityRange)));				if(theValue > 219)theValue = 219;				if(theValue <= 0){continue;}			}else theValue = 1;				l = (short)thePtX + i;			m = (short)thePtY + k;			OffsetRect(&theOval,(short)l,(short)m);			DM_DrawRect(theOval,theIdent,theValue, false);		}			}	D_HUnlock(streaks[crystal]);	KillPtr(theIntensiyM);	return maxX;}void SADObj::PrecipitateSetUp(void){	DialogPtr 		theDialog;	short 			theItem;	Boolean			flag;	theDialog = DM_GetNewDialog(127, 0L,(WindowPtr)-1L);		SetItemValue(theDialog,3,streakRes,0);	SetItemValue(theDialog,4,voxelThickness,4);	SetItemValue(theDialog,5,sliceDistance,4);	SetItemValue(theDialog,6,numOfVoxSlice,0);	SwitchBoolean(theDialog,7,!autoSlice);	SwitchBoolean(theDialog,8,!multiSlice);	SwitchBoolean(theDialog,11,!pptSimple);		SetItemValue(theDialog,9,1 / scaleFactor,5);	SetItemValue(theDialog,10,numGVectors,2);		while(1 != 2){		SystemTask();		ModalDialog(MLogFilterUPP/*TheFilterUPP*/,&theItem);		switch(theItem){			case CANCELCLICK:			case 2:			DM_DisposDialog(&theDialog);			return;			case 1:			autoSlice 		= GetBoolean(theDialog,7);			pptSimple 		= GetBoolean(theDialog,11);			multiSlice 		= GetBoolean(theDialog,8);			streakRes  		= GetItemValue(theDialog,3);			voxelThickness  = GetItemValue(theDialog,4);			sliceDistance  	= GetItemValue(theDialog,5);			numOfVoxSlice  	= GetItemValue(theDialog,6);			numGVectors		= GetItemValue(theDialog,10);				DM_DisposDialog(&theDialog);			return;			case 7:			case 8:			case 11:			 flag = GetBoolean(theDialog,theItem);			 flag = SwitchBoolean(theDialog,theItem,flag);			 break;			case ML_UPDATE_EVT:					BeginUpdate(theDialog);				UpdtDialog(theDialog,theDialog->visRgn);				if(!gAppleEvtsOK){					HiliteOK(theDialog);				}				EndUpdate(theDialog);				break;			}	}}void SADObj::SimplePrecipiStreak(short crystalNum,PointInfoPtr Io,long *totalPoints/*,double devParam*/){	short 	i,ii;	long	l,pointNo;	double 	crystalvector[3][3],orForm[3][3],			xForm[3][3];	Crystal *thisCrystal;	long numberOfPoints;	PointInfoPtr startIo;	CrystalFace 	*theFacePtr;	Vector 			v;	double			linto;	//double orMatrix[3][3];	thisCrystal 			= 	theCrystal[crystalNum];	thisCrystal->Get001XForm(xForm);					D_HLock((Handle)thisCrystal->theFaces);	//thisCrystal->Get001XForm(xForm);	theFacePtr = *thisCrystal->theFaces;	startIo = Io;	for(l = 0; l < *totalPoints;l++,startIo++){			startIo->intensity = 0;			startIo->x = 0;			startIo->y = 0;			startIo->z = 0;	}	startIo = Io;	numberOfPoints = 0;	for(ii = 0; ii <= thisCrystal->face_Count;ii++,theFacePtr++){		double length;		v.x = theFacePtr->x;		v.y = theFacePtr->y;		v.z = theFacePtr->z;		TransFormVector(&v.x,&v.y,&v.z,xForm);		length = 10/theFacePtr->length;		if(fabs(v.x) > fabs(v.y))linto = fabs(v.x);else linto = fabs(v.y);		if(fabs(v.z) > linto)linto = fabs(v.z);		v.x /= linto;		v.y /= linto;		v.z /= linto;				v.x *= length;		v.y *= length;		v.z *= length;				for(i = -streakRes;i < streakRes;i++,startIo++,numberOfPoints++){			double nnn,mmm;						nnn = i;			mmm = streakRes;			nnn /= mmm;			v.x *= nnn;			v.y *= nnn;			v.z *= nnn;			startIo->x = v.x;			startIo->y = v.y;			startIo->z = v.z;			startIo->intensity = 1.1 - fabs(nnn);		}	}	GetOrientation				(crystalvector);		GetPPTXFormMatrixBase		(orForm,thisCrystal,0);	MatrixMultiply				(orForm,crystalvector,orForm); 	ConverttoReciprocal			(orForm,(double)thisCrystal->volume,true);	for(pointNo = 0; pointNo < numberOfPoints && Io->intensity > 0.0001;pointNo++,Io++){			double x,y,z;			x = Io->x;			y = Io->y;			z = Io->z;			TransFormVector(&x,&y,&z,orForm);			Io->x = x;			Io->y = y;			Io->z = z;	}	*totalPoints = numberOfPoints - 1;	D_HUnlock((Handle)thisCrystal->theFaces);	return;}