//m:Desktop.2.0:Crystal 2:JCPDFCrystal.h#ifndef _JCPDF_CRYSTAL#define _JCPDF_CRYSTAL#pragma def_inherited ontypedef struct KeySpaceGrpNBS{//CRYSTAL.S01	int 		offSet;	int			size;	char		spaceGrp[16];	/*int			entryNum;	char		extra;*/}SpaceGrpEntries;typedef struct KeyDMElement{	int 		offSet;	int			size;	long		elements[4];}KeyDMElement;typedef struct	KeyElementNBS{//CRYSTAL.S02	int 		offSet;	int			size;	char		element[8];	/*int			entryNum;	char		extra[2];*/}KeyElementNBS;typedef struct	KeyCODENNBS{//CRYSTAL.S03	int 		offSet;	int			size;	char		cODEN[12];	/*int			entryNum;	char		extra[2];*/}KeyCODENNBS;typedef struct	KeyVolumeNBS{//CRYSTAL.S04	int 		offSet;	int			size;	int			volume;	int			entryNum;}KeyVolumeNBS;typedef struct	KeyDensityNBS{//CRYSTAL.S05	int 		offSet;	int			size;	int			density;	int			entryNum;}KeyDensityNBS;typedef struct	KeySubFileNBS{//CRYSTAL.S06	int 		offSet;	int			size;	char		subFile[8];	/*int			entryNum;	char		extra[3];*/}KeySubFileNBS;typedef struct	KeyMineralNameNBS{//CRYSTAL.S07	int 		offSet;	int			size;	char		name[40];	int			entryNum;}KeyMineralNameNBS;typedef struct	KeyPearsonNBS{//CRYSTAL.S08	int 		offSet;	int			size;	char		name[16];	/*int			entryNum;	char		extra[3];*/}KeyPearsonNBS;typedef struct	KeyUnitCellNBS{//CRYSTAL.S09	int 		offSet;	int			size;	int			a;	int			entryNum;	int			b;	int			c;	short			alpha;	short			beta;	short			gamma;	short			spcGrp;}KeyUnitCellNBS;typedef struct	KeyCDIDNBS{//CRYSTAL.S10	int 		offSet;	int			size;	char		cDID[12];	/*int			entryNum;	char		extra;*/}KeyCDIDNBS;typedef struct	KeyOrgClassNBS{//CRYSTAL.S11	int 		offSet;	int			size;	int			orgClass;	int			entryNum;}KeyOrgClassNBS;typedef struct	KeyYearNBS{//CRYSTAL.S12	int 		offSet;	int			size;	int			year;	int			entryNum;}KeyYearNBS;typedef struct	KeyAuthorNBS{//CRYSTAL.S15	int 		offSet;	int			size;	char		author[20];	int			entryNum;}KeyAuthorNBS;typedef struct	KeyPDFNoJC{ // PDF2.S01	int 		offSet;	int			size;	char		pDFNum[20];	/*char		extra[14];*/}KeyPDFNoJC;typedef struct	KeySubFileJC{ // PDF2.S02	int 		offSet;	int			size;	char		subFile[16];	/*char		extra[13];*/}KeySubFileJC;typedef struct	KeyInOrgNameJC{  // PDF2.S03	int 		offSet;	int			size;	char		name[100];	char		extra[20];}KeyInOrgNameJC;typedef struct	KeyMineralNameJC{  // PDF2.S04	int 		offSet;	int			size;	char		name[40];	char		extra[20];}KeyMineralNameJC;typedef struct	KeyElementsOrgJC{  // PDF2.S05	int 		offSet;	int			size;	char		element[16];	//char		extra[11];}KeyElementsOrgJC;typedef struct	KeyElementsJC{  // PDF2.S06	int 		offSet;	int			size;	char		element[16];	//char		extra[14];}KeyElementsJC;typedef struct	KeyLinesJC{  // PDF2.S07	int 		offSet;	int			size;	short		line;	char		extra[16];	//12th bit intensity, 13-14 2nd strong line,15-16 remaining line}KeyLinesJC;typedef struct	KeyCASJC{  // PDF2.S08	int 		offSet;	int			size;	int			cASnum;	char		extra[12];}KeyCASJC;typedef struct	KeyOrgChemNameJC{  // PDF2.S09	int 		offSet;	int			size;	char		name[100];	char		extra[20];}KeyOrgChemNameJC;typedef struct	KeyInorgChemFragJC{  // PDF2.S010	int 		offSet;	int			size;	char		element[20];	char		extra[20];}KeyInorgChemFragJC;typedef struct	KeyMinCodesJC{  // PDF2.S011	int 		offSet;	int			size;	char		minCode[16];	//char		extra[10];}KeyMinCodesJC;typedef struct	KeyUnitCellJC{  // PDF2.S012	int 		offSet;	int			size;	int			a;	int			entryNum;	int			b;	int			c;	int			alpha;	int			beta;	int			gamma;	short		spcGrp;	char		extra[8];}KeyUnitCellJC;typedef struct	KeyAuthorJC{  // PDF2.S013	int 		offSet;	int			size;	char		author[20];	char		extra[12];}KeyAuthorJC;typedef struct	KeyYearJC{  // PDF2.S014	int 		offSet;	int			size;	char		year[12];	//char		extra[11];}KeyYearJC;typedef struct	KeyCODENJC{  // PDF2.S015	int 		offSet;	int			size;	char		cODEN[20];	//char		extra[12];}KeyCODENJC;typedef struct	KeyColorJC{  // PDF2.S16	int 		offSet;	int			size;	char		color[12];	//char		extra[11];}KeyColorJC;typedef struct	KeyDensityJC{  // PDF2.S017	int 		offSet;	int			size;	int			density;	char		extra[12];}KeyDensityJC;typedef struct	KeyVolumeJC{  // PDF2.S018	int 		offSet;	int			size;	int			volume;	char		extra[12];}KeyVolumeJC;typedef struct	KeyLargestLinesJC{  // PDF2.S019	int 		offSet;	int			size;	short		line;	char		extra[18];//12 intensity & order,13-14 strong line d-space,15-16 other d-space}KeyLargestLinesJC;class	JCPDFCrystal :public Crystal{public:	char	radiation;	char	source;	char	structureCode;	char	editorialCode;	char	leastSquares[2];	float	avgErr;	char	pdfNorID[7];	float	edCodeAveErr;	float	formUnits;	char	edCodeFormU;	long	qualityIndex;	char	*spGrpText;	char	edCodeSpGr;	char	aspectCode;	char	orCodeSpGr;	float	densityMeasured;	char	chemAbsRegistry[12];	char	materialClass;	char	cmpdResidue[6];	char	cmpdSubForm[6][4];	Boolean	addRegisterFlag;	char	*empiricalText;	char	refCode[2];	char	pearsonSym[2];	float	pearsonNum;	char	edCodPearson;	char*	structType;	char*	comments;	UnitCell crystalDataCell;	UnitCell standardDev;	short	reducedForm;	float	firstDetermRatio;	float	secDetermRatio;	float	density;	char	nameCode;	double	cMatrix[3][3];	short	numExpLines;	SpotInfo/*SpotInfoCrys*/		expDspacings[100];	short	dataBaseSource;	Boolean	DecipherJCPDFLine		(char *theText,short *lines);	void	OriginalCellParameters	(char *theText);	void	CellDeviations			(char *theText);	void	SpaceGroupLine			(char *theText);	void	CrystalData				(char *theText);	void	MaterialClass			(char *theText);	void	CompoundName			(char *theText,short *lines);	void	ChemicalFormulaJCPDF	(char *theText,short *lines);	void	EmpiricalFormula		(char *theText,short *lines);	void	LitReference			(char *theText,short *lines);	void	StructureType			(char *theText,short *lines);	void	Comments				(char *theText,short *lines);	void	MatrixInitialCell		(char *theText);	void	ReducedCell				(char *theText);	void	CrystalDataCell			(char *theText);	void	RadiationExact			(char *theText);	void	LinesPosAndHKL			(char *theText,short *lines);	void	Update					(char *theText);	void	ProcessingHistory		(char *theText,short *lines);	void	JustifyUCellToLatType	(UnitCell *theCell);	void	ReadUnitCell			(UnitCell *theUnitCell,char* theText,short length,short angle);	void	DrawCard				(void);	float	GetDataBaseValue		(char *theText,short numberChar);	char* 	ReadLongName			(char *theText,short maxChar,short *lines);	Boolean	InitCrystal				(short theValue);	void	DoClose					(void);	char* 	AtomsFromFormulaUnit	(char *formText);	void 	AtomsFromFormula		(short totalAtoms);	void 	GeneratePotentialAtomPos(void);	void 	CopyCrystalWDspacing	(Crystal *theCrystal);	Handle	SearchMinNameKeyJC		(short logic,long *totalFound);	Handle	SearchSubFileKeyJC		(short logic,long *totalFound);	Handle	SearchInorNameKeyJC		(short logic,long *totalFound);	Handle	SearchElementOrgKeyJC	(short logic,long *totalFound);	Handle	SearchCASKeyJC			(short logic,long *totalFound);	Handle	SearchOrgNameKeyJC		(short logic,long *totalFound);	Handle	SearchInorgChemFragKeyJC(short logic,long *totalFound);	Handle	SearchMinCodesKeyJC		(short logic,long *totalFound);	Handle	SearchUnitCellKeyJC		(short logic,long *totalFound);	Handle	SearchAuthorKeyJC		(short logic,long *totalFound);	Handle	SearchYearKeyJC			(short logic,long *totalFound);	Handle	SearchCODENKeyJC		(short logic,long *totalFound);	Handle	SearchColorKeyJC		(short logic,long *totalFound);	Handle	SearchDensityKeyJC		(short logic,long *totalFound);	Handle	SearchVolumeKeyJC		(short logic,long *totalFound);	Handle	SearchLinesKeyJC		(short logic,long *totalFound);		Handle	SearchElementKey		(short logic,long *totalFound,short dataBase,short allowed,char *theText2);	Handle	SearchCODENKey			(short logic,long *totalFound,short dataBase);	Handle	SearchVolumeKey			(short logic,long *totalFound,short dataBase);	Handle	SearchDensityKey		(short logic,long *totalFound,short dataBase);	Handle	SearchSubFileKey		(short logic,long *totalFound,short dataBase);	Handle	SearchNameKey			(short logic,long *totalFound,short dataBase);	Handle	SearchPearsonKey		(short logic,long *totalFound,short dataBase);	Handle	SearchUnitCellKey		(short logic,long *totalFound,short dataBase);	Handle	SearchCDIDKey			(short logic,long *totalFound,short dataBase);	Handle	SearchOrganicClassKey	(short logic,long *totalFound,short dataBase);	Handle	SearchYearKey			(short logic,long *totalFound,short dataBase);	Handle	SearchElementOrgKey		(short logic,long *totalFound,short dataBase);//	Handle	SearchAxialRatiosKey	(short logic,long *totalFound,short dataBase);	Handle	SearchAuthorKey			(short logic,long *totalFound,short dataBase);	Handle	SearchCASKey			(short logic,long *totalFound,short dataBase);	Handle 	SearchColorKeyNBS  (short logic,long *totalFound,short dataBase);	Handle	SearchSpaceGrpKey		(short logic,long *totalFound,short dataBase);		//void 	MakeElementKey(short dataBase,DialogPtr theDialog);	void 		EliminateBadCrys(DataBaseLoc *crys1,DataBaseLoc *crysNew,long *foundCrys);	PicHandle	DisplayCrystal(Rect	*theRect,short horiz,short vert);	void 		MakeElementKey(short dataBase,DialogPtr zDialog);	void 		MakeElementKeyJC(DialogPtr zDialog);};#endif