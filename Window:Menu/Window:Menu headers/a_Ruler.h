//a_Ruler.h#ifndef	_RULER#define	_RULER#include "RulerButton.h"class	Ruler{public:	long			theOwner;	char			functionPrompt[50];	char			functionText[140];	char			helpText[257];	short			functionID;	short			rulerHeight;	short			infoStart;	Boolean			functionFlag;	Boolean			helpActive;	Boolean			rulerOFF;	Rect			helpRect;	Rect			funcRect;	Rect			infoRect;	short			maxButton;	Ptr				rulerButtons[6];	void			DoInit(long	theOwner);	void			EnableButtons(short maxButtons);	void			DoClose(void);	void			DrawRuler(void);	void			SetPrompt(char*);	void			ClearPrompt(void);	void			DisplayPrompt(void);	void			SetInfo(char*);	void			DisplayInfo(void);	void			SetHelp(char*);	void			ClearHelp(void);	void			DisplayHelp(void);	void			DoContent(Point);	void 			SetBeamButton(short theBeamFlag);	void			DoDblClick(Point thePoint);	RgnHandle SetRulerDrawEnv(PenState *theState,RGBColor *foreColor,RGBColor *backColor);	void ClearRulerDrawEnv(RgnHandle theRgn,PenState theState,RGBColor foreColor,RGBColor backColor);};#define		SCRATCH_MENU_ID	32000#endif