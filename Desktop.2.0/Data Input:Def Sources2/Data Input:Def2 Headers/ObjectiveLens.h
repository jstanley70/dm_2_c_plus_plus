//m:Desktop.2.0:Data Input/Def Sources2:Data Input/Def2 Headers:ObjectiveLens.h#ifndef _OBJECTIVELENS#define _OBJECTIVELENS #include	"SF_MathLib.h"class DiffractObject;class ObjectiveLens{		public:	double 	sphericalAberration;	double 	chromaticAberration;	double 	apperatureRadius;	double 	delVoltage;	double 	delCurrent;	double	voltage;	double 	astigmatism;	double	stigmaticAngle;	double	defocus;	double	defocusSpread;	double	spotDiameter;	double	convergenceAngle;	double	**tFValues;	Boolean	chromeSpread;	double	scherzerDefocus;	double	ptToptRes;	double	infoLimit;	double	maxDelE;	double	gauseDefocus;	double	lensCurrent;	double	wavelength;	short	maxCount;	DiffractObject *owner;	GraphFunction *theGraph;	SpotInfoHandle lines;		void	DoDefine				(void);	void	DrawTransferFunction	(Rect graphRect);	void	CalcPlaneTransferFunction(void);	void	ReadNewValues(DialogPtr theDialog);	void	CalculateScherzer		(void);	void	DrawScherzer(DialogPtr theDialog,short itemNo);	void	CalculateTransferFunction(void);	void	CalculateDefocuSpread	(void);	dcomplex	CalculateTransferValue	(double rValue,double angle/*,short i*/);	void	DoInit					(DiffractObject *theOwner);	void	DoClose					(void);};#endif