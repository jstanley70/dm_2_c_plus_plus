//m:Real Lattice:RL_ImageVector.c#include	"Diffract_INCs.h"#include	"a_LatticeObject.h"#include	"UT_VectorMacros.c"#include	"a_Lat_Image_Vector.h"/******   Image Vector   *********/void		ImageVector::DoInit(latticeVectorPtr v1, latticeVectorPtr v2, unsigned char* text){	x1 		= v1->x;	y1 		= v1->y;	z1 		= v1->z;	length1	= v1->length;		x2 		= v2->x;	y2		= v2->y;	z2 		= v2->z;	length2	= v2->length;		nextObj = (ImageVector*)NUL;	if(text[0] == (char)1){		label[0] = (char)1;		label[1] = text[1];		labelFlag = true;	} else {		labelFlag = false;	}}void	ImageVector::InsertCrystalMorphology(CrystalVerticePtr theVPtr,CrystalEdgePtr 	theEPtr,short count){		CrystalVertice 	theVert;	theVert = theVPtr[theEPtr[count].vert1];	x1 		= theVert.x;	y1 		= theVert.y;	z1 		= theVert.z;	length1 = sqrt(x1 * x1 + y1 * y1 + z1 * z1);	x1		/=	length1;	y1		/=	length1;	z1		/=	length1;		theVert = theVPtr[theEPtr[count].vert2];			x2 		= theVert.x;	y2		= theVert.y;	z2 		= theVert.z;	length2 = sqrt(x2 * x2 + y2 * y2 + z2 * z2);	x2		/=	length2;	y2		/=	length2;	z2		/=	length2;	labelFlag = false;	count++;	if(nextObj != 0L)	{		nextObj->InsertCrystalMorphology( theVPtr, 	theEPtr, count);		return;	}	nextObj = (ImageVector*)NUL;		}	void		ImageVector::InitNext(latticeVectorPtr v1, latticeVectorPtr v2, unsigned char* text){	if(nextObj == (ImageVector*)NUL){		nextObj = (ImageVector*)D_new(ImageVector);		nextObj->DoInit(v1,v2,text);	} else {		nextObj->InitNext(v1,v2,text);	}}void		ImageVector::DoClose(){	if(nextObj != (ImageVector*)NUL)		nextObj->DoClose();	D_delete(this);}void		ImageVector::DoRotate(double theXForm[][3]){	if(nextObj != (ImageVector*)NUL)		nextObj->DoRotate(theXForm);	TRANSFORM_VECTOR(x1,y1,z1,theXForm);	NORMALIZE_VECTOR(x1,y1,z1);	TRANSFORM_VECTOR(x2,y2,z2,theXForm);	NORMALIZE_VECTOR(x2,y2,z2);}		Boolean	ImageVector::SortByZ(ImageVector** previous){		if(nextObj == (ImageVector*)NUL){		return(false);	}			if((z1 * length1 + z2 * length2) >= (nextObj->z1 * nextObj->length1 + nextObj->z2 * nextObj->length2)){		return(nextObj->SortByZ(&nextObj));	}		*previous				= nextObj;	nextObj					= nextObj->nextObj;	(*previous)->nextObj 	= this;		return(true);}						void		ImageVector::DoDraw(lattice* theAtoms){	short	h,v,lineColor,lastH,lastV;	double	x,y,z,xDif,yDif,zDif,i;	Boolean	hidden;		if(nextObj != (ImageVector*)NUL)		nextObj->DoDraw(theAtoms);			xDif 	= x2 * length2 - x1 * length1;	yDif 	= y2 * length2 - y1 * length1;	zDif 	= z2 * length2 - z1 * length1;	hidden 	= true;		lineColor = MED_GREY;	if(z1 < 0.0){		lineColor -= 1;	} else {		lineColor += 1;	}	if(z2 < 0.0){		lineColor -= 1;	} else {		lineColor += 1;	}	if(g_Monitor){		gCurrentObj->DMForeColor/*PMForeColor*/(lineColor);	}else{		PenNormal();		PenPat(&qd.black);	}			lastH = imageCenter.h + (short)(.5 + y1 * scaling * length1);	lastV = imageCenter.v + (short)(.5 + x1 * scaling * length1);	MoveTo(lastH,lastV);			for(i = 0. ; i <= 1. ; i += .01){			x = x1 * length1 + i * xDif;		y = y1 * length1 + i * yDif;		z = z1 * length1 + i * zDif;				if(theAtoms->PointVisible(x,y,z)){			h = imageCenter.h + (short)(.5 + y * scaling);			v = imageCenter.v + (short)(.5 + x * scaling);			if(hidden){				double j;				j = i - .001;				while(j > 0. && theAtoms->PointVisible(x,y,z)){					x = x1 * length1 + j * xDif;					y = y1 * length1 + j * yDif;					z = z1 * length1 + j * zDif;					j -= .001;				}				h = imageCenter.h + (short)(.5 + y * scaling);				v = imageCenter.v + (short)(.5 + x * scaling);				MoveTo(h,v);				hidden = false;			}			lastH = h;			lastV = v;		} else {			if(!hidden){				h = imageCenter.h + (short)(.5 + y * scaling);				v = imageCenter.v + (short)(.5 + x * scaling);				LineTo(h,v);				hidden = true;			}					}	}	if(!hidden)		LineTo(lastH,lastV);	if(labelFlag)		DrawDiffractString((unsigned char*)label);}						void	ImageVector::SetScaling(double	scale){	if(nextObj != (ImageVector*)NUL)		nextObj->SetScaling(scale);	scaling = scale;}					void	ImageVector::SetCenter(Point	theCenter){	if(nextObj != (ImageVector*)NUL)		nextObj->SetCenter(theCenter);	imageCenter = theCenter;}void	ImageVector::OffsetPoints(latticeVectorPtr theOffset){	double	x,y,z,cx,cy,cz;		if(nextObj != (ImageVector*)NUL)		nextObj->OffsetPoints(theOffset);			cx = theOffset->x * theOffset->length;	cy = theOffset->y * theOffset->length;	cz = theOffset->z * theOffset->length;		x 						= x1 * length1 - cx;	y 						= y1 * length1 - cy;	z 						= z1 * length1 - cz;	length1					= sqrt(x * x + y * y + z * z);	x1						= x / length1;	y1						= y / length1;	z1						= z / length1;		x 						= x2 * length2 - cx;	y 						= y2 * length2 - cy;	z 						= z2 * length2 - cz;	length2					= sqrt(x * x + y * y + z * z);	x2						= x / length2;	y2						= y / length2;	z2						= z / length2;}			void	ImageVector::DoFileRead(void){	Boolean anotherOne;	x1 			= gTheFile->ReadFloat();	y1 			= gTheFile->ReadFloat();	z1 			= gTheFile->ReadFloat();	length1 	= gTheFile->ReadFloat();	x2 			= gTheFile->ReadFloat();	y2 			= gTheFile->ReadFloat();	z2 			= gTheFile->ReadFloat();	length2 	= gTheFile->ReadFloat();	scaling 	= gTheFile->ReadFloat();	labelFlag	= gTheFile->ReadBoolean();	gTheFile->ReadDataBlock((Ptr)label,2L);	gTheFile->ReadDataBlock((Ptr)&imageCenter,sizeof(Point));	anotherOne	=	gTheFile->ReadBoolean();		if(anotherOne){//this has been added		if(!nextObj){//this has been added			nextObj = (ImageVector*)D_new(ImageVector);			nextObj->nextObj = 0L;//this has been added		}		nextObj->DoFileRead();//this has been added	}}void	ImageVector::DoFileWrite(void){		gTheFile->WriteFloat((float)x1);	gTheFile->WriteFloat((float)y1);	gTheFile->WriteFloat((float)z1);	gTheFile->WriteFloat((float)length1);	gTheFile->WriteFloat((float)x2);	gTheFile->WriteFloat((float)y2);	gTheFile->WriteFloat((float)z2);	gTheFile->WriteFloat((float)length2);	gTheFile->WriteFloat((float)scaling);	gTheFile->WriteBoolean(labelFlag);	gTheFile->WriteDataBlock((Ptr)label,2L);	gTheFile->WriteDataBlock((Ptr)&imageCenter,sizeof(Point));	if(nextObj)gTheFile->WriteBoolean(true);//this has been added	else  gTheFile->WriteBoolean(false);//this has been added	if(nextObj != (ImageVector*)NUL)//this has been added		nextObj->DoFileWrite();//this has been added}void ImageVector::GetCellCenter(latticeVectorPtr  cellCenter1,latticeVectorPtr  cellCenter2){			if(x1 < cellCenter1->x)			cellCenter1->x = x1;		if(x2 < cellCenter1->x)			cellCenter1->x = x2;		if(y1 < cellCenter1->y)			cellCenter1->y = y1;		if(y2 < cellCenter1->y)			cellCenter1->y = y2;		if(z1 < cellCenter1->z)			cellCenter1->z = z1;		if(z2 < cellCenter1->z)			cellCenter1->z = z2;					if(x1 > cellCenter2->x)			cellCenter2->x = x1;		if(x2 > cellCenter2->x)			cellCenter2->x = x2;		if(y1 > cellCenter2->y)			cellCenter2->y = y1;		if(y2 > cellCenter2->y)			cellCenter2->y = y2;		if(z1 > cellCenter2->z)			cellCenter2->z = z1;		if(z2 > cellCenter2->z)			cellCenter2->z = z2;	if(nextObj != (ImageVector*)NUL){		nextObj->GetCellCenter(  cellCenter1,  cellCenter2);		return;	}else{		double x,y,z,length;		x = cellCenter2->x - cellCenter1->x;		y = cellCenter2->y - cellCenter1->y;		z = cellCenter2->z - cellCenter1->z;		length = sqrt(x * x + y * y + z * z);		if(length > 0){			cellCenter1->x = -x/length;			cellCenter1->y = -y/length;			cellCenter1->z = -z/length;			cellCenter1->length = length;		}else{			cellCenter1->x = 0;			cellCenter1->y = 0;			cellCenter1->z = 0;			cellCenter1->length = 1.;		}	}}void	ImageVector::GetAverageLength(double *x, double *y, double *z,short count){	*x += x1 * length1 + x2 * length2;	*y += y1 * length1 + y2 * length2;	*z += z1 * length1 + z2 * length2;	count++;	if(nextObj != (ImageVector*)NUL){		nextObj->GetAverageLength( x,  y, z, count);	}else{		*x /= (double)count * 2;		*y /= (double)count * 2;		*z /= (double)count * 2;	}}void	ImageVector::ShiftImageVectors(double x,double y,double z){	x1 		= x1 * length1 - x;	y1 		= y1 * length1 - y;	z1 		= z1 * length1 - z;	length1 	= sqrt(	x1 * x1 + y1 * y1 + z1 * z1);	if(length1 > .0000001){		x1			/= length1;		y1			/= length1;		z1			/= length1;	} else {		x1			= 0.0;		y1			= 0.0;		z1			= 0.0;		length1	= 0.0;	}		x2 		= x2 * length2 - x;	y2 		= y2 * length2 - y;	z2 		= z2 * length2 - z;	length2 	= sqrt(	x2 * x2 + y2 * y2 + z2 * z2);	if(length2 > .0000001){		x2			/= length2;		y2			/= length2;		z2			/= length2;	} else {		x2			= 0.0;		y2			= 0.0;		z2			= 0.0;		length2	= 0.0;	}	if(nextObj != (ImageVector*)NUL){		nextObj->ShiftImageVectors( x, y, z);	}}void	ImageVector::InitAll(short count,short lastCount){	if(++count == lastCount){		nextObj = (ImageVector*)NUL;		return;	}	nextObj = (ImageVector*)D_new(ImageVector);	nextObj->InitAll(count,lastCount);}