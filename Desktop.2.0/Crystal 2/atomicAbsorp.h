#ifndef _ABSORB#define _ABSORB int atom_(short *elNum, doublerreal *m, doublerreal *s, doublerreal *kvolts, doublerreal *fe, doublerreal *fa, integer *status/*, ftnlen elname_len*/); doublerreal biqint_(doublerreal *m, doublerreal *ms2, doublerreal *gm, doublerreal *gms2, rreal *f); doublerreal qint_(doublerreal *x, doublerreal *xx, rreal *f); doublerreal cint_(doublerreal *x, doublerreal *xx, rreal *f); dcomplex	AbsorptionCoefficient(double h,double k,double l,double *theUs,double *theVs,double *theWs,Crystal* theCrystal,double *theBs,double kV,double dspace);#endif