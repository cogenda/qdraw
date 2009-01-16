/****************************************************************************
** $Id: rs_spline.h 1914 2004-09-19 11:05:34Z andrew $
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


#ifndef RS_SPLINE_H
#define RS_SPLINE_H

#include "rs_entitycontainer.h"
#include "rs_valuelist.h"

/**
 * Holds the data that defines a line.
 */
class RS_SplineData
{
public:
  /**
   * Default constructor. Leaves the data object uninitialized.
   */
  RS_SplineData() {}

  RS_SplineData(int degree, bool closed)
  {
    this->degree = degree;
    this->closed = closed;
  }

  friend std::ostream& operator << (std::ostream& os, const RS_SplineData& ld)
  {
    os << "( degree: " << ld.degree <<
    " closed: " << ld.closed <<
    ")";
    return os;
  }

public:
  /** Degree of the spline (1, 2, 3) */
  int degree;
  /** Closed flag. */
  bool closed;
  /** Control points of the spline. */
  RS_ValueList<RS_Vector> controlPoints;
};


/**
 * Class for a spline entity.
 *
 * @author Andrew Mustun
 */
class RS_Spline : public RS_EntityContainer
{
public:
  RS_Spline(RS_EntityContainer* parent,
            const RS_SplineData& d);

  virtual RS_Entity* clone();

  virtual ~RS_Spline();

  virtual void initLabel()
  {
    static unsigned long int idCounter=0;
    label = "Spline" + RS_String::number(idCounter++);
  }

  /**
   * @return the extra entity info as RS_String
   * can be override by each derived entities.
   */
  virtual RS_String get_ext_info() const
  {
    RS_String ext_info = "ext: "
        + label + ' '
        + RS_String::number(division);
    return ext_info;
  }

  /**
   * set the extra entity info by RS_String
   * can be override by each derived entities.
   */
  virtual void set_ext_info(RS_String ext_info)
  {
    if(ext_info=="") return;
    RS_String  sec;
    sec   = ext_info.section(' ', 1, 1);
    label = sec;
    sec   = ext_info.section(' ', 2);
    division = sec.toUInt() < 1 ? 1 : sec.toUInt();
  }

  /**   @return RS2::EntitySpline */
  virtual RS2::EntityType rtti() const
  {
    return RS2::EntitySpline;
  }

  /** @return true */
  virtual bool isEdge() const
  {
    return true;
  }

  /** @return Copy of data that defines the spline. */
  RS_SplineData getData() const
  {
    return data;
  }

  /** Sets the splines degree (1-3). */
  void setDegree(int deg)
  {
    if (deg>=1 && deg<=3)
    {
      data.degree = deg;
    }
  }

  /** @return Degree of this spline curve (1-3).*/
  int getDegree()
  {
    return data.degree;
  }

  /** @return 0. */
  int getNumberOfKnots()
  {
    return 0;
  }

  /** @return Number of control points. */
  int getNumberOfControlPoints()
  {
    return data.controlPoints.count();
  }

  /**
   * @retval true if the spline is closed.
   * @retval false otherwise.
   */
  bool isClosed()
  {
    return data.closed;
  }

  /**
   * Sets the closed falg of this spline.
   */
  void setClosed(bool c)
  {
    data.closed = c;
    update();
  }

  /** get Spline division number */
  unsigned int getDivision() const
  { return division; }

  /** set Spline division number */
  void setDivision(unsigned int d)
  { division = d < 1 ? 1 : d; }

  virtual RS_VectorSolutions getRefPoints();
  virtual RS_Vector getNearestRef(const RS_Vector& coord,
                                  double* dist = NULL);
  virtual RS_Vector getNearestSelectedRef(const RS_Vector& coord,
                                          double* dist = NULL);

  /** @return Start point of the entity */
  //virtual RS_Vector getStartpoint() const {
  //    return data.startpoint;
  //}
  /** @return End point of the entity */
  //virtual RS_Vector getEndpoint() const {
  //    return data.endpoint;
  //}
  /** Sets the startpoint */
  //void setStartpoint(RS_Vector s) {
  //    data.startpoint = s;
  //    calculateBorders();
  //}
  /** Sets the endpoint */
  //void setEndpoint(RS_Vector e) {
  //    data.endpoint = e;
  //    calculateBorders();
  //}

  void update();

  //virtual void moveStartpoint(const RS_Vector& pos);
  //virtual void moveEndpoint(const RS_Vector& pos);
  //virtual RS2::Ending getTrimPoint(const RS_Vector& coord,
  //          const RS_Vector& trimPoint);
  //virtual void reverse();
  /** @return the center point of the line. */
  //RS_Vector getMiddlepoint() {
  //    return (data.startpoint + data.endpoint)/2.0;
  //}
  //virtual bool hasEndpointsWithinWindow(RS_Vector v1, RS_Vector v2);

  /**
   * @return The length of the line.
   */
  //virtual double getLength() {
  //    return data.startpoint.distanceTo(data.endpoint);
  //}

  /**
   * @return The angle of the line (from start to endpoint).
   */
  //virtual double getAngle1() {
  //    return data.startpoint.angleTo(data.endpoint);
  //}

  /**
   * @return The angle of the line (from end to startpoint).
   */
  //virtual double getAngle2() {
  //    return data.endpoint.angleTo(data.startpoint);
  //}

  virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                       double* dist = NULL);
  //virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
  //        bool onEntity=true, double* dist = NULL, RS_Entity** entity=NULL);
  virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                     double* dist = NULL);
  virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                     double* dist = NULL);
  virtual RS_Vector getNearestDist(double distance,
                                   const RS_Vector& coord,
                                   double* dist = NULL);
  //virtual RS_Vector getNearestRef(const RS_Vector& coord,
  //                                 double* dist = NULL);
  /*virtual double getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity=NULL,
                                    RS2::ResolveLevel level=RS2::ResolveNone,
                      double solidDist = RS_MAXDOUBLE);*/

  virtual void addControlPoint(const RS_Vector& v);
  virtual void removeLastControlPoint();

  virtual void move(RS_Vector offset);
  virtual void rotate(RS_Vector center, double angle);
  virtual void scale(RS_Vector center, RS_Vector factor);
  virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);
  virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

  virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);
  RS_ValueList<RS_Vector> getControlPoints();

  friend std::ostream& operator << (std::ostream& os, const RS_Spline& l);

  virtual void calculateBorders();

  static void rbasis(int c, double t, int npts, int x[], double h[], double r[]);

  static void knot(int num, int order, int knotVector[]);
  static void rbspline(int npts, int k, int p1,
                       double b[], double h[], double p[]);

  static void knotu(int num, int order, int knotVector[]);
  static void rbsplinu(int npts, int k, int p1,
                       double b[], double h[], double p[]);

protected:
  RS_SplineData data;

  /**
   * the Spline can be divided into sub edges, default division = data.degree
   */
  unsigned int division;
}
;

#endif
