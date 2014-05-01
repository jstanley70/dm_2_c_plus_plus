//0:Sources.Jim:Diffract:DI_DiffractInits.c#include	"Diffract_INCs.h"#include	"ShowIntTabCry.h"//#include	"a_ImageObject.h"#include	"KikuchiObject.h"#include	"a_LatticeObject.h"//#include	"PictureObj.h"#include	"PhaseDObj.h"#include	"a_D_Space.h"  /* Jim Code */Ptr	AllocDiffractObj(void){		DiffractObject	*theObj;		theObj	=	(DiffractObject*)D_new(DiffractObject);	if(HandleIsOK((Handle)theObj)){		theObj->objectType	=	NUL;		theObj->DoInit();		return((Ptr)theObj);	} else {		gQuitFlag = true;		return(NUL_PTR);	} }Ptr	AllocCrystal(short	num){		Crystal	*theObj;	Ptr theObj1;	KeyMap	keyArray;		theObj = 0L;	theObj1 =  0L;	theObj	=	(Crystal*)D_new(Crystal);		if((Ptr)theObj == (Ptr)NUL || MemError() != 0){		StopAlert(OUT_OF_MEM,NUL);		{					GetKeys(keyArray);			if(keyArray[1] == 5){				SysBreak();			}		}		return ((Ptr)NUL);	}	if(!theObj->InitCrystal(num))	{		StopAlert(OUT_OF_MEM,NUL);		theObj->DoClose();		return (Ptr)0L;	}	return((Ptr)theObj);}/*Ptr	AllocImageObject(void){	#ifndef BLANK_ROUTINES_3	ImageObject	*theObj;	theObj	=	(ImageObject*)D_new(ImageObject);	if(HandleIsOK((Handle)theObj)){		theObj->objectType  =	IMAGE_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	gQuitFlag = true;	return(NUL_PTR);}*/Ptr	AllocStereoObject(void){	#ifndef BLANK_ROUTINES_3	StereoObject	*theObj;	theObj	=	(StereoObject*)D_new(StereoObject);	if(HandleIsOK((Handle)theObj)){		theObj->objectType		=	STEREO_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	gQuitFlag = true;	return(NUL_PTR);}Ptr	AllocKikuchiObject(void){	#ifndef BLANK_ROUTINES_3	KikuchiObject	*theObj;	theObj	=	(KikuchiObject*)D_new(KikuchiObject);	if(HandleIsOK((Handle)theObj)){		theObj->objectType		= KIKUCHI_OBJECT;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	gQuitFlag = true;	return(NUL_PTR);}Ptr AllocRealLatObject(void){	#ifndef BLANK_ROUTINES_3	LatticeObject	*theObj;		theObj	=	(LatticeObject*)D_new(LatticeObject);	if(HandleIsOK((Handle)theObj)){		theObj->objectType 	= LATTICE_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	return(NUL_PTR);}Ptr	AllocDummyObj(void){	DiffractObject	*theObj;		theObj	=	(DiffractObject*)D_new(DiffractObject);	if(HandleIsOK((Handle)theObj)){		theObj->objectType	=	1000L;		theObj->DoInit();		return((Ptr)theObj);	} else {		gQuitFlag = true;		return(NUL_PTR);	}}Ptr	AllocPPT_Info(Crystal*	theCrystal){	PPT_Info	*theObj;		theObj	=	(PPT_Info*)D_new(PPT_Info);	if(HandleIsOK((Handle)theObj)){		theObj->DoInit(theCrystal);		return((Ptr)theObj);	} else {		gQuitFlag = true;		return(NUL_PTR);	}}Ptr	AllocTableObj(void){	#ifndef BLANK_ROUTINES_3	D_Spacing_Table	*theObj;		theObj	=	(D_Spacing_Table*)D_new(D_Spacing_Table);	if(HandleIsOK((Handle)theObj)){		theObj->objectType			=	D_SPACE_TABLE;		theObj->DoInit();		return((Ptr)theObj);	}	#endif	return(NUL_PTR);}Ptr	AllocShowIntTabObj(void){	#ifndef BLANK_ROUTINES_3	ShowIntTables	*theObj;		theObj	=	(ShowIntTables*)D_new(ShowIntTables);	if(HandleIsOK((Handle)theObj)){		theObj->objectType					=	100;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	return(NUL_PTR);}/*Ptr	AllocPictureObj(void){	#ifndef BLANK_ROUTINES_3	DiffractObject	*theObj;		theObj	=	(PictureObj*)D_new(PictureObj);	if(HandleIsOK((Handle)theObj)){		theObj->objectType 		= PICTURE_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	gQuitFlag = true;	return(NUL_PTR);}*/Ptr	AllocPhaseDObj(void){	#ifndef BLANK_ROUTINES_3	DiffractObject	*theObj;		theObj	=	(PhaseDiagram*)D_new(PhaseDiagram);	if(HandleIsOK((Handle)theObj)){		theObj->objectType 		= PHASED_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	gQuitFlag = true;	return(NUL_PTR);}