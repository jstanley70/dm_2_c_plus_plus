	#ifndef __ML_ONE_DIS__#define	__ML_ONE_DIS__		#include "ML_Multilog.h"typedef struct ML_OneDisParams{	short  lastSelect;	short  oldCrystal;	}ML_OneDisParams,*ML_OneDisParamsPtr;void		ML_DoOneDisObjSetup(Ptr owner,DialogPtr theDialog,Ptr	*paramBlock);void		ML_DoOneDisObjHit(Ptr	owner,DialogPtr theDialog, short theSelect,	Ptr	paramBlock);void		ML_DoOneDisObjRead(Ptr owner,DialogPtr theDialog,Ptr	paramBlock,Boolean saveData);	#endif