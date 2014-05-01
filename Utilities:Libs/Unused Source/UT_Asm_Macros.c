/* STRUCTURE_FACTOR(double,double,double,double*,short,double)  *//* dataarray format is: U1,V1,W1,Z1,U2,V2,W2,Z2...UN,VN,WN,ZN   */#define	STRUCTURE_FACTOR(h_M,k_M,l_M,dataAddr_M,num_M,result_M)					\{																				\	double	real_Part_M,imag_Part_M;											\	asm{																		\		fmovem		fp0-fp7,-(a7)												\		fmovem		d7/a2-a4,-(a7)												\																				\		fmove.x		h_M,fp7						; setup h						\		fmove.x		k_M,fp6						; setup k						\		fmove.x		l_M,fp5						; setup l						\		fmove.x		#6.28318530717958648,fp4	; setup 2�						\		fmove.x		#0,fp0														\		fmove.x		fp0,real_Part_M				; clear real					\		fmove.x		fp0,imag_Part_M 			; clear imag					\		fmovea		dataAddr_M,a4				; set data addr					\		fmovea		&real_Part_M,a3				; get real addr					\		fmovea		&imag_Part_M,a2				; get imag addr					\		fmove.w		num_m,d7					; get num atoms					\		andi.l		0x0000ffff,d7				; mask off high 16 bits			\		subq.w		#1,d7						; subtract 1					\																				\@loop	fmove.x		(a4),fp0			; get u									\		fmul		fp7,fp0				; (h * u)								\		lea			0x000C(a4),a4		; 										\		fmove.x		(a4),fp1			; get v									\		fmul		fp6,fp1				; (k * v)								\		lea			0x000C(a4),a4		; 										\		fmove.x		(a4),fp2			; get w									\		fmul		fp5,fp2				; (l * w)								\		lea			0x000C(a4),a4		;										\		fadd		fp0,fp1				; (h * u) + (k * v)						\		fadd		fp1,fp2				; (h * u) + (k * v) + (l * w)			\		fmul		fp4,fp2				; 2� * (h * u) + (k * v) + (l * w)		\		fsincos		fp2,fp0:fp1			; sin(fp2)->fp1,cos(fp2)->fp0			\		fmove.x		(a4),fp3			; get Z									\		lea			0x000C(a4),a4		;										\		fmul		fp3,fp0				; z * cos								\		fmul		fp3,fp1				; z * sin								\		fadd		fp0,(a3)			; realpart = realpart + z * cos			\		fadd		fp1,(a2)			; imagpart = imagpart + z * sin			\																				\		dbeq		d7,@loop													\																				\		fmove.x		(a3),fp0													\		fmul		fp0,fp0														\		fmove.x		(a2),fp1													\		fmul		fp1,fp1														\		fadd		fp1,fp0														\		fsqrt		fp0															\		fmove.x		fp0,result_M												\																				\		fmovem		(a7)+,d7/a2-a4												\		fmovem		(a7)+,fp0-fp7												\	}																			\}																				\