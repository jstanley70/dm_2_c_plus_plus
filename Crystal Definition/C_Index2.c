//C_Index2.c #include	"Diffract_INCs.h"#include	"ST_StereoMacros.c"#include	"UT_VectorMacros.c"#include	"SF_MathLib.h"short 		ConvertChar(char);short		ConvertLongChar(char *theText,short *strPos);void 		ConvertText(char *theText,short *strPos,short k);void				Index::WriteIndex(void){	gTheFile->WriteShort(h);	gTheFile->WriteShort(k);	gTheFile->WriteShort(l);	gTheFile->WriteBoolean(direction);	gTheFile->WriteFloat(x);	gTheFile->WriteFloat(y);	gTheFile->WriteFloat(z);	gTheFile->WriteFloat(intensity);	gTheFile->WriteFloat(angle);	gTheFile->WriteLong(flags);	gTheFile->WriteDataBlock((Ptr)&theSpotRect,sizeof(Rect));}short ConvertChar(char theChar){	short 	charVal;		charVal = (short)theChar;	if(charVal > 47 && charVal < 58)		return (charVal - 48);	switch (theChar){		case (char)0xE1:			return -9;			break;		case (char)0xF5:			return -8;			break;		case (char)0xE0:			return -7;			break;		case (char)0xDF:			return -6;			break;		case (char)0xDE:			return -5;			break;		case (char)0xDD:			return -4;			break;		case (char)0xDC:			return -3;			break;		case (char)0xDB:			return -2;			break;		case (char)0xDA:			return -1;			break;		default:			return 0;			break;	}}void ConvertText(char *theText,short *strPos,short k){	short j,l,i;	char theChar;	l = -1;	j = 1;	if(k != 0)	{		while(j > 0){			l++;			j = fabs(k / pow(10,(double)l));		}		l--;	}	else{		l = 0;	}	for(i = l; i >= 0;i--){		j = k / pow(10,(double)i);		switch(j){			/* switch on index which will   */			case -9:			/* be in the range -9 to 9. and */				theChar = ((char)0xE1);	/* return the appropriate char.	*/				break;			case -8:			/* These chars are correct for  */				theChar = ((char)0xF5);	/* Genoa font.					*/				break;			case -7:				theChar = ((char)0xE0);				break;				case -6:				theChar = ((char)0xDF);				break;			case -5:				theChar = ((char)0xDE);				break;			case -4:				theChar = ((char)0xDD);				break;			case -3:				theChar = ((char)0xDC);				break;			case -2:				theChar = ((char)0xDB);				break;			case -1:				theChar = ((char)0xDA);				break;			case 0:				theChar = ('0');				break;			case 1:				theChar = ('1');				break;			case 2:				theChar = ('2');				break;			case 3:				theChar = ('3');				break;			case 4:				theChar = ('4');				break;			case 5:				theChar = ('5');				break;			case 6:				theChar = ('6');				break;			case 7:				theChar = ('7');				break;			case 8:				theChar = ('8');				break;			case 9:				theChar = ('9');				break;			default:			/* switch on index which will   */				theChar = ('*');	/* return * if |index | > 9		*/		}		k -= ((double)j * pow(10,(double)l));		theText[(*strPos)++] = theChar;	}	theText[(*strPos)++] = (' ');}short Reduce(double* U,double* V,double* W){	short u,v,w,i,test;	Boolean flag = false;	short	theValue = 1;	u = (long)*U;	v = (long)*V;	w = (long)*W;	if(u == 0 && v == 0 && w == 0)	{		*W = 1;		return theValue;	}	test = 15;	if(fabs((double)u) > fabs((double)v) && v != 0)		test = abs(v);	else if(u != 0)		test = abs(u);	if(fabs((double)test) > fabs((double)w) && w != 0)		test = abs(w);		for (i = test ; i >= 2.0; i--){		if(u % i)			continue;		if(v % i)			continue;		if(w % i)			continue;		u /= i;		v /= i;		w /= i;		flag = true;		theValue *= i;		i++;	}	if(flag){		*U = (double)u;		*V = (double)v;		*W = (double)w;	}	return theValue;}double Index::IndexStructureFactor(double d,double electronMassRation,short theBeamFlag,dcomplex *cAb){	long i;	short element = -21131;	Atom  atomicPosition,*theAtoms;	double *theUs,*theVs,*theWs,*theZs,*theBs,theResult;	double dH,dK,dL;	long num;	num = theCrystal->atom_Count + 1;		theUs = (double*)D_NewPtr(5L * num * sizeof(double));	if(!PtrIsOK((Ptr)theUs))		return 0.0;	theVs = &theUs[num];	theWs = &theVs[num]; 	theZs = &theWs[num];	theBs = &theZs[num];	D_HLock((Handle)theCrystal->theAtoms);	theAtoms = 	*theCrystal->theAtoms;			for(i = 0; i < num; i++)	{		atomicPosition = theAtoms[i];		theUs[i] = atomicPosition.x;		theVs[i] = atomicPosition.y;		theWs[i] = atomicPosition.z;		theBs[i] = atomicPosition.bi;	}	theCrystal->AtomicScatteringByD( theBeamFlag,d,electronMassRation,theZs);	dH = (double)h;	dK = (double)k;	dL = (double)l;		D_HUnlock((Handle)theCrystal->theAtoms);		{			//intensity = theResult = StructureFactorOld(dH,dK,dL,theUs,theVs,theWs,num,theZs);		sF = TotalStructureFactor(dH,dK,dL,theUs,theVs,theWs,num,theZs,theBs,theCrystal->volume,1/d,theCrystal,cAb);		intensity = theResult = Cabs(sF);		KillPtr((Ptr)theUs);			}	return theResult;}void Index::OrderIndicies(short *H,short *K,short *L){	double u[3],holdU;		*H =  u[0] = fabs((double)h);	*K =  u[1] = fabs((double)k);	*L =  u[2] = fabs((double)l);		if(direction){		if((theCrystal->lattice_Type == HEXAGONAL && g_Hex_Four)  ||			 (theCrystal->lattice_Type == TRIGONAL && g_Hex_Four))		{			*H = u[0] = fabs((double)((2 * h) - k));			*K = u[1] = fabs((double)((2 * k) - h));			*L = u[2] = fabs((double)(l * 3));		}	}	if(theCrystal->lattice_Type == ORTHORHOMBIC ||theCrystal->lattice_Type == MONOCLINIC 		|| theCrystal->lattice_Type == TRICLINIC) return;	if(u[0] < u[1]){		holdU = u[1];		u[1] = u[0];		u[0] = holdU;	}	if(theCrystal->lattice_Type == TETRAGONAL || theCrystal->lattice_Type == HEXAGONAL || theCrystal->lattice_Type == TRIGONAL) goto FINISHED;	if(u[1] < u[2]){		holdU = u[2];		u[2] = u[1];		u[1] = holdU;		if(u[0] < u[1]){			holdU = u[1];			u[1] = u[0];			u[0] = holdU;		}	}FINISHED:	*H = (short)u[0];	*K = (short)u[1];	*L = (short)u[2];}double Index::GetAngleBetweenDirections(Index *theI){	double a,b,c,scale,alpha,beta,gamma,L,I1,I2;		a = theCrystal->theUnitCell.a;	b = theCrystal->theUnitCell.b;	c = theCrystal->theUnitCell.c;	scale = PI / 180.;	alpha = cos(theCrystal->theUnitCell.alpha * scale);	beta = cos(theCrystal->theUnitCell.beta * scale);	gamma = cos(theCrystal->theUnitCell.gamma * scale);	L = (a * a * h * theI->h) + (b * b * k * theI->k) + (c * c * l * theI->l);	L += b * c * (k * theI->l + l * theI->k) * alpha;	L += a * c * (l * theI->h + h * theI->l) * beta;	L += b * a * (h * theI->k + k * theI->h) * gamma;	I1 = (a * a * h * h) + (b * b * k * k) + (c * c * l * l);	I1 += (2 * b * c * k * l ) * alpha;	I1 += (2 * a * c * l * h) * beta;	I1 += (2 * b * a * h * k) * gamma;		I2 = (a * a * theI->h * theI->h) + (b * b * theI->k * theI->k) + (c * c * theI->l * theI->l);	I2 += (2 * b * c * theI->k * theI->l ) * alpha;	I2 += (2 * a * c * theI->l * theI->h) * beta;	I2 += (2 * b * a * theI->h * theI->k) * gamma;	gamma = L / (sqrt(I1) * sqrt(I2));	if (fabs(gamma) > 1){		if(gamma > 0) gamma = 1;else gamma = -1;	}	return  acos(gamma) / scale;	}double Index::GetTheAngle(Index *theI){		long double 	dSpacing1,dSpacing2,angle,p;	long double 	angleCos;	double 		U,V,W;	Index 			*theI1,*theI2;		p = angle = angleCos = 0;	if(this->theCrystal != theI->theCrystal){		return GetAngleBetDifCrystals(theI);	}	if(direction && theI->direction)		return GetAngleBetweenDirections(theI);	theI1 = (Index*)D_new(Index);	theI2 = (Index*)D_new(Index);		this->DoCopy(theI1);	theI->DoCopy(theI2);		U = theI1->h;	V = theI1->k;	W = theI1->l;	if(theI1->direction)  U *= 10;V *= 10;W *= 10;	PlaneNormalOfDir(&U,&V,&W);		theI1->h = theI1->x = (float)U ;	theI1->k = theI1->y = (float)V ;	theI1->l = theI1->z = (float)W ;		U = theI2->h;	V = theI2->k;	W = theI2->l;	if(theI2->direction)  U *= 10;V *= 10;W *= 10;	theI->PlaneNormalOfDir(&U,&V,&W);		theI2->h = theI2->x = (float)U ;	theI2->k = theI2->y = (float)V ;	theI2->l = theI2->z = (float)W ;		theI1->direction = false;	theI2->direction = false;		dSpacing1 = 	theI1->TheDSpacingDouble()/ theCrystal->volume;	dSpacing2 = 	theI2->TheDSpacingDouble()/ theCrystal->volume;	angle	=		(180. / PI);	angleCos = ((theCrystal->sVals[0] * theI1->x * theI2->x +			theCrystal->sVals[1] * theI1->y * theI2->y +			theCrystal->sVals[2] * theI1->z * theI2->z +			theCrystal->sVals[3] * (theI1->x * theI2->y + theI2->x * theI1->y) +			theCrystal->sVals[4] * (theI1->y * theI2->z + theI2->y * theI1->z) +			theCrystal->sVals[5] * (theI1->z * theI2->x + theI2->z * theI1->x)) *			dSpacing1 * dSpacing2);		D_delete(theI1);	D_delete(theI2);	if(angleCos + 1 < .00000001) return 180;	p = (long double)acos(angleCos);		angle  *= p;		return angle;}Boolean Index::CompareTo(short H,short K,short L)	{			Index 	*testIndex;		long 	theSum,theSum1;		short	h1,k1,l1,t1,stub;		if(h == H && k == K && l == L)			return true;		testIndex = (Index*)D_new(Index);		testIndex->theCrystal = this->theCrystal;		testIndex->direction = direction;		testIndex->h = H;		testIndex->k = K;		testIndex->l = L;				OrderIndicies(&h1,&k1,&l1);		if((theCrystal->lattice_Type == HEXAGONAL && g_Hex_Four)  ||			 (theCrystal->lattice_Type == TRIGONAL && g_Hex_Four))		{				if(direction){					H = (double)((2 * h) - k);					K = (double)((2 * k) - h);					L = (double)(l * 3);			}			t1 = abs(H + K);						if(t1 > h1){				stub = h1;				h1 = t1;				t1 = stub;			}			if(t1 > k1){				stub = k1;				k1 = t1;				t1 = stub;			} 		}		theSum = h1;		theSum += 100 * (long)k1;		theSum += 10000 * (long)l1;		if((theCrystal->lattice_Type == HEXAGONAL && g_Hex_Four)  ||			 (theCrystal->lattice_Type == TRIGONAL && g_Hex_Four))				theSum += 1000000 * (long)t1;		testIndex->OrderIndicies(&h1,&k1,&l1);		if((theCrystal->lattice_Type == HEXAGONAL && g_Hex_Four)  ||			 (theCrystal->lattice_Type == TRIGONAL && g_Hex_Four))		{			if(direction){					H = (double)((2 * testIndex->h) - testIndex->k);					K = (double)((2 * testIndex->k) - testIndex->h);					L = (double)(testIndex->l * 3);			}			t1 = abs(H + K);			if(t1 > h1){				stub = h1;				h1 = t1;				t1 = stub;			}			if(t1 > k1){				stub = k1;				k1 = t1;				t1 = stub;			}		}		theSum1 = h1;		theSum1 += 100 * (long)k1;		theSum1 += 10000 * (long)l1;		if((theCrystal->lattice_Type == HEXAGONAL && g_Hex_Four)  ||			 (theCrystal->lattice_Type == TRIGONAL && g_Hex_Four))				theSum1 += 1000000 * (long)t1;		D_delete(testIndex);		if(theSum == theSum1)			return true;		return false;}long Index::MaskValue(long theMask){	long theValue = 0L;	short i = 0;	long	test_Mask = 1;	theValue = flags & theMask;	if(!theValue) return 0L;	while(!(test_Mask & theValue))  theValue >> 1;	return theValue;}Boolean Index::ZoneAxis(Index *theIndex,Index *theZoneAxis){	double 	x1,y1,z1,x2,y2,z2;	double	theXForm[3][3],u1,u2,v1,v2,w1,w2;	short 	h1,k1,l1,h2,k2,l2;	h1 = h;	k1 = k;	l1 = l;	h2 = theIndex->h;	k2 = theIndex->k;	l2 = theIndex->l;	if(theIndex->direction || direction && theCrystal->idNum != theIndex->theCrystal->idNum) return false;	if(theCrystal->idNum != theIndex->theCrystal->idNum){				DirOfPlaneNormal(&u1,&v1,&w1);		gCurrentObj->GetPPTXFormMatrixBase(theXForm,theCrystal,0);		TransFormVector(&u1,&v1,&w1,theXForm);	    NormalizeVector(&u1,&v1,&w1);	    Identity_Matrix(theXForm);	    x = u1;	    y = v1;	    z = w1;	    GetTheUVWFromXYZ(theXForm);	    ConvertXYZToHKLReduce();		if(direction == false) reduceFlag = false;		theIndex->DirOfPlaneNormal(&u2,&v2,&w2);		gCurrentObj->GetPPTXFormMatrixBase(theXForm,theIndex->theCrystal,0);		TransFormVector(&u2,&v2,&w2,theXForm);	    NormalizeVector(&u2,&v2,&w2);	    Identity_Matrix(theXForm);	    theIndex->x = u2;	    theIndex->y = v2;	    theIndex->z = w2;	    theIndex->GetTheUVWFromXYZ(theXForm);		theIndex->ConvertXYZToHKLReduce();		if(theIndex->direction == false) theIndex->reduceFlag = false;	}		x1 = (double)h;	y1 = (double)k;	z1 = (double)l;	x2 = (double)theIndex->h;	y2 = (double)theIndex->k;	z2 = (double)theIndex->l;	CrossProduct(&u2,&v2,&w2,x1,y1,z1,x2,y2,z2);	theZoneAxis->h = (short)u2;	theZoneAxis->k = (short)v2;	theZoneAxis->l = (short)w2;	theZoneAxis->direction = true;	theZoneAxis->reduceFlag = true;	h = h1;	k = k1;	l = l1;	theIndex->h = h2;	theIndex->k = k2;	theIndex->l = l2;	theZoneAxis->ReduceIndex();	return true;}void Index::ClearFlag(long theMask){	flags &= ~theMask;}void Index::SetFlag(long theMask){	flags |= theMask;}void Index::SetORFlag(short oRNum){	flags |= ((oRNum + 1) * 8);}double Index::GetAngleBetDifCrystals(Index *theI){	double 		crystalvector[3][3],				xForm[3][3],				xFormMatrix[3][3],				cosTheta;					Crystal 	*baseCrystal;	double 		x,y,z,x1,y1,z1,a,b,c;	double 		chx,ckx,clx,				chy,cky,cly,				chz,ckz,clz;					PPT_Info 	*pptOR;			baseCrystal = gCurrentObj->theCrystal[0];	baseCrystal->Get001XForm(xForm);	a = (double)0;	b = (double)0;	c = (double)1;		TRANSFORM_VECTOR(a,b,c,xForm);	FIND_CENTER_XFORM(a,b,c,xFormMatrix);		if(theCrystal->idNum > 0){				pptOR = gCurrentObj->the_PPT_Info[theCrystal->idNum - 1];		 gCurrentObj->GetPPTXFormMatrixBase(xForm,theCrystal,0);	}else{		theCrystal->Get001XForm(xForm);	}	MatrixMultiply(xForm,xFormMatrix,crystalvector); 	ConverttoReciprocal(crystalvector,(double)theCrystal->volume,false);	chx = crystalvector[0][0];	chy = crystalvector[1][0];	chz = crystalvector[2][0];	ckx = crystalvector[0][1];	cky = crystalvector[1][1];	ckz = crystalvector[2][1];	clx = crystalvector[0][2];	cly = crystalvector[1][2];	clz = crystalvector[2][2];			x = h * chx + k * ckx + l * clx ;	y = h * chy + k * cky + l * cly;	z = h * chz + k * ckz + l * clz;	baseCrystal->Get001XForm(xForm);	a = (double)0;	b = (double)0;	c = (double)1;	TRANSFORM_VECTOR(a,b,c,xForm);	FIND_CENTER_XFORM(a,b,c,xFormMatrix);	if(theI->theCrystal->idNum > 0){				gCurrentObj->GetPPTXFormMatrixBase(xForm,theI->theCrystal,0);	}else{		theI->theCrystal->Get001XForm(xForm);	}	MatrixMultiply(xForm,xFormMatrix,crystalvector); 	ConverttoReciprocal(crystalvector,(double)theI->theCrystal->volume,false);	chx = crystalvector[0][0];	chy = crystalvector[1][0];	chz = crystalvector[2][0];	ckx = crystalvector[0][1];	cky = crystalvector[1][1];	ckz = crystalvector[2][1];	clx = crystalvector[0][2];	cly = crystalvector[1][2];	clz = crystalvector[2][2];			x1 = theI->h * chx + theI->k * ckx + theI->l * clx ;	y1 =theI->h * chy + theI->k * cky + theI->l * cly;	z1 = theI->h * chz + theI->k * ckz + theI->l * clz;	cosTheta = (x * x1 + y * y1 + z * z1);	cosTheta /= ( sqrt((x * x + y * y + z * z) * (x1 * x1 + y1 * y1 + z1 * z1)));	return 180 * acos(cosTheta) / PI;}double Index::GetLength(void){		return   sqrt(x * x + y * y + z * z);}double Index::GetAngleBetweenProjections(Index * aIndex){	double L1,L2,cosine;	 L1 = sqrt(x * x + y * y);	 L2 = sqrt(aIndex->x * aIndex->x + aIndex->y * aIndex->y);	 return cosine = (x * aIndex->x + y * aIndex->y)/ (L1 * L2);	}double Index::GetTheXYZAngle(Index *aIndex){	double L1,L2,cosine;	L1 = GetLength();	L2 = aIndex->GetLength();	cosine = (x * aIndex->x + y * aIndex->y + z * aIndex->z)/ (L1 * L2);	if(cosine > 1)cosine = 1;	if(cosine < -1 ) cosine = -1;	return acos(cosine);}void Index::GetXYZ(double theXForm[][3]){	x = (double)h;	y = (double)k;	z = (double)l;	TRANSFORM_VECTOR(x,y,z,theXForm);	return;}void Index::GetXYZAndMultByIntensity(double theXForm[][3]){	GetXYZ(theXForm);	x *= intensity;	y *= intensity;	z *= intensity;	return;}void				Index::DirOfPlaneNormalXYZ(double *U,double *V,double *W){	double	*sVals;		if(direction){				return;	}	x = *U;	y = *V;	z = *W;	sVals = theCrystal->sVals;	*U = x * sVals[0] + y * sVals[3] + z * sVals[5];	*V = x * sVals[3] + y * sVals[1] + z * sVals[4];	*W = x * sVals[5] + y * sVals[4] + z * sVals[2];		*U /= theCrystal->volume;	*V /= theCrystal->volume;	*W /= theCrystal->volume;		}void Index::ConvertXYZToHKLReduce(void){	double			j;	Boolean			flag;	double			zz;	short			n;	NormalizeToGreatestXYZ();		flag = true;	j = 1;	while(flag && j < 100){				zz = j * x;		n = Round(zz);		if(fabs(zz - n) > .001){			j++;			continue;		}				zz = j * y;		n = Round(zz);		if(fabs(zz - n) > .001){			j++;			continue;		}				zz = j * z;		n = Round(zz);		if(fabs(zz - n) > .001){			j++;			continue;		}		flag = false;	}	h = Round(j * x);	k = Round(j * y);	l = Round(j * z);	reduceFlag = true;	ReduceIndex();	reduceFlag = false;}void Index::NormalizeToGreatestXYZ(void){	double normalize;		if(fabs(x) > fabs(y))normalize = fabs(x);	else normalize = fabs(y);		if(fabs(z) > normalize)normalize = fabs(z);		if(normalize < .005)normalize = 1;		x /= normalize;	y /= normalize;	z /= normalize;}void Index::IndexToSimpleText(char *theText){		short			H = 0,K = 0,T = 0,L = 1,i;	double			u,v,w;		i = 1;	u = (double)h;	v = (double)k;	w = (double)l;		while(fabs(u) > 100 || fabs(v) > 100 || fabs(w) > 100)	{		u /= 10;		v /= 10;		w /= 10;	} 	h = Round(u);	k = Round(v);	l = Round(w);	if((theCrystal->lattice_Type == HEXAGONAL && g_Hex_Four) || (theCrystal->lattice_Type == TRIGONAL && g_Hex_Four)){		if(direction){			H = u = (double)((2 * h) - k);			K = v = (double)((2 * k) - h);			L = w = (double)(l * 3);			if(u == 0. && v == 0. && w == 0)				w = 1.;						if(reduceFlag)				Reduce(&u,&v,&w);			else{				if(!(H % 3) && !(K % 3) && !(L % 3)){					u /= 3;					v /= 3;					w /= 3;				}							}			H = u;			K = v;			T = -(H + K);			L = w;						sprintf(theText,"[%d,%d,%d,%d]",H,K,T,L);		} else {			if(reduceFlag){				u = h;				v = k;				w = l;				Reduce(&u,&v,&w);				H = u;				K = v;				L = w;				T = - (H + K);				sprintf(theText,"[%d,%d,%d,%d]",H,K,T,L);			}else{								T = - (h + k);				sprintf(theText,"(%d,%d,%d,%d)",h,k,T,l);			}		}	} else {			if(reduceFlag){				u = h;				v = k;				w = l;				Reduce(&u,&v,&w);				H = u;				K = v;				L = w;			}else{				H = h;				K = k;				L = l;			}			if(direction)				sprintf(theText,"[%d,%d,%d]",H,K,L);			else				sprintf(theText,"(%d,%d,%d)",H,K,L);	}}void Index::GetTheUVWFromXYZ(double theXForm[][3])//this takes an xyz coordinate, eliminates or adds//an orientation then converts it to a direction or plane depending on the direction flag.{	static Point	lastPoint;	double			radConvert;	double			matrix[3][3];			radConvert = PI / 180.0;		TRANSFORM_VECTOR(x,y,z,theXForm);//eliminates or reorients xyz	theCrystal->Get001XForm(matrix);	InvertMatrix(matrix);	TRANSFORM_VECTOR(x,y,z,matrix);		if(direction){//if its supposed to be a plane we want to convert the direction to a plane		double H,K,L;		H = x;		K = y;		L = z;		direction = false;		DirOfPlaneNormalXYZ( &H, &K, &L);		//PlaneNormalOfDir(&H, &K, &L);//this takes a direction and converts it to plane normal		x = H;		y = K;		z = L;		direction = true;	}	}Boolean		Index::ContainedIn(Index *theI){	short theValue;	if(direction && !theI->direction || direction && !theI->direction)	{	  theValue = theI->h * h + theI->k * k + theI->l * l;	  if(!theValue) return true;	  else return false;	}else return false;}double	Index::Periodicity(short order,double cameraLength,double wavelength,double *diameter){	Index *theIndex;	double angle,radius,firstOrderDist,thePeriod;	short	theOrder = 0,theOrder2;	Boolean	oldReduceFlag;		theIndex 		= 	(Index*)D_new(Index);;	theIndex		->	DoInit(false,theCrystal);	theIndex		->	direction = false;	theIndex		->	reduceFlag = false;	oldReduceFlag 	= 	reduceFlag;	reduceFlag 		= 	true;	theOrder2 		= 	ReduceIndex();	reduceFlag 		= 	oldReduceFlag;		for(theIndex->h = 	-2;theIndex->h <= 3;theIndex->h++){	for(theIndex->k = 	-2;theIndex->k <= 3;theIndex->k++){		for(theIndex->l = -2;theIndex->l <= 3;theIndex->l++){			theOrder = h * theIndex->h + k * theIndex->k + l * theIndex->l;			if(theOrder != 0){				goto FOUND;			}		}	}	}	goto NOTFOUND;FOUND: 	angle 			= 	GetTheAngle(theIndex);	firstOrderDist 	= 	cos(angle * PI / 180)  / (theIndex->TheDSpacing() * theOrder/theOrder2);	radius  		= 	1./ wavelength;	angle 			= 	acos(((radius - fabs(firstOrderDist)) / radius));	*diameter 		= 	2 * cameraLength * tan(acos(1 - fabs((order * wavelength *  firstOrderDist))));	theIndex		->	DoClose();	thePeriod		=	1./firstOrderDist;	return fabs(thePeriod);NOTFOUND:	*diameter = 0;	theIndex		->	DoClose();	return 0.0;}