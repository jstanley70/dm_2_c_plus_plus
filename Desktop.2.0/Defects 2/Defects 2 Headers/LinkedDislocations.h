//m:Desktop.2.0:Defects 2:Defects 2 Headers:LinkedDislocations.h#ifndef _LINKED_DISLOCATIONS#define _LINKED_DISLOCATIONS#include	"a_Crystal.h"class LinkedDislocation{	DialogPtr		theDialog;	DialogPtr		oldPort;	DislocationObj	*theDislocation;	short			activeRow;	short 			centerX,centerY;	float			scaleFactor;	float			length;	Cell			gTheCell;	ListHandle		theList;	Crystal			*theCrystal;	double			crystalForm[3][3];	short 			count;	long			groupID;	Boolean         DoLinkDefine(DislocationObj *theDislocation);	void			ListClickedLink(DislocationObj *currentDefect);	void			Kill(void);	void			Init(Crystal *crystalObjOld,double xForm[3][3],float scaleFactor,DislocationObj *oldDislocation,short theGroupID);	void			DeleteDislocation(void);	void			EnterHKL(void);	void			SetTheHKLText(void);	void			CorrectList(short hor);	void			HandleListRect(void);	Boolean			DoLibrary(void);	void 			AddResourceToLibrary(PopUpMenu *theCrystalFaceList);	Boolean 		GetShapeName(char *theText);	void			AddLine(void);	void			InitList(void);	void	 		ListDislocationLines(DislocationObj *theDis);	void 			DoCopy(DislocationObj *theCopyDis);	};#endif