//cts:D.M. v2.0:Sources.Jim:Desktop.2.0:Diffract 2:DI2_DiffractInits_Jim_2.c#include	"Diffract_INCs.h"#include "MonteCarloObj.h"#include "TextTableObj.h"#include "OneDisObj.h"#include "TwoDisObj.h"#include "GraphObject.h"Ptr AllocOneDisObj(void){	#ifndef BLANK_ROUTINES	OneDisObj	*theObj;	theObj	=	(OneDisObj*)D_new(OneDisObj);	if(HandleIsOK((Handle)theObj)){		theObj->objectType  =	MICRO_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	return(NUL_PTR);}Ptr AllocTwoDisObj(void){	#ifndef BLANK_ROUTINES	TwoDisObj	*theObj;		theObj	=	(TwoDisObj*)D_new(TwoDisObj);	if(HandleIsOK((Handle)theObj)){		theObj->objectType  =	MICRO_OBJ_2;		theObj->DoInit();		return((Ptr)theObj);	} 	#endif	return(NUL_PTR);}#ifdef BLANK_ROUTINESPtr AllocPattersonObject(void){return(NUL_PTR);}Ptr AllocHREMObj(void){return(NUL_PTR);}Ptr AllocBEKPObj(void){return(NUL_PTR);}#endifPtr AllocMonteCarloObj(void){	//#ifndef BLANK_ROUTINES	MonteCarloObj	*theObj;		theObj	=	(MonteCarloObj*)D_new(MonteCarloObj);	if(HandleIsOK((Handle)theObj)){		theObj->objectType  =		MONTE_CARLO_OBJ;		theObj->DoInit();		return((Ptr)theObj);	} 	//#endif	return(NUL_PTR);}