//s.Jim:Event/Memory:EV_DoActivateEvt.c#include	"Diffract_INCs.h"#include	"MenuDefs.h"#include 	"WindowMenu.h"#define	ENABLE_CONTROLS		0#define	DISABLE_CONTROLS	255void	SetControlsStatus(WindowPeek,short);void	DoActivate(void){	WindowPeek		theWindow;	long	loc;		loc			= LoadScrap();	loc			= GetScrap((Handle)(gTheSelection->selectPict),'PICT',&loc);		theWindow 	= (WindowPeek)FrontWindow();	if(theWindow != (WindowPeek)NUL)		SetControlsStatus(theWindow,ENABLE_CONTROLS);	SetWindowMenu();	return;}	void	DoDeactivate(void){	WindowPeek		theWindow;	Boolean			flag;	long			length;	D_HLock((Handle)(gTheSelection->selectPict));	length 		= ZeroScrap();	length 		= GetHandleSize((Handle)gTheSelection->selectPict);	length		= PutScrap(length,'PICT',(Ptr)*(gTheSelection->selectPict));	D_HUnlock((Handle)(gTheSelection->selectPict));	theWindow 	= (WindowPeek)FrontWindow();	flag 		= false;	if(theWindow != (WindowPeek)NUL)		flag = true;		while(flag){		SetControlsStatus(theWindow,DISABLE_CONTROLS);		theWindow = theWindow->nextWindow;		if(theWindow == (WindowPeek)NUL)			flag = false;	}	return;}void	CloseTheObject(WindowPtr theWindow){	MenuHandle theMenu;	WindowPeek	nextWindow;	Rect theRect;	short i,h,v;	unsigned long time;	long			timeL;	DiffractObject*	owner;	#define THE_INC	17	nextWindow = ((WindowPeek)theWindow)->nextWindow;	theRect = theWindow->portRect;	h = (theRect.right - theRect.left) / THE_INC;	v = (theRect.bottom - theRect.top) / THE_INC;	PenMode(patXor);	D_SetClip(gUpdateAll);	for(i = 1 ; i <= THE_INC ; i++){		dm_FrameRect(&theRect);		SystemTask();		Delay(1,&time);		dm_FrameRect(&theRect);		InsetRect(&theRect,h,v);	}	owner = ((DiffractObject*)(((WindowPeek)theWindow)->refCon));	if(IsValidObj(owner))		owner->DoClose();			PenNormal();		if((long)nextWindow == NUL){		gTopWindow = NUL;		gCurrentObj	= gDefault;		ClearMenuBar();/* July 1992 */		SetMenuBar(gDefault->theMenuBar);		theMenu = GetMHandle(FILE_MENU);		DisableItem(theMenu,CLOSE);		DisableItem(theMenu,SAVE);		DisableItem(theMenu,SAVE_AS);		DisableItem(theMenu,PRINT);		DrawMenuBar();	} else {		SetTopWindow((WindowPtr)NUL);	}	timeL = (long)time;	MaxMem(&timeL);	CompactMem(50000L);	SetWindowMenu();	return;}void	CloseOrphan(Ptr theObj){	long time;	MenuHandle theMenu;	WindowPeek theWindow;		theWindow = (WindowPeek)FrontWindow();		if(!theWindow){			gCurrentObj = gDefault;		}else{			gCurrentObj	= (DiffractObject*)(((WindowPeek)theWindow)->refCon);		}		((DiffractObject*)theObj)->	DoClose();		if(gCurrentObj == gDefault){			gTopWindow = NUL;			gCurrentObj	= gDefault;			ClearMenuBar();/* July 1992 */			SetMenuBar(gDefault->theMenuBar);			theMenu = GetMHandle(FILE_MENU);			DisableItem(theMenu,CLOSE);			DisableItem(theMenu,SAVE);			DisableItem(theMenu,SAVE_AS);			DisableItem(theMenu,PRINT);			DrawMenuBar();		}else{		  SetTopWindow((WindowPtr)NUL);		}		MaxMem(&time);		CompactMem(50000L);		SetWindowMenu();}void	CloseAllObjects(){	WindowPtr theWindow;	WindowPeek	nextWindow;		theWindow = FrontWindow();		while((long)theWindow != NUL){		nextWindow = ((WindowPeek)theWindow)->nextWindow;		CloseTheObject(theWindow);		theWindow = (WindowPtr)nextWindow;	}}void	SetTopWindow(WindowPtr theWindow){		if(theWindow == gTopWindow && !theWindow == 0L && 			(long)gCurrentObj == (long)((WindowPeek)theWindow)->refCon)		return;	if(theWindow == (WindowPtr)NUL){		theWindow = FrontWindow();	}	if(theWindow != (WindowPtr)NUL){		SetPort(theWindow);		gCurrentObj	= (DiffractObject*)(((WindowPeek)theWindow)->refCon);	} else {		gCurrentObj = gDefault;	}	if(!IsValidObj(gCurrentObj))		gCurrentObj = gDefault;	gTopWindow	= theWindow;	ClearMenuBar();/* July 1992 */	SetMenuBar(gCurrentObj->theMenuBar);	DrawMenuBar();	SetWindowMenu();}void	SetControlsStatus(WindowPeek		theWindow,short theStatus){	ControlHandle	theControl;	Boolean			flag;	theControl	= (ControlRecord**)theWindow->controlList;	flag		= false;	if(theControl != (ControlHandle)NUL)		flag 		= true;		while(flag){		HiliteControl((ControlHandle)theControl,theStatus);		theControl = (*theControl)->nextControl;		if(theControl == (ControlHandle)NUL)			flag = false;	}	return;}