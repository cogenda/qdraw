/****************************************************************************
** $Id: rs_painter.cpp 1938 2004-12-09 23:09:53Z andrew $
**
** Copyright (C) 2001-2003 RibbonSoft. All rights reserved.
**
** This file is part of the qcadlib Library project.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** Licensees holding valid qcadlib Professional Edition licenses may use
** this file in accordance with the qcadlib Commercial License
** Agreement provided with the Software.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.ribbonsoft.com for further details.
**
** Contact info@ribbonsoft.com if any conditions of this licensing are
** not clear to you.
**
**********************************************************************/


#include "rs_painter.h"


void RS_Painter::createArc(RS_PointArray& pa,
                           const RS_Vector& cp, double radius,
                           double a1, double a2,
                           bool reversed)
{

  if (radius<1.0e-6)
  {
    RS_DEBUG->print(RS_Debug::D_WARNING,
                    "RS_Painter::createArc: invalid radius: %f", radius);
    return;
  }

  int   cix;            // Next point on circle
  int   ciy;            //
  double aStep;         // Angle Step (rad)
  double a;             // Current Angle (rad)

  if(fabs(2.0/radius)<=1.0)
  {
    aStep=asin(2.0/radius);
  }
  else
  {
    aStep=1.0;
  }

  //if (aStep<0.05) {
  //    aStep = 0.05;
  //}

  // less than a pixel long lines:
  //if (radius*aStep<1.0) {
  //	aStep =
  //}

  //QPointArray pa;
  int i=0;
  pa.resize(i+1);
  pa.setPoint(i++, toScreenX(cp.x+cos(a1)*radius),
              toScreenY(cp.y-sin(a1)*radius));
  //moveTo(toScreenX(cp.x+cos(a1)*radius),
  //       toScreenY(cp.y-sin(a1)*radius));
  if(!reversed)
  {
    // Arc Counterclockwise:
    if(a1>a2-1.0e-10)
    {
      a2+=2*M_PI;
    }
    for(a=a1+aStep; a<=a2; a+=aStep)
    {
      cix = toScreenX(cp.x+cos(a)*radius);
      ciy = toScreenY(cp.y-sin(a)*radius);
      //lineTo(cix, ciy);
      pa.resize(i+1);
      pa.setPoint(i++, cix, ciy);
    }
  }
  else
  {
    // Arc Clockwise:
    if(a1<a2+1.0e-10)
    {
      a2-=2*M_PI;
    }
    for(a=a1-aStep; a>=a2; a-=aStep)
    {
      cix = toScreenX(cp.x+cos(a)*radius);
      ciy = toScreenY(cp.y-sin(a)*radius);
      //lineTo(cix, ciy);
      pa.resize(i+1);
      pa.setPoint(i++, cix, ciy);
    }
  }
  //lineTo(toScreenX(cp.x+cos(a2)*radius),
  //       toScreenY(cp.y-sin(a2)*radius));
  pa.resize(i+1);
  pa.setPoint(i++,
              toScreenX(cp.x+cos(a2)*radius),
              toScreenY(cp.y-sin(a2)*radius));
  //drawPolyline(pa);
}




void RS_Painter::createEllipse(QPointArray& pa,
                             const RS_Vector& cp,
                             double radius1, double radius2,
                             double angle,
                             double angle1, double angle2,
                             bool reversed)
{


    double aStep;         // Angle Step (rad)
    double a;             // Current Angle (rad)

    aStep=0.01;

    RS_Vector vp;
    RS_Vector vc(cp.x, cp.y);
    vp.set(cp.x+cos(angle1)*radius1,
           cp.y-sin(angle1)*radius2);
    vp.rotate(vc, -angle);

    int i=0;
    pa.resize(i+1);
    pa.setPoint(i++, toScreenX(vp.x),toScreenY(vp.y));

    if(!reversed) {
        // Arc Counterclockwise:
        if(angle1>angle2-RS_TOLERANCE) {
            angle2+=2*M_PI;
        }
        for(a=angle1+aStep; a<=angle2; a+=aStep) {
            vp.set(cp.x+cos(a)*radius1,
                   cp.y-sin(a)*radius2);
            vp.rotate(vc, -angle);

	    pa.resize(i+1);
            pa.setPoint(i++, toScreenX(vp.x),toScreenY(vp.y));
        }
    } else {
        // Arc Clockwise:
        if(angle1<angle2+RS_TOLERANCE) {
            angle2-=2*M_PI;
        }
        for(a=angle1-aStep; a>=angle2; a-=aStep) {
            vp.set(cp.x+cos(a)*radius1,
                   cp.y-sin(a)*radius2);
            vp.rotate(vc, -angle);

	    pa.resize(i+1);
            pa.setPoint(i++, toScreenX(vp.x),toScreenY(vp.y));
        }
    }
    vp.set(cp.x+cos(angle2)*radius1,
           cp.y-sin(angle2)*radius2);
    vp.rotate(vc, -angle);

    pa.resize(i+1);
    pa.setPoint(i++, toScreenX(vp.x),toScreenY(vp.y));

}

void RS_Painter::drawRect(const RS_Vector& p1, const RS_Vector& p2)
{
  drawLine(RS_Vector(p1.x, p1.y), RS_Vector(p2.x, p1.y));
  drawLine(RS_Vector(p2.x, p1.y), RS_Vector(p2.x, p2.y));
  drawLine(RS_Vector(p2.x, p2.y), RS_Vector(p1.x, p2.y));
  drawLine(RS_Vector(p1.x, p2.y), RS_Vector(p1.x, p1.y));
}


void RS_Painter::drawHandle(const RS_Vector& p, const RS_Color& c, int size)
{
  if (size<0)
  {
    size = 2;
  }
  fillRect((int)(p.x-size), (int)(p.y-size), 2*size, 2*size, c);
}


