//a_LatticeAtoms.h#ifndef _LATTICE_ATOMS	#define	_LATTICE_ATOMS		#ifndef _LATTICEVECTOR		#define _LATTICEVECTOR		typedef struct	latticeVector{				double		x;				double		y;				double		z;				double		length;			}latticeVector,*latticeVectorPtr,**latticeVectorHandle;	#endif		typedef	struct LatticeAtom{				double			x;				double			y;				double			z;				double			length;				float			a;				float			b;				float			c;				Boolean			hidden;				Boolean			covered;				Boolean			sliced;				Boolean			labeled;				float			radius;				float			atomicRadius;				Rect			spotRect;				short			atNum;				short			color;				short			intensity;				short			h;				short			v;				}LatticeAtom,*LatticeAtomPtr,**LatticeAtomHandle;		class	lattice{	public:			LatticeAtomHandle	theLattice;			Ptr					ownerObj;			short				i;			/* for use in loops */			LatticeAtomPtr		theAtoms;	/* for use in loops */			latticeVector		cellX;			latticeVector		cellY;			latticeVector		cellZ;			double				centeringValues[3];			void				DoInit(Ptr);			void				DoAtomInit(AtomPtr thisAtom);			void				CenterAtoms(void);			void				DoClose(void);			void				DoRotate(double[][3]);			void				AdjustAtomicRadii(short elementNo,float radius);			void				SetSurface(void);			void				DoDraw(void);			void				DoRgnDraw(RgnHandle	theUpdateRgn,short,short,short,short);			void				SortByZ(void);			void				SetIntensity(void);			void				DupAndOffset(latticeVector*);					void				SetColor(Point);	/* sets the colors for the atoms */			void				HideAtom(Point);			void				HideTypeAtom(Point);			void				ShowAll(void);			void				UnitLattice(void);			void				OffsetThisAtom(LatticeAtomPtr,short);			void				SliceLattice(void);			void				UnSliceLattice(void);			Boolean				PointVisible(double,double,double);			void				ZoomAll(void);			void				GetPicSize(Rect*);			void				DoFileRead(void);			void				DoFileWrite(void);			short				SetAtomColor(short colorNum,short atNum,float *atomicRadius);			Boolean				TestAtom(LatticeAtomPtr theAtom,short axis,short comparison,double value);			Boolean 			DupAndOffsetToMorph(latticeVector* theOffset,short noVects[3]);			void				CalculateDisplacements(void);			void				CheckUnitCell(void);		};#endif