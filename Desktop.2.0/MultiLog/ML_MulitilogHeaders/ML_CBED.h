#ifndef __ML_CBEDOBJ__#define	__ML_CBEDOBJ__		#include "ML_Multilog.h"	void		ML_DoCBEDObjSetup(Ptr owner,DialogPtr theDialog,Ptr	*paramBlock);	void		ML_DoCBEDObjHit(Ptr	owner,DialogPtr theDialog, short theSelect,	Ptr	paramBlock);	void		ML_DoCBEDObjRead(Ptr owner,DialogPtr theDialog,Ptr	paramBlock,Boolean saveData);	#endif