/****************************************************************************
** $Id: rs_hatch.h 2367 2005-04-04 16:57:36Z andrew $
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


#ifndef RS_HATCH_H
#define RS_HATCH_H

#include "rs_entity.h"
#include "rs_entitycontainer.h"

/**
 * Holds the data that defines a hatch entity.
 */
class RS_HatchData
{
public:
  /**
   * Default constructor. Leaves the data object uninitialized.
   */
  RS_HatchData():material(""), area_control(-1) {}

  /**
   * @param solid true: solid fill, false: pattern.
   * @param scale Pattern scale or spacing.
   * @param pattern Pattern name.
   */
  RS_HatchData(bool solid,
               double scale,
               double angle,
               const RS_String& pattern
	       )
  :material(""), area_control(-1)
  {
    this->solid = solid;
    this->scale = scale;
    this->angle = angle;
    this->pattern = pattern;
  }

  /**
   * @param solid true: solid fill, false: pattern.
   * @param scale Pattern scale or spacing.
   * @param pattern Pattern name.
   * @param material hatch material
   * @param area_control region mesh size
   */
  RS_HatchData(bool solid,
               double scale,
               double angle,
               const RS_String& pattern,
	       const RS_String& label,
               const RS_String& material,
	       double area_control
	       )
  {
    this->solid = solid;
    this->scale = scale;
    this->angle = angle;
    this->pattern  = pattern;
    this->label    = label;
    this->material = material;
    this->area_control = area_control;
  }

  friend std::ostream& operator << (std::ostream& os, const RS_HatchData& td)
  {
    os << "(" << td.pattern.latin1() << ")";
    return os;
  }

public:
  bool solid;
  double scale;
  double angle;
  RS_String pattern;
  // the same as label in RS_Hatch
  RS_String label;
  RS_String material;
  double area_control;
};



/**
 * Class for a hatch entity.
 *
 * @author Andrew Mustun
 */
class RS_Hatch : public RS_EntityContainer
{
public:
  RS_Hatch(RS_EntityContainer* parent,
           const RS_HatchData& d);
  virtual ~RS_Hatch() {}

  virtual RS_Entity* clone();

  virtual void initLabel()
  {
    static unsigned long int idCounter=0;
    label = "Hatch" + RS_String::number(idCounter++);
  }

  /**	@return RS2::EntityHatch */
  virtual RS2::EntityType rtti() const
  {
    return RS2::EntityHatch;
  }

  /**
   * @return true: if this is a hatch with lines (hatch pattern),
   *         false: if this is filled with a solid color.
   */
  virtual bool isContainer() const
  {
    if (isSolid())
    {
      return false;
    }
    else
    {
      return true;
    }
  }

  /** @return Copy of data that defines the hatch. */
  RS_HatchData getData() const
  {
    return data;
  }

  /** @return writable reference of data that defines the hatch. */
  RS_HatchData & getData()
  {
    return data;
  }

  bool validate();

  int countLoops();

  /** @return true if this is a solid fill. false if it is a pattern hatch. */
  bool isSolid() const
  {
    return data.solid;
  }
  void setSolid(bool solid)
  {
    data.solid = solid;
  }

  RS_String getPattern()
  {
    return data.pattern;
  }
  void setPattern(const RS_String& pattern)
  {
    data.pattern = pattern;
  }

  double getScale()
  {
    return data.scale;
  }
  void setScale(double scale)
  {
    data.scale = scale;
  }

  double getAngle()
  {
    return data.angle;
  }
  void setAngle(double angle)
  {
    data.angle = angle;
  }

  virtual void calculateBorders();
  void update();
  void activateContour(bool on);

  virtual void draw(RS_Painter* painter, RS_GraphicView* view,
                    double patternOffset=0.0);

  virtual double getLength()
  {
    return -1.0;
  }

  virtual double getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity = NULL,
                                    RS2::ResolveLevel level = RS2::ResolveNone,
                                    double solidDist = RS_MAXDOUBLE);

  virtual void move(RS_Vector offset);
  virtual void rotate(RS_Vector center, double angle);
  virtual void scale(RS_Vector center, RS_Vector factor);
  virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);
  virtual void stretch(RS_Vector firstCorner,
                       RS_Vector secondCorner,
                       RS_Vector offset);

  friend std::ostream& operator << (std::ostream& os, const RS_Hatch& p);

protected:
  RS_HatchData data;
  RS_EntityContainer* hatch;
  bool updateRunning;
  bool needOptimization;
};

#endif
