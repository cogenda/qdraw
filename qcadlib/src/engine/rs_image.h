/****************************************************************************
** $Id: rs_image.h 1907 2004-09-04 19:56:42Z andrew $
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


#ifndef RS_IMAGE_H
#define RS_IMAGE_H

#include "rs_atomicentity.h"

#include "rs_img.h"

/**
 * Holds the data that defines a line.
 */
class RS_ImageData
{
public:
  /**
   * Default constructor. Leaves the data object uninitialized.
   */
  RS_ImageData() {}

  RS_ImageData(int handle,
               const RS_Vector& insertionPoint,
               const RS_Vector& uVector,
               const RS_Vector& vVector,
               const RS_Vector& size,
               const RS_String& file,
               int brightness,
               int contrast,
               int fade)
  {

    this->handle = handle;
    this->insertionPoint = insertionPoint;
    this->uVector = uVector;
    this->vVector = vVector;
    this->size = size;
    this->file = file;
    this->brightness = brightness;
    this->contrast = contrast;
    this->fade = fade;
  }

  friend std::ostream& operator << (std::ostream& os, const RS_ImageData& ld)
  {
    os << "(" << ld.insertionPoint << ")";
    return os;
  }

public:
  /** Handle of image definition. */
  int handle;
  /** Insertion point. */
  RS_Vector insertionPoint;
  /** u vector. Points along visual bottom of image. */
  RS_Vector uVector;
  /** v vector. Points along visual left of image. */
  RS_Vector vVector;
  /** Image size in pixel. */
  RS_Vector size;
  /** Path to image file. */
  RS_String file;
  /** Brightness (0..100, default: 50). */
  int brightness;
  /** Contrast (0..100, default: 50). */
  int contrast;
  /** Fade (0..100, default: 0). */
  int fade;
};



/**
 * Class for a line entity.
 *
 * @author Andrew Mustun
 */
class RS_Image : public RS_AtomicEntity
{
public:
  RS_Image(RS_EntityContainer* parent,
           const RS_ImageData& d);

  virtual RS_Entity* clone(bool update_label=false);

  virtual ~RS_Image();

  virtual void initLabel()
  {
    static unsigned long int idCounter=0;
    label = "Image" + RS_String::number(idCounter++);
  }

  /**   @return RS2::EntityImage */
  virtual RS2::EntityType rtti() const
  {
    return RS2::EntityImage;
  }

  virtual void update();

  /** @return Copy of data that defines the image. */
  RS_ImageData getData() const
  {
    return data;
  }

  /** @return Insertion point of the entity */
  virtual RS_Vector getInsertionPoint() const
  {
    return data.insertionPoint;
  }
  /** Sets the insertion point for the image. */
  void setInsertionPoint(RS_Vector ip)
  {
    data.insertionPoint = ip;
    calculateBorders();
  }

  /** @return File name of the image. */
  RS_String getFile() const
  {
    return data.file;
  }

  /** Sets the file name of the image.  */
  void setFile(const RS_String& file)
  {
    data.file = file;
  }

  /** @return u Vector. Points along bottom, 1 pixel long. */
  RS_Vector getUVector() const
  {
    return data.uVector;
  }
  /** @return v Vector. Points along left, 1 pixel long. */
  RS_Vector getVVector() const
  {
    return data.vVector;
  }
  /** @return Width of image in pixels. */
  int getWidth() const
  {
    return (int)data.size.x;
  }
  /** @return Height of image in pixels. */
  int getHeight() const
  {
    return (int)data.size.y;
  }
  /** @return Brightness. */
  int getBrightness() const
  {
    return data.brightness;
  }
  /** @return Contrast. */
  int getContrast() const
  {
    return data.contrast;
  }
  /** @return Fade. */
  int getFade() const
  {
    return data.fade;
  }
  /** @return Image definition handle. */
  int getHandle() const
  {
    return data.handle;
  }
  /** Sets the image definition handle. */
  void setHandle(int h)
  {
    data.handle = h;
  }


  /** @return The four corners. **/
  RS_VectorSolutions getCorners()
  {
    RS_VectorSolutions sol(4);

    sol.set(0, data.insertionPoint);
    sol.set(1,
            data.insertionPoint + data.uVector*RS_Math::round(data.size.x));
    sol.set(3,
            data.insertionPoint + data.vVector*RS_Math::round(data.size.y));
    sol.set(2, sol.get(3) + data.uVector*RS_Math::round(data.size.x));

    return sol;
  }

  /**
   * @return image with in graphic units.
   */
  double getImageWidth()
  {
    return data.size.x * data.uVector.magnitude();
  }

  /**
   * @return image height in graphic units.
   */
  double getImageHeight()
  {
    return data.size.y * data.vVector.magnitude();
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
  virtual double getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity=NULL,
                                    RS2::ResolveLevel level=RS2::ResolveNone,
                                    double solidDist = RS_MAXDOUBLE);

  virtual double getLength()
  {
    return -1.0;
  }

  virtual void move(RS_Vector offset);
  virtual void rotate(RS_Vector center, double angle);
  virtual void scale(RS_Vector center, RS_Vector factor);
  virtual void mirror(RS_Vector axisPoint1, RS_Vector axisPoint2);
  /*virtual void stretch(RS_Vector firstCorner,
                       RS_Vector secondCorner,
                       RS_Vector offset);*/

  virtual void draw(RS_Painter* painter, RS_GraphicView* view, double patternOffset=0.0);

  friend std::ostream& operator << (std::ostream& os, const RS_Image& l);

  virtual void calculateBorders();

protected:
  RS_ImageData data;
  RS_Img img;
  //RS_Img** img;
  //int nx;
  //int ny;
};

#endif
