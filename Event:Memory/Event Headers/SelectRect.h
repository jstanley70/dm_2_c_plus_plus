//Jim:Event/Memory:Event Headers:SelectRect.h#ifndef	_SELECT_RECT#define	_SELECT_RECT#include 	"a_DiffractObject.h"class	SelectRect{public:	WindowPtr		theWindow;	Rect			theRect;	PicHandle		selectPict;	Rect			selectPictRect;	Point			lastClick;	Boolean			isValid; 	SelectRect*		DoInit(WindowPtr);	Boolean			DoSelect(Point);	Boolean			DoObjSelect(Point);	void			DoClose(void);	void			DoFrame(void);	void			DoObjFrame(void);	void			Reset(WindowPtr);	void			DoCopy(DiffractObject*);};#endif