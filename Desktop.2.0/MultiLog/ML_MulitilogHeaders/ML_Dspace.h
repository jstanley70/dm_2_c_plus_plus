#ifndef __ML_DSPACE__#define	__ML_DSPACE__	#include "ML_Multilog.h"typedef struct ML_DspaceParm{	PopUpMenu	*theMenu;	PopUpMenu	*sortMethodMenu;	PopUpMenu	*displayMenu;	}ML_DspaceParm,*ML_DspaceParmPtr;	void		ML_DoDspaceSetup(Ptr owner,DialogPtr theDialog,Ptr	*paramBlock);	void		ML_DoDspaceHit(Ptr	owner,DialogPtr theDialog, short theSelect,	Ptr	paramBlock);	void		ML_DoDspaceRead(Ptr owner,DialogPtr theDialog,Ptr	paramBlock,Boolean saveData);	#endif