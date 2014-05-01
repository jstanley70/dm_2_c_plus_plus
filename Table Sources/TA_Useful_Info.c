#include	"Diffract_INCs.h"#include	"ST_StereoMacros.c"//ects:D.M. v2.0:Sources.Jim:Table Sources:TA_Useful_Info.c#include	"UT_VectorMacros.c"#include	"UsefulInfo.h"#include 	"AtomData.h"#include	"UA2_Text_Box.h"#include 	"UextBoxFree.h"#include	"SF_MathLib.h"void		DrawTheTextBox(short h,short v);void		Useful_Info::DoInit(void){	Rect theRect;	short width;	short left;	RGBColor	theColor;	Point	thePoint;	Point 	cellSize;	Rect	rDataBnds;	inherited::DoInit();	SetObjectWindowTitle	("Useful Information",360,360);	numColumns 			= 	0;	selectOn 			= 	false;	crystalNum 			= 	crystalNum2 = 0;	theListRect.top 	+= 	0;	theListRect.left 	= 	0;	theListRect.right 	= 	0;	theListRect.bottom 	= 	0;	theList							=		0L;	thePictRect = theVisRect = theWindow->portRect;	RectRgn				(picUpdateRgn,&theVisRect);	RectRgn				(drawRgn,&theVisRect);	textBox				=	(TextBoxFree*)D_new(TextBoxFree);//TextBoxFree);	SetRect(&rDataBnds,0,0,2,14);	SetRect(&theRect,4,100,342,342);	cellSize.h = 65;	cellSize.v = 15;	textBox		-> DoInit(&theRect,&rDataBnds,cellSize,0,theWindow,false,false,true,true,false,false,geneva,9,0);	textBox		-> ResetCell();	textBox		-> selected = true;	textBox		-> SetColumnWidth(0,140);	textBox		-> selected = false;	theColor				= backgroundColor;	backgroundColor.red		= 65535;	backgroundColor.green	= 65535;	backgroundColor.blue	= 65535;	theColor.red			= 0;	theColor.green		= 0;	theColor.blue			= 0;		ClipRect(&theWindow->portRect);	if(gInColor){//		backgroundColor.blue	= 0;	//	theColor.blue			= 65535;		}		SetEntryColor	(thePalette,14,&backgroundColor);	SetEntryColor	(thePalette,15,&theColor);	SetPort(theWindow);	if(theWindow != 0L)		ActivatePalette(theWindow);			DMBackColor(14);	DMForeColor(15);	InvalRect(&theWindow->portRect);	dm_EraseRect(&theWindow->portRect);		info1 = (IndexTextRec*)D_new(IndexTextRec);	info1->DoITRInit(false,theCrystal[0],120,60,0);	info2 = (IndexTextRec*)D_new(IndexTextRec);	info2->DoITRInit(false,theCrystal[0],224,60,0);					theCrystalButton 	= 	(CrystalButton*)D_new(CrystalButton);	theCrystalButton	->	DoInit();		theCrystalButton	->	SetOwner((Ptr)theCrystal[crystalNum]);	sprintf					(gTheText,"Crystal %d",crystalNum + 1);	width 				= 	StringWidth("\pCrystal 1");	left 				= 	115;	SetRect					(&theRect,left,6,width + 5 + left,20);		theCrystalButton	->	SetButtonRect(theRect);	theCrystalButton	->	SetName(gTheText);	theCrystalButton	->	Enable();		theCrystalButton2 	= 	(CrystalButton*)D_new(CrystalButton);	theCrystalButton2	->	DoInit();		theCrystalButton2	->	SetOwner((Ptr)theCrystal[crystalNum2]);	left 				= 	222;	SetRect					(&theRect,left,6,width + 5 + left,20);	theCrystalButton2	->	SetButtonRect(theRect);	sprintf					(gTheText,"Crystal %d",crystalNum2 + 1);	theCrystalButton2	->	SetName(gTheText);	theCrystalButton2	->	Enable();	TextFont(0);	TextSize(0);		menu1	= (PopUpMenu*)D_new(PopUpMenu);	menu2	= (PopUpMenu*)D_new(PopUpMenu);		thePoint.h = 118;	thePoint.v = 23;			menu1->InitAtPoint(thePoint,1,52,false,true);/* July 1992 */	thePoint.h = 222;	menu2->InitAtPoint(thePoint,1,52,false,true);/* July 1992 */		/*KillMBHandle*/DisposeHandle		(theMenuBar);	theMenuBar 	= 	GetMenuBar();	DrawMenuBar		();	objectType = USEFUL_INFO;/* New 9/10/92*/	DrawTheText();	return;}void Useful_Info::SetObjectMenu(void){	theMenuBar = GetNewMBar(TABLE_MB);	ClearMenuBar();/* July 1992 */	SetMenuBar(theMenuBar);}void		Useful_Info::DoClose(void){	theCrystalButton->DoClose();	theCrystalButton2->DoClose();		info1->DoClose();	info2->DoClose();	menu1->DoClose();	menu2->DoClose();	textBox->DoClose();		inherited::DoClose();	return;}Boolean		Useful_Info::DoContent(Point thePoint){	PenState theState;	long theResult;		GetPenState(&theState);	PenNormal();	info1->DoHit(thePoint);	info2->DoHit(thePoint);		theResult = menu2->HitPopUp(thePoint);	theResult = menu1->HitPopUp(thePoint);		theCrystalButton->DoContent(thePoint);	theCrystalButton2->DoContent(thePoint);			SetPenState(&theState);		info1->GetValue();	info2->GetValue();	textBox->DoContent(thePoint);	return(inherited::DoContent(thePoint));}void			Useful_Info::DoIdle(void){	GrafPtr		thePort;	GetPort(&thePort);	SetPort(theWindow);	inherited::DoIdle();	textBox->DoIdle();	info1->DoIdle();	info2->DoIdle();		if(		h1 != info1->h || k1 != info1->k || l1 != info1->l ||			h2 != info2->h || k2 != info2->k || l2 != info2->l ||			crystalNum 	!= (menu1->lastResult - 1) || 			crystalNum2 != (menu2->lastResult - 1)) {				h1 = info1->h;		k1 = info1->k;		l1 = info1->l;		h2 = info2->h;		k2 = info2->k;		l2 = info2->l;				crystalNum 	= (menu1->lastResult - 1);		crystalNum2 = (menu2->lastResult - 1);			info1->theCrystal = theCrystal[crystalNum];		info2->theCrystal = theCrystal[crystalNum2];				theCrystalButton->SetOwner((Ptr)theCrystal[crystalNum]);		sprintf(gTheText,"Crystal %d",crystalNum + 1);		theCrystalButton->SetName(gTheText);		theCrystalButton->Enable();							theCrystalButton2->SetOwner((Ptr)theCrystal[crystalNum2]);		sprintf(gTheText,"Crystal %d",crystalNum2 + 1);		theCrystalButton2->SetName(gTheText);		theCrystalButton2->Enable();			DrawTheText();		SetPort(thePort);	} else {	SetPort(thePort);		return;	}							return;}#define DEFINE_TABLE 500void		Useful_Info::DoMenu(long	theResult){	short	theMenu,theItem;		theMenu = HiWord(theResult);	theItem = LoWord(theResult);		switch(theMenu){		case EDIT_MENU:			switch(theItem){				case CUT:				case COPY:				case PASTE:				case DO_CLEAR:					return;					break;				default:					break;			}			break;		case DEFINE_TABLE:			break;		default:			break;	}	inherited::DoMenu(theResult);	return;}void Useful_Info::DrawTheText(void){	Rect 		theRect;	double 		value1,				value2;	Index 		*dir1,				*dir2;	double 		U,V,W;	short		i;	double		electronMassRation,dspace1,dspace2;	Boolean		dbFlag;	char		theText[10];	dcomplex	sF1,sF2;	GrafPtr		thePort;	dcomplex  cAb;		GetPort(&thePort);	SetPort(theWindow);	dbFlag = g_DebyeWaller;		theRect 			= 	thePictRect;	InsetRect				(&theRect,2,2);	dm_FrameRect				(&theRect);	theCrystalButton	->	DrawButtonRect(crystalNum + 10);	theCrystalButton2	->	DrawButtonRect(crystalNum2 + 10);	//DMForeColor(COMPLEMENT_COLOR);	info1->DoRefresh();	info2->DoRefresh();	menu1->SetPopUp();	menu2->SetPopUp();		textBox->SetDrawFlag(false);	textBox->ResetCell();			TextFont(3);	TextSize(8);		if(info1->h == info1->k && info1->h == info1->l && info1->h == 0){/* Added July 1992 */		SetPort(thePort);		return;	}	if(info2->h == info2->k && info2->h == info2->l && info2->h == 0){/* Added July 1992 */		SetPort(thePort);		return;	}/* I have made a number of changes to this subroutine July 1992 */		sprintf(gTheText,"D-spacings:");	textBox->AddListText(gTheText);	dspace1 		= 	info1->TheDSpacing();	sprintf			(gTheText,"%6.3f �",dspace1);	textBox->AddListText(gTheText);		dspace2 		= 	info2->TheDSpacing();	sprintf			(gTheText,"%6.3f �",dspace2);	textBox->AddListText(gTheText);			/*Structure Factor without Debye &  No accel. Voltage*/	electronMassRation = 1. + ( (double)energy / 511.);	for(i = 0;i < 5;i++){		switch(i)		{			case 0:				g_DebyeWaller = false;				g_StructureEV = 0;				electronMassRation = 1. + ( (double)energy / 511.);				sprintf(gTheText,"S.F. no DW, accel.V");				sprintf(theText," �-1");			break;			case 1:				g_DebyeWaller = true;				g_StructureEV = 0;				electronMassRation = 1. + ( (double)energy / 511.);				sprintf(gTheText,"S.F. DW, accel.V");				sprintf(theText," �-1");			break;			case 2:				g_DebyeWaller = false;				g_StructureEV = 1;				electronMassRation = 1;				sprintf(gTheText,"Vg no DW,no accel.V");				sprintf(theText," eV");			break;			case 3:				g_DebyeWaller = false;				g_StructureEV = 2;				electronMassRation = 1. + ( (double)energy / 511.);				sprintf(gTheText,"Ug no DW,accel.V");				sprintf(theText," �-2");			break;						case 4:				g_DebyeWaller = true;				g_StructureEV = 2;				electronMassRation = 1. + ( (double)energy / 511.);				sprintf(gTheText,"Ug DW,accel. V");				sprintf(theText," �-2");			break;		}		textBox->AddListText(gTheText);		if(dspace1 > 0){			value1 	= 	info1->IndexStructureFactor(1./ dspace1,electronMassRation,theBeamFlag,&cAb);			sF1 = ConvertStructureFactor(info1->sF,g_StructureEV,info1->theCrystal->volume);			value1 	= 	Cabs(sF1);			if(value1 < .8) sprintf		(gTheText,"%.4e",value1);			else sprintf		(gTheText,"%6.2f",value1);			strcat(gTheText,theText);		}else{			value1 = 0;			sprintf		(gTheText,"Undefined");		}		textBox->AddListText(gTheText);				if(dspace2 > 0){			value2 = info2->IndexStructureFactor(1./ dspace2,electronMassRation,theBeamFlag,&cAb);			sF2 = ConvertStructureFactor(info2->sF,g_StructureEV,info2->theCrystal->volume);			value2 	= 	Cabs(sF2);			if(value2 < .8) sprintf		(gTheText,"%.4e",value2);			else sprintf		(gTheText,"%6.2f",value2);			strcat(gTheText,theText);		}else{			sprintf(gTheText,"Undefined");			value2 = 0;		}		textBox->AddListText(gTheText);		sprintf		(gTheText," ");		textBox->AddListText(gTheText);		if(dspace1 > 0){									if(value1 < .8)sprintf		(gTheText,"%.4e,%.4e",sF1.r,sF1.i);			else sprintf		(gTheText,"%6.2f,%6.2f",sF1.r,sF1.i);			strcat(gTheText,theText);		}else{			value1 = 0;			sprintf		(gTheText,"Undefined");		}				textBox->AddListText(gTheText);		if(dspace2 > 0){				if(value1 < .8)sprintf		(gTheText,"%.4e,%.4e",sF2.r,sF2.i);			else sprintf		(gTheText,"%6.2f,%6.2f",sF2.r,sF2.i);			strcat(gTheText,theText);		}else{			sprintf(gTheText,"Undefined");			value2 = 0;		}		textBox->AddListText(gTheText);			}	for(i = 0;i < 2;i++){		switch(i)		{			case 0:				g_DebyeWaller = false;				electronMassRation = 1. + ( (double)energy / 511.);				sprintf(gTheText,"Extinc. D. no Debye W");				sprintf(theText," �");			break;			case 1:				g_DebyeWaller = true;				electronMassRation = 1. + ( (double)energy / 511.);				sprintf(gTheText,"Extinc. D.Debye W");				sprintf(theText," �");			break;					}		textBox->AddListText(gTheText);		if(value1 >= .001){			value1 = info1->TheExtinctionDist(1./ wavelength,electronMassRation,theBeamFlag);			sprintf(gTheText,"%6.2f",value1);			strcat(gTheText,theText);		}else{			sprintf(gTheText,"Undefined");		}		textBox->AddListText(gTheText);				if(value2 >= .001){			value2 = info2->TheExtinctionDist(1./ wavelength,electronMassRation,theBeamFlag);			sprintf(gTheText,"%6.2f",value2);			strcat(gTheText,theText);		}else{			sprintf(gTheText,"Undefined");					}		textBox->AddListText(gTheText);	}			dir1 = (Index*)D_new(Index);	dir1->Init(true,info1->theCrystal);	info1->DirOfPlaneNormal(&U,&V,&W);	dir1->h = U * 10;	dir1->k = V * 10;	dir1->l = W * 10;		dir2 = (Index*)D_new(Index);	dir2->Init(true,info1->theCrystal);	info2->DirOfPlaneNormal(&U,&V,&W);	dir2->h = U * 10;	dir2->k = V * 10;	dir2->l = W * 10;	sprintf(gTheText,"Plane Normal");	textBox->AddListText(gTheText);	dir1	->	IndexToSimpleText(gTheText);	textBox->AddListText(gTheText);		dir2->IndexToSimpleText(gTheText);	textBox	->AddListText(gTheText);			sprintf			(gTheText,"Angle Bet. Planes");	textBox	->AddListText(gTheText);	sprintf			(gTheText,"%6.2f�",info1->GetTheAngle(info2));	textBox	->AddListText(gTheText);	sprintf			(gTheText," ");	textBox	->AddListText(gTheText);			sprintf(gTheText,"Zone Axis");	textBox->AddListText(gTheText);		if(info1->h == info2->h  && info1->k == info2->k && info1->l == info2->l && info1->theCrystal == info2->theCrystal){		sprintf(gTheText,"undefined");		textBox->AddListText(gTheText);	}else{		info1->ZoneAxis(info2,dir1);		dir1->IndexToSimpleText(gTheText);		textBox->AddListText(gTheText);	}	dir1->DoClose();	dir2->DoClose();	textBox->SetDrawFlag(true);		TextFont(0);	TextSize(12);	if(selectOn){		if(info1->active){			TESetSelect(0,32767,info1->theTextRec);		}else{			TESetSelect(0,32767,info2->theTextRec);		}		}	g_DebyeWaller = dbFlag;	SetPort(thePort);	return;}void		DrawTheTextBox(short h,short v){	Rect theRect;	short length,height;	FontInfo theFont;		GetFontInfo(&theFont);	c2pstr(gTheText);	height = theFont.ascent + theFont.descent;	length = StringWidth(pTheText);		SetRect(&theRect,h,v - height,h + 80,v);	dm_EraseRect(&theRect);	MoveTo(h,v);	DrawString(pTheText);}void		Useful_Info::MyCalculate(void){			 DrawTheText();	return;}void		Useful_Info::DisplayList(void){	return;}void		Useful_Info::DoPrint(void){	inherited::DoPrint();	return;}void			Useful_Info::DoKey(char	theChar){	Point	thePoint;	if(theChar == (char)3  || theChar == (char)13 || theChar == '\t'){		selectOn = true;		if(info1->active){			info1->active = false;			info2->active = true;			TEActivate(info2->theTextRec);			TESetSelect(0,32767,info2->theTextRec);			TEDeactivate(info1->theTextRec);					}else{			info1->active = true;			info2->active = false;			TEActivate(info1->theTextRec);			TESetSelect(0,32767,info1->theTextRec);			TEDeactivate(info2->theTextRec);					}		info1->GetValue();		info2->GetValue();		thePoint.h = 0;		thePoint.v = 0;		//DrawTheText();		inherited::DoContent(thePoint);				return;	}	selectOn = false;	info1->DoKey(theChar);	info2->DoKey(theChar);	inherited::DoKey(theChar);	return;}void			Useful_Info::DoRefresh(void){	GrafPtr	theOldPort;		GetPort(&theOldPort);	SetPort(theWindow);	DMBackColor(BACKGROUND_COLOR);	DMForeColor(COMPLEMENT_COLOR);	//dm_EraseRect(&theWindow->portRect);	inherited::DoRefresh();	if(theOldPort == theWindow)textBox->DrawList();	SetPort(theOldPort);		return;}void	Useful_Info::DoHeaderPrint(Rect *topRect){	Rect			theRect;	short 			theTop = 16,theLeft = 8,i,n,x,y,z,max,dif;	IndexTextRec	*info;	double			value,U,V,W;	char			theText[100];	Index 			*dir;	double			electronMassRation;	dcomplex		cAb;	dataLabel[0] = 	(char)0;	c2pstr			(dataLabel);	theRect 	= 	*topRect;	n 			= 	16 - theRect.top;	x 			= 	theRect.left + 380;	z 			= 	14;	max 		= 	StringWidth("\pExtinction Distance");	electronMassRation = 1. + ( (double)energy / 511.);		sprintf			(tabelTitle,"Useful Info -- Crystal 1");	c2pstr			(tabelTitle);	i = 			(theRect.right + theRect.left - StringWidth((unsigned char*)tabelTitle)) / 2;	MoveTo			(i,theRect.top + n);	DrawDiffractString		((unsigned char*)tabelTitle);	InsetRect		(&theRect,3,3);	dm_FrameRect		(&theRect);	theTop 		= 	theRect.top + n + 25;	theLeft 	= 	theRect.left + 2;		theCrystal[crystalNum]->TableDisplayCrystal(&theLeft,&theTop);	info 	= info1;		y 			= theRect.top + 54;	dif 		= max - StringWidth("\pPlane:");	sprintf			(theText,"Plane:  (");	info		->	IndexToText(gTheText);	p2cstr			(pTheText);	strcat			(theText,gTheText);	strcat			(theText,")");	MoveTo			(x + dif,y);	y 			+= 	z;	c2pstr			(theText);	DrawTheString	(theText);		value 		= 	info->TheDSpacing();	dif 		= 	max - StringWidth("\pD Spacing:");	sprintf			(theText,"D Spacing:  %6.3f �",value);	MoveTo			(x + dif,y);	y += z;	DrawString(c2pstr(theText));		value = info->IndexStructureFactor(1./ value,1.0,theBeamFlag,&cAb);	dif = max - StringWidth("\pStructure Factor:");	sprintf(theText,"Structure Factor:  %6.2f ",value);	MoveTo(x + dif,y);	y += z;	DrawString(c2pstr(theText));		if(value >= .001){		value = info->TheExtinctionDist(1./ wavelength,electronMassRation,theBeamFlag);		sprintf(theText,"Extinction Distance:  %6.2f �",value);	}else{		sprintf(gTheText,"Undefined");	}	MoveTo(x,y);	y += z;	DrawString(c2pstr(theText));		dir = (Index*)D_new(Index);	dir->Init(true,info->theCrystal);	info->DirOfPlaneNormal(&U,&V,&W);	dir->h = U * 10;	dir->k = V * 10;	dir->l = W * 10;	dif = max - StringWidth("\pPlane Normal:");	sprintf(theText,"Plane Normal:  [");	dir->IndexToText(gTheText);	p2cstr(pTheText);	strcat(theText,gTheText);	strcat(theText,"]");	MoveTo(x + dif,y);	y += z;	c2pstr(theText);	DrawTheString(theText);	dir->DoClose();			/***************Crystal 2 ****************/		theRect = *topRect;	OffsetRect(&theRect,0,theRect.bottom);	sprintf(tabelTitle,"Useful Info -- Crystal 2");	c2pstr(tabelTitle);	i = (theRect.right + theRect.left - StringWidth((unsigned char*)tabelTitle)) / 2;	MoveTo(i,theRect.top + n);	DrawDiffractString((unsigned char*)tabelTitle);	InsetRect(&theRect,3,3);	dm_FrameRect(&theRect);	theTop = theRect.top + n + 25;	theLeft = theRect.left + 2;	theCrystal[crystalNum2]->TableDisplayCrystal(&theLeft,&theTop);	info 	= info2;		y = theRect.top + 54;	dif = max - StringWidth("\pPlane:");	sprintf(theText,"Plane:  (");	info->IndexToText(gTheText);	p2cstr(pTheText);	strcat(theText,gTheText);	strcat(theText,")");	MoveTo(x + dif,y);	y += z;	c2pstr(theText);	DrawTheString(theText);		value 	= info->TheDSpacing();	dif = max - StringWidth("\pD Spacing:");	sprintf(theText,"D Spacing:  %6.3f �",value);	MoveTo(x + dif,y);	y += z;	DrawString(c2pstr(theText));		value = info->IndexStructureFactor(1./ value,1.0,theBeamFlag,&cAb);	dif = max - StringWidth("\pS.F. pure (�-1):");	sprintf(theText,"Structure Factor:  %6.2f ",value);	MoveTo(x + dif,y);	y += z;	DrawString(c2pstr(theText));		electronMassRation = 1. + ( (double)energy / 511.);	value = info->IndexStructureFactor(1./ value,electronMassRation,theBeamFlag,&cAb);	dif = max - StringWidth("\pS.F. corrected for kV:");	sprintf(theText,"Structure Factor:  %6.2f ",value);	MoveTo(x + dif,y);	y += z;	DrawString(c2pstr(theText));			if(value >= .001){		value = info->TheExtinctionDist(1./ wavelength,electronMassRation,theBeamFlag);		sprintf(theText,"Extinction Distance:  %6.2f �",value);	}else{		sprintf(gTheText,"Undefined");	}	MoveTo(x,y);	y += z;	DrawString(c2pstr(theText));		dir = (Index*)D_new(Index);	dir->Init(true,info->theCrystal);	info->DirOfPlaneNormal(&U,&V,&W);	dir->h = U * 10;	dir->k = V * 10;	dir->l = W * 10;	dif = max - StringWidth("\pPlane Normal:");	sprintf(theText,"Plane Normal:  [");	dir->IndexToText(gTheText);	p2cstr(pTheText);	strcat(theText,gTheText);	strcat(theText,"]");	MoveTo(x + dif,y);	y += z;	c2pstr(theText);	DrawTheString(theText);	dir->DoClose();		x = theRect.left + 20;	y = theRect.bottom + 40;	value = info1->GetTheAngle(info2);	MoveTo(x,y);	y += z;	sprintf(theText,"Intersection Angle:  %6.2f�",value);	DrawString(c2pstr(theText));		dir = (Index*)D_new(Index);	dir->Init(true,info1->theCrystal);	info1->ZoneAxis(info2,dir);	sprintf(theText,"Zone Axis:  [");	dir->IndexToText(gTheText);	p2cstr			(pTheText);	strcat			(theText,gTheText);	strcat			(theText,"]");	c2pstr			(theText);	MoveTo			(x,y);	DrawTheString	(theText);	dir->DoClose();}void	Useful_Info::DoDblClick(Point thePoint){	theCrystalButton->DoDblClick(thePoint);	theCrystalButton2->DoDblClick(thePoint);	if(textBox != 0L)textBox->DoDblClick(thePoint);	return ;}void	Useful_Info::SetPict(Boolean eraseFlag){	Boolean dummy;		dummy = eraseFlag; /* This line suppresses a warning */		DrawTheText();}void	Useful_Info::InitList(void){}