#include "ML_CrystalFace.h"#include 	"crystalFaceObj.h"#include	"Diffract_INCs.h"void ML_DoCrysFaceSetup(Ptr owner,DialogPtr theDialog,Ptr	*thePtr){	PutCrystalFaceObj *theCrystalFaceObj;	Crystal*		theOwner;	ML_CrysFaceParamsPtr theParams;		theCrystalFaceObj 	= (PutCrystalFaceObj*)D_new(PutCrystalFaceObj);	theOwner 			= (Crystal*)owner;	theCrystalFaceObj->Init(theOwner);		*thePtr = (Ptr)D_NewPtr(sizeof(ML_CrysFaceParams));	theParams = (ML_CrysFaceParamsPtr)*thePtr;	theCrystalFaceObj->theDialog = theDialog;	theCrystalFaceObj->DoSetUp( owner,  *thePtr);	theParams->theCrystalFaceObj = theCrystalFaceObj;	theParams->theCrystalFaceObj->oldPort  = theDialog;}void ML_DoCrysFaceHit(Ptr	owner,DialogPtr theDialog, short theSelect,	Ptr	thePtr){	ML_CrysFaceParamsPtr theParams;	Boolean		saveData;	owner = owner;	theDialog = theDialog;	theParams = (ML_CrysFaceParamsPtr)thePtr;	theParams->theCrystalFaceObj->DoHit( thePtr, theSelect,&saveData);}void ML_DoCrysFaceRead(Ptr owner,DialogPtr theDialog,Ptr	thePtr,Boolean saveData){	ML_CrysFaceParamsPtr theParams;	theParams = (ML_CrysFaceParamsPtr)thePtr;	theDialog = theDialog;	theParams->theCrystalFaceObj->DoRead( owner, thePtr,saveData);	KillPtr(theParams);}					void PutCrystalFaceObj::DoSetUp(Ptr owner,Ptr thePtr){		Handle 			theHandle;	Handle			theReshandle;	double			xForm[3][3];	Rect 			thePicRect;	Crystal			*originalCrystal;	RgnHandle		newClip;	RgnHandle		oldClip;	Rect			eraseRect;	CrystalFace		**theFacesHandle;	PicHandle		thePict;	PopUpMenu  		*theCrystalFaceList;		ML_CrysFaceParamsPtr theParams;	short			i,j;	short			theResult;	short 			n = 0,type;	owner = owner;	theParams = (ML_CrysFaceParamsPtr)thePtr;		//theDialog = DM_GetNewDialog(DLOG_FACE_LIB, 0L,(WindowPtr)-1L);	oldClip	=	D_NewRgn();	GetClip(oldClip);		ForeColor(BLACK);	gTheCell.v = 0;	gTheCell.h = 1;	GetDItem		(theDialog,LIBRARY_PICT,&type,&theHandle,&thePicRect);	originalCrystal = 	(Crystal*)D_new(Crystal);	originalCrystal	->	InitCrystal(0);	oldCrystal			->	CopyCrystal(originalCrystal);	DoBoxes					(theDialog);		InitFaceList	();	ListFaces		();	SetTheHKLText	();			SelIText(theDialog,HKLINPUT,0,32767);		gTheCell.h = 0;	LSetSelect(true,gTheCell,theFaceList);	LSetSelect(false,gTheCell,theFaceList);	gTheCell.h = 1;	LSetSelect(true,gTheCell,theFaceList);		Identity_Matrix(xForm);				theCrystalFaceList = (PopUpMenu*)D_new(PopUpMenu);	theCrystalFaceList->Init(theDialog,LIBRARY,502,1);	/* InsertResMenu(theCrystalFaceList->theMenu,'FcLb',1);*/	theResult = 1;	theReshandle = GetResource('FcLb',theResult);	while(theReshandle != 0L){		ResType resType;		short resID;		GetResInfo(theReshandle,&resID,&resType,pTheText);		theCrystalFaceList->P_AppendMenu(gTheText);		ReleaseResource(theReshandle);		theResult++;		theReshandle = GetResource('FcLb',theResult);	}	theResult = 1;	theCrystalFaceList->SetPopUp();		theCrystalFaceList->lastResult = 1;	theFacesHandle = 0L;	eraseRect = thePicRect;	InsetRect(&eraseRect,1,1);	ForeColor(BLACK);	thePict = oldCrystal->DrawMorphology(xForm,thePicRect);			newClip = D_NewRgn();	RectRgn(newClip,&thePicRect);	D_SetClip(newClip);		dm_FrameRect(&thePicRect);	DrawPicture(thePict,&thePicRect); 	D_SetClip(oldClip);	InvalRgn(newClip);	for(i = 0;i < 3;i++){for(j = 0; j < 3;j++)theParams->xForm[i][j] = xForm[i][j];}	theParams->thePicRect = thePicRect;	theParams->originalCrystal = originalCrystal;	theParams->newClip = newClip;	theParams->oldClip = oldClip;	theParams->eraseRect = eraseRect;	theParams->theFacesHandle = theFacesHandle;	theParams->thePict = thePict;	theParams->theCrystalFaceList = theCrystalFaceList;	theParams->theResult = theResult;	theParams->theFlag = true;	//Boolean saveData;	//(ML_CrysFaceParamsPtr)theParams->theCrystalFaceObj->DoHit((Ptr)theParams,REDRAW_FACES,&saveData);		}void PutCrystalFaceObj::DoRead(Ptr owner,Ptr thePtr,Boolean saveData){	ML_CrysFaceParamsPtr theParams;	owner = owner;	theParams = (ML_CrysFaceParamsPtr)thePtr;	if(saveData)DoCopy(crystalToSet);	KillPicture(theParams->thePict);	theParams->theCrystalFaceList->DoClose();	theParams->originalCrystal->DoClose();	D_DisposeRgn(&theParams->newClip);	D_DisposeRgn(&theParams->oldClip);	Kill();	}