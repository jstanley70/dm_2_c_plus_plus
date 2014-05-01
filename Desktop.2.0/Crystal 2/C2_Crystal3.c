//C2_Crystal3.c#include	"Diffract_INCs.h"#include	"Dir_Paths.h"#include	"a_Crystal.h"typedef struct CODENS{	char	coden[6];	char	journal[74];}CODENS;short DetermineJournal(CODENS **foundCODENS,short found);void		Crystal::ListDspacings(Rect *frameRect,short size,short font){	short theLeft,theTop;	Index	*theIndex;	short 	i,k,charHeight12;	short	maxWidth = 0,width;	FontInfo	theFont;	char	theText[255];			TextFont(font);	TextSize(size);	GetFontInfo(&theFont);	theTop = frameRect->top;	theLeft = frameRect->left;	theIndex = (Index*)D_new(Index);	charHeight12 = theFont.ascent + theFont.descent + theFont.leading + 3;	for(i = 0,k = 0;i < MAX_STORE_DSPACE;i++,k++){		if(k == 8){			theTop = frameRect->top;			theLeft += maxWidth + 10;			k = 0;		}		if(dspacings[i].x <= 0){			k--;			continue;		}		 theIndex->CopyPtrToIndex((Ptr)(&dspacings[i]));		 theIndex->IndexToStraightText(gTheText);		 sprintf(theText," %6.4f %6.1f",theIndex->x,theIndex->intensity);		 strcat			(gTheText,theText);		 theTop 	+= 	charHeight12;		 MoveTo			(theLeft,theTop);		 DrawDiffractString(c2pstr(gTheText));		 width 		= 	StringWidth(pTheText);		 if(width > maxWidth)maxWidth = width;	}	TextFont(0);	TextSize(0);	theIndex->DoClose();}void		Crystal::ListComments(Rect *dm_FrameRect,short size,short font){	TextFont(font);	TextSize(size);	TextFont(0);	TextSize(0);	SetRect(dm_FrameRect,0,0,0,0);}void		Crystal::ListAtomicPositions(Rect *dm_FrameRect,short size,short font){	short sizeLoc,fontLoc;	SetRect(dm_FrameRect,0,0,0,0);	sizeLoc = size;	fontLoc = font;}void		Crystal::GetReciprocalLattice(UnitCell *theRCell,short dimension){	double	cosAng,volu2D,sinAG;		if(dimension == 2)		{			cosAng		=	 cos(PI * theUnitCell.gamma / 180);			volu2D		=	theUnitCell.a * theUnitCell.b * (1 - (cosAng * cosAng));			(theRCell->a) = theUnitCell.b / volu2D;			(theRCell->b) = theUnitCell.a / volu2D;			(theRCell->gamma) = 180 * acos(-cosAng) / PI;			return;		}		sinAG	=	 sin(PI * theUnitCell.alpha / 180);		(theRCell->a) = theUnitCell.b *  theUnitCell.c * sinAG / volume;		sinAG	=	 sin(PI * theUnitCell.beta / 180);		(theRCell->b) = theUnitCell.a * theUnitCell.c * sinAG / volume;				sinAG	=	 sin(PI * theUnitCell.gamma / 180);		(theRCell->c) = theUnitCell.a * theUnitCell.b * sinAG / volume;				(theRCell->gamma) 	= 180 * acos(-cos(theUnitCell.gamma)) / PI;		(theRCell->alpha) 	= 180 * acos(-cos(theUnitCell.alpha)) / PI;		(theRCell->beta) 	= 180 * acos(-cos(theUnitCell.beta)) / PI;		return;	}void		Crystal::SetCrystalFromBasisSet(Index *index1,Index *index2,Index *index3,Boolean realBasis){	double a1,a2,a3,b1,b2,b3,c1,c2,c3;	theUnitCell.a = index1->GetLength();	theUnitCell.b = index2->GetLength();	theUnitCell.c = index3->GetLength();		theUnitCell.alpha = 180 * index2->GetTheXYZAngle(index3) / PI;	theUnitCell.beta = 180 * index3->GetTheXYZAngle(index1)/ PI;	theUnitCell.gamma = 180 * index1->GetTheXYZAngle(index2) / PI;		GetSvals();	CellVolume();	if(realBasis == true){		NormalizeAlphBetGam();		DetermineBraviasLattice();		return;	}	CrossProduct(&a1,&a2,&a3,(double)index2->x,(double)index2->y,(double)index2->z,			(double)index3->x,(double)index3->y,(double)index3->z);	CrossProduct(&b1,&b2,&b3,(double)index3->x,(double)index3->y,(double)index3->z,	(double)index1->x,(double)index1->y,(double)index1->z);	CrossProduct(&c1,&c2,&c3,(double)index1->x,(double)index1->y,(double)index1->z,		(double)index2->x,(double)index2->y,(double)index2->z);	index1->x = a1 / volume;	index1->y = a2 / volume;	index1->z = a3 / volume;		index2->x = b1 / volume;	index2->y = b2 / volume;	index2->z = b3 / volume;		index3->x = c1 / volume;	index3->y = c2 / volume;	index3->z = c3 / volume;		theUnitCell.a = index1->GetLength();	theUnitCell.b = index2->GetLength();	theUnitCell.c = index3->GetLength();		theUnitCell.alpha = 180 * fabs(index2->GetTheXYZAngle(index3)) / PI;	theUnitCell.beta = 180 * fabs(index3->GetTheXYZAngle(index1)) / PI;	theUnitCell.gamma = 180 * fabs(index1->GetTheXYZAngle(index2)) / PI;		NormalizeAlphBetGam();	GetSvals();	CellVolume();		DetermineBraviasLattice();		DetermineCrystalFaces();	DetermineVertices();	DetermineEdges();}float GetAcuteAngle(float angle);void Crystal::NormalizeAlphBetGam(void){		theUnitCell.alpha = GetAcuteAngle( theUnitCell.alpha);	theUnitCell.beta = GetAcuteAngle( theUnitCell.beta);	theUnitCell.gamma = GetAcuteAngle( theUnitCell.gamma);}float GetAcuteAngle(float angle){	while(angle > 360)angle -= 360;	while(angle < 0)angle += 360;	if(angle > 180) angle -= 180;	return angle;}void Crystal::DetermineReducedCell(float* y){	double m[3][3],t[2][3];	double mSW[3][3],number;	Index	*index1,*index2,*index3;	if(fabs(theUnitCell.a - theUnitCell.b) < (*y * theUnitCell.a)){		number = (theUnitCell.a + theUnitCell.b) / 2;		theUnitCell.a = number;		theUnitCell.b = number;		}	if(fabs(theUnitCell.a - theUnitCell.c) < (*y * theUnitCell.a)){		number = (theUnitCell.a + theUnitCell.c) / 2;		theUnitCell.a = number;		theUnitCell.c = number;		}	if(fabs(theUnitCell.b - theUnitCell.c) < (*y * theUnitCell.b)){		number = (theUnitCell.b + theUnitCell.c) / 2;		theUnitCell.b = number;		theUnitCell.c = number;		}		if(fabs(theUnitCell.alpha - theUnitCell.beta) < (*y * theUnitCell.alpha)){		number = (theUnitCell.alpha + theUnitCell.beta) / 2;		theUnitCell.alpha = number;		theUnitCell.beta = number;		}	if(fabs(theUnitCell.alpha - theUnitCell.gamma) < (*y * theUnitCell.alpha)){		number = (theUnitCell.alpha + theUnitCell.gamma) / 2;		theUnitCell.alpha = number;		theUnitCell.gamma = number;		}	if(fabs(theUnitCell.beta - theUnitCell.gamma) < (*y * theUnitCell.alpha)){		number = (theUnitCell.beta + theUnitCell.gamma) / 2;		theUnitCell.beta = number;		theUnitCell.gamma = number;		}	if(fabs(theUnitCell.alpha - 90) < (*y * theUnitCell.alpha))theUnitCell.alpha 	= 90;	if(fabs(theUnitCell.beta - 90) < (*y * theUnitCell.alpha))theUnitCell.beta 	= 90;	if(fabs(theUnitCell.gamma - 90) < (*y * theUnitCell.alpha))theUnitCell.gamma 	= 90;	if(fabs(theUnitCell.alpha - 120) < (*y * theUnitCell.alpha))theUnitCell.alpha = 120;	if(fabs(theUnitCell.beta - 120) < (*y * theUnitCell.alpha))theUnitCell.beta 	= 120;	if(fabs(theUnitCell.gamma - 120) < (*y * theUnitCell.alpha))theUnitCell.gamma = 120;	Get001XForm(m);	GetReductionMatrix(m,t);	if(t[0][0] > t[0][1]){		SetMatrix(mSW,0,1,0,1,0,0,0,0,1);		MatrixMultiply(m,mSW,m);		GetReductionMatrix(m,t);	}	if(t[0][1] > t[0][2]){		SetMatrix(mSW,1,0,0,0,0,1,0,1,0);		MatrixMultiply(m,mSW,m);		GetReductionMatrix(m,t);	}	if(t[0][0] > t[0][1]){		SetMatrix(mSW,0,1,0,1,0,0,0,0,1);		MatrixMultiply(m,mSW,m);		GetReductionMatrix(m,t);	}	index1	=	(Index*)D_new(Index);	index2	=	(Index*)D_new(Index);	index3	=	(Index*)D_new(Index);	index1->x = m[0][0];index1->y = m[0][1];index1->z = m[0][2];	index2->x = m[1][0];index2->y = m[1][1];index2->z = m[1][2];	index3->x = m[2][0];index3->y = m[2][1];index3->z = m[2][2];	SetCrystalFromBasisSet(index1,index2,index3,true);	index1->DoClose();	index2->DoClose();	index3->DoClose();	}void Crystal::GetReductionMatrix(double m[3][3],double t[2][3]){	t[0][0] = DotProductXYZ(m[0][0],m[0][1],m[0][2],m[0][0],m[0][1],m[0][2]);	t[0][1] = DotProductXYZ(m[1][0],m[1][1],m[1][2],m[1][0],m[1][1],m[1][2]);	t[0][2] = DotProductXYZ(m[2][0],m[2][1],m[2][2],m[2][0],m[2][1],m[2][2]);	t[1][0] = DotProductXYZ(m[2][0],m[2][1],m[2][2],m[1][0],m[1][1],m[1][2]);	t[1][1] = DotProductXYZ(m[0][0],m[0][1],m[0][2],m[2][0],m[2][1],m[2][2]);	t[1][2] = DotProductXYZ(m[1][0],m[1][1],m[1][2],m[0][0],m[0][1],m[0][2]);}void Crystal::ConvertJournalToCODEN(void){	short 		vRefNum,refNum;	short 		i,testLength = 76;	long 		found = 0;	long		noAllowed = 99;	long 		dataLength = 100,number = 500;	long 		posOff = 0,m = -1,sizeOfBlock,curEOF;	char		*theText,*cpText;	CODENS	 	*theSets,**foundCODENS; 	Boolean		flag = true;	OSErr 		fileError,theErr;	char		aText[255];	if(cODEN == 0L) return;		sprintf(aText,"CODENS.DAT;1");	fileError = OpenNamedFile(SEARCH_EDD_CODENS,aText,&vRefNum,&refNum);	if(fileError){		sprintf(aText,"CODENS.DAT");		fileError = OpenNamedFile(SEARCH_EDD_CODENS,aText,&vRefNum,&refNum);		if(fileError){			FileErrorMessages_DirPath( fileError);			return ;		}	}	theErr = GetEOF(refNum,&curEOF);		sizeOfBlock = sizeof(CODENS) * number ;	theSets 		= 	(CODENS*)D_NewPtr(sizeOfBlock);	theText			= 	(char*)D_NewPtr(sizeof(char) * (testLength + 1));	cpText			= 	(char*)D_NewPtr(sizeof(char) * (testLength + 1));	theText[testLength] 	= 0;	cpText[testLength] 	= 0;	foundCODENS 		= 	(CODENS**) D_NewHandle(dataLength * sizeof(CODENS));	strncpy			(theText,cODEN,testLength);	ConvertToCaps	(theText,testLength);		do{		m++;			posOff = (m * sizeOfBlock);		if(curEOF < posOff + sizeOfBlock){			flag = false;			sizeOfBlock = curEOF - posOff;			number = sizeOfBlock / sizeof(CODENS);		}				SetFPos		(refNum,fsFromStart,posOff);		theErr = 	FSRead(refNum,&sizeOfBlock,(Ptr)(theSets));				for(i = 0; i < number;i++)		{			strncpy(cpText,theSets[i].journal,testLength);			if(!StringCompare(cpText,theText)){				if(!CheckHandleSize((Handle)foundCODENS,&noAllowed,found,sizeof(CODENS)))continue;				(*foundCODENS)[found] = theSets[i];				found++;			}		}	}while(flag && !Quit());	if(found > 1){		strncpy(cODEN,(*foundCODENS)[DetermineJournal(foundCODENS,found)].coden,6);	}else if(found == 1){		strncpy(cODEN,(*foundCODENS)[0].coden,6);	}	cODEN[6] = 0;	KillPtr		(theSets);	KillHandle	((Handle*)&foundCODENS);	KillPtr		((Ptr)theText);	KillPtr		((Ptr)cpText);	theErr 	= 	FSClose(refNum);		return;		}void Crystal::GetDWFactor(AtomPtr theAtom){		double bi;	double	phi[7][10] = {1,.975,.951,.928,.904,.882,.860,.839,.818,.797,						.778,.758,.739,.721,.703,.686,.669,.653,.637,.622,						.607,.592,.578,.565,.552,.539,.526,.514,.503,.491,						.480,.47,.46,.45,.44,.431,.422,.413,.404,.396,						.388,.380,.373,.366,.359,.352,.345,.339,.333,.327,						.321,.315,.310,.304,.299,.294,.289,.285,.280,.276,						.271,.267,.263,.259,.255,.251,.248,.244,.241,.237};	double debeyeTheta[92] = {101.00,28.40,356.25,1188.00,1250.00,2021.00,68.00,0.00,0.00,								63.00,163.00,316.00,397.20,581.50,0.00,0.00,115.00,85.00,							131.50,225.00,0.00,386.00,356.50,445.00,380.00,411.00,385.00,							425.50,315.00,252.50,182.50,305.50,254.50,135.00,110.00,63.00,							71.50,159.50,0.00,269.00,276.50,374.00,0.00,413.00,342.50,							271.50,216.00,146.00,103.50,227.00,170.00,120.00,106.00,55.00,							48.00,115.00,142.00,0.00,0.00,0.00,0.00,0.00,0.00,							152.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00,213.00,							237.50,327.00,292.50,253.00,285.00,236.50,172.40,68.50,98.00,							78.06,91.00,0.00,0.00,0.00,0.00,0.00,0.00,157.50,							0.00,200.00};//87	short 			item;	/*pascal Boolean 	TheFilter();*/	DialogPtr		aDialog;	Boolean			theFlag = true;	double			atWt,atZ,dbTheta,temp,ratio;	short			shr,shy;	aDialog = DM_GetNewDialog(135,0L,(WindowPtr)-1L);	bi = theAtom->bi;		AtomicZAtomicWt(theAtom,&atZ,&atWt);	if(theAtom->atomicNum > 92)	{		dbTheta = debeyeTheta[91];	}else{		dbTheta = debeyeTheta[theAtom->atomicNum - 1];	}	SetItemValue(aDialog,5,dbTheta,1);	SetItemValue(aDialog,6,293,3);	SetItemValue(aDialog,4,bi,3);	if(bi == 0)	theFlag = false;	else theFlag = true;	theFlag = SwitchBoolean(aDialog,3,theFlag);	while(1 != 2){		SystemTask();		ModalDialog(TheFilterUPP,&item);		switch(item){			case ML_UPDATE_EVT:					BeginUpdate(aDialog);				UpdtDialog(aDialog,aDialog->visRgn);				if(!gAppleEvtsOK){					HiliteOK(aDialog);				}				EndUpdate(aDialog);				break;			case 1:				theAtom->bi = GetItemValue(aDialog,4);;				DM_DisposDialog(&aDialog);				return ;			case CANCEL:			case CANCELCLICK: 				DM_DisposDialog(&aDialog);				return;			case 3:				theFlag = SwitchBoolean(aDialog,3,theFlag);				break;			default:				if(theFlag){					dbTheta = GetItemValue(aDialog,5);					temp	 = GetItemValue(aDialog,6);					ratio = temp / dbTheta;					shr = ratio;					ratio -= shr;					shy 	= Round(ratio * 10);					bi = 11492 * temp * phi[shr][shy]/(atWt * dbTheta * dbTheta) + (2873/(atWt * dbTheta));					SetItemValue(aDialog,4,bi,3);				}				break;		}	}}void Crystal::AtomicZAtomicWt(AtomPtr theAtomPtr,double *atZ,double *atWt){	short 		atNum;	float		**atomicMassHndl;	float		atomicMass = 0,atomicNumAvg = 0;	atomicMassHndl = (float**)GetResource('info',100);		if(theAtomPtr->atomicNum >= 105 && theAtomPtr->atomicNum < 200){		UserDefAtom*	thisUDAtom;		double	percent;		short m,theAtom;		m = theAtomPtr->atomicNum - 105;		thisUDAtom = theUserAtoms[m];		for(theAtom = 0 ; theAtom <= thisUDAtom->lastAtom ; theAtom++){			percent = thisUDAtom->theElements[theAtom].percent / 100.;			atNum	= thisUDAtom->theElements[theAtom].atomicNum;			ReturnValance(&atNum);			if(atNum > 92)atomicMass += (*atomicMassHndl)[91] * percent;				else				atomicMass += (*atomicMassHndl)[atNum - 1] * percent;			atomicNumAvg += atNum * percent;		}	}else{		atNum = theAtomPtr->atomicNum;		ReturnValance(&atNum);		if(atNum > 92)atomicMass += (*atomicMassHndl)[91];		else			atomicMass += (*atomicMassHndl)[atNum - 1];		atomicNumAvg += atNum;	}			ReleaseResource((Handle)atomicMassHndl);	*atWt	= atomicMass;	*atZ    = atomicNumAvg;	return;}short DetermineJournal(CODENS **foundCODENS,short found){	PopUpMenu 	*logicMenus;	short 		i,item;	DialogPtr	aDialog;	 aDialog	= 	DM_GetNewDialog		(134, 0L,(WindowPtr)-1L);	logicMenus 	= (PopUpMenu*)D_new(PopUpMenu);	logicMenus	->	Init(aDialog,3,114,1);	logicMenus	->	SetPopUp();	for(i = 0; i < found;i++){		short 	k;		short	testLength = 75;		k = 0;		while(k < testLength - 1 && !((*foundCODENS)[i].journal[k] == ' ' && (*foundCODENS)[i].journal[k + 1] == ' '))k++;		strncpy(gTheText,(*foundCODENS)[i].journal,k);		gTheText[k] = 0;		logicMenus	->P_AppendMenu ((char*)c2pstr(gTheText));	}	while(1 != 2){		ModalDialog(TheFilterUPP,&item);		switch(item){			case ML_UPDATE_EVT:					BeginUpdate(aDialog);				UpdtDialog(aDialog,aDialog->visRgn);				if(!gAppleEvtsOK){					HiliteOK(aDialog);				}				EndUpdate(aDialog);				break;			case 1:				i	= logicMenus->lastResult - 1 ;				logicMenus->		DoClose();				DM_DisposDialog(&aDialog);				return i;								case CANCELCLICK:			case	2:				logicMenus->		DoClose();				DM_DisposDialog(&aDialog);				return 0;			default:			 	logicMenus->DoPopUp	()	;			break;		}	}}void Crystal::CorrectDWToTemp(double temperature)//Bloch{	short			i;	AtomPtr	theAtomsPtr;	D_HLock((Handle)theAtoms);	theAtomsPtr = *theAtoms;	for(i = 0; i <= atom_Count;i++,theAtomsPtr++)	{			CorrectAtomDWToTemp(theAtomsPtr, temperature);	}	D_HUnlock((Handle)theAtoms);}double Crystal::CorrectAtomDWToTemp(AtomPtr	theAtomsPtr,double temperature)//Bloch{	double	phi[7][10] = {1,.975,.951,.928,.904,.882,.860,.839,.818,.797,						.778,.758,.739,.721,.703,.686,.669,.653,.637,.622,						.607,.592,.578,.565,.552,.539,.526,.514,.503,.491,						.480,.47,.46,.45,.44,.431,.422,.413,.404,.396,						.388,.380,.373,.366,.359,.352,.345,.339,.333,.327,						.321,.315,.310,.304,.299,.294,.289,.285,.280,.276,						.271,.267,.263,.259,.255,.251,.248,.244,.241,.237};	double debeyeTheta[92] = {101.00,28.40,356.25,1188.00,1250.00,														2021.00,68.00,/*guesses->*/65.00,64.00,/*<-*/63.00,														163.00,316.00,397.20,581.50,/*guesses->*/90.00,														110.00/*<-*/,115.00,85.00,131.50,225.00,														/*guesses->*/280.00/*<-*/,386.00,356.50,445.00,380.00,														411.00,385.00,425.50,315.00,252.50,														182.50,305.50,254.50,135.00,110.00,														63.00,71.50,159.50,159.50/*<-*/,269.00,														276.50,374.00,374.00/*<-*/,413.00,342.50,														271.50,216.00,146.00,103.50,227.00,														170.00,120.00,106.00,55.00,48.00,														115.00,142.00,142.00/*<-*/,142.00/*<-*/,142.00/*<-*/,														142.00/*<-*/,142.00/*<-*/,142.00/*<-*/,152.00,152.00/*<-*/,														152.00/*<-*/,152.00/*<-*/,152.00/*<-*/,152.00/*<-*/,152.00/*<-*/,														152.00/*<-*/,213.00,237.50,327.00,292.50,														253.00,285.00,236.50,172.40,68.50,														98.00,78.06,91.00,/*guesses->*/110.00,123.00,														140.00,148.00,128.00,140.00,/*<-*/157.50,														157.80/*<-*/,200.00};//87double		dbTheta,ratio,atZ,atWt;long			shr,shy;				if(theAtomsPtr->atomicNum > 92)		{			dbTheta = debeyeTheta[91];		}else{			dbTheta = debeyeTheta[theAtomsPtr->atomicNum - 1];		}		if(dbTheta < .001)dbTheta = 1;		AtomicZAtomicWt(theAtomsPtr,&atZ,&atWt);				ratio = temperature / dbTheta;		shr = ratio;		if(shr > 6)shr = 6;				ratio -= shr;		shy 	= Round(ratio * 10);		theAtomsPtr->bi = 11492 * temperature * phi[shr][shy]/(atWt * dbTheta * dbTheta) + (2873/(atWt * dbTheta));	return dbTheta;}void Crystal::SpaceGrpName(char *theText){	short rID;	ResType rType,theResType;	Handle theResHandle;	switch(lattice_Type)	{		case TRICLINIC:			theResType = 'Tric';			break;		case MONOCLINIC:			theResType = 'Mono';			break;		case ORTHORHOMBIC:						theResType = 'Orth';			break;		case TETRAGONAL:						theResType = 'Tetr';			break;		case TRIGONAL:					theResType = 'Trig';			break;		case HEXAGONAL:					theResType = 'Hexa';			break;		case CUBIC:						theResType = 'Cubc';			break;		case TRIGHEX:						theResType = 'Trhx';			break;	}	theResHandle = GetResource(theResType,spaceGrpNm);	GetResInfo(theResHandle,&rID,&rType,(unsigned char*)theText);	p2cstr((unsigned char*)theText);	ReleaseResource(theResHandle);}