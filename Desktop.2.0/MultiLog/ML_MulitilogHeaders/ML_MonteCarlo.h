#ifndef __ML_MONTE_CARLO__#define	__ML_MONTE_CARLO__		#include "ML_Multilog.h"	void		ML_DoMonteCarloObjSetup(Ptr owner,DialogPtr theDialog,Ptr	*paramBlock);	void		ML_DoMonteCarloObjHit(Ptr	owner,DialogPtr theDialog, short theSelect,	Ptr	paramBlock);	void		ML_DoMonteCarloObjRead(Ptr owner,DialogPtr theDialog,Ptr	paramBlock,Boolean saveData);	#endif