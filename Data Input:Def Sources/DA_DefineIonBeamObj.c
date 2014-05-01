//Macintosh HD:CodeWarrior� DR/3 Gold �:Metrowerks  C/C++ �:Projects:D.M. v2.0:Sources.Jim:Data Input/Def Sources:DA_DefineIonBeamObj.c#include	"Diffract_INCs.h"#include 	"defineElectronObj.h"#include 	"DefineIonBeamObj.h"#define		A_CRYSTAL ((DiffractObject*)theOwner)->theCrystal[0]void IonDefObj::DoClose(void){	inherited::DoClose();}void	IonDefObj::DoRead(DialogPtr theDialog,Ptr theParams,Boolean saveData,Boolean closeDialog){	Handle 	theHandle;	short 	type;	Rect 	theRect;	float old;	short oldNum;	if(saveData){		GetDItem(theDialog,MAXENERGY,&type,&theHandle,&theRect);		GetIText(theHandle,pTheText);		oldNum = theIon;		theIon = A_CRYSTAL->TheAtomicNum(p2cstr(pTheText));			if(oldNum != theIon)				((DiffractObject*)theOwner)->changedFlag = true;		old = wavelength;		SetWaveAndEnergy(energy);		if(fabs(wavelength - old) > .01)				((DiffractObject*)theOwner)->changedFlag = true;	}	inherited::DoRead(theDialog, theParams, saveData,closeDialog);	}void IonDefObj::DoInit(long	owner){	theIon = 1;		inherited::DoInit(owner);		charWaveLength 	= 1;		SetWaveAndEnergy(10);}float IonDefObj::SetVOLTAGE(void){	return energy;}void  IonDefObj::SetFluff(DialogPtr theDialog,Boolean *cameraFlag){	*cameraFlag = true;	ShowDItem(theDialog,24);	ShowDItem(theDialog,25);	ShowDItem(theDialog,26);		GetDItem		(theDialog,25,&gType,&gTheHandle,&gTheRect);	sprintf			(gTheText,"");	SetIText		(gTheHandle,c2pstr(gTheText));		GetDItem		(theDialog,24,&gType,&gTheHandle,&gTheRect);	sprintf			(gTheText,"Ion At. Symbol");	SetIText		(gTheHandle,c2pstr(gTheText));	GetDItem		(theDialog,26/*MAXENERGY*/,&gType,&gTheHandle,&gTheRect);	A_CRYSTAL	->	GetAtomicSymbol(theIon,gTheText);	SetIText		(gTheHandle,c2pstr(gTheText));		GetDItem(theDialog,LENGTHBUTTON,&gType,&gTheHandle,&gTheRect);	sprintf(gTheText,"Camera Constant");	SetCTitle((ControlHandle)gTheHandle,c2pstr(gTheText));	}void	IonDefObj::InitPopUpMenu(DialogPtr theDialog,PopUpMenu *popUpMenu){	popUpMenu->Init(theDialog,POPUPMENUKV,132,1);	A_CRYSTAL->GetAtomicSymbol(theIon,gTheText);	popUpMenu->P_SelectItem	(gTheText);	}void  IonDefObj::DoPopMenuComplete(DialogPtr theDialog,PopUpMenu *popUpMenu){	long theStandardIon;		theStandardIon 	= 	popUpMenu->DoPopUp();	popUpMenu		->	P_GetCItemText(gTheText);	GetDItem			(theDialog,MAXENERGY,&gType,&gTheHandle,&gTheRect);	SetIText			(gTheHandle,pTheText);	theIon 			=  A_CRYSTAL->TheAtomicNum(p2cstr(pTheText));	}void IonDefObj::KillDialog(void){		//DeleteMenu(132);}void  IonDefObj::SetWaveAndEnergy(float theValue){		float		**atomicMassHndl;	double		massSingle;	energy 					= theValue;	if		(energy == 0)	{wavelength = 0; return;}	atomicMassHndl 	= (float**)GetResource('info',100);	element 				= theIon;	charge 					=  A_CRYSTAL->ReturnValance(&element);	mass 						= 	(*atomicMassHndl)[element - 1];	massSingle 			= mass / (AVOGADROS_NUMBER  * 1000);	wavelength 			= PLANKS_CONSTANT / sqrt( 2 * EV_TO_JOULES * massSingle * energy * (1 + 										(EV_TO_JOULES * energy/(2 * massSingle * SPEED_OF_LIGHT * SPEED_OF_LIGHT))));	ReleaseResource((Handle)atomicMassHndl);	return;}void IonDefObj::ReadBeam(void){	inherited::ReadBeam();	theIon = gTheFile->ReadShort();	mass   = gTheFile->ReadFloat();	element = gTheFile->ReadShort();	charge  = gTheFile->ReadShort();}void IonDefObj::WriteBeam(void){	inherited::WriteBeam();		gTheFile->WriteShort(theIon);	gTheFile->WriteFloat(mass);	gTheFile->WriteShort(element);	gTheFile->WriteShort(charge);	}void	IonDefObj::SetNewRay(void){	}PicHandle IonDefObj::DisplayPict(Rect *theRect,short horiz,short vert){	short 		h,v,i,width;	PicHandle	thePict;	Rect			frameRect;		TextFont(systemFont);	PenNormal();	sprintf(gTheText,"Log Dynamic Range = %d X %d �",dyRange);	c2pstr(gTheText);	width = StringWidth((unsigned char*)gTheText);	SetRect(theRect,2,2,width + 30,225);	OffsetRect(theRect,horiz,vert);	frameRect = *theRect;	//InsetRect(&frameRect,-2,-2);	thePict = OpenPicture(theRect);			v = 25;		for(i = 1 ; i <= 8 ; i++){		switch(i){			case 1:				sprintf(gTheText,"Energy = %5.2f kV",energy);				break;			case 2:				sprintf(gTheText,"lambda = %5.4f �",wavelength);				break;			case 3:				sprintf(gTheText,"negative size = %d X %d mm",(short)negWidth,(short)negHeight);				break;			case 4:				sprintf(gTheText,"Camera Constant = %5.2f mm-�",cameraConstant);				break;			case 5:				sprintf(gTheText,"Convergence Angle = %5.3f �-1",convergenceAngle);				break;			case 6:				sprintf(gTheText,"Log Dynamic Range = %5.1f",dyRange);				break;			case 7:				sprintf(gTheText,"Exposure Time = %5.2f s",expoTime);				break;			case 8:				if(polarizedFlag)					sprintf(gTheText,"Polarized");				else goto FINISH;				break;					}				h = 5;		MoveTo(h + horiz,v + vert);		DrawDiffractString(c2pstr(gTheText));		v += 20;	}FINISH:	PenSize(2,2);	gCurrentObj->DMForeColor(BLACK);	PenNormal();	FrameRect(&frameRect);	ClosePicture();	return(thePict);}