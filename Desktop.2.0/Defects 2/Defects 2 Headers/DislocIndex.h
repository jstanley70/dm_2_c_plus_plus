//0:Sources.Jim:Desktop.2.0:Defects 2:Defects 2 Headers:DislocIndex.h#ifndef _DISLOC_IND#define _DISLOC_IND#pragma def_inherited onclass	DislocIndex:public Index{	public:	DislocIndex		*next;	void			DoInit(Boolean theFlag,Crystal *aCrystal);	void			CloseAll(void);	void			SubtractIndex(void) ;	void AddIndex(Index* theItem);	void 			AddIntensity(float theValue);	Boolean			CopyNth(Index *theIndex,short num);	DislocIndex* 	InitNext(void);	Boolean	 		CopyToNth(Index *theIndex,short num);	void 			ReadIndex(Crystal *theCrystal[4]);	void  			WriteIndex(void); //Override	void			AddItemIndex(DialogPtr theDialog,short theItem); //Override	};#endif