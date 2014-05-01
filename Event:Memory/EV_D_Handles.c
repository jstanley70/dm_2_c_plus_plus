//s.Jim:Event/Memory:EV_D_Handles.c#include	"Diffract_INCs.h"#include 	"InfoObject.h"InfoObject	*obj_Count;InfoObject	*ptr_Count;InfoObject	*hdn_Count;/*InfoObject	*mnbr_Count;InfoObject	*pic_Count;InfoObject	*rsrc_Count;InfoObject	*rgn_Count;*/void	D_HandleInit(void){	KeyMap		keyArray;		gD_Handle_Debug_Flag	= false;	g_HandleCount			= 0;	doBreaks				= false;		GetKeys(keyArray);		obj_Count 	= (InfoObject*)new(InfoObject);	ptr_Count	= (InfoObjectPtr*)new(InfoObjectPtr);	hdn_Count	= (InfoObjectHdn*)new(InfoObjectHdn);	/*mnbr_Count	= (InfoObjectMnbr*)new(InfoObjectMnbr);	pic_Count	= (InfoObjectPic*)new(InfoObjectPic);	rsrc_Count	= (InfoObjectRsrc*)new(InfoObjectRsrc);	rgn_Count	= (InfoObjectRgn*)new(InfoObjectRgn);	*/		obj_Count	->	DoInit(1000);	ptr_Count	->	DoInit(1000);	hdn_Count	->	DoInit(1000);	/*mnbr_Count	->	DoInit(200);	pic_Count	->	DoInit(200);	rsrc_Count	->	DoInit(200);	rgn_Count	->	DoInit(200);	*/	if(keyArray[1] == 5){		SetBreaks();	}	}void	TestNumObjects(void){	obj_Count	->TestNumObjects();	ptr_Count		->TestNumObjects();	hdn_Count	->TestNumObjects();	/*mnbr_Count	->TestNumObjects();	pic_Count	->TestNumObjects();	rsrc_Count	->TestNumObjects();	rgn_Count	->TestNumObjects();	*/	obj_Count	->DoClose();	ptr_Count		->DoClose();	hdn_Count	->DoClose();	/*mnbr_Count	->DoClose();	pic_Count	->DoClose();	rsrc_Count	->DoClose();	rgn_Count	->DoClose();*/}				void	VerifyObjects(void){		obj_Count	->VerifyObjects();	ptr_Count	->VerifyPtrs();	hdn_Count	->VerifyHandles();	/*mnbr_Count	->VerifyObjects();	pic_Count	->VerifyObjects();	rsrc_Count	->VerifyObjects();	rgn_Count	->VerifyObjects();*/}void	VerifyObjectsEveryWhere(void){		obj_Count	->VerifyObjects();	ptr_Count	->VerifyPtrs();	hdn_Count	->VerifyHandles();}void	ShowData(void){	obj_Count	->ShowData();	ptr_Count	->ShowData();	hdn_Count	->ShowData();	/*mnbr_Count	->ShowData();	pic_Count	->ShowData();	rsrc_Count	->ShowData();	rgn_Count	->ShowData();*/}void	SetBreaks(void){	DialogPtr	theDialog;	short		i,j,theSelect;	Boolean		quit = false;	Handle		theHandle;	short		bugType;		theHandle = GetNewMBar(DLOG_MBAR);	ClearMenuBar();/* July 1992 */	SetMenuBar(theHandle);		DrawMenuBar();	theDialog = GetNewDialog(9998,NUL,IN_FRONT);		while(!quit){		ModalDialog((ModalFilterUPP)NUL,&theSelect);		switch(theSelect){			case 1:				for(i = 3,j = 18 ; i <= 16 ; i++,j++){										bugType = GetItemValue(theDialog,j);					switch(bugType){						case 0:							obj_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));							break;						case 1:							ptr_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));						case 2:							hdn_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));						/*case 3:							pic_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));						case 4:							rsrc_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));						case 5:							mnbr_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));						case 6:							rgn_Count->SetBreaks(i - 3,(short)GetItemValue(theDialog,i));							break;*/						default:							break;					}				}				doBreaks = true;			case 2:			case -1:				quit = true;				break;			default:				break;		}	}		DisposDialog(theDialog);	DisposHandle(theHandle);	theHandle = 		GetNewMBar(1000);	ClearMenuBar();/* July 1992 */	SetMenuBar			(theHandle);	DrawMenuBar();	DisposHandle(theHandle);}			void	D_HLock(void* theHandle){	short			error;	char	handleState;			handleState = HGetState((Handle)theHandle);	error = MemError();	if(error != 0){		D_FlagUser(1); 						/* Trying to Lock an Invalid Handle. */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		if((handleState & 0x80) != 0){/* Handle is already locked */		D_FlagUser(2);								/* Handle is already locked */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		HLock((Handle)theHandle);	error = MemError();	if(error != 0){		D_FlagUser(3); 							/* Lock Failed */	}	g_HandleCount++;	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }			}	void	D_HUnlock(void* theHandle){	short	error;	char	handleState;		handleState = HGetState((Handle)theHandle);		error = MemError();	if(error != 0){		D_FlagUser(4);							/* Trying to UNlock an Invalid Handle */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		if((handleState & 0x80) == 0){/* Handle is already Unlocked */		D_FlagUser(5);									if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		HUnlock((Handle)theHandle);	error = MemError();	if(error != 0){		D_FlagUser(6);							/* Handle Unlock failed */	}	g_HandleCount--;}void	D_HandleReport(void){	if(g_HandleCount != 0 & !gShutdown){		D_FlagUser(7);							/* Handles were left hanging */	}		gD_Handle_Debug_Flag = false;}				void	KillPtr(void*	thePtr)					/*  Checks to make sure we really have a Ptr	*/{	short	error;	long	size;		if(thePtr == (Ptr)NUL){		D_FlagUser(10);		return;	} else {		size = GetPtrSize((Ptr)thePtr);		error = MemError();		if(error != 0){			D_FlagUser(10);		} else if(size == NUL){			D_FlagUser(10);		} else {			ptr_Count->RemoveObj((Ptr)thePtr);			DisposPtr((Ptr)thePtr);		}	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	return;	 }			}void KillMBHandle(Handle	*theMenuBar){	short	error;	char	theHState;	Handle	theHandle;		theHandle = *theMenuBar;	/*if(!mnbr_Count->FoundObj(theHandle))return;*/	theHState = HGetState(theHandle);   /* Not Valid */	error = MemError();	if(error != 0){		D_FlagUser(8);		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }				*theMenuBar = (Handle)NUL;			return;	}		if((theHState & 0x80) == 1){		D_FlagUser(9);				/* Handle locked */		HUnlock(theHandle);	}	/*mnbr_Count->RemoveObj(theHandle);*/	DisposHandle(theHandle);	*theMenuBar = (Handle)NUL;			if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }}	void D_ReleaseResource(Handle	theResource){	short	error;	char	theHState;	Handle	theHandle;		theHandle = theResource;	/*if(!rsrc_Count->FoundObj(theHandle))return;*/	theHState = HGetState(theHandle);   /* Not Valid */	error = MemError();	if(error != 0){		D_FlagUser(8);		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }				theResource = (Handle)NUL;			return;	}		if((theHState & 0x80) == 1){		D_FlagUser(9);				/* Handle locked */		HUnlock(theHandle);	}	/*rsrc_Count->RemoveObj(theHandle);*/	ReleaseResource(theResource);			if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }}		void	KillHandle(Handle	*thisHandle)		/*  Checks to make sure we really have a Handle	*/{	short	error;	char	theHState;	Handle	theHandle;		theHandle = *thisHandle;		theHState = HGetState(theHandle);   /* Not Valid */	error = MemError();	if(error != 0){		D_FlagUser(8);		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }				*thisHandle = (Handle)NUL;			return;	}		if((theHState & 0x80) == 1){		D_FlagUser(9);				/* Handle locked */		HUnlock(theHandle);	}	hdn_Count->RemoveObj((void*)theHandle);	DisposHandle(theHandle);/*changed this*/	*thisHandle = (Handle)NUL;			if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }}				Ptr		D_NewPtr(long theSize){	short	error;	Ptr	theResult;		if(theSize == NUL){		D_FlagUser(26);		if(gD_Handle_Debug_Flag)			SysBreak();		return 0L;	}		theResult 	= NewPtr(theSize);	error 		= MemError();		if(error != 0 || theResult == (Ptr)NUL){		FlagMemError(11);		theResult = (Ptr)NUL;	} else {		ptr_Count->AddObj((void*)theResult);	//	if(ptr_Count->objectID == 182 || ptr_Count->objectID == 184 || ptr_Count->objectID == 185 	//	|| ptr_Count->objectID == 186 )	//		FlagMemError(1);	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;		Debugger();	}	return(theResult);}				Handle	D_NewHandle(long theSize){	short	error;	Handle	theResult;		if(theSize == NUL || theSize == 4112){		D_FlagUser(25);		if(gD_Handle_Debug_Flag)			SysBreak();	}		theResult = NewHandle(theSize);	error = MemError();	if(theResult == (Handle)NUL || error != 0){		FlagMemError(12);	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NUL;	} else {		hdn_Count->AddObj((void*)theResult);			}		if(gD_Handle_Debug_Flag){		gD_Handle_Debug_Flag = false;//		SysBreak();Debugger();	}	return((Handle)theResult);}		Handle	D_TempNewHandle(long theSize,short	*error){	Handle	theResult;	*error = 0;	if(theSize == NUL){		D_FlagUser(25);	}		theResult = MFTempNewHandle((Size)theSize,error);	if(theResult == (Handle)NUL || *error != 0){		FlagMemError(13);	 	if(*error == 0)	 		*error = -1957;	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NULL;	} else {		hdn_Count->AddObj((void*)theResult);	}	if(gD_Handle_Debug_Flag){		gD_Handle_Debug_Flag = false;		SysBreak();	}	return((Handle)theResult);}void	KillTempHandle(Handle	*thisHandle)		/*  Checks to make sure we really have a Handle	*/{	short	error;	short	resultCode;	char	theHState;	Handle	theHandle;		theHandle = *thisHandle;		theHState = HGetState(theHandle);   /* Not Valid */	error = MemError();	if(error != 0){		D_FlagUser(8);		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }				*thisHandle = (Handle)NUL;			return;	}		if((theHState & 0x80) == 1){		D_FlagUser(9);				/* Handle locked */		HUnlock(theHandle);	}	hdn_Count->RemoveObj((void*)theHandle);	MFTempDisposHandle(theHandle,&resultCode);	*thisHandle = (Handle)NUL;	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }}void	D_MFTempHLock(void* theHandle){	short	error;	char	handleState;	OSErr	resultCode;/*	D_HLock(theHandle);	return;*/	handleState = HGetState((Handle)theHandle);	error = MemError();	if(error != 0){		D_FlagUser(1); 						/* Trying to Lock an Invalid Handle. */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		if((handleState & 0x80) != 0){		D_FlagUser(2);								/* Handle is already locked */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		MFTempHLock((Handle)theHandle,&resultCode);	error = MemError();	if(error != 0){		D_FlagUser(3); 							/* Lock Failed */	}	g_HandleCount++;	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }			}	void	D_MFTempHUnlock(void* theHandle){	short	error;	char	handleState;	OSErr	resultCode;/*	D_HUnlock(theHandle);	return;*/	handleState = HGetState((Handle)theHandle);		error = MemError();	if(error != 0){		D_FlagUser(4);							/* Trying to UNlock an Invalid Handle */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		if((handleState & 0x80) == 0){		D_FlagUser(5);							/* Handle is already Unlocked */		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }					return;	}		MFTempHUnlock((Handle)theHandle,&resultCode);	error = MemError();	if(error != 0){		D_FlagUser(6);							/* Handle Unlock failed */	}	g_HandleCount--;}void* 	Z_new(void *result){	if(result == (void*)NUL){		D_FlagUser(14);		if(gD_Handle_Debug_Flag){			gD_Handle_Debug_Flag = false;	 		SysBreak();	 	}	 	return((void*)NUL);	}	obj_Count->AddObj(result);		if(gD_Handle_Debug_Flag){		gD_Handle_Debug_Flag = false;//		SysBreak();		Debugger();	}	return(result);}Boolean		IsValidObj(void*	theObj){		return(obj_Count->FoundObj(theObj));}		void 	Z_delete(void* theClass){		obj_Count->RemoveObj(theClass);	 }			Handle D_GetNewMBar(short theMBar){	short	error;	Handle	theResult;			theResult = GetNewMBar(theMBar);	error = MemError();	if(theResult == (Handle)NUL || error != 0){		FlagMemError(12);	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NUL;	} else {		/*mnbr_Count->AddObj((void*)theResult);*/	}	if(gD_Handle_Debug_Flag){		gD_Handle_Debug_Flag = false;		SysBreak();	}	return((Handle)theResult);}		Boolean	D_SetPtrSize(Ptr*	thePtr,long	theSize){	Ptr		newPtr,basePtr;	short	error;		basePtr = *thePtr;		if(theSize == NUL){		D_FlagUser(26);		if(gD_Handle_Debug_Flag)			SysBreak();	}		SetPtrSize(basePtr,theSize);	error = MemError();		switch(error){			case noErr:				return(true);				break;					case memWZErr:			D_FlagUser(19);			newPtr = D_NewPtr(theSize);			error = MemError();			if(newPtr == (Ptr)NUL || error != 0){				break;			}						*thePtr = newPtr;						if(gD_Handle_Debug_Flag){				gD_Handle_Debug_Flag = false;		 		SysBreak();		 	}		 	return(true);			break;		case memFullErr:			newPtr = D_NewPtr(theSize);			error = MemError();			if(newPtr == (Ptr)NUL || error != 0){				break;			}			BlockMove(basePtr,newPtr,GetPtrSize(basePtr));			KillPtr(basePtr);			*thePtr = newPtr;			return(true);			break;		default:			break;	}	if(gD_Handle_Debug_Flag){		gD_Handle_Debug_Flag = false; 		SysBreak(); 	} 	return(false);}	void	D_SetHandleSize(Handle	*theH,long	theSize){	Boolean	locked;	Handle	theHandle;	char	cMemTags;		if(theSize == NUL){		D_FlagUser(25);		if(gD_Handle_Debug_Flag)			SysBreak();	}		theHandle = (Handle)*theH;		cMemTags = HGetState(theHandle);	if(MemError() != 0){		D_Debug_Message(24);		return;	}	if((cMemTags & 0x80) != 0){		locked = true;		D_HUnlock(theHandle);	} else {		locked = false;	}		if(theSize <= 0){		theSize = 12;	}		SetHandleSize(theHandle,theSize);	if(MemError() != noErr){		FlagMemError(23);	}	*theH = theHandle;	if(locked)		D_HLock(theHandle);}void	FlagMemError(short messageNum){	WarnUser(1);	if(gD_Handle_Debug_Flag && !gInBackground){		D_FlagUser(messageNum);		gD_Handle_Debug_Flag = false;		SysBreak();	}}void	D_Debug_Message(short messageNumber){	D_FlagUser(messageNumber);	if(gD_Handle_Debug_Flag){		gD_Handle_Debug_Flag = false; 		SysBreak(); 	} }  void	D_RecoverMemory(void*	theData,long	objectSize,long  count){	Boolean	locked;	Handle	theHandle;	Size	theSize,oldSize;	char	cMemTags;		theHandle = (Handle)theData;		cMemTags = HGetState(theHandle);	if(MemError() != 0){		D_Debug_Message(24);		return;	}	if((cMemTags & 0x80) != 0){		locked = true;		D_HUnlock(theHandle);	} else {		locked = false;	}		oldSize	=	GetHandleSize(theHandle);	theSize	=	(long)count * objectSize;		if(oldSize < theSize){		D_Debug_Message(22);	}		if(theSize <= 0){		theSize = 12;	}		SetHandleSize(theHandle,theSize);	if(MemError() != noErr){		FlagMemError(23);	}	if(locked)		D_HLock(theHandle);}void	D_FlagUser(short stringResId){	DisplayMessage(1003,stringResId,503);}void	D_ValidObject(Handle	theObject){	obj_Count->FoundObj(theObject);	return;}/* Added By Jim in September 1991 */OSErr	D_HandToHand(Handle *theHandle){	short	error;	short	theSize;	OSErr	iErr;	theSize = GetHandleSize(*theHandle);	if(theSize == NUL){		D_FlagUser(25);		if(gD_Handle_Debug_Flag)			SysBreak();	}		iErr = HandToHand(theHandle);	error = MemError();	if(/* *theHandle == (Handle)NUL || */error != 0){		FlagMemError(12);	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	return iErr;	}else {		hdn_Count->AddObj((void*)*theHandle);			}	return iErr;}PicHandle	D_OpenPicture(Rect *theRect){	short		error;	PicHandle	theResult;	theResult = OpenPicture(theRect);	error = MemError();	if((Handle)theResult == (Handle)NUL || error != 0){		FlagMemError(12);	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NULL;	} else {		/*pic_Count->AddObj((void*)theResult);*/	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	}	return((PicHandle)theResult);}MenuHandle	D_GetMenu(short menuID){		short		error;	MenuHandle  theResult;		theResult = GetMenu( menuID);		/*if(rsrc_Count->FoundObj((Handle)theResult))return theResult;*/	error = MemError();	if((Handle)theResult == (Handle)NUL || error != 0){		FlagMemError(12);	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NULL;	} else {		/*rsrc_Count->AddObj((void*)theResult);*/	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	}	return((MenuHandle)theResult);}Handle	D_GetResource(ResType rType,short rID){	short		error;	Handle  	theResult;		theResult = GetResource( rType, rID);	/*if(rsrc_Count->FoundObj(theResult))return theResult;*/	error = MemError();	if((Handle)theResult == (Handle)NUL || error != 0){		FlagMemError(12);	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NULL;	} else {		/*rsrc_Count->AddObj((void*)theResult);*/	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	}	return((Handle)theResult);}void D_SetClip(RgnHandle newClipRgn){	char	cMemTags;	cMemTags = HGetState((Handle)newClipRgn);	if(MemError() != 0){		D_Debug_Message(24);		return;	}	SetClip(newClipRgn);}RgnHandle	D_NewRgn(void){	short		error;	RgnHandle	theResult;			theResult 	= NewRgn();		error 		= MemError();	if((Handle)theResult == (Handle)NUL || error != 0){		FlagMemError(12);	}		if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 	theResult = NULL;	} else {		hdn_Count->AddObj((void*)theResult);	}	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	}	return((RgnHandle)theResult);}void	D_KillPicture(PicHandle	thePicture)		/*  Checks to make sure we really have a Handle	*/{	short		error;	char		theHState;	Handle		theHandle;			if((long)thePicture == 0L) return;	/*if(!pic_Count->FoundObj((Handle)thePicture))return;*/	theHandle = (Handle)thePicture;			theHState = HGetState(theHandle);   /* Not Valid */	error = MemError();	if(error != 0){		D_FlagUser(8);		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }				thePicture = (PicHandle)NUL;			return;	}		if((theHState & 0x80) == 1){		D_FlagUser(9);				/* Handle locked */		HUnlock(theHandle);	}	/*pic_Count->RemoveObj(theHandle);*/	KillPicture(thePicture);	thePicture = (PicHandle)NUL;	if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }}void	D_DisposeRgn(RgnHandle	*theRgn)		/*  Checks to make sure we really have a Handle	*/{	short	error;	char	theHState;	Handle	theHandle;		if((long)theRgn == 0L) return;		theHandle = (Handle)*theRgn;		theHState = HGetState(theHandle);   /* Not Valid */	error = MemError();	if(error != 0){		D_FlagUser(8);		if(gD_Handle_Debug_Flag){		 	gD_Handle_Debug_Flag = false;		 	SysBreak();		 }				*theRgn = (RgnHandle)NUL;			return;	}		if((theHState & 0x80) == 1){		D_FlagUser(9);				/* Handle locked */		HUnlock(theHandle);	}		hdn_Count->RemoveObj(theHandle);	DisposeRgn(*theRgn);	*theRgn = (RgnHandle)NUL;			if(gD_Handle_Debug_Flag){	 	gD_Handle_Debug_Flag = false;	 	SysBreak();	 }}