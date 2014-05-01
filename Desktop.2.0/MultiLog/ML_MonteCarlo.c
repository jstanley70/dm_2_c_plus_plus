#include	"Diffract_INCs.h"#include "ML_MonteCarlo.h"#include "MonteCarloObj.h"#include "ML_SADObj.h"#include "ML_Crystal.h"#include "ML_PPT_Info.h"#include "ML_BeamOrientation.h"#include  "DiffractometerObj.h"#include "ML_CrystalFace.h"#include "ML_Default.h"#include "ML_Misc_Plot.h"void	ML_DoMonteCarloObjSetup(Ptr	owner,DialogPtr theDialog,Ptr *theParams){	MonteCarloObj*			theOwner;	theOwner					= (MonteCarloObj*)owner;	theOwner->SetDefineObj( theDialog);	*theParams = (Ptr)0L;	DrawDialog(theDialog);}void	ML_DoMonteCarloObjHit(Ptr	owner,DialogPtr theDialog, short theSelect,Ptr theParams){	MonteCarloObj*		theOwner;	Boolean		saveData;		theOwner		= (MonteCarloObj*)owner;	theParams		= (Ptr)NUL;             // Eliminates warning;  theParams is required		theOwner->DefineSwitch( theDialog, theSelect,&saveData);}void	ML_DoMonteCarloObjRead(Ptr owner,DialogPtr theDialog,Ptr theParams,Boolean saveData){	MonteCarloObj*		theOwner;	theOwner		= (MonteCarloObj*)owner;	theParams		= (Ptr)NUL; 	theOwner->ReadDefineDialog( theDialog, saveData);}Boolean	MonteCarloObj::DefineSwitch(DialogPtr theDialog,short theSelect,Boolean *saveData){	double theValue;	switch(theSelect){			case -1:			case 2:				*saveData = false;				return true;				break;			case DLOG_ENTER_OR_CR:			case 1:				*saveData = true;					return true;				break;			case _3_:			case _4_:			case _5_:			case _6_:			case _7_:				SwitchBoolean(theDialog,theSelect,GetBoolean(theDialog,theSelect));				break;			case _8_:				theValue = GetItemValue( theDialog,_8_);				theValue /= noOfChannels;				SetItemValue(theDialog,_12_,theValue,4);				break;			case _12_:				theValue = GetItemValue( theDialog,_12_);				noOfChannels = GetItemValue( theDialog,_8_) / theValue;				break;			case _14_:				SwitchBoolean(theDialog,theSelect,GetBoolean(theDialog,theSelect));				break;			default:				break;			case ML_UPDATE_EVT:			DrawDialog(theDialog);			break;					}		return false;}void  MonteCarloObj::SetDefineObj(DialogPtr theDialog){	short i;	double theValue;	noOfChannels = theFoilThickness / channelWidth;		for(i = _3_; i <= _7_;i++){		GetDItem(theDialog,i,&gType,&gTheHandle,&gTheRect);		SetCtlValue((ControlHandle)gTheHandle,plotFlags[i - _3_]);	}	for(i = _8_; i <= _13_;i++){		switch(i){			case _8_:				theValue = theFoilThickness / 10;				break;						case _9_:				theValue = ionFinalCutoffEnergy;				break;			case _10_:				theValue = stoppingCorrection;				break;			case _11_:				theValue = numberOfIons;				break;			case _12_:				theValue = channelWidth / 10;				break;			case _13_:				theValue = channel1Start / 10;				break;			default:				break;		}		SetItemValue(theDialog,i,theValue,6);	}	SwitchBoolean(theDialog,_14_,!gridFlag);}void	MonteCarloObj::DoDefine(void){	Boolean			calculateFlag;	MultiLog*		theMultiLog;	short				n = 0;		theMultiLog	= (MultiLog*)new(MultiLog);	theMultiLog->DoInit(263,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoMonteCarloObjSetup,(HitProcPtr)ML_DoMonteCarloObjHit,(ReadProcPtr)ML_DoMonteCarloObjRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoBeamESetup,(HitProcPtr)ML_DoBeamEHit,(ReadProcPtr)ML_DoBeamERead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[2]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[3]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_SetMiscParam,(HitProcPtr)ML_SwitchMiscParam,(ReadProcPtr)ML_ReadMiscParam,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoDefaultSetup,(HitProcPtr)ML_DoDefaultHit,(ReadProcPtr)ML_DoDefaultRead,(Ptr)this);	calculateFlag = theMultiLog->DoMultiLog();	theMultiLog->DoClose();		if(calculateFlag  && screenPict == NUL  && gMyOffG == 0L){		changedFlag = true;		reSizeFlag = true;		newPictReq = true;/*eliminated line dec 1992*/	} else if(screenPict == NUL  && gMyOffG == 0L){		CloseTheObject(theWindow);	}	return;}void		MonteCarloObj::ReadDefineDialog(DialogPtr theDialog,Boolean saveData){	short i;	double oldValue;	if(saveData){				/* Eric re-wrote this section July 1993 */		SetTheCursor(WATCH_CURSOR);		gridFlag = GetBoolean(theDialog,_14_);		for(i = _3_; i <= _7_;i++){			GetDItem(theDialog,i,&gType,&gTheHandle,&gTheRect);			if(GetCtlValue((ControlHandle)gTheHandle) == 0){				if(plotFlags[i - _3_] == true)changedFlag = true;				plotFlags[i - _3_] = false;			} else {				if(plotFlags[i - _3_] == false)changedFlag = true;				plotFlags[i - _3_] = true;			}		}						oldValue 		= 	theFoilThickness / 10;		theFoilThickness = GetItemValue( theDialog,_8_);		if(fabs(oldValue - theFoilThickness) > 1)changedFlag = true;		theFoilThickness *= 10;				oldValue 		= 	ionFinalCutoffEnergy;		ionFinalCutoffEnergy = GetItemValue( theDialog,_9_);		if(fabs(oldValue - ionFinalCutoffEnergy) > 1)changedFlag = true;				oldValue 		= 	stoppingCorrection;		stoppingCorrection = GetItemValue( theDialog,_10_);		if(fabs(oldValue - stoppingCorrection) > .01)changedFlag = true;				oldValue 		= 	numberOfIons;		numberOfIons = GetItemValue( theDialog,_11_);		if(fabs(oldValue - numberOfIons) > 1)changedFlag = true;				oldValue 		= 	channelWidth / 10;		channelWidth = GetItemValue( theDialog,_12_);				if(fabs(oldValue - channelWidth) > .01)changedFlag = true;		channelWidth	*= 10;				oldValue 		= 	channel1Start / 10;		channel1Start = GetItemValue( theDialog,_13_);		if(fabs(oldValue - channel1Start) > .01)changedFlag = true;				channel1Start	*= 10;		baseCrystal = 0;		while(!plotFlags[baseCrystal] && baseCrystal < 5)baseCrystal++;		setCrystal = theCrystal[baseCrystal];				}}void MonteCarloObj::DoSetUp(void){	DialogPtr 		theDialog;	short			theSelect;	Boolean			quit = false;	Boolean			saveData = false;		theDialog	= DM_GetNewDialog(350,NUL,IN_FRONT);		SetDefineObj( theDialog);	while(!quit){		ModalDialog(TheFilterUPP,&theSelect);		switch(theSelect){			case ML_UPDATE_EVT:				BeginUpdate(theDialog);			UpdtDialog(theDialog,theDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(theDialog);			}			quit = DefineSwitch( theDialog, theSelect,&saveData);			EndUpdate(theDialog);			break;			default:			quit = DefineSwitch( theDialog, theSelect,&saveData);			break;			}			}	ReadDefineDialog( theDialog, saveData);	DM_DisposDialog(&theDialog);		return;}