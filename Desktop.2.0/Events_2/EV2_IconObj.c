#include	"Icons.h"#include	"IconObj.h"void IconObj::DoInit(Rect theRect,long iconID){	OSErr	myErr;	myErr = GetIconSuite(&theIcon,iconID,svAllAvailableData);	iconRect = theRect;}void IconObj::DoDraw(void)//ttSelected{	PlotIconSuite(&iconRect,0,mode,theIcon);}Boolean IconObj::DoHit(Point thePoint){	if(PtInRect(thePoint,&iconRect)){		if(!mode){			mode  = ttSelected;		}		else{			mode  = 0;		}		DoDraw();		return true;	}else{		mode = 0;			}	DoDraw();	return false;}void IconObj::DoClose(void){	OSErr myErr;//DisposHandle(theIcon);	myErr = DisposeIconSuite(theIcon,true);	D_delete(this);}void IconObj::SetIconRect(Rect theRect){	iconRect = theRect;}