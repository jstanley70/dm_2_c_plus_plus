#ifndef _ICON_OBJ#define _ICON_OBJclass IconObj{	public:	Handle theIcon;	Rect	iconRect;	short mode;	void 	DoClose(void);	void 	DoDraw(void);	void 	DoInit(Rect theRect,long iconID);	Boolean DoHit(Point thePoint);	void 	SetIconRect(Rect theRect);};#endif