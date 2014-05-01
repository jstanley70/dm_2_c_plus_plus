#include "ML_BeamOrientation.h"#include	"Diffract_INCs.h"void	ML_DoBeamORSetup(Ptr	owner,DialogPtr theDialog,Ptr	*paramBlock){	ML_BeamORParamsPtr		theMLParms;	DiffractObject*			theOwner;	double				sL,sZ;	theOwner					= (DiffractObject*)owner;	theMLParms					= (ML_BeamORParamsPtr)D_NewPtr(sizeof(ML_BeamORParams));	theMLParms->thisCrystal	=	theOwner->theCrystal[theOwner->baseCrystal];	theMLParms->theIndex	=	(Index*)D_new(Index);	theMLParms->aIndex		=	(Index*)D_new(Index);	theMLParms->bIndex		=	(Index*)D_new(Index);	theMLParms->cIndex		=	(Index*)D_new(Index);	theMLParms->method		=	(PopUpMenu*)D_new(PopUpMenu);	theMLParms->oldIr		=	theOwner->calcIrrational;	theMLParms->theFoil		=	theOwner->foilParallel;	theMLParms->x 			=	theOwner->rotationAngle;	theMLParms->y 			=	theOwner->offLaueDistance;	theMLParms->z			=	0;	theMLParms->theFoil 	= 	SwitchBoolean(theDialog,ML_FOIL_PARALLEL,!theOwner->foilParallel);	theMLParms->method			->Init(theDialog,ML_METHOD_B,38,theMLParms->oldIr);	theOwner->theZoneAxis		->DoCopy(theMLParms->theIndex);	theOwner->theRotationAxis	->DoCopy(theMLParms->aIndex);	theOwner->g_Vector		->DoCopy(theMLParms->bIndex);	theMLParms->theIndex->theCrystal = theMLParms->thisCrystal;	theMLParms->aIndex->theCrystal = theMLParms->thisCrystal;	theMLParms->bIndex->theCrystal = theMLParms->thisCrystal;	theOwner->SetForZone(theDialog,theMLParms->oldIr,theMLParms->theIndex,theMLParms->aIndex,theMLParms->bIndex,	theMLParms->x,theMLParms->y);			SetItemValue(theDialog,ML_W_VALUE,theOwner->CalculateW_OffBragg(theMLParms->theIndex,theMLParms->aIndex,theMLParms->bIndex, theMLParms->x, theMLParms->y, theMLParms->z, theMLParms->oldIr,&sZ,&sL),3);	SetItemValue(theDialog,ML_EXTINCT_B,(theMLParms->bIndex->TheExtinctionDist(1/theOwner->wavelength, 1,  theOwner->theBeamFlag)),1);	*paramBlock = (Ptr)theMLParms;}void		ML_DoBeamORHit(Ptr	owner,DialogPtr theDialog, short theSelect,	Ptr	paramBlock){	DiffractObject *theOwner;	Boolean			saveData;	theOwner = (DiffractObject*)owner;	theOwner->DoBeamORSwitch(theDialog,theSelect,(ML_BeamORParamsPtr)paramBlock,&saveData);	return ;}void		ML_DoBeamORRead(Ptr owner,DialogPtr theDialog,Ptr	paramBlock,Boolean saveData){	ML_BeamORParamsPtr		theMLParms;	DiffractObject*			theOwner;		theMLParms		= (ML_BeamORParamsPtr)paramBlock;	theOwner		= (DiffractObject*)owner;	theOwner->foilParallel = theMLParms->theFoil;		theOwner->ReadBeamORData(theDialog, saveData,theMLParms->oldIr,&theMLParms->x,&theMLParms->y);			theMLParms->theIndex->DoClose();	theMLParms->aIndex->DoClose();	theMLParms->bIndex->DoClose();	theMLParms->cIndex->DoClose();	theMLParms->method->DoClose();	KillPtr(paramBlock);}Boolean DiffractObject::DoBeamORSwitch(DialogPtr theDialog,short theSelect,ML_BeamORParamsPtr theMLParams,Boolean *saveData){		double 			matrix[3][3],oldMatrix[3][3];	short 			theResult;	Index			*theIndex,*aIndex,*bIndex,*cIndex;	double			x,y,z,sZ,sL;	PopUpMenu		*method;	short			oldIr;	Boolean			theFoil,theChangedFlag = false;	Crystal			*thisCrystal;	Boolean			quit;	Boolean			resetBeam;		thisCrystal =	theMLParams->thisCrystal;	theIndex = theMLParams->theIndex;	aIndex 	=	theMLParams->aIndex;	bIndex	=	theMLParams->bIndex;	cIndex	=	theMLParams->cIndex;	method	=	theMLParams->method;	oldIr		=	theMLParams->oldIr;	theFoil	=	theMLParams->theFoil;	x				=	theMLParams->x ;	y				=	theMLParams->y;	z				=	theMLParams->z;	resetBeam = theMLParams->resetBeam;	quit		=	false;	*saveData  = false;	switch(theSelect){			case -1:			case ML_BEAM_CANCEL:				quit = true;				*saveData  = false;				break;			case ML_BEAM_OK:											ReadBeamORData(theDialog,resetBeam, oldIr, &x, &y);				cIndex->x = x;				cIndex->y = y;				cIndex->z = z;								*saveData = true;				quit = true;				break;												case ML_TOWARD:				if(oldIr >= 5){					bIndex->GetIndex(theDialog,ML_TOWARD);					bIndex->SetIndex(theDialog,ML_GVECTOR);				}				break;			case ML_FOIL_PARALLEL:				theFoil = SwitchBoolean(theDialog,ML_FOIL_PARALLEL,theFoil);				break;			case ML_ZONE_AXIS:				GetDialogValues(theDialog,oldIr,theIndex,aIndex,bIndex,&x,&y);				if(oldIr <= 2){					theIndex->x = theIndex->h;					theIndex->y = theIndex->k;					theIndex->z = theIndex->l;				}				break;			case ML_MATRIX_DISPLAY:				GetDialogValues(theDialog,oldIr,theIndex,aIndex,bIndex,&x,&y);				GetOrientation		(oldMatrix);				z = GetItemValue(theDialog,ML_HOR_ROT);				CalculateOrientation(theIndex,aIndex,bIndex,x,y,z,0,oldIr,matrix);				SetOrientation		(matrix);//the OrientationMatrixDialogBox() expects to use the stored oreintation matrix				OrientationMatrixDialogBox();				SetOrientation		(oldMatrix);				SetPort				(theDialog);				DrawDialog			(theDialog);				method->SetPopUp();				break;			case ML_METHOD_B:				method->DoPopUp();				theResult = method->lastResult;				//if(theResult != oldIr){					float azi = 0;					float xf,yf;					//get the old orientation values and convert to new method					GetDialogValues(theDialog,oldIr,theIndex,aIndex,bIndex,&x,&y);//get values					xf = (float)x;					yf = (float)y;					ConvertToORType(oldIr,theResult,theIndex,bIndex,(float*)&xf,(float*)&yf);//this converts from method 1 or 5 to any method					oldIr = theResult;					x = (double)xf;					y = (double)yf;					SetForZone(theDialog,oldIr,theIndex,aIndex,bIndex,x,y);//sets new values into the dialog box									//}else					break;										default:				GetDialogValues(theDialog,oldIr,theIndex,aIndex,bIndex,&x,&y);				z = GetItemValue(theDialog,ML_HOR_ROT);				CalculateOrientation(theIndex,aIndex,bIndex,x,y,z,0,oldIr,matrix);				theIndex->DoCopy(cIndex);				cIndex->x = cIndex->y = 0; cIndex->z = 1;				InvertMatrix(matrix);				cIndex->GetTheUVWFromXYZ(matrix);				if(!cIndex->direction){					double H,K,L;					H = cIndex->x;					K = cIndex->y;					L = cIndex->z;					cIndex->DirOfPlaneNormal(&H,&K,&L);					cIndex->x = H;					cIndex->y = K;					cIndex->z = L;				}				cIndex->NormalizeToGreatestXYZ();				sprintf(gTheText,"[%6.5f , %6.5f , %6.5f ]",cIndex->x,cIndex->y,cIndex->z);				SetItemText( theDialog, ML_BEAM_IRRATION,c2pstr(gTheText));				SetItemValue(theDialog,ML_W_VALUE,CalculateW_OffBragg(theIndex,aIndex,bIndex, x, y, z, oldIr,&sZ,&sL),3);				SetItemValue(theDialog,ML_EXTINCT_B,(bIndex->TheExtinctionDist(1/wavelength, 1,  theBeamFlag)),1);					break;			case ML_UPDATE_EVT:			DrawDialog(theDialog);			break;		}	theMLParams->thisCrystal	= thisCrystal;	theMLParams->theIndex = theIndex;	theMLParams->aIndex 	= aIndex;	theMLParams->bIndex	=	bIndex;	theMLParams->cIndex	=	cIndex;	theMLParams->method	=	method;	theMLParams->oldIr		=	oldIr;	theMLParams->theFoil	=	theFoil;	theMLParams->x			=	x ;	theMLParams->y			=	y;	theMLParams->z			=	z;	theMLParams->resetBeam = resetBeam;	return quit;}