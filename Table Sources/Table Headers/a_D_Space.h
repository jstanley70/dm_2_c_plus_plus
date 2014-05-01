
#include	"a_Table.h" 
#include	"ML_Dspace.h" 
#ifndef		DSPACE_TABLE
#define DSPACE_TABLE
//a_D_Space.h
#pragma def_inherited on
class	D_Spacing_Table:public Table{
		public:
Index	*firstPlane;
CrystalButton		*theCrystalButton;
short	displayDspace;
float	dspaceLow;
float	minSF;

/*in Base*/	void            DoInit(void);
/*in Base*/	void		DoClose(void);
/*in Base*/	Boolean		DoContent(Point);
/*in Base*/	void		DoDblClick(Point thePoint);
/*in Base*/	void		DoIdle(void);	/*	Idle Routine		*/
/*in Base*/	void	DoMenu(long);
/*in Base*/	void	MyCalculate(void);
/*Parent*/	void	DisplayList(void);
/*in Base*/	void	DoPrint(void);
/*in Base*/	void	DoKey(char);
/*in Base*/	void	DoRefresh(void);
 void DoDefine(void);
/*Parent*/	void 	DrawTheText(void);
/*Parent*/	void	DoHeaderPrint(Rect *topRect);
/*in Base*/	void 	SetObjectMenu(void);
/*in Base*/	void	DoAuxRead(void);
/*in Base*/	void	DoAuxWrite(void);
/*in Base*/	void 	RationalizeData(void);
/*Parent*/	void	SetButton(void);
virtual void 	SetDefineObjTab(DialogPtr theDialog,ML_DspaceParmPtr theParams);
virtual Boolean 	DefineSwitchTab(short  theSelect,ML_DspaceParmPtr theParams,Boolean *saveData);
virtual void	ReadDefineDialogTab(DialogPtr theDialog,ML_DspaceParmPtr theParams,Boolean saveData);
virtual void  		DoDefineSetUp(void);

};

#endif