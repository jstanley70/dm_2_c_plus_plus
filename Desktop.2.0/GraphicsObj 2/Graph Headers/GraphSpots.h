//ces.Jim:Desktop.2.0:GraphicsObj 2:Graph Headers:GraphSpots.h#ifndef		GRAPH_SPOT_OBJ#define		GRAPH_SPOT_OBJ#include 	"SpotObject.h"#include 	"GraphFunction.h"#pragma def_inherited onclass GraphSpots:public GraphFunction{	public:	void		DoInit(Rect frameRect,RGBColor theColor,Boolean addMenu,Boolean grow);	void		SetValues(Handle xD,Handle yD);	void		AdditionalPointers(void);	void		PlotGraph(void);	GraphFunction* GetDerivative(void);	GraphFunction* ConvertToSpectra(long noPts,float minium,float maximum,SADObj *theOwner);	Boolean	SaveText(void);	void  DrawSpotText(void);	void 	GetMinMax(Handle theData,double *minV,double *maxV,short noPts);};#endif