//ects:D.M. v2.0:Sources.Jim:X-Ray Diffract:XR_LaueTransObj.c#include	"Diffract_INCs.h"#include	"ST_StereoMacros.c"#include	"UT_VectorMacros.c"#include	"LaueTransObj.h"#include	"SF_MathLib.h"#include "ML_CBED.h"#include	"CBEDObj.h"#include "ML_Crystal.h"#include "ML_PPT_Info.h"#include "ML_BeamOrientation.h"#include "ML_CrystalFace.h"#include "ML_Default.h"#include "ML_Misc_Plot.h"void LaueTransObj::DoInit(){	inherited::DoInit();}void	LaueTransObj::SetObjectWindowTitle(void){		theBeamFlag = 2;	energy =  theXRay->energy;	wavelength = theXRay->wavelength;	maxEnergy = theXRay->maxEnergy;	theRuler->SetBeamButton(theBeamFlag);		if(gTheFile->file_is_Open){		SetWTitle(theWindow,fileInfo.sfFile.name);		return;	}	sprintf(gTheText,"Laue Trans. %d",g_Window_Number);	SetWTitle(theWindow,c2pstr(gTheText));	g_Window_Number++;}void LaueTransObj::SetObjectMenu(void){	theMenuBar = GetNewMBar(LAUE_TRANS_MB);	ClearMenuBar();/* July 1992 */	SetMenuBar(theMenuBar);}short data_Count_Old;void	LaueTransObj::MyCalculate(void){		long 		thehMax,thekMax,thelMax,i;		long		hmax,kmax,lmax;				double 		x,y,z,					chx,ckx,clx,					chy,cky,cly,					chz,ckz,clz,					x1,x2,					y1,y2,					z1,z2,					theDist,reciprocalLatticeSize,intens,					RofEwaldSph,					aCos,distZ;		SpotInfoPtr	thisSpot;		double 							xForm[3][3],					xFormMatrix[3][3],					maxIntensity[4];		PPT_Info 	*pptOR;		short		k = 0;		short		orNumber,orNum;		short		plotCrystal;		Crystal 	*thisCrystal;			double 		*theUs,*theVs,*theWs,*theZs,*theBs;		double  	dH,dK,dL,maxZeroOrderDist;		double 		oldWavelength,theRecpWavelength,angle;		double		maxRatio[4];		Ptr			*theHandle;		Index			*anAxis;		dcomplex	sF,cAb;		double		cutOff;	inherited::MyCalculate();	if(maxEnergy == energy) return;	numSpots = data_Count;				thisCrystal = theCrystal[baseCrystal];		GetOrientation(	xFormMatrix);	oldWavelength = wavelength;	wavelength = 12.4 / maxEnergy;	RofEwaldSph = 1 / wavelength;	reciprocalLatticeSize = (double)(RofEwaldSph);	maxZeroOrderDist = .2 * RofEwaldSph - .01;	anAxis = (Index*)D_new(Index);	anAxis->x = 0.0;	anAxis->y = 0.0;	anAxis->z = 1.0;			for(i = 0;i <= 3;i++){		maxIntensity[i] = 0.0;		maxRatio[i] = 0.001;	}		for(plotCrystal  = baseCrystal; plotCrystal <= 3&& gNoBreak ; plotCrystal++){			short  atom_Count;		if(!plotFlags[plotCrystal] || (plotCrystal > baseCrystal && baseCrystal > 0))			continue;		if(plotCrystal  == baseCrystal)			orNumber = 0;		else{			thisCrystal = theCrystal[plotCrystal];			pptOR = the_PPT_Info[plotCrystal- 1];			orNumber = pptOR->lastOR;		}		obj_Index->DoInit(false,thisCrystal);					if(!SetScatteringPosAIntensity(thisCrystal,&theUs,&theVs,&theWs,&theZs,&theBs,&atom_Count))goto FINISHLAUE;		theHandle = thisCrystal->AtomicScatteringSet( 1,theBeamFlag);		AtomicScatterForDFromSet(0,theHandle,theZs,atom_Count);		sF = TotalStructureFactor(0,0,0,theUs,theVs,theWs,atom_Count,theZs,theBs,thisCrystal->volume,10000,thisCrystal,&cAb);		cutOff = sF.r *  sF.r * .0001;		hmax = SizeofReciprocalLattice(1.,0.,0.,reciprocalLatticeSize,thisCrystal);		kmax = SizeofReciprocalLattice(0.,1.,0.,reciprocalLatticeSize,thisCrystal);		lmax = SizeofReciprocalLattice(0.,0.,1.,reciprocalLatticeSize,thisCrystal);		if(hmax > 40) hmax = 40;		if(kmax > 40) kmax = 40;		if(lmax > 40) lmax = 40;								for(orNum = 0 ; orNum <= orNumber&& gNoBreak ; orNum++){						GetPPTXFormMatrixBase(xForm,thisCrystal,orNum);			MatrixMultiply(xForm,xFormMatrix,xForm); 			ConverttoReciprocal(xForm,(double)thisCrystal->volume,true);			 												chx = xForm[0][0];			chy = xForm[1][0];			chz = xForm[2][0];						ckx = xForm[0][1];			cky = xForm[1][1];			ckz = xForm[2][1];						clx = xForm[0][2];			cly = xForm[1][2];			clz = xForm[2][2];																							thehMax = -hmax;			thekMax = -kmax;			thelMax = -lmax;			x = thehMax * chx;			y = thehMax * chy;			z = thehMax * chz;						for(obj_Index->h = thehMax ; obj_Index->h <= hmax && gNoBreak ; obj_Index->h++)			{				x1 = x + thekMax * ckx;				y1 = y + thekMax * cky;				z1 = z + thekMax * ckz;				for(obj_Index->k = thekMax; obj_Index->k <= kmax && gNoBreak ; obj_Index->k++)				{					x2 = x1 + thelMax * clx ;					y2 = y1 + thelMax * cly;					z2 = z1 + thelMax * clz;					for(obj_Index->l =  thelMax; obj_Index->l <= lmax && gNoBreak ; obj_Index->l++)					{												if( obj_Index->h == 0 && obj_Index->k == 0 && obj_Index->l == 0)							goto NEWSTART;						if(z2 < 0.0) goto NEWSTART;																			if(z2 <= 0.001){ 							theDist = sqrt(x2 * x2 + y2 * y2);							if(theDist <= maxZeroOrderDist){								theRecpWavelength = RofEwaldSph;								angle = 	10001;							}else goto NEWSTART;						}else{							distZ = (z2 * z2);							theDist = sqrt((x2 * x2) + (y2 * y2) + distZ);								obj_Index->x = x2;							obj_Index->y = y2;							obj_Index->z = z2;							angle = anAxis->GetTheXYZAngle(obj_Index);							aCos = cos(angle);														if(aCos == 0) goto NEWSTART;							theRecpWavelength = .5 * theDist / fabs(aCos);							if(theRecpWavelength > RofEwaldSph ) goto NEWSTART;							if(sqrt(distZ) >= theRecpWavelength * .8) goto NEWSTART; 													}												dH = (double)obj_Index->h;						dK = (double)obj_Index->k;						dL = (double)obj_Index->l;						AtomicScatterForDFromSet(theDist,theHandle,theZs,atom_Count);						sF = TotalStructureFactor(dH,dK,dL,theUs,theVs,theWs,atom_Count,theZs,theBs,thisCrystal->volume,1/theDist,thisCrystal,&cAb);						intens = Cabs(sF);						intens *= intens;						AllowBackground();						if(intens < cutOff)							goto NEWSTART;						CompareSpots(x2,y2,z2,&intens,sF,angle,theRecpWavelength,thisCrystal,plotCrystal);												if(theRecpWavelength / RofEwaldSph > maxRatio[plotCrystal]) maxRatio[plotCrystal] = theRecpWavelength / RofEwaldSph;						if(intens > maxIntensity[plotCrystal]) maxIntensity[plotCrystal] = intens;																NEWSTART:		x2 += clx;						y2 += cly;						z2 += clz;					}					x1 += ckx;					y1 += cky;					z1 += ckz;				}				x += chx;				y += chy;				z += chz;			}		}				KillPtr((Ptr)theUs);		thisCrystal->ClearAtomicScatSet(theHandle);	}	FINISHLAUE:	D_HLock((Handle)theDataHandle);	thisSpot = &((SpotInfoPtr)*theDataHandle)[numSpots];	anAxis->DoClose();	for(i = numSpots;i < data_Count;thisSpot++,i++){		if(thisSpot->flags != baseCrystal){			thisSpot->intensity /= (maxIntensity[thisSpot->flags] / the_PPT_Info[thisSpot->flags - 1]->intensity);		}else			thisSpot->intensity /= maxIntensity[thisSpot->flags];		thisSpot->intensity *= (.8 * thisSpot->angle / (RofEwaldSph * maxRatio[thisSpot->flags]));					}		D_HUnlock			((Handle)theDataHandle);	if(data_Length >  data_Count){		D_RecoverMemory		(theDataHandle,data_Size,(long)data_Count);	}	data_Length 	= 	data_Count;	wavelength = oldWavelength;}void LaueTransObj::CompareSpots(double x,double y,double z,double *intens,dcomplex sF,double angle,					double theRecpWavelength,Crystal *thisCrystal,short plotCrystal){	SpotInfoPtr thisSpot,theSpots;	double 			radius;	short				i;	D_HLock(theDataHandle);	thisSpot = &((SpotInfoPtr)*theDataHandle)[numSpots];	for(i = numSpots; i < data_Count && gNoBreak;i++,thisSpot++){				if(fabs(thisSpot->x - x) <= .01 && fabs(thisSpot->y - y) <= .01){			thisSpot->intensity += (float)*intens;			thisSpot->sF	=	Cadd(thisSpot->sF, sF);			thisSpot->x += x;			thisSpot->y += y;			*intens = thisSpot->intensity;			thisSpot->x /= 2;			thisSpot->y /= 2;			D_HUnlock(theDataHandle);			return;		}	}	D_HUnlock(theDataHandle);	theSpots = (SpotInfoPtr)*theDataHandle;	if(angle >= 10000){		if(data_Count >= data_Length){		ExpandMemory();				}		theSpots = (SpotInfoPtr)*theDataHandle;		thisSpot = &(theSpots[data_Count++]);		thisSpot->intensity = (float)*intens;		thisSpot->x =  (float)x;		thisSpot->y = (float)y;		thisSpot->z =  (float)z;		thisSpot->h = (short)obj_Index->h;		thisSpot->k = (short)obj_Index->k;		thisSpot->l = (short)obj_Index->l;		thisSpot->direction = false;		thisSpot->theCrystal = thisCrystal;		thisSpot->angle = 0.0;		thisSpot->flags = (long)(plotCrystal);		return;	}else{		angle = (PI - 2. * angle);		radius = fabs(theRecpWavelength * tan(angle));		radius /= sqrt(x * x + y * y);		x *= radius;		y *= radius; 	}	if(data_Count >= data_Length){		ExpandMemory();	}		theSpots = (SpotInfoPtr)*theDataHandle;	thisSpot = &(theSpots[data_Count++]);	thisSpot->intensity = (float)*intens;	thisSpot->x =  (float)x;	thisSpot->y = (float)y;	thisSpot->z =  (float)z;	thisSpot->h = (short)obj_Index->h;	thisSpot->k = (short)obj_Index->k;	thisSpot->l = (short)obj_Index->l;	thisSpot->direction = false;	thisSpot->theCrystal = thisCrystal;	thisSpot->angle = theRecpWavelength;	thisSpot->flags = (long)(plotCrystal);		}void	LaueTransObj::DoPictDraw(void){	double 		intensityRange,oldScaleFactor;	Rect 		theOval,oldOval;	long 		theX,theY;	SpotInfoPtr thisSpot;	short 		theIdent,i,theSize;	short 		hRes,vRes,hLim,vLim;	double		magComp;	double		aMaxSize;	inherited::DoPictDraw();				SetDrawEnviron();			obj_Index->DoInit(false,theCrystal[0]);		ScreenRes		(&hRes,&vRes);	hLim 		=  .5 * negWidth * hRes / 25.4;	vLim 		=  .5 * negHeight * vRes / 25.4;	aMaxSize 	= (short)expoTime + 2;	intensityRange 	= 1. / (double)dyRange;	oldScaleFactor 	= scaleFactor;	magComp         = projLengthScreen * magnification;		thisSpot 		= (SpotInfoPtr)*theDataHandle;	thisSpot++;	SetRect(&oldOval,0,0,0,0);		for(i = 1 ; i <= data_Count - 1; i++,thisSpot++){		if(thisSpot->angle == 0.0) continue;		theIdent = thisSpot->flags & CRYSTAL_MASK;		if(!plotFlags[theIdent] && theIdent <= 3 && i > 1) continue;		scaleFactor = (magComp / thisSpot->angle) ;				/*theX	= centerX + (short)(scaleFactor * thisSpot->x);		theY	= centerY - (short)(scaleFactor * thisSpot->y);*/		CalculateSpotLocation(&theX,&theY, thisSpot);				if(fabs((double)(theX - centerX)) > hLim || fabs((double)(theY - centerY)) > vLim){			SetRect(&oldOval,0,0,0,0);				thisSpot->spotRect = oldOval;		 continue; 		}		DMForeColor(theIdent + 10);					theSize = Round(aMaxSize * pow((double)thisSpot->intensity,intensityRange));		SetRect(&oldOval,-theSize,-theSize,theSize,theSize);			theOval = oldOval;		OffsetRect(&theOval,(short)theX,(short)theY);		thisSpot->spotRect = theOval;		if(greyFlag){			short theValue;			theValue = (short)(210 *  pow((double)thisSpot->intensity,intensityRange));			DMForeColor(theValue + 15);		}else{			DMForeColor(theIdent + 10);		}		dm_PaintOval(&theOval);					if(!gInColor){			DMForeColor(BLACK);				dm_FrameOval(&theOval);			DMForeColor(theIdent + 10);		}		 	}	scaleFactor = oldScaleFactor;	/*theSearchObj->PlotRings(scaleFactor,scaleFactor,centerX,centerY,0,500);*/	ClearDrawEnviron();		}Boolean	LaueTransObj::DefineSwitch(DialogPtr theDialog,short theSelect,Boolean *saveData){	switch(theSelect){			Boolean theFlag;			case -1:			case 2:				*saveData = false;				return true;				break;			case DLOG_ENTER_OR_CR:			case 1:				*saveData = true;					return true;				break;			case 5:			case 6:			case 7:			case 8:			case 9:			case 10:			case 11:				theFlag = GetBoolean(theDialog,theSelect);				theFlag = SwitchBoolean(theDialog,theSelect,theFlag);				break;			case ML_UPDATE_EVT:				DrawDialog(theDialog);			break;			default:				break;		}		return false;}void		LaueTransObj::ReadDefineDialog(DialogPtr theDialog,Boolean saveData){	short i;	short n = 5;	if(saveData){				/* Eric re-wrote this section July 1993 */		Boolean theFlag;		DoDefineOR( theDialog, false,_3_);		for(i = n ; i <= n + 3 ; i++){			theFlag = GetBoolean(theDialog,i);			if(plotFlags[i - n] < theFlag)changedFlag = true;			plotFlags[i - n] = theFlag;		}		if(greyFlag 			!= GetBoolean(theDialog,9)) newPictReq = true;		greyFlag 			= GetBoolean(theDialog,9);		if(showPeriods 	!= GetBoolean(theDialog,10))newPictReq = true;		showPeriods 	= GetBoolean(theDialog,10);		if(kikuchiLines 	!= GetBoolean(theDialog,11))newPictReq = true;		kikuchiLines 	= GetBoolean(theDialog,11);	}}void  LaueTransObj::SetDefineObj(DialogPtr theDialog){	short i,n = 5;	for(i = n; i <= n + 3;i++){		plotFlags[i-n] = SwitchBoolean(theDialog,i,!plotFlags[i-n]);	}	greyFlag = SwitchBoolean(theDialog,9,!greyFlag);	showPeriods = SwitchBoolean(theDialog,10,!showPeriods);	kikuchiLines = SwitchBoolean(theDialog,11,!kikuchiLines);	DrawDialog(theDialog);}#pragma segment Mainvoid LaueTransObj::DoSetUp(void){	DialogPtr 		theDialog;	short			theSelect;	Boolean			quit = false;	Boolean			saveData = false;			theDialog	= DM_GetNewDialog(11123,NUL,IN_FRONT);	SetDefineObj(theDialog);	while(!quit){		ModalDialog(TheFilterUPP,&theSelect);		switch(theSelect){			case ML_UPDATE_EVT:				BeginUpdate(theDialog);			UpdtDialog(theDialog,theDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(theDialog);			}			quit = DefineSwitch( theDialog,theSelect,&saveData);			EndUpdate(theDialog);			break;			default:			quit = DefineSwitch( theDialog,theSelect,&saveData);			break;		}			}	ReadDefineDialog( theDialog, saveData);	DM_DisposDialog(&theDialog);		return;}void LaueTransObj::DoDefine(void)//Override{	Boolean			calculateFlag;	MultiLog*		theMultiLog;	short				n = 0;		theMultiLog	= (MultiLog*)new(MultiLog);	theMultiLog->DoInit(265,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCBEDObjSetup,(HitProcPtr)ML_DoCBEDObjHit,(ReadProcPtr)ML_DoCBEDObjRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoBeamESetup,(HitProcPtr)ML_DoBeamEHit,(ReadProcPtr)ML_DoBeamERead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoBeamORSetup,(HitProcPtr)ML_DoBeamORHit,(ReadProcPtr)ML_DoBeamORRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoStageObjSetup,(HitProcPtr)ML_DoStageObjHit,(ReadProcPtr)ML_DoStageObjRead,(Ptr)stage);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[2]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[3]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[2]);		theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoFoilSetup,(HitProcPtr)ML_DoFoilHit,(ReadProcPtr)ML_DoFoilRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_SetMiscParam,(HitProcPtr)ML_SwitchMiscParam,(ReadProcPtr)ML_ReadMiscParam,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoDefaultSetup,(HitProcPtr)ML_DoDefaultHit,(ReadProcPtr)ML_DoDefaultRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoSFSetup,(HitProcPtr)ML_DoSFHit,(ReadProcPtr)ML_DoSFRead,(Ptr)this);		calculateFlag = theMultiLog->DoMultiLog();	theMultiLog->DoClose();		if(calculateFlag){		changedFlag = true;		reSizeFlag = true;		newPictReq = true;/*eliminated line dec 1992*/	} else if(screenPict == NUL  && gMyOffG == 0L){		CloseTheObject(theWindow);	}	return;}