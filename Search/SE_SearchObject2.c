#include	"Diffract_INCs.h"//Jim:Search:SE_SearchObject2.c#include	"Dir_Paths.h"void 			TranslateRadix50		(short lengthOfASCII,unsigned short *theRadixLetter,char *theAsciiLetter);Boolean 		QuickReject				(short *theElements,unsigned short *theCompareMap,short *lastFound);void 			ConvertCODENToJournal	(char **cODEN);long			START_SEARCH_EDD;short			BLOCKS_EDD;Boolean SearchObject::GetJPDSSearchCrystal(long blockNum,long crystalNum){	short 			i,j,theElement,k;	float 		  	theDspacings[MAX_STORE_DSPACE];	EDDSearchFile 	*theSearchFile,*theSearch;	OSErr 			errCode;	long 			posOff;	short 			vRefNum;	short 			refNum,theFile,theBlock;	long 			sizeOfEDD;	unsigned short	theEdge[3];	unsigned short 	flag[16],macFlag[16];	unsigned short 	lowByte = 0xFF00;	unsigned short  highByte = 0x00FF;			sizeOfEDD 		= 	18 * sizeof(EDDSearchFile);	testCrystal->	noDifrntElem = 0;		SetConversionFlags(flag,macFlag);		if(OpenJCPDFile(&vRefNum,&refNum))	{				theSearchFile 	= (EDDSearchFile*)D_NewPtr(sizeOfEDD);				if(theSearchFile == 0L) 			return false;		posOff 		= 	(1024L * (long)blockNum) + (START_SEARCH_EDD);		SetFPos			(refNum,fsFromStart,posOff);		errCode 	= 	FSRead(refNum,&sizeOfEDD,(Ptr)theSearchFile);		theSearch 	= 	&theSearchFile[crystalNum];		ReturnSearchFileDspace(theSearch->theRMap,theDspacings);				for(i = 0; i < MAX_STORE_DSPACE; i++){			 testCrystal->dspacings[i].x 			= theDspacings[i];			 testCrystal->dspacings[i].y			=	0;			 testCrystal->dspacings[i].z			=	0;			 testCrystal->dspacings[i].theCrystal 	= testCrystal;			 testCrystal->dspacings[i].intensity	=	0.0;			 testCrystal->dspacings[i].h			=	1;			 testCrystal->dspacings[i].k			=	0;			 testCrystal->dspacings[i].l			=	0;			 testCrystal->dspacings[i].angle		=	0;			 testCrystal->dspacings[i].flags		=	0;			 		}		for(i = 0; i <= 5; i++){			for(j = 0; j <= 15;j++){				if(flag[j] & theSearch->theChemMap[i]){					testCrystal->noDifrntElem++;					theElement = i * 16 + j;					if(theElement >= 86){						testCrystal->elementList[testCrystal->noDifrntElem - 1] = theElement - 86;					}else testCrystal->elementList[testCrystal->noDifrntElem - 1] = theElement + 10;									}			}		}		KillPtr((Ptr)theSearchFile);		if(errCode){		 	return false;		}				FSClose(refNum);		refNum = 0;		for(k = 0; k <= 2; k++){			theEdge[k] = ConvertToMacBits(theSearch->unitCellEdge[k]);		}		testCrystal->theUnitCell.a = (double)theEdge[0] / 100;		testCrystal->theUnitCell.b = (double)theEdge[1] / 100;		testCrystal->theUnitCell.c = (double)theEdge[2] / 100;			for(k = 0; k <= 2; k++){			theEdge[k] = ConvertToMacBits(theSearch->reducedUCEdge[k]);		}		testCrystal->theReducedCell.a = (double)theEdge[0] / 100;		testCrystal->theReducedCell.b = (double)theEdge[1] / 100;		testCrystal->theReducedCell.c = (double)theEdge[2] / 100;				theEdge[0] = ConvertToMacBits(theSearch->spaceGroup);				theBlock = (short)ConvertToMacBits((unsigned short)theSearch->refFilePtr.filePosition);		testCrystal->spaceGrpNm = theEdge[0] & highByte;		theEdge[0] = ConvertToMacBits(theSearch->flags);		theFile = theEdge[0] / 256;						GetReferenceFileInfo(theFile,theBlock); 				if(testCrystal->spaceGrpNm < 3){			testCrystal->lattice_Type 	= TRICLINIC;					}else if(testCrystal->spaceGrpNm < 16){			testCrystal->lattice_Type 	= MONOCLINIC;					}else if(testCrystal->spaceGrpNm < 75){			testCrystal->lattice_Type 	= ORTHORHOMBIC;					}else if(testCrystal->spaceGrpNm < 143){/*dec 1992*/			testCrystal->lattice_Type 	= TETRAGONAL;					}else if(testCrystal->spaceGrpNm < 168){/*dec 1992*/			testCrystal->lattice_Type 	= TRIGONAL;			testCrystal->theUnitCell.gamma = 120.;		}else if(testCrystal->spaceGrpNm < 195){/*dec 1992*/			testCrystal->lattice_Type 	= HEXAGONAL;			testCrystal->theUnitCell.gamma = 120.;		}		else if(testCrystal->spaceGrpNm < 231){			testCrystal->lattice_Type 	= CUBIC;					}else if(testCrystal->spaceGrpNm < 1168)			testCrystal->lattice_Type 	= TRIGHEX;					testCrystal->volumeReduced = (float)ConvertToMacBits(theSearch->reducedVolume);		testCrystal->ConvertCODENToJournal();		testCrystal->			CellVolume();				return true;	} 		return false;}void SearchObject::FindJCPDS(void){	short vRefNum,refNum;	unsigned short theCSReq[6],theCSPos[6],theCSX[6];	OSErr fileError;	char volName[255];		if(OpenJCPDFile(&vRefNum,&refNum))	{		SetChemScreen		(theCSReq,theCSPos,theCSX,false);		ReadIndexFile		(refNum,theCSReq,theCSX); 		fileError 		= 	FSClose(refNum);		volName[0] 		= 	(char)0;		FlushVol			("\p",vRefNum);		}			}void SearchObject::SetRMap(unsigned  short *theRMap){	short 				i;	double 				value,fNumber;	short 				expon;	unsigned short 		flag[16],macFlag[16],theNum,shortNum,holdNum,tempNum;		SetConversionFlags		(flag,macFlag);	for(i = 0 ; i <= 10;i++)			theRMap[i] = 0;				for(i = 0; i <= noOfDspace;i++){		if(dspacings[i] <= .02) continue;		value = (2.5) / dspacings[i];		if(value <= 3.2){			fNumber = (value) / .018;			theNum = fNumber;			if(fNumber - (double)theNum > .5) theNum++;			shortNum = (double)theNum / (double)16;			holdNum = theRMap[shortNum];			expon = (long)(theNum - (shortNum * 16));			theRMap[shortNum] = holdNum | (unsigned short)flag[expon];			holdNum = theRMap[shortNum];			if(expon < 15)				theRMap[shortNum] = holdNum | (unsigned short)flag[expon + 1];			else{				tempNum = theRMap[shortNum + 1];				theRMap[shortNum + 1] = tempNum | (unsigned short)flag[0];			}			holdNum = theRMap[shortNum];			if(expon > 0)				theRMap[shortNum] = holdNum | (unsigned short)flag[expon - 1];			else{				tempNum = theRMap[shortNum - 1];				theRMap[shortNum - 1] = tempNum | (unsigned short)flag[15];			}		}	}}void SearchObject::SetChemScreen(unsigned  short *theCSReq,unsigned  short *theCSPos,unsigned  short *theCSX,Boolean macCompatible){	short i,k,l;	unsigned short flag[16],macFlag[16];	unsigned short theNum;	if(!macCompatible)SetConversionFlags(flag,macFlag);	else	SetConversionFlags(macFlag,flag);		for(k = 0; k <= 5; k++) theCSReq[k] = 0;	for(i = 10; i <= 95;i++){		if(theElements[1][i]){			k = (i - 10)/ 16;			l = (i - 10) - (k * 16);			theNum = flag[l];			theCSReq[k] += theNum;		}	}	for(i = 86; i <= 95;i++){		if(theElements[1][i - 86]){			k = (i) / 16;			l = i - (k * 16);			theNum = flag[l];			theCSReq[k] += theNum;		}	}	for(k = 0; k <= 5; k++) theCSPos[k] = 0;	for(i = 10; i <= 95;i++){		if(theElements[0][i]){			k = (i - 10)/ 16;			l = (i - 10) - (k * 16);			theNum = flag[l];			theCSPos[k] += theNum;		}	}	for(i = 86; i <= 95;i++){		if(theElements[0][i - 86]){			k = (i) / 16;			l = i - (k * 16);			theNum = flag[l];			theCSPos[k] += theNum;		}	}		for(i = 0; i <= 5; i++){		theCSX[i] = 0;		theCSX[i] = (unsigned short)~(theCSPos[i] | theCSReq[i]);	}}void SearchObject::ReadIndexFile(short refNum,unsigned short *theCSReq,unsigned short *theCSX){	EDDIndex *theEDDSetIndex;		OSErr 			errCode;	short 			i,j,k;	short 			flag2;	short 			noReq;	short 			maxElements;	 short 			maxNum;	long 			sizeOfEDD,sizeOfBlock = 512L,posOff,blocks;	unsigned short theRMap[11],flag[16],macFlag[16];	short 			l,theElements[95];		errCode 	= 	noErr;	noReq 		= 	0;	maxElements = 	-1;		SetConversionFlags(flag,macFlag);	for(i = 0; i <= 5; i++){		for(k = 0 ; k <= 15;k++){			flag2 = flag[k];			maxNum = theCSReq[i] & flag2;			if(maxNum == flag2) noReq++;			maxNum = theCSX[i] & flag2;			if(!maxNum)theElements[++maxElements] =  i * 16 + k;		}	}		j = 0;	sizeOfEDD = 42 * sizeof(EDDIndex);	theEDDSetIndex = (EDDIndex*)D_NewPtr(sizeOfEDD);	if(!theEDDSetIndex) 		return;			SetRMap(theRMap); 		posOff = 0;	blocks = BLOCKS_EDD;	//else blocks = 94;		l = 0;	for(i = 0;i <= blocks;i++){		posOff = (sizeOfBlock * (long)i);		SetFPos(refNum,fsFromStart,posOff);		errCode = FSRead(refNum,&sizeOfEDD,(Ptr)theEDDSetIndex);				if(errCode){			KillPtr((Ptr)theEDDSetIndex);			return;		}				 for(k = 0; k <= 41; k++){		 	if( Quit()) goto ENDSEARCH;/*dec 1992*/		 	if(QuickReject(theElements,theEDDSetIndex[k].theChemMap,&l)){		 	 	j++;		 	 	if(l > maxElements) goto ENDSEARCH;		 	 			 	}else{ 				if(CompareMap(theEDDSetIndex[k].theChemMap,theCSReq,5) == noReq){				   MatchSearchFile(refNum,j,theCSReq,theCSX,theRMap,noReq); 				} 				j++;			} 		}	}	ENDSEARCH:		KillPtr((Ptr)theEDDSetIndex);	return;}Boolean QuickReject(short *theElements,unsigned short *theCompareMap,short *lastFound){	short 			k,m;	unsigned short 	flag[16],macFlag[16];	short			elementNo;	static Boolean 	theMark = false;			SetConversionFlags(flag,macFlag);	k = 0;	m = 0;	while(!(theCompareMap[k] & flag[m]) && k <= 5)	{		m++;		if(m == 16){			m = 0;			k++;		}	}	elementNo = k * 16 + m;	if(elementNo < (theElements[*lastFound] - 1)) return true;	if(elementNo > (theElements[*lastFound] + 1)){		(*lastFound)++;		 return true;	}	return false;}void SearchObject::DrawSearchBitMap(short refNum,short offSet){	Boolean 		volFound = false;	char 			volName[255];	char 			*theText;	short 			j,k,l;	short 			theTop,theLeft;	short 			vRefNum;	static short 	lastPage,fileRefNum;	unsigned short 	theNum,compare[16],macFlag[16];	long 			sizeOfEDD,sizeOfBlock,posOff;	short			fileError;	OSErr 			errCode;	GrafPtr 		oldPort;		EDDIndex 		*theEDDSetIndex,*eddSetIndex;		lastPage = refNum;	GetPort(&oldPort);	sprintf(volName,"CHEM11.EXT;1");	fileError = OpenNamedFile(SEARCH_EDD,volName,&vRefNum,&fileRefNum);	if(fileError){		sprintf(volName,"CHEM11.EXT");		fileError = OpenNamedFile(SEARCH_EDD,volName,&vRefNum,&fileRefNum);		if(fileError){			FileErrorMessages_DirPath( fileError);			return ;		}	}	SetConversionFlags(compare,macFlag);	errCode = noErr;		TextFont(geneva);	TextSize(9);	sizeOfEDD = 42 * sizeof(EDDIndex);	theEDDSetIndex = (EDDIndex*)D_NewPtr(sizeOfEDD);	if(theEDDSetIndex == NULL){		SetPort(oldPort);		return;	}	sizeOfBlock = 512;	posOff = 0L;	errCode = 0;	theTop = 70;		posOff = (512L * (long)lastPage) + offSet; 		SetFPos(fileRefNum,fsFromStart,posOff);		errCode = FSRead(fileRefNum,&sizeOfEDD,(Ptr)theEDDSetIndex);				if(errCode){			SetPort(oldPort);			KillPtr((Ptr)theEDDSetIndex);			FSClose(refNum);			volName[0] = (char)0;			FlushVol("\p",vRefNum);			return;		}		theLeft = 5;				for(k = 0; k <= 41;k++){			theText = gTheText;			eddSetIndex = &theEDDSetIndex[k];			for(j = 0; j <= 5;j++){				theNum =   eddSetIndex->theChemMap[j];								for(l = 0; l <= 15;l++){					if(theNum & compare[l]){						*theText = '1';						theText++;					}else{						*theText = '0';						theText++;					}				}				*theText = ' ';				theText++;			}			*theText = NUL;			theTop += 15;			MoveTo(theLeft,theTop);			DrawDiffractString(c2pstr(gTheText));		}			FSClose(refNum);	volName[0] = (char)0;	FlushVol("\p",vRefNum);	KillPtr((Ptr)theEDDSetIndex);	}void SearchObject::MatchSearchFile(short refNum, short blockNums,					 unsigned short *theCSReq,unsigned short *theCSX,					 unsigned short *theRMap,short noReq){	EDDSearchFile 	theSearchFile[18],					*theSearch;	OSErr 			errCode;	Boolean 		theHit;	short 			i,k,					posMode = 1,					length,sizeOfFilePtr;	short 			anFOM;	short			type;	unsigned short 	flag[16],					macFlag[16];	long 			posOff;	long 			sizeOfEDD;			float 			screen,theValue;	Rect 			theRect;	Handle 			theHandle;		errCode = noErr;				SetConversionFlags		(flag,macFlag);	sizeOfEDD 			= 	(18 * sizeof(EDDSearchFile));			i = -1;		sizeOfFilePtr = sizeof(FilePtr);	length = 99;	posOff = (1024L * (long)blockNums) + (START_SEARCH_EDD);	SetFPos(refNum,fsFromStart,posOff);	errCode = FSRead(refNum,&sizeOfEDD,(Ptr)theSearchFile);	if(errCode){	 	return;	}			screen = expVolume * .10;		for(k = 0; k <= 17;k++){		theSearch = &theSearchFile[k];		if(CompareMap	(theSearch->theChemMap,theCSReq,5) == noReq ){			if(!CompareMap	(theSearch->theChemMap,theCSX,5)){				theHit = false;				if(searchTypePopUp->lastResult >= VOL_SEARCH_D){					theValue = (float)ConvertToMacBits(theSearch->reducedVolume);					if(fabs(theValue - expVolume) <= screen)					{					 	theHit = true;					 	anFOM = (short)theValue;					 }				 }else{				 	if(CompareMap(theSearch->theRMap,theRMap,10) >= minMatches){					 	anFOM = SearchFOM(theSearch->theRMap);						if(anFOM >= requiredFOM){							theHit = true;						}					}				} 				if(theHit){					ListCrystal *lastCrystal;					totalFound++;					if(Quit()){						totalFound--;						return;					}					GetDItem(theDialogSearch,TOTALMATCH,&type,&theHandle,&theRect);					sprintf(gTheText,"%d",totalFound); 					SetIText(theHandle,c2pstr(gTheText));				    lastCrystal = foundCrystals->DoAdd(testCrystal,1,(long)blockNums,(long)k);               		lastCrystal->FOM = anFOM;               	}		             			}		}	}	return;}void SearchObject::ReturnSearchFileDspace(unsigned short *theRMap,float* theDspacings){	short i,k,l;	unsigned short flag2[16],macFlag[16],theValue;	for(l = 0; l < MAX_STORE_DSPACE; l++) theDspacings[l] = 0;	l = 0;		SetConversionFlags(flag2,macFlag);	for(i = 0; i <= 10; i++)	{		theValue = theRMap[i];		for(k = 0; k <= 15; k++){						if(theValue & flag2[k]){				k++;				if(l >= MAX_STORE_DSPACE) continue;				theDspacings[l] = 2.5 / (((i * 16) + k) * .018);				l++;				k++;			}		}	}}short SearchObject::CompareMap(unsigned short *Map1,unsigned short *Map2,short size){	short numHit = 0;	short i;	unsigned short flag2;	unsigned short theMap1,theMap2;		for(i = 0; i <= size;i++){		flag2 = (unsigned short) 1;		theMap1 = (unsigned short)Map1[i];		theMap2 = (unsigned short)Map2[i];		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = ( unsigned short) 2;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 4;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 8;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 16;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 32;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 64;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 128;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 256;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 512;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 1024;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 2048;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 4096;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 8192;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 16384;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;				flag2 = (unsigned short) 32768;		if(theMap1 & flag2 && theMap2 & flag2 ) numHit++;	}	return numHit;}Boolean SearchObject::OpenJCPDFile(short *vRefNum,short *refNum){	OSErr fileError,iErr;	char	theText[255];	long	curEOF;	sprintf(theText,"TN0700.EXT;1");	fileError = OpenNamedFile(SEARCH_EDD,theText,vRefNum,refNum);	if(!fileError){		START_SEARCH_EDD = 48640;/*(95-1) * 512 FOR 1987*/		BLOCKS_EDD = 94;	}		if(fileError){		sprintf(theText,"EDD0700.EXT");		fileError = OpenNamedFile(SEARCH_EDD,theText,vRefNum,refNum);		iErr = GetEOF(*refNum, &curEOF);		if(!fileError){				//START_SEARCH_EDD = 55296;/*(109 - 1) * 512 FOR 1993*/				BLOCKS_EDD = (double)(curEOF / 43520.) + .5;//(round up by one)				BLOCKS_EDD -= 1;				START_SEARCH_EDD = BLOCKS_EDD * 512;						}		if(fileError){			FileErrorMessages_DirPath( fileError);			return false;		}	}	return true;}Boolean SearchObject::GetReferenceFileInfo(short theFile,short theBlockNo){	Boolean 		volFound = false,oddFlag = false;	char 			name[255];		short 			refNum,theBlock,theWord,j,theShort;	short 			noOfIntensities,cellAngleFlag;	short 			redCellAngles;	short 			i,k,vRefNum;	short 			theNumber;	unsigned short 		highNibble = 0x000F,lowByte = 0x00FF,x[20];	unsigned short 		theLocation[2];unsigned short 		theHeader[14],					theValues[255],					theValue;						long 			posOff;	long 			noOfBytes;	long 			nameSize,formSize,theLong;		OSErr 			fileError;		testCrystal->referenceInfoSet = true;	i = 0;	testCrystal->formulaText[0] = NUL;	testCrystal->crystalName[0] = NUL;	testCrystal->year = 1800;	testCrystal->author[0] = NUL;	testCrystal->reference[0] = NUL;	testCrystal->cODEN[0] = NUL;			sprintf(name,"CHEM%02d.EXT;1",theFile); 		fileError = OpenNamedFile(SEARCH_EDD,name,&vRefNum,&refNum);		if(fileError){		sprintf(name,"CHEM%02d.EXT",theFile); 			fileError = OpenNamedFile(SEARCH_EDD,name,&vRefNum,&refNum);		if(fileError){			FileErrorMessages_DirPath( fileError);			return false;		}	}		/***/	theBlockNo--;	theNumber = theBlockNo/170; 	posOff = (512L * (long)theNumber) + ((theBlockNo - (theNumber * 170)) * 3);	 	if(fmod((double)posOff,2)){ oddFlag = true; posOff--;}	SetFPos(refNum,fsFromStart,posOff);	noOfBytes = 4L;	fileError = FSRead(refNum,&noOfBytes,(Ptr)&theLocation);	if(!oddFlag){		theBlock =  (short)ConvertToMacBits(theLocation[0]);		theWord = (ConvertToMacBits(theLocation[1]) & lowByte);	}else{		theBlock =  (short)(ConvertToMacBits(theLocation[0]) / 256) + ((ConvertToMacBits(theLocation[1]) & lowByte) * 256);		theWord = (short)(ConvertToMacBits(theLocation[1]) / 256);	}	posOff = (512L * (long)(theBlock - 1)) + (2L * (long)(theWord - 1));		SetFPos(refNum,fsFromStart,posOff);/****/	noOfBytes 		= 28;	fileError 		= FSRead(refNum,&noOfBytes,(Ptr)theHeader);	theValue 		= ConvertToMacBits(theHeader[0]);	nameSize 		= theValue & lowByte;	formSize 		= theValue / 256;	theValue 		= ConvertToMacBits(theHeader[1]);		noOfIntensities = theValue & lowByte;	if(noOfIntensities >= MAX_STORE_DSPACE)		noOfIntensities = MAX_STORE_DSPACE;			cellAngleFlag = (theValue / 256) & highNibble;	redCellAngles = (theValue / 4096);		testCrystal->atom_Count = (short)ConvertToMacBits(theHeader[3]) - 1; 		if(testCrystal->atom_Count > 500  || testCrystal->atom_Count < -1){		testCrystal->atom_Count = 0;		FSClose(refNum);		refNum = 0;				return false;	}else if(testCrystal->atom_Count == 0 || testCrystal->atom_Count == -1)		testCrystal->atom_Count = 0;testCrystal->atom_Count = 0;//this value is totally unrealiable	for(i = 0; i <= 1; i++){		x[i] = ConvertToMacBits(theHeader[4 + i]);	}	TranslateRadix50(2,&x[0],gTheText);	if(GetPtrSize((Ptr)testCrystal->cODEN) <= strlen(gTheText) + 1){			KillPtr((Ptr)testCrystal->cODEN);			testCrystal->cODEN = (char*)D_NewPtr((long) (strlen(gTheText) + 1) * sizeof(char));	}	strcpy(testCrystal->cODEN,gTheText);	for(i = 0; i <= 2; i++){		x[i] = ConvertToMacBits(theHeader[6 + i]);	}	TranslateRadix50(3,x,gTheText);	if(GetPtrSize((Ptr)testCrystal->reference) <= strlen(gTheText) + 1){			KillPtr((Ptr)testCrystal->reference);			testCrystal->reference = (char*)D_NewPtr((long) (strlen(gTheText) + 1) * sizeof(char));	}	strcpy(testCrystal->reference,gTheText);				for(i = 0; i <= 2; i++){		x[i] = ConvertToMacBits(theHeader[9 + i]);	}	TranslateRadix50(3,x,gTheText);	if(GetPtrSize((Ptr)testCrystal->author) <= strlen(gTheText) + 1){			KillPtr((Ptr)testCrystal->author);			testCrystal->author = (char*)D_NewPtr((long) (strlen(gTheText) + 1) * sizeof(char));	}	strcpy(testCrystal->author,gTheText);				testCrystal->year = 1800 + (theHeader[12] / 256);		if(testCrystal->atom_Count < 0) testCrystal->atom_Count = 0;	testCrystal->fileNo = (short)ConvertToMacBits(theHeader[12]) / 256;	testCrystal->cardNo = (short)ConvertToMacBits(theHeader[13]);	noOfBytes = 0;	for(i = 0; i <= 3;i++){		if((unsigned short)cellAngleFlag & ipow(2,i))			noOfBytes++;	}	for(i = 0; i <= 3;i++){		if((unsigned short)redCellAngles & ipow(2,i))			noOfBytes++;	}	noOfBytes *=  2 ;	fileError = FSRead(refNum,&noOfBytes,(Ptr)theValues);	i = 0;	if((unsigned short)cellAngleFlag & ipow(2,0)) testCrystal->theUnitCell.alpha = (float)ConvertToMacBits(theValues[i++]) / 100;	else  testCrystal->theUnitCell.alpha = 90;	if((unsigned short)cellAngleFlag & ipow(2,1)) testCrystal->theUnitCell.beta = (float)ConvertToMacBits(theValues[i++]) / 100;	else  testCrystal->theUnitCell.beta = 90;	if((unsigned short)cellAngleFlag & ipow(2,2)) testCrystal->theUnitCell.gamma = (float)ConvertToMacBits(theValues[i]) / 100;	else  testCrystal->theUnitCell.gamma = 90;		if((unsigned short)redCellAngles & ipow(2,0)) 		testCrystal->theReducedCell.alpha = (float)ConvertToMacBits(theValues[i++]) / 100;	else  testCrystal->theReducedCell.alpha = 90;		if((unsigned short)redCellAngles & ipow(2,1)) 		testCrystal->theReducedCell.beta = (float)ConvertToMacBits(theValues[i++]) / 100;	else  testCrystal->theReducedCell.beta = 90;		if((unsigned short)redCellAngles & ipow(2,2)) 		testCrystal->theReducedCell.gamma = (float)ConvertToMacBits(theValues[i]) / 100;	else  testCrystal->theReducedCell.gamma = 90;		/*if(testCrystal->theUnitCell.gamma == 120. && 		testCrystal->spaceGrpNm <= 167 && testCrystal->spaceGrpNm >= 146)		testCrystal->spaceGrpNm += 1000; eliminated by J.T. dec 1992*/			oddFlag = false;		if(fmod((double)nameSize,2))	{ 		oddFlag = true; 		nameSize++;	} 		fileError = FSRead(refNum,&nameSize,(Ptr)theValues);	if(nameSize > 200){		nameSize = 200;	}	for(i = 0; i <= (nameSize/2) - 1;i++)	{		x[i] = ConvertToMacBits(theValues[i]);	}	if(oddFlag){		x[(nameSize/2) - 1] = (x[(nameSize/2) - 1] & lowByte);		SetFPos(refNum,fsFromMark,-1);	}				TranslateRadix50(nameSize/2,x,gTheText);      i = strlen(gTheText);    if(i % 2)         i++;    theLong = i;   			if(GetPtrSize((Ptr)testCrystal->crystalName) <= strlen(gTheText) + 1){			KillPtr((Ptr)testCrystal->crystalName);			testCrystal->crystalName = (char*)D_NewPtr((long) (strlen(gTheText) + 1) * sizeof(char));	}		 strcpy((testCrystal->crystalName),gTheText);		oddFlag = false;	if(fmod((double)formSize,2)){ oddFlag = true; formSize++;} 	fileError = FSRead(refNum,&formSize,(Ptr)theValues);	if(fileError != 0){/*correction to keep from crashing October 1993*/		FSClose(refNum);		refNum = 0;		return false;	}	if(formSize > 0){		theLong = (long)(formSize + 5) * (long)sizeof(char);				KillPtr((Ptr)(testCrystal->formulaText));		testCrystal->formulaText = (char*)D_NewPtr(theLong);		for(i = 0; i <= (formSize/2) - 1;i++)		{			theValues[i] = ConvertToMacBits(theValues[i]);		}		j = 0;		for(i = 0; i <= (formSize/2) - 1;i++)		{						if(j >= formSize)continue;			j++;			testCrystal->formulaText[2 * i] = theValues[i] & lowByte;			if(j >= formSize)continue;			j++;			testCrystal->formulaText[(2 * i) + 1] = theValues[i] / 256;		}		testCrystal->formulaText[formSize - oddFlag] = NUL;			}		theShort = testCrystal->atom_Count;	//if(testCrystal->atom_Count > 1)testCrystal->AtomsFromFormula(theShort);	//the above does not work very well.	if(noOfIntensities <= 0){		FSClose(refNum);		refNum = 0;				return false;	}	if(oddFlag){		SetFPos(refNum,fsFromMark,-1);	}		noOfBytes = (noOfIntensities + 1 )/ 2;	fileError = FSRead(refNum,&noOfBytes,(Ptr)theValues);	FSClose(refNum);	refNum = 0;		for(i = 0,k = 0; i <= (noOfIntensities + 1 )/ 2;i++,k++){		if(i >= MAX_STORE_DSPACE)continue;		testCrystal->dspacings[i++].intensity = theValues[k] / 4096;		if(i >= MAX_STORE_DSPACE)continue;		testCrystal->dspacings[i++].intensity = (theValues[k] / 256) & highNibble;		if(i >= MAX_STORE_DSPACE)continue;		testCrystal->dspacings[i++].intensity = (theValues[k] & lowByte) / 16;		if(i >= MAX_STORE_DSPACE)continue;		testCrystal->dspacings[i++].intensity = (theValues[k] & lowByte) & highNibble;	}	return true;} void SearchObject::PlotComparativeLines(Rect bigRect){	double scaleFactor;	short i,radius;	Rect theRect,lineRect;     short theLeft,theRight,theTop,theBottom,middle, del;          theLeft     = bigRect.left;     theRight    = bigRect.right - 15;     theTop      = bigRect.top + 30;     theBottom   = bigRect.bottom - 15;     middle      = (theTop + theBottom) / 2; 	SetRect(&theRect,theLeft,theTop,theRight,theBottom + 2);	scaleFactor = 380;	dm_EraseRect(&theRect);	TextFont(geneva);	TextSize(9);	MoveTo(theLeft + 5,theTop);	sprintf(gTheText,"High d-space");	DrawDiffractString(c2pstr(gTheText));	MoveTo(theRight - 55,theTop);	sprintf(gTheText,"low d-space");	DrawDiffractString(c2pstr(gTheText));	TextFont(0);	TextSize(0);	del = (short)(scaleFactor * .0095);	for(i = 0; i < MAX_STORE_DSPACE;i++)	{		if(i > 0){			if(fabs((double)(testCrystal->dspacings[i].x - 					testCrystal->dspacings[i - 1].x)) < .05)				continue;		}		if((double)testCrystal->dspacings[i].x < .1)			continue;		radius = (short)(scaleFactor / (double)testCrystal->dspacings[i].x);				if(theLeft + radius > theRight - 5)				continue;		MoveTo(theLeft + radius,middle);		LineTo(theLeft + radius,theBottom);				SetRect(&lineRect,theLeft + radius - del,middle,theLeft + radius + del + 1,(theBottom + middle) * .5);		dm_FrameRect( &lineRect);	}	MoveTo(theLeft,middle);	LineTo(theRight - 5,middle);	for(i = 0; i < MAXEXPDSPACE;i++)	{		if((double)dspacings[i]< .1)			continue;		radius = (short)(scaleFactor / (double)dspacings[i]);		if(theLeft + radius > theRight - 5)continue;		MoveTo(theLeft + radius,middle);		LineTo(theLeft + radius,theTop);	}}void TranslateRadix50(short number,unsigned short *theRadixLetter,char *theAsciiLetter){       long  temperNum;       short i,j,k,theChar,jMax;       char TAB[40] = {' ','A','B','C','D','E','F','G','H','I','J','K',     					'L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',     					'$','.','?','0','1','2','3','4','5','6','7','8','9'};            number--;       jMax = 0;       for(i = 0; i <= number;i++){       		        		temperNum = theRadixLetter[i];        		j = (3 * (i + 1)) - 1;       			       for(k = 0; k <= 2;k++){	       		theChar = (short)fmod((double)temperNum,40);		     	theAsciiLetter[j] =  TAB[theChar];	       		temperNum  /= 40;	       		j--;	       		++jMax ;	       }      }	theAsciiLetter[jMax] = NUL;}