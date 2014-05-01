//Macintosh HD:CodeWarrior� DR/3 Gold �:Metrowerks  C/C++ �:Projects:D.M. v2.0:Sources.Jim:Data Input/Def Sources:DA_DefineElectronObj.c#include	"Diffract_INCs.h"#include "defineElectronObj.h"#include "ML_BeamE.h"void DefineElecObj::DoInit(long	owner){		theOwner 		= owner;	SetWaveAndEnergy(120.);	negWidth 		= 90;	negHeight 		= 90.;	cameraConstant 	= 24.;	dyRange 		= 1.5;	expoTime 		= 1.0;	convergenceAngle = .1;	polarizedFlag 	= false;	charWaveLength 	= 3;	maxEnergy 		= 120.;	energy			= 120.;}void DefineElecObj::DoCopy(DefineElecObj*	theBeam){	theBeam->energy = energy;	theBeam->wavelength = wavelength;	theBeam->negWidth = negWidth;	theBeam->negHeight = negHeight;	theBeam->dyRange = dyRange;	theBeam->expoTime = expoTime;	theBeam->cameraConstant = cameraConstant;	theBeam->convergenceAngle = convergenceAngle;	theBeam->polarizedFlag = polarizedFlag;	theBeam->charWaveLength = charWaveLength;	theBeam->maxEnergy = maxEnergy;	((DiffractObject*)(theBeam->theOwner))->theBeamFlag = ((DiffractObject*)theOwner)->theBeamFlag;}Boolean DefineElecObj::DoBeamESwitch(DialogPtr theDialog,short theSelect,Ptr  thePtr,Boolean *saveData){	ML_BeamEParamsPtr theParams;	theParams = (ML_BeamEParamsPtr)thePtr;	switch(theSelect){			case REPLACE:				*saveData = true;				return true;			case CANCEL:			case CANCELCLICK: 				*saveData = false;				return true;			case LENGTHBUTTON:				SetLengthButton(theDialog,&theParams->cameraFlag);				break;			case POPUPMENUKV:				DoPopMenuComplete(theDialog,theParams->popUpMenu);				break;			case SETGLOBAL:				((DiffractObject*)theOwner)->theBeamFlag = theParams->theBeamFlag;				DoRead(theDialog,thePtr,true,false);				((DiffractObject*)theOwner)->DoBeamESetUp( theDialog,theParams, theParams->theBeamFlag);				switch(((DiffractObject*)theOwner)->theBeamFlag ){					case 1:						DoCopy(gDefault->theEBeam);						break;					case 2:						DoCopy(gDefault->theXRay);						break;					case 3:						DoCopy(gDefault->theIon);						break;				}				break;			default:				break;			case ML_UPDATE_EVT:			DrawDialog(theDialog);			break;		}		return false;}Boolean DefineElecObj::DoDefine(void){	short 			theSelect;	DialogPtr		theDialog;	Boolean 		quit = false;	Boolean			saveData;	ML_BeamEParamsPtr		 theParams;		theDialog = DM_GetNewDialog(E_DEF_DLOG,0L,(WindowPtr)-1L);	theParams = (ML_BeamEParamsPtr)D_NewPtr(sizeof(ML_BeamEParams));		theParams->theMenu  = (PopUpMenu*)D_new(PopUpMenu);	theParams->theMenu->Init(theDialog,BEAMTYPEMENU,72,((DiffractObject*)theOwner)->theBeamFlag);	theParams->theBeamFlag = ((DiffractObject*)theOwner)->theBeamFlag;		DoSetUp( theDialog,theParams);	while(!quit){		SystemTask();		ModalDialog(TheFilterUPP,&theSelect);		switch(theSelect){			case ML_UPDATE_EVT:							BeginUpdate(theDialog);					UpdtDialog(theDialog,theDialog->visRgn);					if(!gAppleEvtsOK){						HiliteOK(theDialog);					}					quit = DoBeamESwitch( theDialog, theSelect,(Ptr)theParams,&saveData);					EndUpdate(theDialog);					break;			default:				quit = DoBeamESwitch( theDialog, theSelect,(Ptr)theParams,&saveData);			break;		}	}	DoRead( theDialog,(Ptr)theParams, saveData,true);	theParams->popUpMenu->DoClose();//new	KillPtr((Ptr)theParams);//new	DM_DisposDialog(&theDialog);	return saveData;}void	DefineElecObj::DoPopMenuComplete(DialogPtr theDialog,PopUpMenu *popUpMenu){	Rect theRect;	short type;		charWaveLength 	= popUpMenu->DoPopUp();		popUpMenu		->	P_GetCItemText(gTheText);	SetWaveAndEnergy	(atof((p2cstr(pTheText))));	GetDItem			(theDialog,VOLTAGE,&type,&gTheHandle,&theRect);	sprintf				(gTheText,"%5.2f",energy);	SetIText			(gTheHandle,c2pstr(gTheText));}void	DefineElecObj::DoRead(DialogPtr theDialog,Ptr thePtr,Boolean saveData,Boolean closeDialog){	Handle theHandle;	short i,type;	Rect theRect;	float	theValue,old;	Boolean cameraFlag;	ML_BeamEParamsPtr theParams;	theParams = (ML_BeamEParamsPtr)thePtr;		cameraFlag = theParams->cameraFlag;	if(closeDialog){		KillDialog();	}	theParams->popUpMenu->DoClose();	if(!saveData) return;		for(i = VOLTAGE; i <= EXPO;i++){		GetDItem(theDialog,i,&type,&theHandle,&theRect);		GetIText(theHandle,pTheText);		theValue = atof(p2cstr(pTheText));		switch(i)		{			case VOLTAGE:				old = energy;				SetWaveAndEnergy(theValue);				if(fabs(energy - old) > .01)				((DiffractObject*)theOwner)->changedFlag = true;				break;			case NEGW:				negWidth = theValue;				break;			case NEGH:				negHeight = theValue;				break;			case CAMCON:				if(!cameraFlag){					ReturnCameraConstant(&theValue);				}				if(cameraConstant != theValue)					((DiffractObject*)theOwner)->newPictReq = true;				cameraConstant = theValue;				break;			case CONANG:			if(fabs(convergenceAngle - theValue) > .01)				((DiffractObject*)theOwner)->newPictReq = true;				convergenceAngle = theValue;								break;			case DRANG:				if(fabs(dyRange - theValue) > .01)				((DiffractObject*)theOwner)->newPictReq = true;				dyRange = theValue;				break;			case EXPO:				if(fabs(expoTime - theValue) > .01)					((DiffractObject*)theOwner)->newPictReq = true;				expoTime = theValue;				break;								}	}	if(maxEnergy < energy)maxEnergy = energy;	if(fabs(((DiffractObject*)theOwner)->energy	- 	energy) > .01)((DiffractObject*)theOwner)->changedFlag = true;	if(fabs(((DiffractObject*)theOwner)->wavelength	- 	wavelength) > .01)((DiffractObject*)theOwner)->changedFlag = true;	if(fabs(((DiffractObject*)theOwner)->cameraConstant -	cameraConstant) > .01)((DiffractObject*)theOwner)->newPictReq = true;	if(fabs(((DiffractObject*)theOwner)->dyRange 	-	dyRange) > .01)((DiffractObject*)theOwner)->newPictReq = true;	if(fabs(((DiffractObject*)theOwner)->expoTime		- 	expoTime) > .01)((DiffractObject*)theOwner)->newPictReq = true;	if(fabs(((DiffractObject*)theOwner)->negWidth 	-	negWidth) > .01)((DiffractObject*)theOwner)->newPictReq = true;	if(fabs(((DiffractObject*)theOwner)->negHeight 	-	negHeight) > .01)((DiffractObject*)theOwner)->newPictReq = true;	if(fabs(((DiffractObject*)theOwner)->maxEnergy 	-  maxEnergy) > .01)((DiffractObject*)theOwner)->changedFlag = true;	if(fabs(((DiffractObject*)theOwner)->convergenceAngle -   convergenceAngle) > .01)((DiffractObject*)theOwner)->newPictReq = true;			((DiffractObject*)theOwner)->energy				= 	energy;	((DiffractObject*)theOwner)->wavelength			= 	wavelength;	((DiffractObject*)theOwner)->cameraConstant 	= 	cameraConstant;	((DiffractObject*)theOwner)->dyRange 			= 	dyRange;	((DiffractObject*)theOwner)->expoTime			= 	expoTime;	((DiffractObject*)theOwner)->negWidth 			= 	negWidth;	((DiffractObject*)theOwner)->negHeight 			= 	negHeight;	((DiffractObject*)theOwner)->maxEnergy 			=   maxEnergy;	((DiffractObject*)theOwner)->convergenceAngle 	=   convergenceAngle;		return ;}void	DefineElecObj::DoClose(void){	D_delete(this);}void	DefineElecObj::DoSetUp(DialogPtr theDialog,ML_BeamEParamsPtr  theParams)/*change here, Mar 1995*/{	short i;	double theValue;		theParams->cameraFlag = true;/*change here, Mar 1995*/	for(i = VOLTAGE; i <= EXPO;i++){		GetDItem(theDialog,i,&gType,&gTheHandle,&gTheRect);		switch(i)		{			case VOLTAGE:				theValue = SetVOLTAGE();				break;			case NEGW:				theValue = (double)negWidth;				break;			case NEGH:				theValue = (double)negHeight;				break;			case CAMCON:				theValue = (double)cameraConstant;				break;			case CONANG:				theValue = (double)convergenceAngle;				break;			case DRANG:				theValue = (double)dyRange;				break;			case EXPO:				theValue = (double)expoTime;				break;		}		sprintf(gTheText,"%5.2f",theValue);		SetIText(gTheHandle,c2pstr(gTheText));	}	/*GetDItem(theDialog,POLARIZED,&gType,&gTheHandle,&gTheRect);	SetCtlValue((ControlHandle)gTheHandle,polarizedFlag);	*/	theParams->popUpMenu = (PopUpMenu*)D_new(PopUpMenu);	InitPopUpMenu(theDialog,theParams->popUpMenu);	SetFluff(theDialog,&(theParams->cameraFlag));	DrawDialog(theDialog);}float DefineElecObj::SetVOLTAGE(void){	return energy;}void DefineElecObj::SetFluff(DialogPtr theDialog,Boolean *cameraFlag){	*cameraFlag = true;	HideDItem(theDialog,24);	HideDItem(theDialog,25);	HideDItem(theDialog,26);	GetDItem(theDialog,LENGTHBUTTON,&gType,&gTheHandle,&gTheRect);	sprintf(gTheText,"Camera Constant");	SetCTitle((ControlHandle)gTheHandle,c2pstr(gTheText));}void DefineElecObj::InitPopUpMenu(DialogPtr theDialog,PopUpMenu *popUpMenu){		popUpMenu->Init(theDialog,POPUPMENUKV,111,charWaveLength);	popUpMenu->SetPopUp();}void DefineElecObj::SetLengthButton(DialogPtr theDialog,Boolean *buttonFlag){float theDistance;	GetDItem(theDialog,CAMCON,&gType,&gTheHandle,&gTheRect);	GetIText(gTheHandle,pTheText);	theDistance = atof((p2cstr(pTheText)));		GetDItem(theDialog,VOLTAGE,&gType,&gTheHandle,&gTheRect);	GetIText(gTheHandle,pTheText);	SetWaveAndEnergy(atof((p2cstr(pTheText))));	if(*buttonFlag){		GetDItem(theDialog,LENGTHBUTTON,&gType,&gTheHandle,&gTheRect);		sprintf(gTheText,"Camera Length");		SetCTitle((ControlHandle)gTheHandle,c2pstr(gTheText));		GetDItem(theDialog,LENGTHUNITS,&gType,&gTheHandle,&gTheRect);		sprintf(gTheText,"mm");		SetIText(gTheHandle,c2pstr(gTheText));		*buttonFlag = false;		ReturnCameraLength(&theDistance);	}else{		GetDItem(theDialog,LENGTHBUTTON,&gType,&gTheHandle,&gTheRect);		sprintf(gTheText,"Camera Constant");		SetCTitle((ControlHandle)gTheHandle,c2pstr(gTheText));		GetDItem(theDialog,LENGTHUNITS,&gType,&gTheHandle,&gTheRect);		sprintf(gTheText,"mm-�");		SetIText(gTheHandle,c2pstr(gTheText));		ReturnCameraConstant(&theDistance);		*buttonFlag = true;	}	GetDItem(theDialog,CAMCON,&gType,&gTheHandle,&gTheRect);	sprintf(gTheText,"%6.4f",theDistance);	SetIText(gTheHandle,c2pstr(gTheText));	SelIText(theDialog,CAMCON,0,32767);}void DefineElecObj::SetWaveAndEnergy(float theValue){	double re;	energy = theValue;	if(energy == 0){ wavelength = 0; return;}	re = (1000. * energy);			wavelength =  12.264 / (sqrt(re) * sqrt((1. + (0.00097848 * energy))));}float DefineElecObj::ReturnWavelength(float theEnergy){	double re;	if(theEnergy == 0){ 		return 0.0;}	re = (1000. * theEnergy);			return 12.26 / (sqrt(re) * sqrt((1. + (0.0009788 * theEnergy))));}void DefineElecObj::ReturnCameraLength(float *theValue){		if(fabs(wavelength) > 0.0)		*theValue /= (wavelength);}void DefineElecObj::ReturnCameraConstant(float *theValue){		*theValue *= (wavelength);}void DefineElecObj::KillDialog(void){//	DeleteMenu(111);}PicHandle DefineElecObj::DisplayPict(Rect *theRect,short horiz,short vert){	short 		h,v,i,width;	PicHandle	thePict;	Rect			frameRect;		TextFont		(systemFont);	PenNormal		();	sprintf			(gTheText,"Log Dynamic Range = %d X %d �",dyRange);	c2pstr			(gTheText);	width 			= 	StringWidth((unsigned char*)gTheText);	SetRect			(theRect,2,2,width + 5,225);	OffsetRect		(theRect,horiz,vert);	//InsetRect		(theRect,-2,-2);	frameRect 	= 	*theRect;			thePict 	= 	OpenPicture(theRect);			v = 25;		for(i = 1 ; i <= 8 ; i++){		switch(i){			case 1:				sprintf(gTheText,"Energy = %5.2f kV",energy);				break;			case 2:				sprintf(gTheText,"lambda = %5.4f �",wavelength);				break;			case 3:				sprintf(gTheText,"negative size = %d X %d mm",(short)negWidth,(short)negHeight);				break;			case 4:				sprintf(gTheText,"Camera Constant = %5.2f mm-�",cameraConstant);				break;			case 5:				sprintf(gTheText,"Convergence Angle = %5.3f �-1",convergenceAngle);				break;			case 6:				sprintf(gTheText,"Log Dynamic Range = %5.1f",dyRange);				break;			case 7:				sprintf(gTheText,"Exposure Time = %5.2f s",expoTime);				break;			case 8:				if(polarizedFlag)					sprintf(gTheText,"Polarized");				else goto FINISH;				break;					}				h = 5;		MoveTo(h + horiz,v + vert);		DrawDiffractString(c2pstr(gTheText));		v += 20;	}FINISH:	PenSize(2,2);	gCurrentObj->DMForeColor(BLACK);	PenNormal();	FrameRect(&frameRect);		ClosePicture();	return(thePict);}void DefineElecObj::ReadBeam(void){	energy = gTheFile->ReadFloat();	wavelength = gTheFile->ReadFloat();	negWidth = gTheFile->ReadFloat();	negHeight = gTheFile->ReadFloat();	cameraConstant = gTheFile->ReadFloat();	convergenceAngle = gTheFile->ReadFloat();	dyRange = gTheFile->ReadFloat();	expoTime = gTheFile->ReadFloat();	polarizedFlag = gTheFile->ReadBoolean();	charWaveLength = gTheFile->ReadShort();}void DefineElecObj::WriteBeam(void){	gTheFile->WriteFloat(energy);	gTheFile->WriteFloat(wavelength);	gTheFile->WriteFloat(negWidth);	gTheFile->WriteFloat(negHeight);	gTheFile->WriteFloat(cameraConstant);	gTheFile->WriteFloat(convergenceAngle);	gTheFile->WriteFloat(dyRange);	gTheFile->WriteFloat(expoTime);	gTheFile->WriteBoolean(polarizedFlag);	gTheFile->WriteShort(charWaveLength);}