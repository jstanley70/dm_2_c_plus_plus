//s.Jim:Event/Memory:EV_ResFuncs.c#include	"Diffract_INCs.h"Handle	GrabResource(ResType	theType,short theNum,unsigned char *theName,long theSize){	Handle theHandle;		theHandle = GetResource(theType,theNum);	if(ResError() != 0 || theHandle == (Handle)NUL){		theHandle = D_NewHandle(theSize);		if(theHandle == (Handle)NUL){			StopAlert(OUT_OF_MEM,NUL);			return ((Handle)NUL);		}		AddResource(theHandle,theType,theNum,(unsigned char*)theName);		if(ResError() != 0){			StopAlert(ADD_RES_FAILED,NUL);			return ((Handle)NUL);		}else{			WriteResource(theHandle);		}	}	if(theSize != SizeResource(theHandle)){		SetHandleSize(theHandle,theSize);		if(MemError() != 0){			StopAlert(OUT_OF_MEM,NUL);			return ((Handle)NUL);		} else {			ChangedResource(theHandle);			WriteResource(theHandle);		}	}	return (theHandle);}