//Jim:Desktop.2.0:SearchObj2:SE2_SearchObject.cPopUpMenu#include	"Diffract_INCs.h"#include	"SearchObject.h"#include	"Dir_Paths.h"#include	"JCPDFCrystal.h"void SetSearchValues(DialogPtr theDialog,short theField,short *logicFlags,LogicObj logicFields,PopUpMenu* logicMenu);void GetSearchValues(DialogPtr theDialog,short field,short *logicFlags,LogicObj *logicFields);short	PosOffSetCompare(DataBaseLoc* spot1,DataBaseLoc* spot2);void WriteOutTitles(DialogPtr theDialog,unsigned char* lowTitle,unsigned char* highTitle,unsigned char* lowUnits7,unsigned char* lowUnits10,unsigned char* highUnits13,unsigned char* highUnits16);void WriteSearchValues(DialogPtr theDialog,short *logicFlags,LogicObj logicFields);void SearchObject::SearchJCPDFFiles(short dataBase){ 			long			i,marker = 3200;	long			k = 3200;	long			posOff = 0;	long			size = 200;	JCPDFCrystal	*jcpdfCrystal;	Handle			values[18];	DataBaseLoc		**theValueTotal;	long			numberFound[18];	long			noAllowed;	long			locFound = 0;	if(!OpenMasterIndex(0,&posOff,&size,1,dataBase))return;	jcpdfCrystal = (JCPDFCrystal*)D_new(JCPDFCrystal);	jcpdfCrystal->InitCrystal(0);		comparisonCrystal->CopyCrystal(jcpdfCrystal);	for(i = 0; i < 18;i++){		numberFound[i] 	= 	0;		values[i]		=	0L;	}	i = 0;	if(searchTypePopUp->lastResult > VOL_SEARCH_TWO){		values[i++] = (Handle)jcpdfCrystal->SearchSpaceGrpKey		(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchCODENKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchElementKey		(logicFlags[i],&numberFound[i],dataBase,-1,0L);		values[i++] = (Handle)jcpdfCrystal->SearchVolumeKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchDensityKey		(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchSubFileKey		(logicFlags[i],&numberFound[i],dataBase);//will need some modification		values[i++] = (Handle)jcpdfCrystal->SearchPearsonKey		(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchNameKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchPearsonKey		(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchUnitCellKey		(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchCDIDKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchOrganicClassKey	(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchYearKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchAuthorKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchCASKey			(logicFlags[i],&numberFound[i],dataBase);		values[i++] = (Handle)jcpdfCrystal->SearchElementKey		(logicFlags[i],&numberFound[i],dataBase,-1,0L);//simulates EDD search	}else{		i = 1;		logicFlags[i] = 1;//to simulate EDD we just do a chemistry search!!!		values[i] = (Handle)jcpdfCrystal->SearchElementKey		(logicFlags[i],&numberFound[i],dataBase,-1,0L);	}			jcpdfCrystal->DoClose();	theValueTotal	=	(DataBaseLoc**)D_NewHandle(100 * sizeof(DataBaseLoc));	D_HLock(theValueTotal);	noAllowed		=	100;	i = 0;	while( i < 16 && locFound == 0L){		DataBaseLoc	**theValueHd,*theVPtr;		if(values[i] == 0L || logicFlags[i] != 1){			i++;			continue;		}		theValueHd = (DataBaseLoc**)values[i];		theVPtr		=	*theValueHd;		D_HLock(theValueHd);		for(k = 0;k < numberFound[i];k++){			locFound++;			if(!CheckHandleSize((Handle)theValueTotal,&noAllowed,(short)locFound,sizeof(DataBaseLoc)))continue;			(*theValueTotal)[k] = theVPtr[k];		}		D_HUnlock(theValueHd);		i++;	}//Create a group of crystals from one AND set, this is the largest number of allowed crystals	D_HUnlock(theValueTotal);			i = 0;	while(i < 16){		if(logicFlags[i] == 1 && values[i]){			if(locFound == 0)goto EndIt;			theValueTotal = TestForUnion((DataBaseLoc**)values[i],theValueTotal,numberFound[i],&locFound);		}		i++;	}	i = 0;	while(i < 16){		if(values[i] == 0L){			i++;			 continue;		}		if(logicFlags[i] == 2){			DataBaseLoc	**theValueHd,*theVPtr;			theValueHd = (DataBaseLoc**)values[i];			theVPtr		=	*theValueHd;			D_HLock(theValueHd);			for(k = 0;k < numberFound[i];k++)			{				locFound++;				if(!CheckHandleSize((Handle)theValueTotal,&noAllowed,(short)locFound,sizeof(DataBaseLoc)))continue;				(*theValueTotal)[locFound] = theVPtr[k];			}			D_HUnlock(theValueHd);		}		i++;	}	i = 0;	while(i < 16){		if(logicFlags[i] == 3){			theValueTotal = TestForExclusion((DataBaseLoc**)values[i],theValueTotal,numberFound[i],&locFound);		}		i++;	}	if(locFound == 0) goto EndIt;	{		ListCrystal	*lastCrystal;		DataBaseLoc	*theVPtr;		D_HLock(theValueTotal);		theVPtr		=	*theValueTotal;		GetDItem			(theDialogSearch,TOTALMATCH,&gType,&gTheHandle,&gTheRect);		sprintf				(gTheText,"%d",locFound);		SetIText			(gTheHandle,c2pstr(gTheText));			for(k = 0;k < locFound;k++)		{						if(Quit()){D_HUnlock(theValueTotal);goto EndIt;}			totalFound++;			if(Quit())continue ;						GetSingleJCPDFSearchCrystal	(theVPtr[k].offSet,theVPtr[k].size,dataBase,0);			lastCrystal = foundCrystals->DoAdd(testCrystal,dataBase,theVPtr[k].offSet,theVPtr[k].size);			lastCrystal->FOM = TestCrystalFOM(dspacings);					}		D_HUnlock(theValueTotal);	}	GetDItem			(theDialogSearch,TOTALMATCH,&gType,&gTheHandle,&gTheRect);	sprintf				(gTheText,"%d",totalFound);	SetIText			(gTheHandle,c2pstr(gTheText));		EndIt:	KillHandle((Handle*)&theValueTotal);	for(i = 0; i < 18;i++){		if(values[i] == 0) continue;		KillHandle(&(values[i]));	}}short	PosOffSetCompare(DataBaseLoc* spot1,DataBaseLoc* spot2){	double z1,z2;		z1 = fabs((double)spot1->offSet);	z2 = fabs((double)spot2->offSet);		return((short)((z1 > z2) ? -1 : ((z1 == z2) ? 0 : 1)));}Boolean  SearchObject::OpenMasterIndex(long entryNumber,long *offSet,long *size,short crystAtTime,short dataBase){		long int  		flag[32],macFlag[32];	long			sizeOfBlock = 8;	long			posOff;	long int		*values,*oldValues;	long 			long1,long2;	static short 	refNum,vRefNum;	short			i,j;	OSErr			theErr;	char		aText[255];	switch(dataBase){		case 3:			sprintf(aText,"PDF2.S00");			if(OpenNamedFileEM(SEARCH_JCPDF_INDEX,aText,&vRefNum,&refNum))return false;		break;		case 4:			sprintf(aText,"CRYSTAL.S00");			if(OpenNamedFileEM(SEARCH_NBS_INDEX,aText,&vRefNum,&refNum))return false;		break;		default:		break;	}	SetConversionFlags2(flag,macFlag);	sizeOfBlock	=	2 * sizeof(long int) * crystAtTime;	oldValues = values = (long int*)D_NewPtr(sizeOfBlock);		posOff = (8 * (long)entryNumber);	SetFPos(refNum,fsFromStart,posOff);	theErr = FSRead(refNum,&sizeOfBlock,(Ptr)values);	theErr = FSClose(refNum);	long1 = 0;	long2 = 0;	for(i = 0; i <= 31;i++){		if(values[0] & flag[i]){			long1 += macFlag[i];		}	}	*offSet 	= 	long1;	for(j = 0; j < crystAtTime;j++,values++){		values++;		long2 = 0;		for(i = 0; i <= 31;i++){			if(*values & flag[i])			{				long2 += macFlag[i];			}					}		size[j] = long2; 	}	KillPtr(oldValues);	return true;}Boolean  SearchObject::GetJCPDFSearchCrystal	(long k,short crysAtATime,short dataBase,short print){	OSErr 				theErr;	static char*		theText = 0L;	char	 			*theText1;	short				refNum,vRefNum,jp,startV;	static long		*sizeOfBlock = 0L;	long				posOff;	long				i,oldJ;	static JCPDFCrystal	*jcpdfCrystal = 0L;	static Boolean		startFlag = true;	static long		j = 0,m = 0;	Boolean				flag;	char		aText[255];						if(crysAtATime <= 0){		if(jcpdfCrystal != 0L)jcpdfCrystal->DoClose();		if(theText != 0L) KillPtr(theText);		if(sizeOfBlock != 0L) KillPtr(sizeOfBlock);		 startFlag = true;		 return false;	}			if(startFlag)	{		jcpdfCrystal = (JCPDFCrystal*)D_new(JCPDFCrystal);		jcpdfCrystal->InitCrystal(0);			sizeOfBlock = (long*)D_NewPtr(sizeof(long) * crysAtATime);			}	if(m >= crysAtATime || startFlag){		long	totSize;		if(!startFlag){			 KillPtr(theText);		}		startFlag = false;				if(!OpenMasterIndex(k,&posOff,sizeOfBlock,crysAtATime,dataBase))return false;		switch(dataBase){			case 3:			sprintf(aText,"PDF2.DAT");			if(OpenNamedFileEM(SEARCH_JCPDF,aText,&vRefNum,&refNum))return false;			break;			case 4:			sprintf(aText,"CRYSTAL.DAT");			if(OpenNamedFileEM(SEARCH_NBS,aText,&vRefNum,&refNum))return false;			break;			default:			break;		}		m = 0;		totSize = 0L;		while(m < crysAtATime)totSize += sizeOfBlock[m++];		theText = (char*)D_NewPtr(sizeof(char) * (totSize + 10));		SetFPos		(refNum,fsFromStart,posOff);		theErr = 	FSRead(refNum,&totSize,(Ptr)(theText));				theErr = FSClose(refNum);		j = 0;		m = 0;		}		oldJ = j * 80;	flag = true;	jp = j;	startV = 40;	if(print){		while( ((jp + 1) * 80) - oldJ <= sizeOfBlock[m]){			MoveTo(20,startV);			sprintf(aText," ");			strncat(aText,&theText[jp * 80],80);			c2pstr(aText);			DrawDiffractString((unsigned char*)aText);			jp++;		}	}	while( ((j + 1) * 80) - oldJ <= sizeOfBlock[m]){		short lines;		i = 0;		theText1 = &theText[j * 80];		lines = 0;		if(!jcpdfCrystal->DecipherJCPDFLine(theText1,&lines)){			flag = false;		}		j += lines + 1;	}	m++;	if(flag == false)return false;		jcpdfCrystal->AtomsFromFormula(0);	if(jcpdfCrystal->atom_Count > 300){		jcpdfCrystal->atom_Count = 300;	}	jcpdfCrystal->CopyCrystal(testCrystal);	return flag;}Boolean  SearchObject::GetSingleJCPDFSearchCrystal	(long offSet,long fileSize,short dataBase,short display){	OSErr 			theErr;	char*			theText;	char	 		*theText1;	short			refNum,vRefNum;	long			i;	JCPDFCrystal	*jcpdfCrystal;	long			j = 0;	Boolean			flag;	char		aText[255];	short		jp;	short		startV = 30;					switch(dataBase){		case 3:		sprintf(aText,"PDF2.DAT");		if(OpenNamedFileEM(SEARCH_JCPDF,aText,&vRefNum,&refNum))return false;		break;		case 4:		sprintf(aText,"CRYSTAL.DAT");		if(OpenNamedFileEM(SEARCH_NBS,aText,&vRefNum,&refNum))return false;		break;		default:		break;	}	if(fileSize <= 0)return false;	theText = (char*)D_NewPtr(fileSize);	SetFPos		(refNum,fsFromStart,offSet);	theErr = 	FSRead(refNum,&fileSize,(Ptr)(theText));		theErr = FSClose(refNum);	if(theErr){		KillPtr(theText);		return false;	}	jcpdfCrystal = (JCPDFCrystal*)D_new(JCPDFCrystal);	jcpdfCrystal->InitCrystal(0);		jp = j = 0;	TextFont(geneva);	TextSize(9);	if(display == 1){		Rect theRect;		SetRect(&theRect,10,10,600,600);		EraseRect(&theRect);		while( ((jp + 1) * 80) <= fileSize){			MoveTo(20,startV);			sprintf(aText," ");			strncat(aText,&theText[jp * 80],80);			c2pstr(aText);			DrawDiffractString((unsigned char*)aText);			jp++;			startV += 12;		}	}	TextFont(0);	TextSize(0);	flag = true;	while( ((j + 1) * 80) <= fileSize){		short lines;		i = 0;		theText1 = &theText[j * 80];		lines = 0;		if(!jcpdfCrystal->DecipherJCPDFLine(theText1,&lines)){			flag = false;		}		j += lines + 1;		if(flag == false)continue;		jcpdfCrystal->AtomsFromFormula(0);		if(jcpdfCrystal->atom_Count > 300){			jcpdfCrystal->atom_Count = 300;		}else if(jcpdfCrystal->atom_Count < 0){			jcpdfCrystal->atom_Count = 0;		}	}	if(display == 2){		Rect theRect;		SetRect(&theRect,10,10,600,600);		EraseRect(&theRect);		jcpdfCrystal->DisplayCrystal(&theRect,20,20);	}	jcpdfCrystal->CopyCrystalWDspacing(testCrystal);	jcpdfCrystal->DoClose();	KillPtr		(theText);	return true;}/*void SearchObject::SetSearchLogic(void){	PopUpMenu 		*logicMenus[16];	short 				i,item;	DialogPtr			aDialog;	 	aDialog	= 	DM_GetNewDialog		(360, 0L,(WindowPtr)-1L);	for(i = 0; i < 16;i++)	{		logicMenus[i] = (PopUpMenu*)D_new(PopUpMenu);		logicMenus[i]->	Init(aDialog,i + 3,180,logicFlags[i] + 1);		logicMenus[i]->	SetPopUp();	}	while(1 != 2){		ModalDialog(TheFilterUPP,&item);		switch(item){			case 1:				for(i = 0; i < 16;i++) logicFlags[i] = logicMenus[i]->lastResult - 1;								case CANCELCLICK:			case	2:				for(i = 0; i < 16;i++)logicMenus[i]->		DoClose();				DM_DisposDialog(&aDialog);				return;			case 21:				comparisonCrystal->DoDefine();				for(i = 0; i < 16;i++)	logicMenus[i]->SetPopUp();								break;			case ML_UPDATE_EVT:				BeginUpdate(aDialog);			UpdtDialog(aDialog,aDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(aDialog);			}			EndUpdate(aDialog);			break;			 default:			 	if(item <= 18 && item >= 3)	logicMenus[item - 3]->DoPopUp	()	;								break;		}	}}*/void SearchObject::SetSearchLogic(void){	PopUpMenu 		*logicMenu;	PopUpMenu 		*fieldMenu;	short 				item,oldResult;	DialogPtr			aDialog;	 	aDialog	= 	DM_GetNewDialog		(402, 0L,(WindowPtr)-1L);		logicMenu = (PopUpMenu*)D_new(PopUpMenu);	logicMenu->	Init(aDialog,4,180,logicFlags[0]);	logicMenu->	SetPopUp();	fieldMenu = (PopUpMenu*)D_new(PopUpMenu);	fieldMenu->	Init(aDialog,3,55,1);	fieldMenu->	SetPopUp();	oldResult = 1;	SetSearchValues(aDialog,0,logicFlags,logicFields,logicMenu);	WriteSearchValues( theDialog,logicFlags, logicFields);	while(1 != 2){		ModalDialog(TheFilterUPP,&item);		switch(item){			case 1:				 logicFlags[oldResult - 1] = logicMenu->lastResult - 1;				 GetSearchValues(aDialog,oldResult - 1,logicFlags,&logicFields);			case CANCELCLICK:			case	2:				logicMenu->		DoClose();				fieldMenu->		DoClose();				DM_DisposDialog(&aDialog);				return;						case ML_UPDATE_EVT:				BeginUpdate(aDialog);			UpdtDialog(aDialog,aDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(aDialog);			}			EndUpdate(aDialog);			break;			case 4:				logicMenu->DoPopUp();				logicFlags[oldResult - 1] = logicMenu->lastResult - 1;				GetSearchValues(aDialog,oldResult - 1,logicFlags,&logicFields);				WriteSearchValues( theDialog,logicFlags, logicFields);			break;			case 3:				fieldMenu->DoPopUp();				GetSearchValues(aDialog,oldResult - 1,logicFlags,&logicFields);				if(oldResult != fieldMenu->lastResult){					GetSearchValues(aDialog,oldResult - 1,logicFlags,&logicFields);					SetSearchValues(aDialog,fieldMenu->lastResult - 1,logicFlags,logicFields,logicMenu);				}				oldResult = fieldMenu->lastResult;				WriteSearchValues( theDialog,logicFlags, logicFields);			break;			 default:			break;		}	}}void GetSearchValues(DialogPtr theDialog,short field,short *logicFlags,LogicObj *logicFields){	short j;	switch(field)	{		case 0:						logicFields->spaceGrpLow =  GetItemValue(theDialog,10);			if(logicFields->spaceGrpLow > 0 && logicFields->spaceGrpLow < 231){				logicFields->spaceGrpHigh =  GetItemValue(theDialog,13);			}else{				logicFlags[0] = 0;				logicFields->spaceGrpLow = 1;				logicFields->spaceGrpHigh = 230;			}		break;		case 1:		GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);		GetIText(gTheHandle,(unsigned char*)logicFields->CODEN);		case 3:		logicFields->volumeLow =  GetItemValue(theDialog,10);		logicFields->volumeHigh =  GetItemValue(theDialog,13);		break;		case 4:			logicFields->densityLow =  GetItemValue(theDialog,10);			logicFields->densityHigh =  GetItemValue(theDialog,13);		break;		case 5:			GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);			GetIText(gTheHandle,(unsigned char*)logicFields->subFile);		break;		case 6:			logicFields->crysSys =  GetItemValue(theDialog,10);		break;		case 7:			GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);			GetIText(gTheHandle,(unsigned char*)logicFields->mineralName);		break;		case 8:			GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);			GetIText(gTheHandle,(unsigned char*)logicFields->pearsonSym);		break;		case 9:			for(j = 0; j < 6;j++){				logicFields->crysLow[j] =  GetItemValue(theDialog,7 + j);				logicFields->crysHigh[j] =  GetItemValue(theDialog,13 + j);			}		break;		case 10:			logicFields->cdfID  =  GetItemValue(theDialog,10);;		break;		case 11:			GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);			GetIText(gTheHandle,(unsigned char*)logicFields->organic);		break;		case 12:			logicFields->yearLow =  GetItemValue(theDialog,10);			logicFields->yearHigh =  GetItemValue(theDialog,13);		break;		case 13:			GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);			GetIText(gTheHandle,(unsigned char*)logicFields->author);		break;		case 14:			logicFields->casNum =  GetItemValue(theDialog,10);		break;		default:		break;			}}void SetSearchValues(DialogPtr theDialog,short theField,short *logicFlags,LogicObj logicFields,PopUpMenu* logicMenu){	char  aText[30];	short j;	aText[0] = ' ';	logicMenu->lastResult = logicFlags[theField] + 1;	logicMenu->	SetPopUp();	for(j = 7;j <= 18;j++)HideDItem(theDialog,j);	ShowDItem(theDialog,10);	TextFont(geneva);	TextSize(10);	switch(theField){		case 0:		ShowDItem(theDialog,13);		WriteOutTitles(theDialog,"\pLowest Sp. Grp No = 1,Not JCPDF!!","\pHighest Sp. Grp No = 230",	"\p","\pInteger Value","\pInteger Value","\p");		SetItemValue(theDialog,10,logicFields.spaceGrpLow,0);		SetItemValue(theDialog,13,logicFields.spaceGrpHigh,0);		break;		case 1:		WriteOutTitles(theDialog,"\pCODEN SEARCH - Journal Title","\p",	"\p","\p7 Alphanumeric","\p","\p");		GetDItem(theDialog,10,&gType,&gTheHandle,&gTheRect);		GetIText(gTheHandle,(unsigned char*)logicFields.CODEN);		break;		case 2:			HideDItem(theDialog,10);			WriteOutTitles(theDialog,"\pUse EDD Search To Set Chemistry","\p","\p","\p","\p","\p");	break;		case 3:		WriteOutTitles(theDialog,"\pVolume Search","\pMust Be > 0",	"\p","\pLow Value �3","\pHigh Value �3","\p");		ShowDItem(theDialog,13);		SetItemValue(theDialog,10,logicFields.volumeLow,2);		SetItemValue(theDialog,13,logicFields.volumeHigh,2);		break;		case 4:			WriteOutTitles(theDialog,"\pDenisty Search","\pMust Be > 0","\p","\pLow Value gm/cm3","\pHigh Value gm/cm3","\p");			ShowDItem(theDialog,13);			SetItemValue(theDialog,10,logicFields.densityLow,2);			SetItemValue(theDialog,13,logicFields.densityHigh,2);		break;		case 5:			WriteOutTitles(theDialog,"\pSubfield Search","\p",	"\p","\p5 Alphanumeric","\p","\p");			SetItemText(theDialog,10,(unsigned char*)logicFields.subFile);		break;		case 6:			WriteOutTitles(theDialog,"\pCrystal System","\p1-7, Tric, M,O,Tet,Trig,Hex,C",	"\p","\pNum. 1-7","\p","\p");			SetItemValue(theDialog,10,(float)logicFields.crysSys,0);		break;		case 7:			WriteOutTitles(theDialog,"\pMineral Name","\p",	"\p","\p40 Alphanumeric","\p","\p");			SetItemText(theDialog,10,(unsigned char*)logicFields.mineralName);		break;		case 8:			WriteOutTitles(theDialog,"\pPearson Symbol","\poI23  Not JCPDF!!",	"\p","\p","\p","\p");			SetItemText(theDialog,10,(unsigned char*)logicFields.pearsonSym);		break;		case 9:			WriteOutTitles(theDialog,"\pUnit Cell Search","\pa,b,c,alp,bet,gam",	"\pLow a,b,c �","\pLow angles, degrees","\pHigh a,b,c","\pHigh angles,degrees");			for(j = 7;j < 19;j++)ShowDItem(theDialog,j);			for(j = 0; j < 6;j++){				SetItemValue(theDialog,7 + j,logicFields.crysLow[j],2);				SetItemValue(theDialog,13 + j,logicFields.crysHigh[j],2);			}		break;		case 10:			WriteOutTitles(theDialog,"\pStandard CDF-ID","\p Not JCPDF!!",	"\p","\pCDF-ID","\p","\p");			SetItemValue(theDialog,10,(float)logicFields.cdfID,0);		break;		case 11:			WriteOutTitles(theDialog,"\pOrganic System","\pMust Be Name Not JCPDF!!",	"\p","\p10 Alpha Values","\p","\p");			SetItemText(theDialog,10,(unsigned char*)logicFields.organic);		break;		case 12:			WriteOutTitles(theDialog,"\pSearch Year Reported","\pMust Be > 1900",	"\p","\pEarliest Year","\pLatest Year","\p");			ShowDItem(theDialog,13);			SetItemValue(theDialog,10,(float)logicFields.yearLow,0);			SetItemValue(theDialog,13,(float)logicFields.yearHigh,0);		break;		case 13:			WriteOutTitles(theDialog,"\pAuthor Name","\p",	"\p","\pAlpha (short better)","\p","\p");			SetItemText(theDialog,10,(unsigned char*)logicFields.author);		break;		case 14:			WriteOutTitles(theDialog,"\pCAS Number Search","\pMust Be > 0 Not NBS!!","\p","\pInteger","\p","\p");			SetItemValue(theDialog,10,logicFields.casNum,0);		break;		case 15:			HideDItem(theDialog,10);			WriteOutTitles(theDialog,"\pUse EDD Search To Setup","\p","\p","\p","\p","\p");	break;		default:		break;			}	TextFont(0);	TextSize(12);}void WriteOutTitles(DialogPtr theDialog,unsigned char* lowTitle,unsigned char* highTitle,unsigned char* lowUnits7,unsigned char* lowUnits10,unsigned char* highUnits13,unsigned char* highUnits16){	if(lowTitle[0] != 0){		ShowDItem(theDialog,5);		SetItemText(theDialog,5,lowTitle);	}else{		HideDItem(theDialog,5);	}	if(highTitle[0] != 0){		ShowDItem(theDialog,6);		SetItemText(theDialog,6,highTitle);	}else{		HideDItem(theDialog,6);	}		if(lowUnits7[0] != 0){		ShowDItem(theDialog,22);		SetItemText(theDialog,22,lowUnits7);	}else{		HideDItem(theDialog,22);	}	if(lowUnits10[0] != 0){		ShowDItem(theDialog,23);		SetItemText(theDialog,23,lowUnits10);	}else{		HideDItem(theDialog,23);	}			if(highUnits13[0] != 0){		ShowDItem(theDialog,24);		SetItemText(theDialog,24,highUnits13);	}else{		HideDItem(theDialog,24);	}	if(highUnits16[0] != 0){		ShowDItem(theDialog,25);		SetItemText(theDialog,25,(unsigned char*)highUnits16);	}else{		HideDItem(theDialog,25);	}}void WriteSearchValues(DialogPtr theDialog,short *logicFlags,LogicObj logicFields){	char  theText[600],aText[120];	Rect	theRect;	short i,n = 233;	theDialog = theDialog;	TextFont(geneva);	TextSize(10);	sprintf(theText," ");	SetRect(&theRect,24,218,401,378);	EraseRect(&theRect);	FrameRect(&theRect);	for(i = 0; i < 16;i++){		switch(logicFlags[i]){			case 0:				continue;			break;			case 1:			sprintf(aText,"AND:  ");			break;			case 2:			sprintf(aText,"OR: ");			break;			case 3:			sprintf(aText,"NOT: ");			break;					}		strcat(theText,aText);						switch(i){			case 0:				sprintf(aText,"SPACE GROUP: LOW %d,  HIGH: %d ",logicFields.spaceGrpLow,logicFields.spaceGrpHigh);				strcat(theText,aText);			break;			case 2:				sprintf(aText,"CHEMISTRY: SET IN PREVIOUS DIALOG BOX ");				strcat(theText,aText);			break;			case 1:				sprintf(aText,"CODEN: ");				strcat(theText,aText);				p2cstr((unsigned char*)logicFields.CODEN);				strcat(theText,logicFields.CODEN);				c2pstr(logicFields.CODEN);				break;			case 3:			sprintf(aText,"VOLUME: LOW %6.2f �3,  HIGH %6.2f �3 ",logicFields.volumeLow,logicFields.volumeHigh);			strcat(theText,aText);			break;			case 4:			sprintf(aText,"DENSITY: LOW %6.2f gm/cm3, HIGH %6.2f gm/cm3 ",logicFields.densityLow,logicFields.densityHigh);			strcat(theText,aText);			break;			case 5:				sprintf(aText,"SubFile: ");				strcat(theText,aText);				p2cstr((unsigned char*)logicFields.subFile);				strcat(theText,logicFields.subFile);				c2pstr(logicFields.subFile);			break;			case 6:				switch(logicFields.crysSys)				{					case 1:						sprintf(aText,"Crystal: Triclinic");					break;					case 2:						sprintf(aText,"Crystal: Monoclinic");					break;					case 3:						sprintf(aText,"Crystal: Orthorhombic");					break;					case 4:						sprintf(aText,"Crystal: Tetragonal");					break;					case 5:						sprintf(aText,"Crystal: Trigonal");					break;					case 6:						sprintf(aText,"Crystal: Hexagonal");					break;					case 7:						sprintf(aText,"Crystal: Cubic");					break;				}				strcat(theText,aText);			break;			case 7:				sprintf(aText,"MINERAL NAME: ");				strcat(theText,aText);				p2cstr((unsigned char*)logicFields.mineralName);				strcat(theText,logicFields.mineralName);				c2pstr(logicFields.mineralName);			break;			case 8:				sprintf(aText,"PEARSON SYMBOL: ");				strcat(theText,aText);				p2cstr((unsigned char*)logicFields.pearsonSym);				strcat(theText,logicFields.pearsonSym);				c2pstr(logicFields.pearsonSym);			break;			case 9:				if(n > 401)break;				sprintf(aText,"UNIT Cell LOW: a = %5.2f,b = %5.2f,c = %5.2f,",logicFields.crysLow[0],				logicFields.crysLow[1],logicFields.crysLow[2]);				strcat(theText,aText);				MoveTo(24,n); 				c2pstr(theText);				DrawDiffractString((unsigned char*)theText);				n += 15;				if(n > 401)break;				sprintf(theText," alpha = %5.2f,beta = %5.2f,gamma = %5.2f",logicFields.crysLow[3],logicFields.crysLow[4],				logicFields.crysLow[5]);				MoveTo(110,n); 				c2pstr(theText);				DrawDiffractString((unsigned char*)theText);				n += 15;				if(n > 401)break;				 				 				sprintf(theText,"High: a = %5.2f,b = %5.2f,c = %5.2f",logicFields.crysHigh[0],				logicFields.crysHigh[1],logicFields.crysHigh[2]);								MoveTo(80,n); 				c2pstr(theText);				DrawDiffractString((unsigned char*)theText);				n += 15;				if(n > 401)break;				 				 				sprintf(theText,"alpha = %5.2f,beta = %5.2f,gamma = %5.2f",logicFields.crysHigh[3],logicFields.crysHigh[4],				logicFields.crysHigh[5]);				MoveTo(110,n); 				c2pstr(theText);				DrawDiffractString((unsigned char*)theText);				n += 15;												sprintf(theText," ");				break;			case 10:				sprintf(aText,"CDF-ID: %d ",logicFields.cdfID);				strcat(theText,aText);			break;			case 11:				sprintf(aText,"ORGANIC SYMBOL: ");				strcat(theText,aText);				p2cstr((unsigned char*)logicFields.organic);				strcat(theText,logicFields.organic);				c2pstr(logicFields.organic);			break;			case 12:				sprintf(aText,"YEAR: EARLIEST %d,LATEST %, ",logicFields.yearLow,logicFields.yearLow);				strcat(theText,aText);			break;			case 13:				sprintf(aText,"AUTHOR: ");				strcat(theText,aText);				p2cstr((unsigned char*)logicFields.author);				strcat(theText,logicFields.author);				c2pstr(logicFields.author);			break;			case 14:				sprintf(aText,"CAS NUM: %d ",logicFields.casNum);				strcat(theText,aText);			break;			case 15:				sprintf(aText,"EDD SEARCH:  See Previous Dialog Box ");				strcat(theText,aText);			break;			default:			break;					}		if(i == 9 || n > 401)continue;		MoveTo(24,n); 		c2pstr(theText);		DrawDiffractString((unsigned char*)theText);		sprintf(theText," ");		n += 15;	}	TextFont(0);	TextSize(12);}void	SearchObject::PrintFoundCrystals(void){	TPPrPort 	thePrintPort;	GrafPtr 	oldPort;	short 		theTop,theLeft,i,oldLeft;	TPrStatus 	theStatus;	Rect 		theRect;	short		secnColLeft;	short 		firstTop;		short 		pageLeft = 20;	short 		pageTop = 20;	short 		charWidth12 = 10;	short 		charHeight12 = 14;	short 		pageNo = 1;	Rect		frameRect;	FontInfo 	theFont;	GetPort		(&oldPort);	PrOpen();	//gPrintRecHandle = (THPrint)GetResource('PREC',(short)1000);	if(!gPrintRecHandle) gPrintRecHandle = (THPrint)GetResource('PREC',(short)1000);		if(GetHandleSize((Handle)gPrintRecHandle) != sizeof(TPrint)){		SetHandleSize((Handle)gPrintRecHandle,sizeof(TPrint));		ChangedResource((Handle)gPrintRecHandle);		WriteResource((Handle)gPrintRecHandle);	}	if(!PrJobDialog(gPrintRecHandle)){		SetPort(oldPort);		//ReleaseResource((Handle)gPrintRecHandle);		PrClose();		return;	}			thePrintPort = PrOpenDoc(gPrintRecHandle,0L,0L);	theRect = (*gPrintRecHandle)->prInfo.rPage;	PrOpenPage(thePrintPort,0L);	SetPort((GrafPtr)thePrintPort);		TextFont(geneva);	TextSize(9);	GetFontInfo(&theFont);	charHeight12 = theFont.ascent + theFont.descent + theFont.leading;	charWidth12 = theFont.widMax;	firstTop		=	pageTop;	theTop = pageTop + (2 * charHeight12) + 10;	gCurrentObj->printing = true;		theTop -= 2 * charHeight12 + 10;	oldLeft = theLeft = pageLeft;	theTop = pageTop;	secnColLeft = 0;			for(i = 2;i <= theCRYSMenu->P_CountMItems();i++){		long place1,place2;		DoNewCrystalList	(i,&place1,&place2,0);		theTop += 200;		if(pageNo == 1){			TestPosition( &thePrintPort, theRect, &theTop,&theLeft, firstTop, pageLeft,secnColLeft,&pageNo);			if(pageNo > 1){				theTop = pageTop;			}		}else			TestPosition( &thePrintPort, theRect, &theTop,&theLeft, pageTop, pageLeft,secnColLeft,&pageNo);		if(theTop > 200)theTop -= 200;		TextFont(geneva);		TextSize(9);		testCrystal->		TableDisplayCrystal	(&theLeft,&theTop);		theTop		-= 		45;		theLeft 	= 		oldLeft;		SetRect(&frameRect,theLeft,theTop,theLeft + 400,theTop + 100);		PlotComparativeLines(frameRect);		theTop 		= 		frameRect.bottom;		SetRect				(&frameRect,oldLeft,theTop,oldLeft + 500,theTop + 150);		testCrystal	->		ListDspacings(&frameRect,9,geneva);		dm_FrameRect			(&frameRect);		theTop 		+= 		160;		SetRect				(&frameRect,oldLeft,theTop,oldLeft + 500,theTop + 150);		testCrystal	->		ListComments(&frameRect,9,geneva);		theTop		+= 4;	}			PrClosePage(thePrintPort);	PrCloseDoc(thePrintPort);	if((*gPrintRecHandle)->prJob.bJDocLoop == bSpoolLoop){		PrPicFile(gPrintRecHandle,0L,0L,0L,&theStatus);	}	//ReleaseResource((Handle)gPrintRecHandle);	SetPort(oldPort);	gCurrentObj->printing = false;	TextFont(0);	TextSize(0);	PrClose();	return;}DataBaseLoc** TestForUnion(DataBaseLoc** values,DataBaseLoc** values2,long total1,long *total2){	long 		i,k;	Boolean 	flag;	long		total;	DataBaseLoc *valuesPtr2,*valuesPtr;	DataBaseLoc** theHandle;	if((long)values == 0L ||total1 == 0 || *total2 == 0 || (long)values2 == 0L){		return values2;	}	if(values == values2) return values2;	total = *total2;	D_HLock((Handle)values);	D_HLock((Handle)values2);	valuesPtr2 = *values2;	for(i = 0; i < *total2;i++,valuesPtr2++){		flag = false;		valuesPtr = *values;		for(k = 0;flag == false && k < total1;k++,valuesPtr++){			if(valuesPtr->offSet == valuesPtr2->offSet)flag = true;		}		if(flag == false){		 	valuesPtr2->offSet = -1;			total--;		}	}	if(total <= 0){		*total2 = 0;		D_HUnlock((Handle)values);		D_HUnlock((Handle)values2);		return values2;	}	theHandle = (DataBaseLoc**)D_NewHandle(total * sizeof(DataBaseLoc));	k = 0;	valuesPtr2 = *values2;	for(i = 0; i < *total2;i++,valuesPtr2++){		if(valuesPtr2->offSet != -1){			if(k < *total2){				(*theHandle)[k] = *valuesPtr2;				k++;			}		}	}	*total2 = total;	D_HUnlock((Handle)values);	D_HUnlock((Handle)values2);	KillHandle((Handle*)&values2);	return theHandle;}DataBaseLoc** TestForExclusion(DataBaseLoc** values,DataBaseLoc** values2,long total1,long *total2){	long	 i,k;	long	total;	Boolean flag;	DataBaseLoc** theHandle;	if(values == 0L || total1 == 0 || *total2 == 0)return values2;	total = *total2;	for(i = 0; i < *total2;i++){		flag = true;		for(k = 0;k < total1;k++){			if((*values)[k].offSet == (*values2)[i].offSet)flag = false;		}		if(flag == false){		 (*values2)[i].offSet = -1;			total--;		}	}	if(total <= 0){		*total2 = 0;		return values2;	}	theHandle = (DataBaseLoc**)D_NewHandle(total * sizeof(DataBaseLoc));	k = 0;	for(i = 0; i < *total2;i++){		if((*values2)[i].offSet != -1){			(*theHandle)[k] = (*values2)[i];			k++;		}	}	*total2 = k;	KillHandle((Handle*)&values2);	return theHandle;}void SearchObject::JCPDFFullDisplay(short lastCrystal){	DialogPtr aDialog;	long 	place1,place2;	short	item,i;	ListCrystal *hitCrystal;	short theNumber = 0;	hitCrystal = foundCrystals->GetCrystal(lastCrystal - 1,&theNumber);	if(hitCrystal->dataBase < 3)return;	aDialog	= 	DM_GetNewDialog		(403, 0L,(WindowPtr)-1L);	DoNewCrystalList	(lastCrystal,&place1,&place2,2);	for(i = 1; i <= 3;i++){ HideDItem(aDialog,i); ShowDItem(aDialog,i);}	BeginUpdate(aDialog);			UpdtDialog(aDialog,aDialog->visRgn);			if(!gAppleEvtsOK){				HiliteOK(aDialog);		}	EndUpdate(aDialog);	while(1 != 0)	{		ModalDialog(TheFilterUPP,&item);		switch(item){			case 1:			case CANCELCLICK: 				DM_DisposDialog(&(aDialog));			return;			case 2:				DoNewCrystalList	(lastCrystal,&place1,&place2,1);				for(i = 1; i <= 3;i++){ HideDItem(aDialog,i) ;ShowDItem(aDialog,i);}			BeginUpdate(aDialog);				UpdtDialog(aDialog,aDialog->visRgn);				if(!gAppleEvtsOK){					HiliteOK(aDialog);				}			EndUpdate(aDialog);			break;			case 3:				DoNewCrystalList	(lastCrystal,&place1,&place2,2);				for(i = 1; i <= 3;i++){ HideDItem(aDialog,i) ;ShowDItem(aDialog,i);}			BeginUpdate(aDialog);					UpdtDialog(aDialog,aDialog->visRgn);					if(!gAppleEvtsOK){						HiliteOK(aDialog);				}			EndUpdate(aDialog);			default:			break;		}	}}