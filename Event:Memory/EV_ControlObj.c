#include	"Diffract_INCs.h"#include	"MenuDefs.h"//s.Jim:Event/Memory:EV_ControlObj.c#include	<QDOffscreen.h>pascal	void	PopUpPictMDEF(short,MenuHandle,Rect*,Point,short*);void			PopUpPicture::DoInit(Ptr	theObj){	Rect	theRect;		ownerObj = theObj;	SetRect(&theRect,0,0,100,100);	thePicture 	= OpenPicture(&theRect);	MoveTo(0,0);	LineTo(100,100);	ClosePicture();			ClearPopUpPict();		theResult = 0;}void			PopUpPicture::DoClose(void){		KillPicture(thePicture);	D_delete(this);}void			PopUpPicture::SetPopUpPict(PicHandle	newPicture){		ClearPopUpPict();	thePicture 		= newPicture;	gTempPicHandle 	= thePicture;}void			PopUpPicture::DrawPopUpPict(short h,short v,Boolean local){	Point			location;	Rect			srcRect,picFrame,offScreenRect;	CGrafPtr		theWindow;	QDErr			theError;	GWorldPtr		theGWorld;		if(!thePicture)return;	location.h	= h;					/* these lines do nothing */	location.v	= v;		if(!local)		GlobalToLocal(&location);				picFrame		= (*thePicture)->picFrame;				// Get Picture rect	if(h > 0 || v > 0){										//If h,v != orient frame to new start point		OffsetRect(&picFrame,-picFrame.left,-picFrame.top);		OffsetRect(&picFrame,h,v);	}	ClipRect(&picFrame);	srcRect 		= picFrame;								// Copy Picture rect	InsetRect(&srcRect,-3,-3);								// Expand rect to cover picture -> Target	offScreenRect 	= srcRect;								// Copy target area rect	OffsetRect(&offScreenRect,-srcRect.left,-srcRect.top);	// offset target to define gworld bounds		theWindow		= (CGrafPtr)((DiffractObject*)ownerObj)->theWindow;		// Get the window as a CGrafPtr	theError 		= NewGWorld (&theGWorld,0,&offScreenRect,NULL,NULL,0);	// Setup new GWorld (ref Think Ref)		((DiffractObject*)ownerObj)->DMForeColor(BLACK);		// Set drawing environment for CopyBits	((DiffractObject*)ownerObj)->DMBackColor(WHITE);/* CopyBits from window to new GWorld */	CopyBits ((BitMap*)(*(theWindow->portPixMap)),(BitMap*)(*(theGWorld->portPixMap)), &srcRect, &offScreenRect, srcCopy, NULL);	dm_EraseRect(&picFrame);									//  Set picture background by erasing	DrawPicture(thePicture,&picFrame);						//	Draw PopUp Pict	while(StillDown())		;		((DiffractObject*)ownerObj)->DMForeColor(BLACK);		// Set drawing environment for CopyBits	((DiffractObject*)ownerObj)->DMBackColor(WHITE);/* CopyBits from new GWorld to window */	CopyBits((BitMap*)(*(theGWorld->portPixMap)),(BitMap*)(*(theWindow->portPixMap)), &offScreenRect, &srcRect, srcCopy, NULL);	DisposeGWorld (theGWorld);								// Remove new gWorld}void			PopUpPicture::ClearPopUpPict(void){	if(thePicture)KillPicture(thePicture);	thePicture = (PicHandle)NUL;}pascal	void	PopUpPictMDEF(short message,MenuHandle theMenu,Rect* theRect,Point where,short* theItem){	long	ourA5;		ourA5 = SetCurrentA5();		switch(message){		case mDrawMsg:			DrawPicture(gTempPicHandle,theRect);			break;		case mChooseMsg:			*theItem = 0;			break;		case mSizeMsg:			(*theMenu)->menuHeight	= (*gTempPicHandle)->picFrame.bottom - (*gTempPicHandle)->picFrame.top;			(*theMenu)->menuWidth 	= (*gTempPicHandle)->picFrame.right - (*gTempPicHandle)->picFrame.left;			break;		case mPopUpMsg:	/*                               Left in because the weren't generating errors			TopMenuItem 	= 50;			AtMenuBottom 	= 80;	*/			theRect->top 	= where.v;			theRect->bottom = where.v + (*theMenu)->menuHeight;			theRect->left 	= where.h;			theRect->right 	= where.h + (*theMenu)->menuWidth;			*theItem = 1;			break;		default:			break;	}	ourA5 = SetA5(ourA5);}void				IndexTextRec::DoITRInit(Boolean dirFlag,Crystal* thatCrystal,short theH,short theV,short theStringNum){	FontInfo	theFont;	short		width;		//TextFont	(systemFont);	//TextFont	(geneva);//new	TextFont(1123);	GetFontInfo	(&theFont);	sprintf		(gTheText,"18 18 18 18");	width 	= 	StringWidth(c2pstr(gTheText));			inherited::DoInit(dirFlag,thatCrystal);		labelID 			= 	theStringNum;		horiz 				= 	theH;	vert				= 	theV;	active				= 	false;	theViewRect.left 	= 	horiz - 1;	theViewRect.bottom 	= 	vert + theFont.descent + theFont.leading + 1;	theViewRect.top		= 	vert - theFont.ascent - 1;	theViewRect.right 	= 	horiz + width + 1; 	theTextRec 			= 	TENew(&theViewRect,&theViewRect);		TESetJust				(teJustCenter,theTextRec);	(*theTextRec)->crOnly = -1;	return;}void				IndexTextRec::DoRefresh(void){	short		width;	Rect		destRect;	TextFont(1123);	gCurrentObj->DMBackColor(BACKGROUND_COLOR);	gCurrentObj->DMForeColor(COMPLEMENT_COLOR);	TextFont		(systemFont);	GetIndString	(pTheText,INDEX_TEXT_REC_LABELS,labelID);	width 		= 	StringWidth(pTheText);	MoveTo			(horiz - width - 8,vert);//	DrawDiffractString		(pTheText);		destRect = theViewRect;	InsetRect(&destRect,-3,-3);	OffsetRect(&destRect,0,-1);	SetValue();	PenSize(2,2);	dm_FrameRect(&destRect);	PenNormal();	SetValue();}				void				IndexTextRec::DoClose(void){	TEDispose(theTextRec);	D_delete(this);	TextFont(0);	return;}Boolean				IndexTextRec::DoHit(Point	thePoint){	TextFont(1123);	if(PtInRect(thePoint,&theViewRect)){		if(!active){			TEActivate(theTextRec);			TESetSelect(0,32767,theTextRec);		} else {			TEClick(thePoint,gShifted,theTextRec);		}		active = true;	} else {		if(active){			TEDeactivate(theTextRec);		}		active = false;	}	return active;}void				IndexTextRec::DoKey(char	theChar){	short	end;		end = 8;	TextFont(1123);	if(theCrystal->lattice_Type == HEXAGONAL || theCrystal->lattice_Type == TRIGONAL)		end = 10;	if(active && theChar != (char)13){		TEKey(theChar,theTextRec);		if((*theTextRec)->teLength > end){			TESetSelect(end,32767,theTextRec);			TEDelete(theTextRec);		}	}}void				IndexTextRec::DoIdle(void){	gCurrentObj->DMBackColor(BACKGROUND_COLOR);	gCurrentObj->DMForeColor(COMPLEMENT_COLOR);	TextFont(1123);	if(active)		TEIdle(theTextRec);}void				IndexTextRec::DoTScrap(short	theItem){	short	end;	TextFont(1123);	if(active){		switch(theItem){			case CUT:				TECut(theTextRec);				break;			case COPY:				TECopy(theTextRec);				break;			case PASTE:				TEPaste(theTextRec);				end = 5;				if(theCrystal->lattice_Type == HEXAGONAL || theCrystal->lattice_Type == TRIGONAL)					end = 7;				if((*theTextRec)->teLength > end){					TESetSelect(end,32767,theTextRec);					TEDelete(theTextRec);				}				break;			case DO_CLEAR:				TEDelete(theTextRec);				break;			default:				break;		}	}}void				IndexTextRec::SetValue(void){	long	length;	TextFont(1123);	IndexToText(gTheText);	length = (unsigned long)gTheText[0];	TESetSelect(0L,32767L,theTextRec);	TEDelete(theTextRec);	TESetText(&(gTheText[1]),length,theTextRec);	TEUpdate(&theViewRect,theTextRec);	return;}void				IndexTextRec::GetValue(void){	short			i;		for(i = 0; i <= 15;i++) pTheText[i] = (char)0;	GetIText((*theTextRec)->hText,pTheText);		IndexFromText(gTheText);	return;}void				IndexTextRec::VerifyText(void){	GetValue();	SetValue();}Boolean				ControlRec::DoInit(short theRes,short h,short v){	PenSize(2,2);	theControl = GetNewControl(theRes,((DiffractObject*)gCurrentObj)->theWindow);	if((long)theControl == NUL){		PenNormal();		return false;	}	MoveControl(theControl,h,v);	ShowControl(theControl);	HiliteControl((ControlHandle)theControl,0);	(*theControl)->contrlRfCon = (long)this;	theFlag = false;	PenNormal();	return true;}void				ControlRec::DoClose(void){	/*  Closing a window kills all controls  */	D_delete(this);	return;}Boolean				ControlRec::DoHit(Point	thePoint){	if(TrackControl(theControl,thePoint,NUL)){		theFlag = true;		return true;	}	return false;}Boolean				ControlRec::TestControl(void){	Boolean		flag;		flag = theFlag;	theFlag = false;	return(flag);}void		ScrollBar::DoInit(WindowPtr	ownerWind,Boolean vert){		if(this == (ScrollBar*)NUL)		return;	theWindow 	= ownerWind;	theObj		= (Ptr)((WindowPeek)theWindow)->refCon;	vertical	= vert;	scrollRect 	= theWindow->portRect;	updateRgn	= D_NewRgn();		if(vertical){		scrollRect.right	+= 1;		scrollRect.top		-= 1;		scrollRect.top		+= ((DiffractObject*)theObj)->rulerHeight;		scrollRect.left 	= scrollRect.right - 16;		scrollRect.bottom	-= 14;	} else {		scrollRect.bottom	+= 1;		scrollRect.left		-= 1;		scrollRect.top 		= scrollRect.bottom - 16;		scrollRect.right 	-= 14;	}	theControl = NewControl(theWindow,&scrollRect,"\pscroll bar",true,0,							0,0,scrollBarProc,(long)this);}void	ScrollBar::ReInit(void){	if(this == (ScrollBar*)NUL)		return;	scrollRect 	= theWindow->portRect;		if(vertical){		scrollRect.right	+= 1;		scrollRect.top		-= 1;		scrollRect.top		+= ((DiffractObject*)theObj)->rulerHeight;		scrollRect.left 	= scrollRect.right - 16;		scrollRect.bottom	-= 14;	} else {		scrollRect.bottom	+= 1;		scrollRect.left		-= 1;		scrollRect.top 		= scrollRect.bottom - 16;		scrollRect.right 	-= 14;	}	SetCtlValue(theControl,0);	SetCtlMax(theControl,0);}void	ScrollBar::DoResize(void){	short			w,h,theMax,theValue;	DiffractObject	*ownerObj;		if(this == (ScrollBar*)NUL)		return;		scrollRect	= theWindow->portRect;	ownerObj 	= (DiffractObject*)theObj;	theValue	= GetCtlValue(theControl);		if(vertical){										/* Vertical Bar	*/		theMax = ownerObj->objectSize.v - scrollRect.bottom + ((DiffractObject*)theObj)->rulerHeight + 16;		scrollRect.right	+= 1;		scrollRect.top		-= 1;		scrollRect.top		+= ((DiffractObject*)theObj)->rulerHeight;		scrollRect.left 	= scrollRect.right - 16;		scrollRect.bottom	-= 14;	} else {											/* Horiz Bar  */		theMax = ownerObj->objectSize.h - scrollRect.right + 16;		scrollRect.bottom	+= 1;		scrollRect.left		-= 1;		scrollRect.top 		= scrollRect.bottom - 16;		scrollRect.right 	-= 14;	}	if(theMax < theValue)		theMax = theValue;	SetCtlMax(theControl,theMax);	w 	= scrollRect.right 	- scrollRect.left;	h	= scrollRect.bottom - scrollRect.top;	HideControl(theControl);	MoveControl(theControl,scrollRect.left,scrollRect.top);	SizeControl(theControl,w,h);	ShowControl(theControl);}void				ScrollBar::DoScroll(Point	thePoint){ 		ControlActionUPP	DmyScrollProcUPP;		DmyScrollProcUPP	= NewControlActionProc(DmyScrollProc);	thePart		= TestControl(theControl,thePoint);	last		= GetCtlValue(theControl);	D_SetClip(((DiffractObject*)theObj)->eventRgn);	switch(thePart){		case	inThumb:			if(TrackControl(theControl,thePoint,(ControlActionUPP)NUL)){				ScrollProc();			}			InvalRect(&(((DiffractObject*)theObj)->thePictRect));						break;						case	inUpButton:		case	inDownButton:		case	inPageUp:		case	inPageDown:			startTime	= TickCount();			TrackControl(theControl,thePoint,(ControlActionUPP)DmyScrollProcUPP);			InvalRect(&(((DiffractObject*)theObj)->thePictRect));						/*  For continuous (but slow) scrolling, remove the following    */			/*  line and add in the 11 commented out in 'ScrollProc' below	*/					/*	InvalRect(&(((DiffractObject*)theObj)->thePictRect)); */						/* */			break;		default:			break;	}}void				ScrollBar::DoClose(void){	D_DisposeRgn(&updateRgn);	D_delete(this);	return;}pascal void			DmyScrollProc(ControlHandle	theControl,short  thePart){	short	i;		i = thePart;  /* this prevents an error due to the non use of the Part */	((ScrollBar*)((*theControl)->contrlRfCon))->ScrollProc();}void		ScrollBar::ScrollProc(void){	Point				thePoint;	short				step,theValue,h,v,theMax;	long				delay;	unsigned long		time;	Rect				theRect;	DiffractObject		*ownerObj;	static short		jMark;		switch(thePart){		case	inThumb:			theValue	=	GetCtlValue(theControl);			step 		=	theValue - last;			delay		=	1;			break;		case	inUpButton:			step = -2;			delay = 2;			break;		case	inDownButton:			step = 2;			delay = 2;			break;		case	inPageUp:			if(vertical){				step = -theWindow->portRect.right / 8;			} else {				step = -theWindow->portRect.bottom / 8;			}			if(step > -1)				step = -1;			delay = 20;			delay = 120;			break;		case	inPageDown:			if(vertical){				step = theWindow->portRect.right / 8;			} else {				step = theWindow->portRect.bottom / 8;			}			if(step < 1)				step = 1;			delay = 20;			delay = 120;			break;		default:			return;			break;	}	GetMouse(&thePoint);	if(TestControl(theControl,thePoint) == thePart || thePart == inThumb){		if(thePart != inThumb){							/* Clip movement to control bounds   */			last = GetCtlValue(theControl);			theValue = last + step;			SetCtlValue(theControl,theValue);			theValue 	= GetCtlValue(theControl);			step		= theValue - last;		}		ownerObj 	= (DiffractObject*)theObj;		theRect 	= ownerObj->theVisRect;		if(vertical){			h		= 0;			v		= -step;			theMax = ownerObj->objectSize.v - scrollRect.bottom + ((DiffractObject*)theObj)->rulerHeight + 16;		}else{			h		= -step;			v		= 0;			theMax = ownerObj->objectSize.h - theWindow->portRect.right + 16;		}				if(theMax < theValue)			theMax = theValue;				SetCtlMax(theControl,theMax);   /*  allows zeroing of scroll bars  */				D_SetClip(ownerObj->drawRgn);			CopyRgn(ownerObj->drawRgn,gTheRgn);		ScrollRect(&theRect,h,v,updateRgn);			InvalRgn(updateRgn);			/*  Add the following 11 lines and subtract the one indicated in 'DoScroll'  */		/* above if continuous (but slow) scrolling is desired						*//*			h = GetCtlValue(ownerObj->hScroll->theControl);		v = GetCtlValue(ownerObj->vScroll->theControl);		ownerObj->DoSetOrigin();		OffsetRgn(gTheRgn,h,v);		D_SetClip(gTheRgn);			OffsetRgn(updateRgn,h,v);		InvalRgn(updateRgn);		BeginUpdate(theWindow);		DrawPicture(ownerObj->screenPict,&ownerObj->thePictRect);		EndUpdate(theWindow);		ownerObj->DoResetOrigin();*/		D_SetClip(ownerObj->eventRgn);			}	if(StillDown()){		if((TickCount() - gTheEvent.when) > 30){			Delay(delay,&time);			InvalRgn(ownerObj->drawRgn);		} else {			Delay(30,&time);		}	}	return;}	void		ScrollBar::DoScrollSetRange(short hi,short lo){	short 	theValue;	double	value,range;		range = GetCtlMax(theControl) - GetCtlMin(theControl);	value = (double)(GetCtlValue(theControl) - GetCtlMin(theControl)) / range;	range = hi - lo;	value *= range;	theValue = value + lo;	SetCtlMax(theControl,hi);	SetCtlMin(theControl,lo);	SetCtlValue(theControl,theValue);}void		ScrollBar::DoScrollSetValue(short theValue){	short			step,theMax,h,v;	DiffractObject	*ownerObj;	Rect			theRect;		last = GetCtlValue(theControl);	SetCtlValue(theControl,theValue);	theValue 	= GetCtlValue(theControl);	step		= theValue - last;	ownerObj 	= (DiffractObject*)theObj;	theRect 	= ownerObj->theVisRect;	if(vertical){		h		= 0;		v		= -step;		theMax = ownerObj->objectSize.v - theWindow->portRect.bottom;	}else{		h		= -step;		v		= 0;		theMax = ownerObj->objectSize.h - theWindow->portRect.right;	}		if(theMax < theValue)		theMax = theValue;	SetCtlMax(theControl,theMax);   /*  allows zeroing of scroll bars  */		D_SetClip(ownerObj->drawRgn);		CopyRgn(ownerObj->drawRgn,gTheRgn);	ScrollRect(&theRect,h,v,updateRgn);		D_SetClip(ownerObj->eventRgn);	InvalRect(&(((DiffractObject*)theObj)->thePictRect));}