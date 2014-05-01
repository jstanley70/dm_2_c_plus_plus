#include	"AngleBetDirObj.h"#include "Diffract_INCs.h"#include "ML_Crystal.h"#include "ML_AngleTab.h"#include "ML_AngleDir.h"#include "ML_Crystal.h"#include "ML_PPT_Info.h"#include "ML_Default.h"#include "ML_Misc_Plot.h"void	ML_DoAngleDirObjSetup(Ptr	owner,DialogPtr theDialog,Ptr *theParams){	AngleBetDirObj*			theOwner;	ML_AngleTabObjParmPtr	thePtr;	theOwner					= (AngleBetDirObj*)owner;	thePtr	=	(ML_AngleTabObjParmPtr)D_NewPtr(sizeof(ML_AngleTabObjParm));	theOwner->SetDefineObjAng( theDialog,thePtr);	*theParams = (Ptr)thePtr;}void	ML_DoAngleDirObjHit(Ptr	owner,DialogPtr theDialog, short theSelect,Ptr theParams){	AngleBetDirObj*		theOwner;	Boolean		saveData;	ML_AngleTabObjParmPtr	thePtr;		theDialog 	= theDialog;//warning	thePtr		=	(ML_AngleTabObjParmPtr)theParams;	theOwner	= (AngleBetDirObj*)owner;		theOwner->DefineSwitchAng(  theSelect,thePtr,&saveData);}void	ML_DoAngleDirObjRead(Ptr owner,DialogPtr theDialog,Ptr theParams,Boolean saveData){	AngleBetDirObj*		theOwner;	ML_AngleTabObjParmPtr	thePtr;	thePtr	=	(ML_AngleTabObjParmPtr)theParams;	theOwner		= (AngleBetDirObj*)owner;	theOwner->ReadDefineDialogAng( theDialog,thePtr, saveData);	}void AngleBetDirObj::SetDefineObjAng(DialogPtr theDialog,ML_AngleTabObjParmPtr theParams){		refDir	->	SetIndex(theDialog,_3_);		SetItemValue(theDialog,_4_,dirRange,0);	SetItemValue(theDialog,_5_,angle,2);	SetItemValue(theDialog,_6_,angleRange,2);		theParams->theMenu 	= 	(PopUpMenu*)D_new(PopUpMenu);	theParams->theMenu		->	Init(theDialog,_7_,52,crystalNum + 1);	theParams->theMenu2 	= 	(PopUpMenu*)D_new(PopUpMenu);	theParams->theMenu2	->	Init(theDialog,_8_,52,crystalNum2 + 1);}Boolean AngleBetDirObj::DefineSwitchAng(short  theSelect,ML_AngleTabObjParmPtr theParams,Boolean *saveData){	switch(theSelect){		case -1:		case 2:			*saveData = false;			return true;		case DLOG_ENTER_OR_CR:		case 1:			*saveData = true;			return true;			case _7_:				theParams->theMenu->DoPopUp();				theParams->theMenu2->lastResult = theParams->theMenu->lastResult;				theParams->theMenu2->SetPopUp();				break;			case _8_:				theParams->theMenu2->DoPopUp();				break;			case ML_UPDATE_EVT:			break;			default:				break;		}		return false;}void	AngleBetDirObj::ReadDefineDialogAng(DialogPtr theDialog,ML_AngleTabObjParmPtr theParams,Boolean saveData){	if(saveData){		short h,k,l;		valid 		= 	true;		if(crystalNum != theParams->theMenu->lastResult - 1){			changedFlag = true;			crystalNum  = theParams->theMenu->lastResult - 1;		}		if(crystalNum2 != theParams->theMenu2->lastResult - 1){			changedFlag = true;			crystalNum2  = theParams->theMenu2->lastResult - 1;		}		h = refDir->h;k = refDir->k,l = refDir->l;		refDir		->	GetIndex(theDialog,_3_);		if(h != refDir->h || k != refDir->k || l != refDir->l)changedFlag = true;		if(dirRange != (short)GetItemValue(theDialog,_4_))newPictReq = true;		if(dirRange < (short)GetItemValue(theDialog,_4_))changedFlag = true;		dirRange	=	(short)GetItemValue(theDialog,_4_);		if(fabs(angle - GetItemValue(theDialog,_5_)) > .01)changedFlag = true;		angle		= 	GetItemValue(theDialog,_5_);				if(angleRange > GetItemValue(theDialog,_6_))changedFlag = true;		angleRange	= 	GetItemValue(theDialog,_6_);				setCrystal = theCrystal[crystalNum];		baseCrystal = crystalNum;	}	theParams->theMenu->DoClose();	theParams->theMenu2->DoClose();	KillPtr((Ptr)theParams);}void	AngleBetDirObj::DoDefine(void){	Boolean			calculateFlag;	MultiLog*		theMultiLog;	short				n = 0;		theMultiLog	= (MultiLog*)new(MultiLog);	theMultiLog->DoInit(259,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoAngleDirObjSetup,(HitProcPtr)ML_DoAngleDirObjHit,(ReadProcPtr)ML_DoAngleDirObjRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[2]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[3]);		theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoPPTSetup,(HitProcPtr)ML_DoPPTHit,(ReadProcPtr)ML_DoPPTRead,(Ptr)the_PPT_Info[2]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_SetMiscParam,(HitProcPtr)ML_SwitchMiscParam,(ReadProcPtr)ML_ReadMiscParam,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoDefaultSetup,(HitProcPtr)ML_DoDefaultHit,(ReadProcPtr)ML_DoDefaultRead,(Ptr)this);	calculateFlag = theMultiLog->DoMultiLog();	theMultiLog->DoClose();		if(calculateFlag   && screenPict == NUL  && gMyOffG == 0L){		changedFlag = true;		reSizeFlag = true;		newPictReq = true;/*eliminated line dec 1992*/	} else if(screenPict == NUL  && gMyOffG == 0L){		CloseTheObject(theWindow);	}	return ;}#define TABLE_DIALOG 502void  AngleBetDirObj::DoDefineSetUp(void){	DialogPtr 		theDialog;	short			theSelect;	Boolean			quit = false;	Boolean			saveData;	ML_AngleTabObjParmPtr	thePtr;	thePtr	=	(ML_AngleTabObjParmPtr)D_NewPtr(sizeof(ML_AngleTabObjParm));	theDialog	= DM_GetNewDialog(TABLE_DIALOG,NUL,IN_FRONT);	SetDefineObjAng( theDialog,thePtr);	while(!quit){		ModalDialog(TheFilterUPP,&theSelect);		switch(theSelect){			case ML_UPDATE_EVT:				BeginUpdate(theDialog);			UpdtDialog(theDialog,theDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(theDialog);			}			quit = DefineSwitchAng( theSelect,thePtr,&saveData);			EndUpdate(theDialog);			break;			default:			quit = DefineSwitchAng( theSelect,thePtr,&saveData);			break;			}	}	ReadDefineDialogAng( theDialog,thePtr, saveData);  	DM_DisposDialog(&theDialog);	return;}