#ifndef	_SCAT_OBJ#define	_SCAT_OBJclass ScatObj{	public:	double *z;	short	numSame;	ScatObj *next;	void Init(double *theZ);	void CloseAll(long lastPtr);	void GetValues(double *theZ,short n,double ratio);	void Add(double *theZ,short p);};#endif