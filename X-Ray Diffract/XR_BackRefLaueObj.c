 //ects:D.M. v2.0:Sources.Jim:X-Ray Diffract:XR_BackRefLaueObj.c#include	"Diffract_INCs.h"#include	"ST_StereoMacros.c"#include	"UT_VectorMacros.c"#include	"BackRefLaueObj.h"#include	"SF_MathLib.h"#include "ML_CBED.h"#include	"CBEDObj.h"#include "ML_Crystal.h"#include "ML_PPT_Info.h"#include "ML_BeamOrientation.h"#include "ML_CrystalFace.h"#include "ML_Default.h"#include "ML_Misc_Plot.h"void BackRefLaueObj::DoInit(){	inherited::DoInit();} void BackRefLaueObj::SetObjectMenu(void){	theMenuBar 	= 	GetNewMBar(LAUE_BACK_MB);	SetMenuBar		(theMenuBar);}void	BackRefLaueObj::SetObjectWindowTitle(void){		theBeamFlag = 2;	energy =  theXRay->energy;	maxEnergy = theXRay->maxEnergy;	wavelength = theXRay->wavelength;		theRuler->SetBeamButton(theBeamFlag);		if(gTheFile->file_is_Open){		SetWTitle(theWindow,fileInfo.sfFile.name);		return;	}	sprintf(gTheText,"Back Reflec. Laue %d",g_Window_Number);	SetWTitle(theWindow,c2pstr(gTheText));	g_Window_Number++;}void	BackRefLaueObj::MyCalculate(void){		long 		thehMax,thekMax,thelMax,i;		long		hmax,kmax,lmax;				double 	x,y,z,						chx,ckx,clx,						chy,cky,cly,						chz,ckz,clz,						x1,x2,						y1,y2,						z1,z2,						theDist,reciprocalLatticeSize,intens,						RofEwaldSph,						aCos,distZ;		SpotInfoPtr	thisSpot;		double theD,							xForm[3][3],					xFormMatrix[3][3],					maxIntensity[4],maxRatio[4];		PPT_Info 	*pptOR;		short		k = 0;		short		orNumber,orNum;		short		plotCrystal;		Crystal 	*thisCrystal;			double		*theUs,*theVs,*theWs,							*theZs,*theBs;		double  	dH,dK,dL;		double 		cameraLength,oldWavelength,							theRecpWavelength,angle;		double		spikingFactor;		Ptr			*theHandle;		dcomplex	sF,cAb;		double		cutOff;	data_Length			=	100;	data_Expand			=	100;	data_Count			=	0;	data_Size				=	sizeof(SpotInfo);	if(theDataHandle != 0L)		/*Temp*/KillHandle				(&theDataHandle);	theDataHandle		= 	/*Temp*/D_NewHandle(data_Length * data_Size);//,&gTheOSError	/*MFTemp*/D_HLock(theDataHandle);	thisSpot  = (SpotInfoPtr)*theDataHandle;	thisSpot = InsertData( (double)theZoneAxis->h,  (double)theZoneAxis->k,   (double)theZoneAxis->l,  1, 0, 0, 0,0,theCrystal[baseCrystal], baseCrystal, Complex(1, 0),thisSpot);	((SpotInfoPtr)*theDataHandle)->direction = true;				thisCrystal = theCrystal[baseCrystal];	for(i = 0; i <= 3;i++)calculateFlags[i] = false;			GetOrientation(xFormMatrix);//	InvertMatrix(xFormMatrix);		cameraLength 			= 	cameraConstant / wavelength;	if(theBeamFlag == 1){		oldWavelength 			= 	wavelength;		RofEwaldSph 			= 	1 / wavelength;		reciprocalLatticeSize 	= (double)(.5 * RofEwaldSph);	}else{		oldWavelength 			= 	wavelength;		/*maxEnergy 				= 	energy;*/		wavelength 				= 	12.4 / maxEnergy;		RofEwaldSph 			= 	1 / wavelength;		reciprocalLatticeSize 	= 	(double)(RofEwaldSph);	}		for(plotCrystal  = baseCrystal; plotCrystal <= 3 && gNoBreak ; plotCrystal++){			short atom_Count;		if(!plotFlags[plotCrystal] || (plotCrystal > baseCrystal && baseCrystal > 0))			continue;		if(plotCrystal  == baseCrystal)			orNumber = 0;		else{			thisCrystal = theCrystal[plotCrystal];			pptOR = the_PPT_Info[plotCrystal- 1];			orNumber = pptOR->lastOR;		}		obj_Index->theCrystal = thisCrystal;		theHandle = thisCrystal->AtomicScatteringSet( 1. + ( (double)energy / 511.),theBeamFlag);		if(!SetScatteringPosAIntensity(thisCrystal,&theUs,&theVs,&theWs,&theZs,&theBs,&atom_Count)){			/*MFTemp*/D_HUnlock(theDataHandle);			return;		}		AtomicScatterForDFromSet(0,theHandle,theZs,atom_Count);		sF = TotalStructureFactor(0,0,0,theUs,theVs,theWs,atom_Count,theZs,theBs,thisCrystal->volume,10000,thisCrystal,&cAb);		cutOff = sF.r * .000001;		hmax = SizeofReciprocalLattice(1.,0.,0.,reciprocalLatticeSize,thisCrystal) * 2;		kmax = SizeofReciprocalLattice(0.,1.,0.,reciprocalLatticeSize,thisCrystal) * 2;		lmax = SizeofReciprocalLattice(0.,0.,1.,reciprocalLatticeSize,thisCrystal) * 2;		if(lmax > 40){			hmax = 40;			kmax = 40;			lmax = 40;		}		thehMax = -hmax;		thekMax = -kmax;		thelMax = -lmax;					for(orNum = 0 ; orNum <= orNumber && gNoBreak ; orNum++){						GetPPTXFormMatrixBase(xForm,thisCrystal,orNum);			MatrixMultiply(xForm,xFormMatrix,xForm); 			ConverttoReciprocal(xForm,(double)thisCrystal->volume,false);			 			chx = xForm[0][0];			chy = xForm[1][0];			chz = xForm[2][0];			ckx = xForm[0][1];			cky = xForm[1][1];			ckz = xForm[2][1];			clx = xForm[0][2];			cly = xForm[1][2];			clz = xForm[2][2];										spikingFactor = GetSpikingFactor(theFoilNormal,theFNRotationAxis,theFoilBragg,theFNRotationAngle,theFNOffLaueDistance,theFoilThickness,calcFoil,thisCrystal);						maxIntensity[plotCrystal] 	= 0.0;			maxRatio[plotCrystal] 		= 0.0;							x = thehMax * chx;			y = thehMax * chy;			z = thehMax * chz;			for(obj_Index->h = thehMax ; obj_Index->h <= hmax  && gNoBreak ; obj_Index->h++)			{				x1 = x + thekMax * ckx;				y1 = y + thekMax * cky;				z1 = z + thekMax * ckz;				for(obj_Index->k = thekMax; obj_Index->k <= kmax && gNoBreak ; obj_Index->k++)				{					x2 = x1 + thelMax * clx ;					y2 = y1 + thelMax * cly;					if(theBeamFlag == 2)						z2 = z1 + thelMax * clz;					else						z2 = z1 + (thelMax * clz) - RofEwaldSph;					for(obj_Index->l =  thelMax; obj_Index->l <= lmax && gNoBreak ; obj_Index->l++)					{						AllowBackground();						if( obj_Index->h == 0 && obj_Index->k == 0 && obj_Index->l == 0)							goto NEWSTART;						if(z2 <= 0.0 && theBeamFlag == 2) goto NEWSTART;						distZ = (z2 * z2);						theD = theDist = sqrt((x2 * x2) + (y2 * y2) + distZ);							if(theBeamFlag == 2){							aCos =  z2/ (theDist);							angle = acos(aCos);														if(aCos == 0) goto NEWSTART;							theRecpWavelength = .5 * theDist / fabs(aCos);							if(theRecpWavelength > RofEwaldSph ) goto NEWSTART;							if(theRecpWavelength > fabs( z2 * .8) ) goto NEWSTART; 						}else{							theDist = fabs((RofEwaldSph - sqrt( x2 * x2 + y2 * y2 + distZ))); 							if(theDist > spikingFactor)								goto NEWSTART;						}						dH = (double)obj_Index->h;						dK = (double)obj_Index->k;						dL = (double)obj_Index->l;						AtomicScatterForDFromSet(theDist,theHandle,theZs,atom_Count);						sF = TotalStructureFactor(dH,dK,dL,theUs,theVs,theWs,atom_Count,theZs,theBs,thisCrystal->volume,1/theD,thisCrystal,&cAb);						intens = Cabs(sF);						//intens *= intens;						if(intens < cutOff)							goto NEWSTART;												thisSpot = InsertData( obj_Index->h, obj_Index->k,  obj_Index->l,  intens, x2, y2, z2, angle,thisCrystal, plotCrystal, sF,thisSpot);								NEWSTART:		x2 += clx;						y2 += cly;						z2 += clz;					}					x1 += ckx;					y1 += cky;					z1 += ckz;				}				x += chx;				y += chy;				z += chz;			}		}				KillPtr((Ptr)theUs);		thisCrystal->ClearAtomicScatSet(theHandle);	}		for(i = 0; i <= 3;i++){		maxIntensity[i] = -1000000.0001;		maxRatio[i]		=	.01;	}	thisSpot = (SpotInfoPtr)*theDataHandle;	thisSpot->intensity = 1;	thisSpot++;	for(i = 1; i < data_Count;i++,thisSpot++){		double theValue;		theValue = thisSpot->angle / RofEwaldSph;		if(theValue > maxRatio[thisSpot->flags]) maxRatio[thisSpot->flags] = theValue;	}	thisSpot = (SpotInfoPtr)*theDataHandle;	thisSpot->intensity = 1;	thisSpot++;	for(i = 0; i <= 3;i++){maxRatio[i] = .8 / (RofEwaldSph * maxRatio[i]);}	for(i = 1;i < data_Count;thisSpot++,i++){		thisSpot->intensity *= (thisSpot->angle * (maxRatio[thisSpot->flags]));	}		CompareSpots(0,0,0,(double*)0L,sF,0,0,(Crystal*)0L,0);		thisSpot = (SpotInfoPtr)*theDataHandle;	thisSpot->intensity = 1;	thisSpot++;		for(i = 1; i < data_Count;i++,thisSpot++){		if(thisSpot->intensity > maxIntensity[thisSpot->flags]){			 maxIntensity[thisSpot->flags] = thisSpot->intensity;		}	}	thisSpot = (SpotInfoPtr)*theDataHandle;	thisSpot->intensity = 1;	thisSpot++;	maxIntensity[0] = 1/ maxIntensity[0];	for(i = 1; i <= 3;i++){maxIntensity[i] = the_PPT_Info[i - 1]->intensity / maxIntensity[i];}	for(i = 1;i < data_Count;thisSpot++,i++){			thisSpot->intensity *= maxIntensity[thisSpot->flags];	}		/*MFTemp*/D_HUnlock			((Handle)theDataHandle);		if(data_Length >  data_Count){		D_RecoverMemory		(theDataHandle,data_Size,(long)data_Count);	}	numSpots = data_Length 	= 	data_Count;	wavelength = oldWavelength;}SpotInfoPtr		BackRefLaueObj::InsertData(double H,double K, double L, double intens,double x,double y,double z,							double angle,Crystal *thisCrystal,short plotCrystal,dcomplex sF,SpotInfoPtr thisSpot){	double xC,yC,zC,length,aCos,theRecpWavelength;	double	TwoTheta,radius;	length = sqrt(x * x + y * y + z * z);	xC = x / length;	yC = y / length;	zC = z / length;	thisSpot =	inherited::InsertData(H,K, L,intens,x,y,z,angle,thisCrystal,plotCrystal,sF,thisSpot);		thisSpot--;		aCos =  z/ (length);	theRecpWavelength = .5 * length / fabs(aCos);	if(theBeamFlag == 2){		TwoTheta = 2 * angle;		if(angle != PI){			radius = fabs(theRecpWavelength * tan(TwoTheta));			TwoTheta = sqrt(x * x + y * y);			if(TwoTheta < .000001){data_Count--; return thisSpot;}			radius /= TwoTheta;			thisSpot->x *= radius;			thisSpot->y *= radius;			thisSpot->z *= radius;  		}	}else{		theRecpWavelength = 1;	}	thisSpot->angle = theRecpWavelength;	thisSpot++;	return thisSpot;}void BackRefLaueObj::CompareSpots(double x,double y,double z,double *intens,dcomplex sF,double angle,					double theRecpWavelength,Crystal *thisCrystal,short plotCrystal){	SpotInfoPtr thisSpot,compSpot,sourceSpot,destSpot;	short i,k,j;	double xC,yC,zC,length,dummy;	dcomplex dumSF;//warning	Crystal*	aCrystal;//warning	dummy = x;//warning	dummy = y;//warning	dummy = z;//warning	dummy = *intens;//warning	dummy = theRecpWavelength;//warning	dumSF = sF;//warning	aCrystal = thisCrystal;//warning	dummy = (double)plotCrystal;//warning	dummy = angle;	thisSpot	= 	(SpotInfoPtr)*theDataHandle;	thisSpot++;	compSpot = thisSpot;	for(k = 1; k < data_Count;k++,compSpot++){			length = sqrt(compSpot->x * compSpot->x + compSpot->y * compSpot->y + compSpot->z * compSpot->z);			xC			=	compSpot->x / length;			yC			=	compSpot->y / length;			zC			=	compSpot->z / length;			thisSpot	= 	&((SpotInfoPtr)*theDataHandle)[k];		for(i = k; i < data_Count;i++,thisSpot++){			if(i == k)continue;			length = sqrt(thisSpot->x * thisSpot->x + thisSpot->y * thisSpot->y + thisSpot->z * thisSpot->z);			if(fabs((thisSpot->x/length) - xC) <= .005 && fabs((thisSpot->y/length) - yC) <= .005 && fabs((thisSpot->z/length) - zC) <= .005					&& thisSpot->theCrystal == compSpot->theCrystal){				compSpot->intensity += thisSpot->intensity;				compSpot->sF	=	Cadd(thisSpot->sF, compSpot->sF);				sourceSpot = destSpot = thisSpot;				sourceSpot++;				data_Count--;				for(j = i ; j < data_Count ; j++,sourceSpot++,destSpot++){					*destSpot = *sourceSpot;				}								i--;				thisSpot--;			}		}	}}void	BackRefLaueObj::DoPictDraw(void){	double intensityRange,oldScaleFactor;	Rect theOval,oldOval;	long 		theX,theY;	SpotInfoPtr thisSpot;	short 		theIdent,i,				theSize;					short		hRes,vRes,				hLim,vLim;					double 		cameraLength,totalMag,aMaxSize;		inherited::DoPictDraw();		SetDrawEnviron();			ScreenRes(&hRes,&vRes);	hLim 			=  .5 * negWidth * hRes / 25.4;	vLim 			=  .5 * negHeight * vRes / 25.4;	aMaxSize 		= 	expoTime + 2;	intensityRange 	= 	1. / (double)dyRange;	cameraLength 	= 	cameraConstant / wavelength;	oldScaleFactor 	= 	scaleFactor;	totalMag	 	= 	magnification * cameraLength * (double)hRes / 25.4;	thisSpot 		= 	(SpotInfoPtr)(*theDataHandle);	SetRect			(&theOval,-aMaxSize,-aMaxSize,aMaxSize,aMaxSize);		OffsetRect		(&theOval,(short)centerX,(short)centerY);	DM_DrawRect(theOval,thisSpot->flags & CRYSTAL_MASK,thisSpot->flags & CRYSTAL_MASK,thisSpot->direction);	thisSpot->spotRect = theOval;	thisSpot++;		for(i = 1 ; i < data_Count; i++,thisSpot++){		short theValue;		if(thisSpot->angle == 0.0) continue;		theValue = theIdent = thisSpot->flags & CRYSTAL_MASK;		if(!plotFlags[theIdent] && theIdent <= 3 && i > 1) continue;		scaleFactor = (totalMag / thisSpot->angle) ;				theX	= centerX + (short)(scaleFactor * thisSpot->y);		theY	= centerY + (short)(scaleFactor * thisSpot->x);				//CalculateSpotLocation(&theX,&theY, thisSpot);	/*	if(fabs(theX - centerX) > hLim || fabs(theY - centerY) > vLim){				SetRect(&oldOval,0,0,0,0);					thisSpot->spotRect = oldOval;			 continue; 		}	*/					theSize 	= 	Round(aMaxSize * pow((double)thisSpot->intensity,intensityRange));		SetRect			(&oldOval,-theSize,-theSize,theSize,theSize);			theOval 	= 	oldOval;		OffsetRect		(&theOval,(short)theX,(short)theY);		thisSpot	->	spotRect = theOval;		if(greyFlag){			theValue = (short)(150 *  pow((double)thisSpot->intensity,intensityRange));		}				DM_DrawRect(theOval,theIdent,theValue,thisSpot->direction);	}	scaleFactor = oldScaleFactor;	/*theSearchObj->PlotRings(scaleFactor,scaleFactor,centerX,centerY,0,500);*/	ClearDrawEnviron();	}#pragma segment Mainvoid  BackRefLaueObj::SetDefineObj(DialogPtr theDialog){	short i,n = 5;	for(i = n; i <= n + 3;i++){		plotFlags[i-n] = SwitchBoolean(theDialog,i,!plotFlags[i-n]);	}	greyFlag = SwitchBoolean(theDialog,9,!greyFlag);	showPeriods = SwitchBoolean(theDialog,10,!showPeriods);	kikuchiLines = SwitchBoolean(theDialog,11,!kikuchiLines);	DrawDialog(theDialog);}void BackRefLaueObj::DoDefine(void)//Override{	Boolean			calculateFlag;	MultiLog*		theMultiLog;	short				n = 0;		theMultiLog	= (MultiLog*)new(MultiLog);	theMultiLog->DoInit(265,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCBEDObjSetup,(HitProcPtr)ML_DoCBEDObjHit,(ReadProcPtr)ML_DoCBEDObjRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoBeamESetup,(HitProcPtr)ML_DoBeamEHit,(ReadProcPtr)ML_DoBeamERead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoBeamORSetup,(HitProcPtr)ML_DoBeamORHit,(ReadProcPtr)ML_DoBeamORRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoStageObjSetup,(HitProcPtr)ML_DoStageObjHit,(ReadProcPtr)ML_DoStageObjRead,(Ptr)stage);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[2]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[3]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[2]);		theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoFoilSetup,(HitProcPtr)ML_DoFoilHit,(ReadProcPtr)ML_DoFoilRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_SetMiscParam,(HitProcPtr)ML_SwitchMiscParam,(ReadProcPtr)ML_ReadMiscParam,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoDefaultSetup,(HitProcPtr)ML_DoDefaultHit,(ReadProcPtr)ML_DoDefaultRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoSFSetup,(HitProcPtr)ML_DoSFHit,(ReadProcPtr)ML_DoSFRead,(Ptr)this);		calculateFlag = theMultiLog->DoMultiLog();	theMultiLog->DoClose();		if(calculateFlag){		changedFlag = true;		reSizeFlag = true;		newPictReq = true;/*eliminated line dec 1992*/	} else if(screenPict == NUL  && gMyOffG == 0L){		CloseTheObject(theWindow);	}	return;}void BackRefLaueObj::DoSetUp(void){	DialogPtr 		theDialog;	short			theSelect;	Boolean			quit = false;	Boolean			saveData = false;			theDialog	= DM_GetNewDialog(11123,NUL,IN_FRONT);	SetDefineObj(theDialog);	while(!quit){		ModalDialog(TheFilterUPP,&theSelect);		switch(theSelect){			case ML_UPDATE_EVT:				BeginUpdate(theDialog);			UpdtDialog(theDialog,theDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(theDialog);			}			quit = DefineSwitch( theDialog,theSelect,&saveData);			EndUpdate(theDialog);			break;			default:			quit = DefineSwitch( theDialog,theSelect,&saveData);			break;			}			}	ReadDefineDialog( theDialog, saveData);	DM_DisposDialog(&theDialog);		return;}void		BackRefLaueObj::ReadDefineDialog(DialogPtr theDialog,Boolean saveData){	short i;	short n = 5;	short	oldLimit;	if(saveData){				/* Eric re-wrote this section July 1993 */		Boolean theFlag;		DoDefineOR( theDialog, false,_3_);		for(i = n ; i <= n + 3 ; i++){			theFlag = GetBoolean(theDialog,i);			if(plotFlags[i - n] < theFlag)changedFlag = true;			plotFlags[i - n] = theFlag;		}		if(greyFlag 			!= GetBoolean(theDialog,9)) newPictReq = true;		greyFlag 			= GetBoolean(theDialog,9);		if(showPeriods 	!= GetBoolean(theDialog,10))newPictReq = true;		showPeriods 	= GetBoolean(theDialog,10);		if(kikuchiLines 	!= GetBoolean(theDialog,11))newPictReq = true;		kikuchiLines 	= GetBoolean(theDialog,11);		oldLimit = kLimit;		kLimit = GetItemValue(theDialog,12);		if(kLimit != oldLimit && kikuchiLines)changedFlag = true;		baseCrystal = -1;		i = -1;		while(baseCrystal < 0 && i < 3){			i++;			if(plotFlags[i]){				setCrystal = theCrystal[i];				baseCrystal = i;			}		}		if(baseCrystal < 0){			setCrystal = theCrystal[0];			baseCrystal = 0;		}	}}Boolean	BackRefLaueObj::DefineSwitch(DialogPtr theDialog,short theSelect,Boolean *saveData){	switch(theSelect){			Boolean theFlag;			case -1:			case 2:				*saveData = false;				return true;				break;			case DLOG_ENTER_OR_CR:			case 1:				*saveData = true;					return true;				break;			case 5:			case 6:			case 7:			case 8:			case 9:			case 10:			case 11:				theFlag = GetBoolean(theDialog,theSelect);				theFlag = SwitchBoolean(theDialog,theSelect,theFlag);				break;			case ML_UPDATE_EVT:				DrawDialog(theDialog);			break;			default:				break;		}		return false;}