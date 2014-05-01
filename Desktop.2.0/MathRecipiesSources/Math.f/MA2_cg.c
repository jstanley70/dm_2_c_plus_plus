/* cg.f -- translated by f2c (version 19941113).   You must link the resulting object file with the libraries:	-lf2c -lm   (in that order)*/#include "f2c.h"/* Subroutine */ int cg_(integer *nm, integer *n, rreal **ar, rreal **ai, rreal *	wr, rreal *wi, integer *matz, rreal **zr, rreal **zi, rreal *fv1, rreal *fv2,	 rreal *fv3, integer *ierr){    /* System generated locals */    /* Local variables */    extern /* Subroutine */ int cbal_(integer *, integer *, rreal **, rreal **, 	    integer *, integer *, rreal *), corth_(integer *, integer *, 	    integer *, integer *, rreal **, rreal **, rreal *, rreal *), comqr_(	    integer *, integer *, integer *, integer *, rreal **, rreal **, rreal *	    , rreal *, integer *), cbabk2_(integer *, integer *, integer *, 	    integer *, rreal *, integer *, rreal **, rreal **), comqr2_(integer *, 	    integer *, integer *, integer *, rreal *, rreal *, rreal **, rreal **, 	    rreal *, rreal *, rreal **, rreal **, integer *);    integer is1, is2;/* ***FIRST EXECUTABLE STATEMENT  CG */    /* Parameter adjustments *///    zi_dim1 = *nm;//    zi_offset = zi_dim1 + 1;//    zi -= zi_offset;//    zr_dim1 = *nm;//    zr_offset = zr_dim1 + 1;//    zr -= zr_offset;//    ai_dim1 = *nm;//    ai_offset = ai_dim1 + 1;//    ai -= ai_offset;//    ar_dim1 = *nm;//    ar_offset = ar_dim1 + 1;//    ar -= ar_offset;//    --wr;//    --wi;//    --fv1;//    --fv2;//    --fv3;    /* Function Body */    if (*n <= *nm) {	goto L10;    }    *ierr = *n * 10;    goto L50;L10:    cbal_(nm, n, ar, ai, &is1, &is2, fv1);    corth_(nm, n, &is1, &is2, ar, ai, fv2, fv3);    if (*matz != 0) {	goto L20;    }/*     .......... FIND EIGENVALUES ONLY .......... */    comqr_(nm, n, &is1, &is2, ar, ai, wr, wi, 	    ierr);    goto L50;/*     .......... FIND BOTH EIGENVALUES AND EIGENVECTORS .......... */L20:    comqr2_(nm, n, &is1, &is2, fv2, fv3, ar, ai, wr, wi, zr, zi, ierr);    if (*ierr != 0) {	goto L50;    }    cbabk2_(nm, n, &is1, &is2, fv1, n, zr, zi);L50:    return 0;} /* cg_ */