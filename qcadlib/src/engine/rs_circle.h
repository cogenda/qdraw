/****************************************************************************
** $Id: rs_circle.h 1907 2004-09-04 19:56:42Z andrew $
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


#ifndef RS_CIRCLE_H
#define RS_CIRCLE_H

#include "rs_atomicentity.h"


/**
 * Holds the data that defines a circle.
 */
class RS_CircleData
{
public:
  RS_CircleData() {}

  RS_CircleData(const RS_Vector& center,
                double radius)
  {

    this->center = center;
    this->radius = radius;
  }

  void reset()
  {
    center = RS_Vector(false);
    radius = 0.0;
  }

  bool isValid()
  {
    return (center.valid && radius>RS_TOLERANCE);
  }

  friend class RS_Circle;

  friend std::ostream& operator << (std::ostream& os,
                                    const RS_CircleData& ad)
  {
    os << "(" << ad.center <<
    "/" << ad.radius <<
    ")";
    return os;
  }

public:
  RS_Vector center;
  double radius;
};



/**
 * Class for a circle entity.
 *
 * @author Andrew Mustun
 */
class RS_Circle : public RS_AtomicEntity
{
public:
  RS_Circle (RS_EntityContainer* parent,
             const RS_CircleData& d);
  virtual ~RS_Circle() {}

  virtual RS_Entity* clone(bool update_label=false)
  {
    RS_Circle* c = new RS_Circle(*this);
    c->initId();
    if(update_label) c->initLabel();
    return c;
  }

  virtual void initLabel()
  {
    static unsigned long int idCounter=0;
    label = "Circle" + RS_String::number(idCounter++);
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
    sec   = ext_info.section(' ', 0, 0);
    if(sec != "ext:") return;
    sec   = ext_info.section(' ', 1, 1);
    label = sec;
    sec   = ext_info.section(' ', 2);
    division = sec.toUInt() < 3 ? 3 : sec.toUInt();
  }

  /**   @return RS2::EntityCircle */
  virtual RS2::EntityType rtti() const
  {
    return RS2::EntityCircle;
  }
  /** @return true */
  virtual bool isEdge() const
  {
    return true;
  }

  /** @return Copy of data that defines the circle. **/
  RS_CircleData getData()
  {
    return data;
  }

  virtual RS_VectorSolutions getRefPoints();

  virtual RS_Vector getStartpoint() const
  {
    return data.center + RS_Vector(data.radius, 0.0);
  }
  virtual RS_Vector getEndpoint() const
  {
    return data.center + RS_Vector(data.radius, 0.0);
  }

  /** get circle division number */
  unsigned int getDivision() const
  { return division; }

  /** set circle division number */
  void setDivision(unsigned int d)
  { division = d < 3 ? 3 : d; }

  /**
   * @return Direction 1. The angle at which the arc starts at
   * the startpoint.
   */
  double getDirection1() const
  {
    return M_PI/2.0;
  }
  /**
   * @return Direction 2. The angle at which the arc starts at
   * the endpoint.
   */
  double getDirection2() const
  {
    return M_PI/2.0*3.0;
  }

  /** @return The center point (x) of this arc */
  RS_Vector getCenter() const
  {
    return data.center;
  }
  /** Sets new center. */
  void setCenter(const RS_Vector& c)
  {
    data.center = c;
  }
  /** @return The radius of this arc */
  double getRadius() const
  {
    return data.radius;
  }
  /** Sets new radius. */
  void setRadius(double r)
  {
    data.radius = r;
  }
  double getAngleLength() const;
  virtual double getLength();

  bool createFromCR(const RS_Vector& c, double r);
  bool createFrom2P(const RS_Vector& p1, const RS_Vector& p2);
  bool createFrom3P(const RS_Vector& p1, const RS_Vector& p2,
                    const RS_Vector& p3);

  virtual RS_Vector getNearestEndpoint(const RS_Vector& coord,
                                       double* dist = NULL);
  virtual RS_Vector getNearestPointOnEntity(const RS_Vector& coord,
      bool onEntity = true, double* dist = NULL, RS_Entity** entity=NULL);
  virtual RS_Vector getNearestCenter(const RS_Vector& coord,
                                     double* dist = NULL);
  virtual RS_Vector getNearestMiddle(const RS_Vector& coord,
                                     double* dist = NULL);
  virtual RS_Vector getNearestDist(double distance,
                                   const RS_Vector& coord,
                                   double* dist = NULL);
  virtual RS_Vector getNearestDist(double distance,
                                   bool startp);
  virtual double getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity=NULL,
                                    RS2::ResolveLevel level=RS2::ResolveNone,
                                    double solidDist = RS_MAXDOUBLE);

  virtual void move(RS_Vector offset);
  virtual void rotate(RS_Vector center, double angle);
  virtual void scale(RS_Vector center, RS_Vector factor);
  virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);
  virtual void moveRef(const RS_Vector& ref, const RS_Vector& offset);

  virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

  friend std::ostream& operator << (std::ostream& os, const RS_Circle& a);

  virtual void calculateBorders();

protected:
  RS_CircleData data;

  /**
   * the circle can be divided into sub edges, default division = 12
   */
  unsigned int division;
};

#endif
