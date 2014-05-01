//rces.Jim:Stereographic:ST_DrawSmallCircle.c#include	"Diffract_INCs.h"#include	"StereoObject.h"#include	"ST_StereoMacros.c"#include	"UT_VectorMacros.c"#include	"SF_MathLib.h"double FindEdge(double startAngle,double nextInc,double cx,double cy,double rad,double projRad2);void	StereoObject::DrawSmallCircle(SpotInfo *thisPole,Boolean erase){	double				theXForm[3][3],theRadius,theAngle,x,y,z,x1,y1,z1,factor;	double				theX2,theY2,theX1,theY1,cx,cy,dx,dy,inc,start,end;	double				projRad2,centX,centY,aInc;	RgnHandle			oldClip,circleClip,screenClip;	short					theValue,count,h1,h2,v1,v2;	double 				angle,py,px,rad;	Boolean				plotOK = false,initFlag = true;	double 				xMin,xMax,yMin,yMax,a1,a2;	Boolean				loopFlag = true,plotFlag = false;	Rect					clipRect;	theAngle	= thisPole->angle;	theRadius	= fabs(sin(theAngle));		if(objectType !=  KIKUCHI_OBJECT || objectType !=  KIKUCHI_OBJECT + 10){		if(thisPole->z < 0.0 && thisPole->z < -theRadius)			return;	}		centX		= centerX;	centY		= centerY;		x = thisPole->x;	y = thisPole->y;	z = thisPole->z;	FIND_CENTER_XFORM(x,y,z,theXForm);	z			= fabs(cos(theAngle));	theAngle	= atan2((double)thisPole->y,(double)thisPole->x) + PI;	x 			= theRadius * cos(theAngle);	y 			= theRadius * sin(theAngle);		z1			= x * theXForm[2][0] + y * theXForm[2][1] + z * theXForm[2][2];	x1			= x * theXForm[0][0] + y * theXForm[0][1] + z * theXForm[0][2];	y1			= x * theXForm[1][0] + y * theXForm[1][1] + z * theXForm[1][2];		factor		= 1.0 + z1;	if(factor != 0.0){		factor		= radius / factor;		theX2		= centX - factor * y1;		theY2		= centY - factor * x1;	} else {		theX2		= 30000;		theY2		= 30000;	}		x 			= -x;	y 			= -y;		z1			= x * theXForm[2][0] + y * theXForm[2][1] + z * theXForm[2][2];	x1			= x * theXForm[0][0] + y * theXForm[0][1] + z * theXForm[0][2];	y1			= x * theXForm[1][0] + y * theXForm[1][1] + z * theXForm[1][2];		factor		= 1.0 + z1;	if(factor != 0.0){		factor		= radius / factor;		theX1		= centX - factor * y1;		theY1		= centY - factor * x1;	} else {		theX1		= 30000;		theY1		= 30000;	}	oldClip 		= D_NewRgn();	circleClip 	= D_NewRgn();	screenClip 	= D_NewRgn();	GetClip(oldClip);	h1	= centerX - radius;	v1	= centerY - radius;	h2	= centerX + radius;	v2	= centerY + radius;	SetRect(&gTheRect,h1,v1,h2,v2);	if(!printing){		OpenRgn();		dm_FrameOval(&gTheRect);		CloseRgn(circleClip);		D_SetClip(circleClip);	}	PenNormal();	PenPat(&qd.black);	if(erase){		DMForeColor(BACKGROUND_COLOR);	} else {		if(g_Monitor){			if(greyFlag){				theValue = 220 * pow((double)thisPole->intensity,1/dyRange);				DMForeColor(theValue + 15);			} else {				thisPole->theCrystal->SetColor();			}		} else {			DMForeColor(BLACK);		}	}		cx 			= (theX1 + theX2) / 2.0;	cy 			= (theY1 + theY2) / 2.0;	dx 			= theX1 - cx;	dy 			= theY1 - cy;	theRadius 	= sqrt(dx * dx + dy * dy);		x 			= cx - theRadius;	x1			= cx + theRadius;	y 			= cy - theRadius;	y1			= cy + theRadius;		projRad2 	= radius;	projRad2 	*= projRad2;  /* here */			if(theRadius > 75000){		start 	= atan2((cy - centY),(centX - cx));		end 	= start + PI * .5;		x		= cx + theRadius * cos(start);		y		= cy - theRadius * sin(start);		dx		= 5000. * cos(end);		dy		= -5000. * sin(end);		MoveTo((short)(x + dx),(short)(y + dy));		LineTo((short)(x - dx),(short)(y - dy));		thisPole->spotRect.left = (x + dx);		thisPole->spotRect.top = (y + dy);		thisPole->spotRect.right = (x - dx);		thisPole->spotRect.bottom = (y - dy);		thisPole->flags |= TOO_BIG;		D_DisposeRgn(&oldClip);		D_DisposeRgn(&circleClip);		D_DisposeRgn(&screenClip);		return;	} else if(x < -25000. || x1 > 25000. || y < -25000. || y1 > 25000 || (printing && theRadius < radius)){			//	if(objectType ==  KIKUCHI_OBJECT || objectType ==  KIKUCHI_OBJECT + 10){			start 	= atan2((cy - centY),(centX - cx));			end 	= start + PI * .5;			x		= cx + theRadius * cos(start);			y		= cy - theRadius * sin(start);			dx		= 5000. * cos(end);			dy		= -5000. * sin(end);			MoveTo((short)(x + dx),(short)(y + dy));			LineTo((short)(x - dx),(short)(y - dy));			thisPole->spotRect.left = (x + dx);			thisPole->spotRect.top = (y + dy);			thisPole->spotRect.right = (x - dx);			thisPole->spotRect.bottom = (y - dy);			thisPole->flags |= TOO_BIG;			D_DisposeRgn(&oldClip);			D_DisposeRgn(&circleClip);			D_DisposeRgn(&screenClip);			return;	//	}		/*		if(thisPole->flags & TOO_BIG)thisPole->flags -= TOO_BIG;		rad		= theRadius;		start 	= atan2((cy - centY),(centX - cx)) - PI; 				// Begin off screen 		end 	= start + 2.0 * PI; 									// One full rotation 		aInc	= 2.0  / rad;											// 2 pixel arc length		bInc	= aInc * 32;											// 64 pixel search arc length  				inc		= bInc;																		for(angle = start ; angle <= end ; angle += inc){			px = cx + rad * cos(angle);			py = cy - rad * sin(angle);						x 	= px - centX;			x	*= x;			y	= py - centY;			y	*= y;									if((x + y) <= projRad2){				if(initFlag){					if(px >= 0.0){						thePoint.h = (short)(px + .5);					} else {						thePoint.h = (short)(px - .5);					}									if(py >= 0.0){						thePoint.v = (short)(py + .5);					} else {						thePoint.v = (short)(py - .5);					}										MoveTo(thePoint.h,thePoint.v);					initFlag = false;				}								if(!plotOK){					inc2 = inc * .5;					angle -= inc2;										for(i = 1 ; i <= 9 ; i++){						px = cx + rad * cos(angle);						py = cy - rad * sin(angle);									x 	= px - centX;						x	*= x;						y	= py - centY;						y	*= y;						inc2 *= .5;												if((x + y) <= projRad2){							angle -= inc2;						} else {							angle += inc2;						}					}										if(px >= 0.0){						thePoint.h = (short)(px + .5);					} else {						thePoint.h = (short)(px - .5);					}									if(py >= 0.0){						thePoint.v = (short)(py + .5);					} else {						thePoint.v = (short)(py - .5);					}										MoveTo(thePoint.h,thePoint.v);					plotOK 	= true;					inc		= aInc;				} else {					if(px >= 0.0){						thePoint.h = (short)(px + .5);					} else {						thePoint.h = (short)(px - .5);					}									if(py >= 0.0){						thePoint.v = (short)(py + .5);					} else {						thePoint.v = (short)(py - .5);					}					LineTo(thePoint.h,thePoint.v);				}			} else if(plotOK){				inc2 = inc * .5;				angle -= inc2;									for(i = 1 ; i <= 5 ; i++){					px = cx + rad * cos(angle);					py = cy - rad * sin(angle);										x 	= px - centX;					x	*= x;					y	= py - centY;					y	*= y;										inc2 *= .5;										if((x + y) <= projRad2){						angle += inc2;					} else {						angle -= inc2;					}									}				if(px >= 0.0){					thePoint.h = (short)(px + .5);				} else {					thePoint.h = (short)(px - .5);				}							if(py >= 0.0){					thePoint.v = (short)(py + .5);				} else {					thePoint.v = (short)(py - .5);				}				LineTo(thePoint.h,thePoint.v);				plotOK 	= false;					inc		= bInc;				} 			initFlag = false;		}		*/	} else {			if(x > 0){			x += .5;		} else {			x -= .5;		}			if(x1 > 0){			x1 += .5;		} else {			x1 -= .5;		}			if(y > 0){			y += .5;		} else {			y -= .5;		}			if(y1 > 0){			y1 += .5;		} else {			y1 -= .5;		}				SetRect(&gTheRect,(short)x,(short)y,(short)x1,(short)y1);		if(objectType ==  KIKUCHI_OBJECT || objectType ==  KIKUCHI_OBJECT + 10){			thisPole->spotRect = gTheRect;			if(greyFlag){				theValue = 220 * pow((double)thisPole->intensity,1/dyRange);				DMForeColor(theValue + 15);			}			dm_FrameOval(&gTheRect);			D_DisposeRgn(&oldClip);			D_DisposeRgn(&circleClip);			D_DisposeRgn(&screenClip);			return;		}								start 		= atan2((cy - centY),(centX - cx)); 					/* Begin on screen */		end 		= start + 2.0 * PI;		rad			= theRadius;		aInc		= PI / 2.;							inc			= aInc;				xMin		= 32000;		yMin		= 32000;		xMax		= -32000;		yMax		= -32000;		count		= 0;		angle 		= start;		px			= -projRad2;				while(fabs(px - projRad2) > 1.0){			x 	= cx + rad * cos(angle);			y 	= py = cy - rad * sin(angle);			px	= x - centX;			py	= y - centY;			px	= px * px + py * py;						a2 	= angle;			if(px <= projRad2){				angle += inc;			} else {				angle -= inc;			}			inc *= .51;			if(count++ > 40){				a2 =  2.0 * PI;				break;			}		}						if(x < xMin){			xMin = x;		}		if(x > xMax){			xMax = x;		}		if(y < yMin){			yMin = y;		}		if(y > yMax){			yMax = y;		}				angle 		= start;		px			= -projRad2;		inc			= aInc;		count 		= 0;		while(fabs(px - projRad2) > 1.0){			x 	= cx + rad * cos(angle);			y 	= py = cy - rad * sin(angle);			px	= x - centX;			py	= y - centY;			px	= px * px + py * py;						a1 = angle;			if(px <= projRad2){				angle -= inc;			} else {				angle += inc;			}			inc *= .51;			if(count++ > 40){				a1 = 0.0;				break;			}		}				if(x < xMin){			xMin = x;		}		if(x > xMax){			xMax = x;		}		if(y < yMin){			yMin = y;		}		if(y > yMax){			yMax = y;		}				aInc = PI / 90.0;		inc  = aInc;				for(angle = a1 ; angle <= a2 ; angle += inc){			x 	= cx + rad * cos(angle);			y 	= py = cy - rad * sin(angle);			px	= x - centX;			py	= y - centY;			px	= px * px + py * py;					loopFlag = false;						if(px <= projRad2){							if(x < xMin){					xMin = x;					loopFlag = true;				}				if(x > xMax){					xMax = x;					loopFlag = true;				}				if(y < yMin){					yMin = y;					loopFlag = true;				}				if(y > yMax){					yMax = y;					loopFlag = true;				}			}						if(loopFlag && count < 8){				angle -= inc;				inc *= .51;				count++;			} else {				count = 0;				inc = aInc;			}		}								clipRect.left 	= xMin - 1;		clipRect.top 	= yMin - 1;		clipRect.right 	= xMax + 1;		clipRect.bottom = yMax + 1;		RectRgn(circleClip,&clipRect);				clipRect.left 	= (short)(centX - radius);		clipRect.top 	= (short)(centY - radius);		clipRect.right 	= (short)(centX + radius);		clipRect.bottom = (short)(centY + radius);		if(!printing){		OpenRgn();		dm_FrameOval(&clipRect);		CloseRgn(screenClip);		SectRgn(circleClip,screenClip,screenClip);		D_SetClip(screenClip);	}		if(greyFlag){			theValue = 220 * pow((double)thisPole->intensity,1/dyRange);			DMForeColor(theValue + 15);		}						dm_FrameOval(&gTheRect);		}	if(!printing)D_SetClip(oldClip);	D_DisposeRgn(&oldClip);	D_DisposeRgn(&circleClip);	D_DisposeRgn(&screenClip);	return;}void StereoObject::GetXYThetas(Point thePoint,double *x,double *y){	double a,b,z;	double length,projL;	projL = projLengthScreen / 2;	a = thePoint.h - centerX;	b = thePoint.v - centerY;	length = sqrt(a * a + b * b);	if(length > projLengthScreen){		*x = PI / 2;		*y = PI / 2;		return;	}	z = sqrt((projLengthScreen * projLengthScreen) - ((a * a) + (b * b)));	*x = asin(a / sqrt(z * z + a * a));	*y = asin(b / sqrt(z * z + b * b));/*	*x = asin(a / projLengthScreen);	*y = asin(b / projLengthScreen);*/}Boolean StereoObject::ConvertPointToXYZCart(Point thePoint,double *x,double *y,double *z){	static Point	lastPoint;	double			v,u;	double			rad,newRad;	double			angleA;			if(EqualPt(thePoint,lastPoint) || !PtInRgn(thePoint,drawRgn))		return false;			lastPoint = thePoint;		/*LocalToObj(&thePoint);*/	onScreen = true;	LocalToObj(&thePoint);	v = thePoint.h - centerX;	u = thePoint.v - centerY;	rad = sqrt(u * u + v * v);		if(rad == 0){		*x = 0.0;		*y = 0.0;		*z = 1.0;	} else {		if(rad > radius)			return false;				angleA = 2.0 * atan(rad / radius);		newRad = sin(angleA) / rad;		*x = newRad * u;		*y = newRad * v;		*z = cos(angleA);	}	return true;}void StereoObject::RotateAroundX(Point where,short type,Boolean shifted,Boolean click){	static Boolean	initFlag;	static double	startAngle,endAngle;	static Point	lastPoint;	static Rect		theRect;	double			angle,x,y;	short			start,end,theRad;	static double	xx,yy,zz;	Point			cP;	PenState thePenState;		LocalToObj(&where);	cP.h = centerX;	cP.v = centerY;//	LocalToObj(&cP);		theRad = (short)projLengthScreen / 3;	theRad = 40;	if(funcInit){				if(theRad > 7500)			theRad = 7500;		startAngle 		= endAngle = 0.0;		if(type == 1){				theRect.top		= cP.v - theRad - projLengthScreen;				theRect.bottom	= cP.v + theRad  - projLengthScreen;				theRect.left	= cP.h - theRad;				theRect.right	= cP.h + theRad;		}		if(type == 2){				theRect.top		= cP.v - 40;//theRad;				theRect.bottom	= cP.v + 40;//theRad;				theRect.left	= cP.h - 40;//theRad - projLengthScreen;				theRect.right	= cP.h + 40;//theRad - projLengthScreen;		}				funcInit 		= false;		initFlag		= false;		return;	}		if(shifted){		angle = atan2(THE_CURRENT_SPOT.y,THE_CURRENT_SPOT.x);		xx = THE_CURRENT_SPOT.x;		yy = THE_CURRENT_SPOT.y;		zz = THE_CURRENT_SPOT.z;		initFlag = true;		if(type == 3){				theRect.top		= THE_CURRENT_SPOT.spotRect.top - theRad;				theRect.bottom	= THE_CURRENT_SPOT.spotRect.top + theRad;				theRect.left	= THE_CURRENT_SPOT.spotRect.left - theRad;				theRect.right	= THE_CURRENT_SPOT.spotRect.left + theRad;					ObjRectToLocal(&theRect);				angle -= (PI / 2.0);				if(initFlag){					start	= 90 - (short)(.5 + startAngle * 180.0 / PI);					end		= 90 - (short)(.5 + endAngle * 180.0 / PI);					InvertArc(&theRect,start,end - start);				}				if(angle < 0.0)					angle += (2.0 * PI);				startAngle = endAngle = angle;				angle = 0;		}		if(type == 2){				if(!theLine){					theLine = (LineObj*)D_new(LineObj);									}				theLine->DoInit(0,theCrystal[0],false,0L,where,where);				theLine->hor1 = cP.h;				theLine->ver1 = where.v;				theLine->hor2 = where.h;				theLine->ver2 = where.v;				PenNormal();				PenMode						(patXor);				theLine->DrawLine(false);				PenNormal();				MoveTo(cP.h,cP.v - projLengthScreen);				LineTo(cP.h,cP.v + projLengthScreen);				//GetXYThetas(where,&x,&y);				GetMuLambda(where,&x,&y,false);				angle = y * PI / 180;				if(angle < 0.0)					angle += (2.0 * PI);				startAngle = endAngle = angle;		}		if(type == 1){				if(!theLine){					theLine = (LineObj*)D_new(LineObj);									}				theLine->DoInit(0,theCrystal[0],false,0L,where,where);								theLine->hor1 = where.h;				theLine->ver1 = cP.v;								theLine->hor2 = where.h;				theLine->ver2 = where.v;				PenNormal();				MoveTo(cP.h - projLengthScreen,cP.v);				LineTo(cP.h + projLengthScreen,cP.v);								PenMode						(patXor);				theLine->DrawLine(false);				PenNormal();				//GetXYThetas(where,&x,&y);				GetMuLambda(where,&x,&y,true);				angle = -y * PI / 180;				if(angle < 0.0)					angle += (2.0 * PI);				startAngle = endAngle = angle;		}		sprintf(gTheText,"Rotation angle = %5.1f",angle * 180/PI);		theRuler		->SetInfo(gTheText);		theRuler		->SetHelp("Single click to rotate.");	} else if			(click && initFlag){		double				theXForm[3][3],x,y,restore[3][3];		if(theLine != 0L)theLine->KillAll();		theLine = 0L;		/*x = where.h - cP.h;		y = cP.v - where.v;*/				SetTheCursor(WATCH_CURSOR);		switch(type){				case 1:				angle = endAngle;				x = cos(angle);				y = sin(angle);				FindYAxisXform(y,x,theXForm);/*mislabeled subroutines*/				break;				case 2:				angle = endAngle;				x = cos(angle);				y = sin(angle);				FindXAxisXform(y,x,theXForm);/*mislabeled subroutines*/				break;				case 3:				angle = endAngle - startAngle;				x = cos(angle);				y = sin(angle);				FindRotateAboutPoleXform((double)xx,(double)yy,(double)zz,x,y,theXForm,restore);				break;		}		RotateSpotsBase(theXForm);		if(type == 3) RotateSpotsBase(restore);		startAngle = endAngle;		sprintf				(gTheText,"Rotation angle = 0.0�");		theRuler	->	SetInfo(gTheText);		initFlag 	= 	false;		theRuler	->	SetHelp("Shift click to select spot to rotate.");			} else if(!EqualPt(where,lastPoint) && initFlag){		double x,y;		if(type == 3){			x = where.h - cP.h;			y = cP.v - where.v;			angle = atan2(y,x);			if(angle < 0.0)				angle += (2.0 * PI);			if((angle - startAngle) > PI){				angle -= (2.0 * PI);			} else if((angle - startAngle) <= -PI) {				angle += (2.0 * PI);			}			start	= 90 - (short)(.5 + endAngle * 180.0 / PI);			end		= 90 - (short)(.5 + angle * 180.0 / PI);			InvertArc(&theRect,start,end - start);			endAngle = angle;			angle = (endAngle - startAngle) * 180.0 / PI;		}		GetPenState(&thePenState);			if(type == 2){				PenNormal();				PenMode						(patXor);				theLine->DrawLine(false);				theLine->hor1 = cP.h;				theLine->ver1 = where.v;								theLine->hor2 = where.h;				theLine->ver2 = where.v;				DMForeColor(COMPLEMENT_COLOR);				DMBackColor(BACKGROUND_COLOR);								theLine->DrawLine(false);								PenMode(	gCurrentObj->gMode);				PenNormal();								//GetXYThetas(where,&x,&y);				GetMuLambda(where,&x,&y,false);				angle = y * PI / 180;				endAngle = angle;				angle = (endAngle) * 180.0 / PI;		}		if(type == 1){				if(!theLine){					return;				}				PenNormal();				PenMode						(patXor);				theLine->DrawLine(false);								theLine->hor1 = where.h;				theLine->ver1 = cP.v;								theLine->hor2 = where.h;				theLine->ver2 = where.v;								theLine->DrawLine(false);				GetMuLambda(where,&x,&y,true);				angle = -y * PI / 180;				//angle = -y;								endAngle = angle;				angle = (endAngle) * 180.0 / PI;		}		PenMode(thePenState.pnMode);		sprintf(gTheText,"Rotation angle = %5.1f�",angle);		theRuler->SetInfo(gTheText);	}	lastPoint = where;}