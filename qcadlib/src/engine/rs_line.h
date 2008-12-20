/****************************************************************************
** $Id: rs_line.h 1907 2004-09-04 19:56:42Z andrew $
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


#ifndef RS_LINE_H
#define RS_LINE_H

#include "rs_atomicentity.h"

/**
 * Holds the data that defines a line.
 */
class RS_LineData
{
public:
  /**
   * Default constructor. Leaves the data object uninitialized.
   */
  RS_LineData(){}

  RS_LineData(const RS_Vector& startpoint,
              const RS_Vector& endpoint)
  {
    this->startpoint = startpoint;
    this->endpoint = endpoint;
  }

  friend class RS_Line;
  friend class RS_ActionDrawLine;

  friend std::ostream& operator << (std::ostream& os, const RS_LineData& ld)
  {
    os << "(" << ld.startpoint <<
    "/" << ld.endpoint <<
    ")";
    return os;
  }

public:

  RS_Vector startpoint;

  RS_Vector endpoint;

};


/**
 * Class for a line entity.
 *
 * @author Andrew Mustun
 */
class RS_Line : public RS_AtomicEntity
{
public:
  //RS_Line(RS_EntityContainer* parent);
  //RS_Line(const RS_Line& l);
  RS_Line(RS_EntityContainer* parent,
          const RS_LineData& d);

  virtual RS_Entity* clone();

  virtual ~RS_Line();

  virtual void initLabel()
  {
    static unsigned long int idCounter=0;
    label = "Line" + RS_String::number(idCounter++);
  }

  /**	@return RS2::EntityLine */
  virtual RS2::EntityType rtti() const
  {
    return RS2::EntityLine;
  }
  /** @return true */
  virtual bool isEdge() const
  {
    return true;
  }

  /** @return Copy of data that defines the line. */
  RS_LineData getData() const
  {
    return data;
  }

  virtual RS_VectorSolutions getRefPoints();

  /** @return Start point of the entity */
  virtual RS_Vector getStartpoint() const
  {
    return data.startpoint;
  }

  /** @return End point of the entity */
  virtual RS_Vector getEndpoint() const
  {
    return data.endpoint;
  }

  /** Sets the startpoint */
  void setStartpoint(RS_Vector s)
  {
    data.startpoint = s;
    calculateBorders();
  }

  /** Sets the endpoint */
  void setEndpoint(RS_Vector e)
  {
    data.endpoint = e;
    calculateBorders();
  }

  /** get line division number */
  unsigned int getDivision()
  { return division; }

  /** set line division number */
  void setDivision(unsigned int d)
  { division = d; }

  /**
   * @return Direction 1. The angle at which the line starts at
   * the startpoint.
   */
  double getDirection1() const
  {
    return getAngle1();
  }

  /**
   * @return Direction 2. The angle at which the line starts at
   * the endpoint.
   */
  double getDirection2() const
  {
    return getAngle2();
  }
  virtual void moveStartpoint(const RS_Vector& pos);
  virtual void moveEndpoint(const RS_Vector& pos);
  virtual RS2::Ending getTrimPoint(const RS_Vector& coord,
                                   const RS_Vector& trimPoint);
  virtual void reverse();
  /** @return the center point of the line. */
  RS_Vector getMiddlepoint()
  {
    return (data.startpoint + data.endpoint)/2.0;
  }
  /** Sets the y coordinate of the startpoint */
  void setStartpointY(double val)
  {
    data.startpoint.y = val;
    calculateBorders();
  }
  /** Sets the y coordinate of the endpoint */
  void setEndpointY(double val)
  {
    data.endpoint.y = val;
    calculateBorders();
  }
  virtual bool hasEndpointsWithinWindow(RS_Vector v1, RS_Vector v2);

  /**
   * @return The length of the line.
   */
  virtual double getLength()
  {
    return data.startpoint.distanceTo(data.endpoint);
  }

  /**
   * @return The angle of the line (from start to endpoint).
   */
  virtual double getAngle1() const
  {
    return data.startpoint.angleTo(data.endpoint);
  }

  /**
   * @return The angle of the line (from end to startpoint).
   */
  virtual double getAngle2() const
  {
    return data.endpoint.angleTo(data.startpoint);
  }

  virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                       double* dist = NULL);
  virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
      bool onEntity=true, double* dist = NULL, RS_Entity** entity=NULL);
  virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                     double* dist = NULL);
  virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                     double* dist = NULL);
  virtual RS_Vector getNearestDist(double distance,
                                   const RS_Vector& coord,
                                   double* dist = NULL);
  virtual RS_Vector getNearestDist(double distance,
                                   bool startp);
  //virtual RS_Vector getNearestRef(const RS_Vector& coord,
  //                                 double* dist = NULL);
  virtual double getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity=NULL,
                                    RS2::ResolveLevel level=RS2::ResolveNone,
                                    double solidDist = RS_MAXDOUBLE);

  virtual void move(RS_Vector offset);
  virtual void rotate(RS_Vector center, double angle);
  virtual void scale(RS_Vector center, RS_Vector factor);
  virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);
  virtual void stretch(RS_Vector firstCorner,
                       RS_Vector secondCorner,
                       RS_Vector offset);
  virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

  virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

  friend std::ostream& operator << (std::ostream& os, const RS_Line& l);

  virtual void calculateBorders();

protected:
  RS_LineData data;

  /**
   * the line can be divided into sub edges
   */
  unsigned int division;
}
;

#endif
