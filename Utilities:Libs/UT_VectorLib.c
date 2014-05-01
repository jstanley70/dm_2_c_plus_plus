//ects:D.M. v2.0:Sources.Jim:Utilities/Libs:UT_VectorLib.c #include	"Diffract_INCs.h"#include	"UT_VectorMacros.c"void InterchangeRows(dcomplex *a,dcomplex *b,short nCol);#define SWAP(a,b) {t=(a);(a)=(b);(b)=t;}void MatrixMultiply(double A[][3],double B[][3],double C[][3]){	register double b0,b1,b2,a0,a1;	//double b0,b1,b2,a0,a1;	double D[3][3];		a0 = A[0][0];	a1 = A[0][1];	b0 = B[0][0];	b1 = B[0][1];	b2 = B[0][2];	D[0][0] = a0 * b0 + A[1][0] * b1 + A[2][0] * b2;		D[0][1] = a1 * b0 + A[1][1] * b1 + A[2][1] * b2;	D[0][2] = A[0][2] * b0 + A[1][2] * b1 + A[2][2] * b2;		b0 = B[1][0];	b1 = B[1][1];	b2 = B[1][2];	D[1][0] = a0 * b0 + A[1][0] * b1 + A[2][0] * b2;	D[1][1] = a1 * b0 + A[1][1] * b1 + A[2][1] * b2;	D[1][2] = A[0][2] * b0 + A[1][2] * b1 + A[2][2] * b2;		b0 = B[2][0];	b1 = B[2][1];	b2 = B[2][2];	D[2][0] = a0 * b0 + A[1][0] * b1 + A[2][0] * b2;	D[2][1] = a1 * b0 + A[1][1] * b1 + A[2][1] * b2;	D[2][2] = A[0][2] * b0 + A[1][2] * b1 + A[2][2] * b2;		C[0][0] = D[0][0];	C[0][1] = D[0][1];	C[0][2] = D[0][2];	C[1][0] = D[1][0];	C[1][1] = D[1][1];	C[1][2] = D[1][2];	C[2][0] = D[2][0];	C[2][1] = D[2][1];	C[2][2] = D[2][2];}void TransFormVector(double* a,double* b,double* c,double xForm[][3]){	register double x,y,z;	//double x,y,z;	x = *a;	y = *b;	z = *c;	*a = x * xForm[0][0] + y * xForm[0][1] + z * xForm[0][2];	*b = x * xForm[1][0] + y * xForm[1][1] + z * xForm[1][2];	*c = x * xForm[2][0] + y * xForm[2][1] + z * xForm[2][2];}void TransFormVectorFl(float* a,float* b,float* c,double xForm[][3]){	double x,y,z;		x = *a;	y = *b;	z = *c;	 TransFormVector(&x,&y,&z,xForm);	*a = x;	*b = y;	*c = z;}void ReverseTransFormVector(double* a,double* b,double* c,double xForm[][3]){	register double x,y,z;	// double x,y,z;	x = *a;	y = *b;	z = *c;		*a = x * xForm[0][0] + y * xForm[1][0] + z * xForm[2][0];	*b = x * xForm[0][1] + y * xForm[1][1] + z * xForm[2][1];	*c = x * xForm[0][2] + y * xForm[1][2] + z * xForm[2][2];}void FindCenterXform(double x,double y,double z,double xForm[][3]){	FIND_CENTER_XFORM(x,y,z,xForm);		return;}void FindVertXform(double x,double y,double xForm[][3]){	FIND_VERT_XFORM(x,y,xForm);	return;}void FindXAxisXform(double x,double y,double xForm[][3]){	double r_M;																								r_M 		= sqrt(x * x + y * y);						Identity_Matrix(xForm);													if(r_M == 0.){												return;												}			xForm[2][2] = xForm[1][1] = y / r_M;					xForm[1][2] = -x / r_M;									xForm[2][1] = -xForm[1][2];								return;}														void FindYAxisXform(double x,double y,double xForm[][3]){	double r_M;																								r_M 		= sqrt(x * x + y * y);						Identity_Matrix(xForm);														if(r_M == 0.){												return;												}			xForm[2][2] = xForm[0][0] = y / r_M;					xForm[2][0] = -x / r_M;									xForm[0][2] = -xForm[2][0];				return;}														void FindRotateAboutPoleXform(double xx,double yy,double zz,double x,double y,double xForm[][3],double restoreM[][3]){	double horTilt[3][3];	FindCenterXform			(xx,yy,zz,restoreM);		FindHorizXform			(y , x, horTilt);	MatrixMultiply			(restoreM,horTilt,xForm);	InvertMatrix(restoreM);	return;}void FindHorizXform(double x,double y,double xForm[][3]){	FIND_HORIZ_XFORM(x,y,xForm);	return;}														void NormalizeMatrixRows(double matrix[][3]){	short i,j;	double length;	for (i = 0 ; i <= 2 ; i++){		length = 0.0;		for(j = 0 ; j <= 2 ; j++){			length += (matrix[i][j] * matrix[i][j]);		}		length = sqrt(length);		for(j = 0 ; j <= 2 ; j++)			matrix[i][j] /= length;	}}void NormalizeMatrixColumn (double matrix[][3]){	short i,j;	double length;	for (i = 0 ; i <= 2 ; i++){		length = 0.0;		for(j = 0 ; j <= 2 ; j++){			length += (matrix[j][i] * matrix[j][i]);		}		length = sqrt(length);		for(j = 0 ; j <= 2 ; j++)			matrix[j][i] /= length;	}}void InvertMatrix(double theMatrix[][3]){	//register double x;	double x;	if(fabs(theMatrix[0][1]) < 1e-20)		theMatrix[0][1] = 0;	x = theMatrix[0][1];	if(fabs(theMatrix[1][0]) < 1e-20)		theMatrix[1][0] = 0;			theMatrix[0][1] = theMatrix[1][0];	theMatrix[1][0] = x;		if(fabs(theMatrix[0][2]) < 1e-20)		theMatrix[0][2] = 0;	x = theMatrix[0][2];	if(fabs(theMatrix[2][0]) < 1e-20)		theMatrix[2][0] = 0;	theMatrix[0][2] = theMatrix[2][0];	theMatrix[2][0] = x;		if(fabs(theMatrix[1][2]) < 1e-20)		theMatrix[1][2] = 0;	x = theMatrix[1][2];	if(fabs(theMatrix[2][1]) < 1e-20)		theMatrix[2][1] = 0;	theMatrix[1][2] = theMatrix[2][1];	theMatrix[2][1] = x;}void NormalizeVector(double *x,double *y,double *z){	double length;	length = sqrt( *x * *x + *y * *y + *z * *z);	*x /= length;	*y /= length;	*z /= length;}void NegMatrix(double  XForm[][3]){	double UT[3][3];		UT[0][0] = UT[1][1] = UT[2][2] = -1.0;	UT[0][1] = UT[0][2] = UT[1][0] = UT[1][2] = UT[2][0] = UT[2][1] = 0.0;	MATRIX_MULTIPLY(XForm,UT,XForm);}void CrossProduct(double *x,double *y,double *z,double x1,double y1,double z1,double x2,double y2,double z2){	*x = (y1 * z2 - y2 * z1);	*y = -(x1 * z2 - x2 * z1);	*z = (x1 * y2 - x2 * y1);}	void 		VectorCrossProd(Vector v1,Vector v2,Vector *v3){	double vector1[3],vector2[3],vector3[3];	vector1[0] = v1.x; vector1[1] = v1.y; vector1[2] = v1.z;	vector2[0] = v2.x; vector2[1] = v2.y; vector2[2] = v2.z;		crossprod(vector1,vector2,vector3);	v3->x = vector3[0]; v3->y = vector3[1]; v3->z = vector3[2];}void crossprod(double *vector1,double *vector2,double *vector3){	vector3[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];	vector3[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];	vector3[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2];}double Determinate3x3Matrix(double *matrix){	double determ,determ1,determ2;	determ = matrix[0] * ( matrix[4] * matrix[8] - matrix[5] * matrix[7]);	determ1 = matrix[1] * ( -matrix[3] * matrix[8] + matrix[5] * matrix[6]);	determ2 = matrix[2] * ( matrix[3] * matrix[7] - matrix[4] * matrix[6]);	return (determ + determ1 + determ2);	/*	determ =  matrix[0] *  matrix[4] *  matrix[8];	determ += matrix[2] *  matrix[3] *  matrix[7];	determ += matrix[1] *  matrix[5] *  matrix[6];	determ -= matrix[2] *  matrix[4] *  matrix[6];	determ -= matrix[1] *  matrix[3] *  matrix[8];	determ -= matrix[0] *  matrix[5] *  matrix[7];	return determ;	*/}double VectorMag(Vector A){	return sqrt(A.x * A.x + A.y * A.y + A.z * A.z);}double VectorDot(Vector A,Vector B){	return A.x * B.x + A.y * B.y + A.z * B.z;}short VectorsEqual(Vector A,Vector B){	if(A.x - B.x < .001 && A.y - B.y < .001 && A.y - B.y < .001)		return 1;	if(A.x + B.x < .001 && A.y + B.y < .001 && A.y + B.y < .001)		return 2;	return 0;}void VectorMultConstant(Vector *A,double C){	A->x *= C;	A->y *= C;	A->z *= C;}void	Identity_Matrix(double xForm[][3])					{														xForm[0][0] = xForm[1][1] = xForm[2][2] = 1.0;		xForm[0][1] = xForm[0][2] = 0.0;					xForm[1][0] = xForm[1][2] = 0.0;					xForm[2][0] = xForm[2][1] = 0.0;				}													void SetMatrixAEqB(double xForm[][3],double zForm[][3]){	short k,i;	for(i = 0; i <= 2;i++){		for(k = 0; k <= 2; k++){			if(fabs(zForm[k][i]) < 1e-20)zForm[k][i] = 0.0;			xForm[k][i] = zForm[k][i];		}	}}	double VectorsLength(Vector start,Vector end){	return 	sqrt((start.x - end.x) * (start.x - end.x) +				(start.y - end.y) * (start.y - end.y) +				(start.z - end.z) * (start.z - end.z));}#include 	"nrutil.h"#include 	"mathRecipies.h"void RotatateMatrixAxis(double theXForm[3][3],double rotAngle,short axis){	double x,y,length;	//float **a,**v;	//float *w;	//short i,j;	//double matrix2[3][3];		x = cos(rotAngle);	y = sin(rotAngle);	length = sqrt(x * x + y * y);	x /= length;	y /= length;	switch(axis)	{		case 0:/*about x*/			theXForm[0][0] = 1.0;			theXForm[0][1] = theXForm[1][0] = theXForm[0][2] = theXForm[2][0] = 0.0;			theXForm[1][1] = theXForm[2][2] = x;			theXForm[1][2] = -y;			theXForm[2][1] = y;		break;		case 1:/*about y*/			theXForm[0][0] = theXForm[2][2] = x;			theXForm[0][2] = -y;			theXForm[0][1] = theXForm[1][0] = theXForm[1][2] = theXForm[2][1] = 0.0;			theXForm[2][0] = y;			theXForm[1][1] = 1.0;			break;		case 2:			FindHorizXform(y,x,theXForm);		break;		default:		break;	}	NormalizeRotationMatrix(theXForm);/*	w = vector(1,3);	v = matrix(1,3,1,3);	a = matrix(1,3,1,3);	for(i = 0; i < 3; i++){		for(j = 0; j < 3;j++){			a[i + 1][j + 1] = theXForm[j][i];		}	}	svdcmp(a,3,3,w,v);		for(i = 0; i < 3; i++){		for(j = 0; j < 3;j++){			theXForm[j][i] = a[i + 1][j + 1] ;			matrix2[j][i] = v[j + 1][i + 1] ;		}	}	MatrixMultiply(theXForm,matrix2,theXForm);	free_matrix(a,1,3,1,3);	free_matrix(v,1,3,1,3);	free_vector(w, 1,3);*/}double	VectorLengthXYZ(double x,double y,double z){	return sqrt(x * x + y * y + z * z);}void	SetVectorToOrigin		(double *vector,short length){	short i;	for(i = 0; i < length;i++)vector[i] = 0.0;	return;}void	MatrixVectorMultiply	(double matrix[][3],double* vector,double* vectorEnd){	short i;	for(i = 0 ; i <= 2;i++)		vectorEnd[i] = vector[i];		TransFormVector(&vectorEnd[0],&vectorEnd[1],&vectorEnd[2],matrix);}void SetVector1EqualTo2(double *v1,double *v2,short length,Boolean negative){	short i;	if(negative)		for(i = 0; i < length;i++) v1[i] = -v2[i];	else{		for(i = 0; i < length;i++) v1[i] = v2[i];	}}double VectorDotProduct(double* theV1,double* theV2,short length){	double theValue;	short i;	theValue = 0.0;	for(i = 0; i < length;i++) theValue += theV1[i] * theV2[i];	return theValue;}void	VectorCrossProduct(double *v1,double *v2,double *v3){	CrossProduct(&v3[0],&v3[1],&v3[2],v1[0],v1[1],v1[2],v2[0],v2[1],v2[2]);}void MatrixVectorMatrixMultiply(double theM[][3],double *v,double result[][3],Boolean row){	short j,k;	if(row){		for(j = 0; j <= 2 ; j++){			for(k = 0; k <= 2 ; k++){				result[j][k] = v[j] * theM[j][k];			}		}	}else{		for(j = 0; j <= 2 ; j++){			for(k = 0; k <= 2 ; k++){				result[j][k] = v[j] * theM[k][j];			}		}	}}void AddMatrix(double m1[][3],double m2[][3],double m3[][3],Boolean add){	short i,j;	if(add){		for(i = 0; i <= 2;i++){			for(j = 0; j <= 2;j++){			m3[i][j] = m1[i][j] + m2[i][j];			}		}	}else{		for(i = 0; i <= 2;i++){			for(j = 0; j <= 2;j++){			m3[i][j] = m1[i][j] - m2[i][j];			}		}	}}double RotateSingleConstantValue(double elastic_Const[6][6],double cos_Cry_Dis[6][6],short k,short l,short lt){	return (cos_Cry_Dis[k][0] * (cos_Cry_Dis[l][0] * elastic_Const[lt][0] + cos_Cry_Dis[l][1] * elastic_Const[lt][5] + cos_Cry_Dis[l][2] * elastic_Const[lt][4]) +				 cos_Cry_Dis[k][1] * (cos_Cry_Dis[l][0] * elastic_Const[lt][5] + cos_Cry_Dis[l][1] * elastic_Const[lt][1] + cos_Cry_Dis[l][2] * elastic_Const[lt][3]) +				 cos_Cry_Dis[k][2] * (cos_Cry_Dis[l][0] * elastic_Const[lt][4] + cos_Cry_Dis[l][1] * elastic_Const[lt][3] + cos_Cry_Dis[l][2] * elastic_Const[lt][2]));		}double DotProductXYZ(double x,double y,double z,double x1,double y1,double z1){	return x * x1 + y * y1 + z * z1;}void	SetMatrix(double matrix[3][3],double a1,double a2,double a3,double b1,double b2,double b3		,double c1,double c2,double c3){	 matrix[0][0] = a1;matrix[0][1] = a2;matrix[0][2] = a3;	 matrix[1][0] = b1;matrix[1][1] = b2;matrix[1][2] = b3;	 matrix[2][0] = c1;matrix[2][1] = c2;matrix[2][2] = c3;}/*#define		KJ  		k  + j * nCol#define		JK 			j  + k  * nCol#define		IJ  		i  + j * nCol#define		JI  		j  + i * nCol#define		II			i  + i  * nCol#define		IK			i  + k * nCol#define		MK			m  + k * nCol#define		MJ			m  + j * nCol#define		KK  		k  + k * nCol#define		_1J  		0  + j * nCol#define		I_JPVT 	i  + jpvt * nCol#define		I_N 		i  + nColMinus * nCol#define		_1I			0  + i * nCol#define		N_I			nColMinus + i * nCol*//*#define		KJ  		k][j#define		JK 			j][k#define		IJ  		i][j#define		JI  		j][i#define		II			i][i#define		IK			i][k#define		MK			m][k#define		MJ			m][j#define		KK  		k][k#define		_1J  		1][j#define		I_JPVT 	i][jpvt#define		I_N 		i][nCol#define		_1I			1][i#define		N_I			nCol][i*/#define		KJ  		j][k#define		JK 			k][j#define		IJ  		j][i#define		JI  		i][j#define		II			i][i#define		IK			k][i#define		MK			k][m#define		MJ			j][m#define		KK  		k][k#define		_1J  		j][1#define		I_JPVT 	jpvt][i#define		I_N 		nCol][i#define		_1I			i][1#define		N_I			i][nCol/*#define		KJ  		k * nCol   + j#define		JK 			j * nCol  + k#define		IJ  		i * nCol + j#define		JI  		j * nCol  + i#define		II			i * nCol + i#define		IK			i * nCol  + k#define		MK			m * nCol  + k#define		MJ			m * nCol  + j#define		KK  		k * nCol  + k#define		_1J  		0 * nCol   + j#define		I_JPVT 	i * nCol  + jpvt#define		I_N 		i * nCol  +  nColMinus#define		_1I			0 * nCol   + i#define		N_I			nColMinus * nCol + i*/void C_InverseRow(short nmRows,short nCol,dcomplex **a,dcomplex *b,long *err){	short jpvt,kp1;	dcomplex zero,t,temp,p,one;	short i,j,k,m,nmRow1,ikb;	double amX;//	short	ipvt[nCol]	*err = 100;	zero = Complex(0,0);	one = Complex(1,0);	if(nmRows == 1) return;	amX = 0;	for(j = 1; j <= nCol;j++)	{		if(Cabs(a[_1J]) > amX){			amX = Cabs(a[_1J]);			jpvt = j;		}	}	//Pivoting the first row	for(i = 1; i <= nCol;i++){		SWAP(a[I_JPVT],a[I_N]);	}	//interchange first row and last row//	InterchangeRows(a,&a[nmRows * nCol],nCol);	for(i = 1; i <= nCol;i++){		SWAP(a[_1I],a[N_I]);	}	//if(flag == 0){flag++; return;}	nmRow1 = nCol - 1;	for(k = 1; k <= nmRow1;k++){		//short kp1;		kp1 = k + 1;		m = k;		if(kp1 == nmRow1 || k == nmRow1){			for(i = kp1;i <= nmRow1;i++){				if(Cabs(a[IK]) > Cabs(a[MK])) m = i;			}		}	//	ipvt[k] = m;		temp = a[MK];		SWAP(a[MK],a[KK]);		if(Cabs(temp) <= 1.0e-07) continue;		for(i = kp1; i <= nCol;i++){			a[IK] = Cdiv(RCmul(-1,a[IK]) , temp);		}				for(j = kp1;j <= nCol; j++){/*cp1 needs to be set & N*/			temp = a[MJ];			SWAP(a[MJ],a[KJ]);			if(temp.r == zero.r && temp.i == zero.i) continue;			for(i = kp1;i <= nCol;i++)			{				p = Cmul(a[IK] , temp);//check this out				a[IJ] = Cadd(a[IJ],p);			}		}	}//	if(flag == 1){flag++; return;}	if(Cabs(a[nCol][nCol]) < 1.0e-07){ //(nCol *  nCol)/* - 1*/		*err = 0;	}	//back substitution to get x	temp = a[nCol][nCol];	temp =  Cdiv(one,temp);	b[nCol] = temp;	for(ikb = 2;ikb <= nCol;ikb++)	{		k = nCol - ikb + 1;		kp1 = k + 1;		b[k] = zero;		for(j = kp1; j <= nCol;j++){			p = Cmul( a[KJ] ,b[j]);			b[k] = Csub(b[k],p);		}		b[k] = Cdiv(b[k],a[KK]);	}		SWAP(b[jpvt],b[nCol]);}void InterchangeRows(dcomplex *a,dcomplex *b,short nCol){	dcomplex *aP,*bP,t;	short 	i;	aP = a;	bP = b;	for(i = 0; i < nCol;i++,aP++,bP++ ){		t = *aP;		*aP = *bP;		*bP = t;	}}void NormalizeRotationMatrix(double matrix1[3][3]){	float **a,**v;	float *w;	short i,j;	double matrix2[3][3];		//w = (float*)D_NewPtr(sizeof(float) * 4);	w = vector(1,3);	v = matrix(1,3,1,3);	a = matrix(1,3,1,3);	for(i = 0; i < 3; i++){		for(j = 0; j < 3;j++){			a[i + 1][j + 1] = matrix1[j][i];		}	}	for(i = 0; i < 3; i++){		for(j = 0; j < 3;j++){			 matrix1[j][i] = 0.0;			 matrix2[j][i] = 0.0;		}	}	svdcmp(a,3,3,w,v);		for(i = 0; i < 3; i++){		for(j = 0; j < 3;j++){			matrix1[j][i] = (float)(a[i + 1][j + 1]);			matrix2[j][i] = (float)(v[j + 1][i + 1]);		}	}	MatrixMultiply(matrix1,matrix2,matrix1);	for(i = 0; i < 3;i++){		for(j = 0; j < 3;j++){			if(fabs(matrix1[i][j]) < 1e-20)matrix1[i][j] = 0;		}	}	free_matrix(a,1,3,1,3);	free_matrix(v,1,3,1,3);	free_vector(w, 1,3);}#undef SWAP