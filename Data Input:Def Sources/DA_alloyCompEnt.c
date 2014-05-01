//Macintosh HD:CodeWarrior¬ DR/3 Gold ─:Metrowerks  C/C++ ─:Projects:D.M. v2.0:Sources.Jim:Data Input/Def Sources:DA_alloyCompEnt.c#include	"Diffract_INCs.h"void AlloyCompDef::Kill(){	D_delete(this);}void AlloyCompDef::Init(void){	short i,j;			noOfElements		= 0;	oldElement 			= -20;	oldHorRect			= -1;	oldVerRect 			= -1;	maxElmnts 			= 19;	for(j = 0; j <= 4;j++){		for(i = 0; i <= 104; i++) theElements[j][i] = false;		for(i = 0; i <= 19; i++) storedComps[j][i] = 0.0;	}	}Boolean AlloyCompDef::DoDefine(void){	short 			i,j,item,type,oldItem = 1,theNumber;	Handle 			theHandle;	Rect 			theRect;	/*pascal Boolean 	TheFilter();*/	Boolean 		newFlag = true;	Boolean			oldElements[5][105];	float			oldComps[5][20];	//gScreenSize = oldPort->portRect;	GetPort(&oldPort);	for(i = 0; i <= 4; i++){		for(j = 0; j <= 104;j++)oldElements[i][j] = theElements[i][j];		for(j = 0; j <= 19;j++)oldComps[i][j]= storedComps[i][j];	}	theDialog = DM_GetNewDialog(DLOG_FIRST, 0L,(WindowPtr)-1L);		GetDItem(theDialog,PICTRECT,&type,&theHandle,&thePerTabRect);	stackNo = 0;	GetDItem(theDialog,ALLOYCOMP,&type,&theHandle,&theRect);	SetCtlValue((ControlHandle)theHandle,1);	oldItem = ALLOYCOMP;	while(1 != 2){		SystemTask();		ModalDialog(TheFilterUPP,&item);		switch(item){			case REPLACE:				if(this->theDialog != 0L)					DM_DisposDialog(&(this->theDialog));					SetPort(oldPort);				return true;			case CANCEL:			case CANCELCLICK: 				for(i = 0; i <= 4; i++){					for(j = 0; j <= 104;j++)theElements[i][j] = oldElements[i][j];					for(j = 0; j <= 19;j++)storedComps[i][j]= oldComps[i][j];				}				if(this->theDialog != 0L)					DM_DisposDialog(&(this->theDialog));				SetPort(oldPort);				return false;										case ENTERCLICK:			case ENTER:				SetPeriodicTable(stackNo);				InPutNewComp();				SetPeriodicTable(stackNo);				break;			case ALLOYCOMP:			case ALLOYCOMP1:			case ALLOYCOMP2:			case ALLOYCOMP3:			case ALLOYCOMP4:				SetPeriodicTable(stackNo);				if(stackNo > 0)					IncludeElements(0,stackNo);				GetDItem(theDialog,oldItem,&type,&theHandle,&theRect);				SetCtlValue((ControlHandle)theHandle,0);				HiliteControl((ControlHandle)theHandle,0);				GetDItem(theDialog,item,&type,&theHandle,&theRect);				SetCtlValue((ControlHandle)theHandle,1);				stackNo = item - ALLOYCOMP;				SetPeriodicTable(stackNo);				oldItem = item;				break;			case ALOOYACEPT:			case ALOOYACEPT1:			case ALOOYACEPT2:			case ALOOYACEPT3:			case ALOOYACEPT4:				GetDItem(theDialog,item,&type,&theHandle,&theRect);				theNumber = GetCtlValue((ControlHandle)theHandle);				if(theNumber){					HiliteControl((ControlHandle)theHandle,0);					SetCtlValue((ControlHandle)theHandle,0);				}				break;			case PICTRECT:				PeriodicTable();				break;			default:				break;			case ML_UPDATE_EVT:					BeginUpdate(theDialog);				UpdtDialog(theDialog,theDialog->visRgn);				if(!gAppleEvtsOK){					HiliteOK(theDialog);				}				EndUpdate(theDialog);				break;		}		if(newFlag == true){			SetPeriodicTable(0);			newFlag = false;		}	}}Boolean 	AlloyCompDef::PeriodicTable(){	Point thePoint;		Rect  theRect;	short	theHorRect,theVerRect;	short	theElement;		GetMouse(&thePoint);	if(DblClick(thePoint))		return true;	if(maxElmnts >= 2){		oldElement = -20;	}		while(PtInRect(thePoint,&thePerTabRect)){		theHorRect = (thePoint.h - thePerTabRect.left) / 25;		theVerRect = (thePoint.v - thePerTabRect.top - 7) / 25;		theElement = -1;		switch(theVerRect){			case 0:				if(theHorRect == 0 || theHorRect == 17){					if(theHorRect == 0)						theElement = 0;					else						theElement = 1;				}				break;			case 1:				if(theHorRect <= 1 || theHorRect >= 12){					if(theHorRect <= 1)						theElement = theHorRect + 2;					else						theElement = theHorRect - 8;				}				break;			case 2:				if(theHorRect <= 1 || theHorRect >= 12){					if(theHorRect <= 1)						theElement = theHorRect + 10;					else						theElement = theHorRect;				}				break;			case 3:				theElement = theHorRect + 18;				break;			case 4:				theElement = theHorRect + 36;				break;			case 5:				if(theHorRect <= 2)					theElement = theHorRect + 54;				else					theElement = theHorRect + 68;				break;			case 6:				if(theHorRect <= 3){					if(theHorRect <= 2)						theElement = theHorRect + 86;					else						theElement = theHorRect + 100;				}				break;			case 7:				if(theHorRect > 2 && theHorRect <= 16){					theElement = theHorRect + 54;				}else					return false;				break;			case 8:				if(theHorRect > 2 && theHorRect <= 16){					theElement = theHorRect + 86;				}				break;			default:				break;		}		if(theElement != oldElement){			if(maxElmnts == 1 && noOfElements == 1){				SetRect(&theRect,thePerTabRect.left +  (oldHorRect * 25),thePerTabRect.top + 7 + (oldVerRect * 25),				thePerTabRect.left + 25 + (oldHorRect * 25),thePerTabRect.top + 32 + (oldVerRect * 25));				InvertRect(&theRect);				theElements[stackNo][oldElement] = false;				noOfElements = 0;			}			if(!ToggleElement(theElement)) return false;			oldElement = theElement;			SetRect(&theRect,thePerTabRect.left +  (theHorRect * 25),thePerTabRect.top + 7 + (theVerRect * 25),			thePerTabRect.left + 25 + (theHorRect * 25),thePerTabRect.top + 32 + (theVerRect * 25));			InvertRect(&theRect);			oldHorRect = theHorRect;			oldVerRect = theVerRect;		}				GetMouse(&thePoint);		if(!StillDown()){			return false; 		}	}	return false;}void	AlloyCompDef::InPutNewComp(void){}Boolean	AlloyCompDef::ToggleElement(short theElement){	if(theElement <= -1)		return false;	if(theElements[stackNo][theElement]){		theElements[stackNo][theElement] = false;		noOfElements--;	}else{		if(noOfElements >= maxElmnts){			return false;		}		noOfElements++;		theElements[stackNo][theElement] = true;	}	return true;}Boolean	AlloyCompDef::DblClick(Point thePoint){	static long oldTime = 0;	long newTime;	static Point oldPoint;	long	dblTime;	dblTime = LMGetDoubleTime();		newTime = TickCount();	if(newTime - oldTime < dblTime && fabs((double)(oldPoint.h - thePoint.h)) <= 2  		&& fabs((double)(oldPoint.v - thePoint.v)) <= 2){		return true;	}	oldPoint = thePoint;	oldTime = newTime;	return false;}void	AlloyCompDef::DoCopyAlloyComp(AlloyCompDef* theAlloy){	short i,k;			for(i = 0; i <= 4;i++){			for(k = 0; k <= 104;k++)			theElements[i][k] =	theAlloy->theElements[i][k];	}	for(i = 0; i <= 4;i++){		for(k = 0; k <= 19;k++)				storedComps[i][k] = theAlloy->storedComps[i][k];	}}void	AlloyCompDef::DoCopy(short *elementStack){	short i,j;	j = -1;	for(i = 0; i <= 19; i++) elementStack[i] = -1;			for(i = 0; i <= 104; i++){		if(theElements[stackNo][i]) elementStack[++j] = i;	}}void	AlloyCompDef::CopyElStack(short *elementStack,short stackNo){	short i;	i = 0;	while(elementStack[i] >= 0 && i <= 19){		theElements[stackNo][elementStack[i++]] = true;	}}void	AlloyCompDef::SetPeriodicTable(short stackNo){	short i;	noOfElements = -1;	for(i = 0; i <= 104;i++){		if(theElements[stackNo][i]){			InvertRectPerTable(i);			noOfElements++;		}	}}void		AlloyCompDef::IncludeElements(short i,short j){	short k;	for(k = 0; k <= 104; k++){		if(theElements[j][k]) theElements[i][k] = true;	}}void	AlloyCompDef::InvertRectPerTable(short theElement){	short theHorRect,theVerRect;	Rect theRect;		switch(theElement){			case 0:				theHorRect = 0;				theVerRect = 0;				break;			case 1:				theHorRect = 17;				theVerRect = 0;				break;			case 2:			case 3:				theHorRect = theElement - 2;				theVerRect = 1;				break;			case 4:			case 5:			case 6:			case 7:			case 8:			case 9:				theHorRect = theElement + 8;				theVerRect = 1;				break;			case 10:			case 11:				theVerRect = 2;				theHorRect = theElement - 10;				break;			case 12:			case 13:			case 14:			case 15:			case 16:			case 17:				theVerRect = 2;				theHorRect = theElement;				break;			case 18:			case 19:			case 20:			case 21:			case 22:			case 23:			case 24:			case 25:			case 26:			case 27:			case 28:			case 29:			case 30:			case 31:			case 32:			case 33:			case 34:			case 35:				theVerRect = 3;				theHorRect = theElement - 18;				break;			case 36:			case 37:			case 38:			case 39:			case 40:			case 41:			case 42:			case 43:			case 44:			case 45:			case 46:			case 47:			case 48:			case 49:			case 50:			case 51:			case 52:			case 53:				theVerRect = 4;				theHorRect = theElement - 36;				break;			case 54:			case 55:			case 56:				theVerRect = 5;				theHorRect	 = theElement - 54;				break;			case 71:			case 72:			case 73:			case 74:			case 75:			case 76:			case 77:			case 78:			case 79:			case 80:			case 81:			case 82:			case 83:			case 84:			case 85:				theVerRect = 5;				theHorRect = theElement -  68;				break;			case 86:			case 87:			case 88:				theVerRect = 6;				theHorRect	= theElement  - 86;				break;			case 103:			case 104:			case 105:				theVerRect = 6;				theHorRect = theElement - 100;				break;			case 57:			case 58:			case 59:			case 60:			case 61:			case 62:			case 63:			case 64:			case 65:			case 66:			case 67:			case 68:			case 69:			case 70:				theHorRect = theElement - 54;				theVerRect = 7;				break;			case 89:			case 90:			case 91:			case 92:			case 93:			case 94:			case 95:			case 96:			case 97:			case 98:			case 99:			case 100:			case 101:			case 102:				theHorRect = theElement - 86;				theVerRect = 8;				break;			default:				break;		}						SetRect(&theRect,thePerTabRect.left +  (theHorRect * 25),thePerTabRect.top + 7 + (theVerRect * 25),		thePerTabRect.left + 25 + (theHorRect * 25),thePerTabRect.top + 32 + (theVerRect * 25));		InvertRect(&theRect);}void AlloyCompDef::CopyElementFiles(short rfElmnt, Boolean newElements[105]){	short j;	for(j = 0; j <= 104;j++)newElements[j] = theElements[rfElmnt][j];}void AlloyCompDef::WriteComposition(void){	gTheFile->WriteDataBlock((Ptr)theElements,(sizeof(Boolean) * 525));	gTheFile->WriteDataBlock((Ptr)storedComps,(sizeof(float) * 100));	gTheFile->WriteShort(noOfElements);}void AlloyCompDef::ReadComposition(void){	gTheFile->ReadDataBlock((Ptr)theElements,(sizeof(Boolean) * 525));	gTheFile->ReadDataBlock((Ptr)storedComps,(sizeof(float) * 100));	noOfElements = gTheFile->ReadShort();}