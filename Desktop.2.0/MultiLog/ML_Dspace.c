#include "ML_Dspace.h"#include "a_D_Space.h"#include "Diffract_INCs.h"#include "ML_Crystal.h"#include "ML_StructFactor.h"#include "ML_Default.h"#include "ML_Misc_Plot.h"void	ML_DoDspaceSetup(Ptr	owner,DialogPtr theDialog,Ptr *theParams){	D_Spacing_Table*			theOwner;	ML_DspaceParmPtr	thePtr;	theOwner					= (D_Spacing_Table*)owner;	thePtr	=	(ML_DspaceParmPtr)D_NewPtr(sizeof(ML_DspaceParm));	theOwner->SetDefineObjTab( theDialog,thePtr);	*theParams = (Ptr)thePtr;}void	ML_DoDspaceHit(Ptr	owner,DialogPtr theDialog, short theSelect,Ptr theParams){	D_Spacing_Table*		theOwner;	Boolean		saveData;	ML_DspaceParmPtr	thePtr;	theDialog = theDialog;//warning	thePtr	=	(ML_DspaceParmPtr)theParams;	theOwner		= (D_Spacing_Table*)owner;		theOwner->DefineSwitchTab(  theSelect,thePtr,&saveData);}void	ML_DoDspaceRead(Ptr owner,DialogPtr theDialog,Ptr theParams,Boolean saveData){	D_Spacing_Table*		theOwner;	ML_DspaceParmPtr	thePtr;	thePtr	=	(ML_DspaceParmPtr)theParams;	theOwner		= (D_Spacing_Table*)owner;	theOwner->ReadDefineDialogTab( theDialog,thePtr, saveData);	}void D_Spacing_Table::SetDefineObjTab(DialogPtr theDialog,ML_DspaceParmPtr theParams){	theParams->theMenu 	= 	(PopUpMenu*)D_new(PopUpMenu);	theParams->theMenu		->	Init(theDialog,_4_,52,crystalNum + 1);		theParams->sortMethodMenu = (PopUpMenu*)D_new(PopUpMenu);	theParams->sortMethodMenu->Init(theDialog,_5_,501,sortMethod);			theParams->displayMenu = (PopUpMenu*)D_new(PopUpMenu);	theParams->displayMenu->Init(theDialog,_6_,504,displayDspace);		SetItemValue(theDialog,_3_,dspaceLow,2);	SetItemValue(theDialog,_7_,minSF,2);	DrawDialog(theDialog);}Boolean D_Spacing_Table::DefineSwitchTab(short  theSelect,ML_DspaceParmPtr theParams,Boolean *saveData){	switch(theSelect){		case -1:		case 2:			*saveData = false;			return true;		case DLOG_ENTER_OR_CR:		case 1:			*saveData = true;			return true;		case _4_:			theParams->theMenu->DoPopUp();			break;		case _5_:			theParams->sortMethodMenu->DoPopUp();			break;		case _6_:			theParams->displayMenu->DoPopUp();			break;		case ML_UPDATE_EVT:			//DrawDialog(theDialog);			break;		default:			break;	}	return false;}void	D_Spacing_Table::ReadDefineDialogTab(DialogPtr theDialog,ML_DspaceParmPtr theParams,Boolean saveData){	if(saveData){		if(sortMethod != theParams->sortMethodMenu->lastResult)newPictReq = true;		sortMethod = theParams->sortMethodMenu->lastResult;		if(displayDspace != theParams->displayMenu->lastResult)newPictReq = true;		displayDspace = theParams->displayMenu->lastResult;				valid 		= 	true;		if(crystalNum != theParams->theMenu->lastResult - 1)changedFlag = true;		crystalNum 	= 	theParams->theMenu->lastResult - 1;		if(crystalNum < 0 || crystalNum > 3) crystalNum = 0;		if(dspaceLow > GetItemValue(theDialog,_3_))changedFlag = true;		dspaceLow 	= 	GetItemValue(theDialog,_3_);		if(minSF < GetItemValue(theDialog,_7_))changedFlag = true;		minSF		=	GetItemValue(theDialog,_7_);			}	theParams->theMenu		->	DoClose();		theParams->sortMethodMenu->DoClose();	theParams->displayMenu->DoClose();	KillPtr((Ptr)theParams);}void	D_Spacing_Table::DoDefine(void){	Boolean			calculateFlag;	MultiLog*		theMultiLog;	short				n = 0;		theMultiLog	= (MultiLog*)new(MultiLog);	theMultiLog->DoInit(257,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoDspaceSetup,(HitProcPtr)ML_DoDspaceHit,(ReadProcPtr)ML_DoDspaceRead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[0]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[1]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[2]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoCrystalSetup,(HitProcPtr)ML_DoCrystalHit,(ReadProcPtr)ML_DoCrystalRead,(Ptr)theCrystal[3]);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoBeamESetup,(HitProcPtr)ML_DoBeamEHit,(ReadProcPtr)ML_DoBeamERead,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_SetMiscParam,(HitProcPtr)ML_SwitchMiscParam,(ReadProcPtr)ML_ReadMiscParam,(Ptr)this);	theMultiLog->AssignFunction(n++,(SetupProcPtr)ML_DoDefaultSetup,(HitProcPtr)ML_DoDefaultHit,(ReadProcPtr)ML_DoDefaultRead,(Ptr)this);		calculateFlag = theMultiLog->DoMultiLog();	theMultiLog->DoClose();		if(calculateFlag   && screenPict == NUL  && gMyOffG == 0L){		changedFlag = true;		reSizeFlag = true;		newPictReq = true;/*eliminated line dec 1992*/	} else if(screenPict == NUL  && gMyOffG == 0L){		CloseTheObject(theWindow);	}	return ;}void  D_Spacing_Table::DoDefineSetUp(void){	DialogPtr 		theDialog;	short			theSelect;	Boolean			quit = false;	Boolean			saveData;	ML_DspaceParmPtr	thePtr;	thePtr	=	(ML_DspaceParmPtr)D_NewPtr(sizeof(ML_DspaceParm));	theDialog	= DM_GetNewDialog(500,NUL,IN_FRONT);	SetDefineObjTab( theDialog,thePtr);	while(!quit){		ModalDialog(TheFilterUPP,&theSelect);		switch(theSelect){			case ML_UPDATE_EVT:				BeginUpdate(theDialog);			UpdtDialog(theDialog,theDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(theDialog);			}			quit = DefineSwitchTab(  theSelect,thePtr,&saveData);			EndUpdate(theDialog);			break;			default:			quit = DefineSwitchTab(  theSelect,thePtr,&saveData);			break;			}			}	ReadDefineDialogTab( theDialog,thePtr, saveData);	DM_DisposDialog(&theDialog);	return;}