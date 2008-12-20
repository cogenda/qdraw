/****************************************************************************
** $Id: rs_image.cpp 2368 2005-04-04 17:00:52Z andrew $
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


#include "rs_image.h"

#include "rs_constructionline.h"
#include "rs_debug.h"
#include "rs_graphicview.h"
#include "rs_painter.h"
#include "rs_painterqt.h"


/**
 * Constructor.
 */
RS_Image::RS_Image(RS_EntityContainer* parent,
                   const RS_ImageData& d)
    :RS_AtomicEntity(parent), data(d)
{

  update();
  calculateBorders();
  initLabel();
}



/**
 * Destructor.
 */
RS_Image::~RS_Image()
{
  /*if (img!=NULL) {
  	delete[] img;
  }*/
}




RS_Entity* RS_Image::clone()
{
  RS_Image* i = new RS_Image(*this);
  i->setHandle(getHandle());
  i->initId();
  i->update();
  return i;
}



void RS_Image::update()
{

  RS_DEBUG->print("RS_Image::update");

  // the whole image:
  //RS_Img image = RS_Img(data.file);
  img = RS_Img(data.file);
  if (!img.isNull())
  {
    data.size = RS_Vector(img.width(), img.height());
  }

  RS_DEBUG->print("RS_Image::update: OK");

  /*
  // number of small images:
  nx = image.width()/100;
  ny = image.height()/100;

  // create small images:
  img = new RS_Img*[nx];
  RS_Pixmap pm;
  int w,h;
  for (int x = 0; x<nx; ++x) {
  	img[x] = new RS_Img[ny];
  	for (int y = 0; y<ny; ++y) {
  		if (x<nx-1) {
  			w = 100;
  		}
  		else {
  			w = image.width()%100;
  		}

  		if (y<ny-1) {
  			h = 100;
  		}
  		else {
  			h = image.height()%100;
  		}

  		pm = RS_Pixmap(w, h);
  		RS_PainterQt painter(&pm);
  		painter.drawImage(-x*100, -y*100, image);
  		img[x][y] = pm.convertToImage();
  	}
  }
  */
}



void RS_Image::calculateBorders()
{
  resetBorders();

  RS_VectorSolutions sol = getCorners();

  for (int i=0; i<4; ++i)
  {
    minV = RS_Vector::minimum(minV, sol.get(i));
    maxV = RS_Vector::maximum(maxV, sol.get(i));
  }
}



RS_Vector RS_Image::getNearestEndpoint(const RS_Vector& coord,
                                       double* dist)
{
  RS_VectorSolutions corners = getCorners();
  return corners.getClosest(coord, dist);
}



RS_Vector RS_Image::getNearestPointOnEntity(const RS_Vector& coord,
    bool onEntity, double* dist, RS_Entity** entity)
{

  if (entity!=NULL)
  {
    *entity = this;
  }

  RS_VectorSolutions corners = getCorners();
  RS_VectorSolutions points(4);

  RS_Line l[] =
    {
      RS_Line(NULL, RS_LineData(corners.get(0), corners.get(1))),
      RS_Line(NULL, RS_LineData(corners.get(1), corners.get(2))),
      RS_Line(NULL, RS_LineData(corners.get(2), corners.get(3))),
      RS_Line(NULL, RS_LineData(corners.get(3), corners.get(0)))
    };

  for (int i=0; i<4; ++i)
  {
    points.set(i, l[i].getNearestPointOnEntity(coord, onEntity));
  }

  return points.getClosest(coord, dist);
}



RS_Vector RS_Image::getNearestCenter(const RS_Vector& coord,
                                     double* dist)
{

  RS_VectorSolutions points(4);
  RS_VectorSolutions corners = getCorners();

  points.set(0, (corners.get(0) + corners.get(1))/2.0);
  points.set(1, (corners.get(1) + corners.get(2))/2.0);
  points.set(2, (corners.get(2) + corners.get(3))/2.0);
  points.set(3, (corners.get(3) + corners.get(0))/2.0);

  return points.getClosest(coord, dist);
}



RS_Vector RS_Image::getNearestMiddle(const RS_Vector& coord,
                                     double* dist)
{
  return getNearestCenter(coord, dist);
}



RS_Vector RS_Image::getNearestDist(double distance,
                                   const RS_Vector& coord,
                                   double* dist)
{

  RS_VectorSolutions corners = getCorners();
  RS_VectorSolutions points(4);

  RS_Line l[] =
    {
      RS_Line(NULL, RS_LineData(corners.get(0), corners.get(1))),
      RS_Line(NULL, RS_LineData(corners.get(1), corners.get(2))),
      RS_Line(NULL, RS_LineData(corners.get(2), corners.get(3))),
      RS_Line(NULL, RS_LineData(corners.get(3), corners.get(0)))
    };

  for (int i=0; i<4; ++i)
  {
    points.set(i, l[i].getNearestDist(distance, coord, dist));
  }

  return points.getClosest(coord, dist);
}



double RS_Image::getDistanceToPoint(const RS_Vector& coord,
                                    RS_Entity** entity,
                                    RS2::ResolveLevel /*level*/,
                                    double /*solidDist*/)
{
  if (entity!=NULL)
  {
    *entity = this;
  }

  RS_VectorSolutions corners = getCorners();
  double dist;
  double minDist = RS_MAXDOUBLE;

  RS_Line l[] =
    {
      RS_Line(NULL, RS_LineData(corners.get(0), corners.get(1))),
      RS_Line(NULL, RS_LineData(corners.get(1), corners.get(2))),
      RS_Line(NULL, RS_LineData(corners.get(2), corners.get(3))),
      RS_Line(NULL, RS_LineData(corners.get(3), corners.get(0)))
    };

  for (int i=0; i<4; ++i)
  {
    dist = l[i].getDistanceToPoint(coord, NULL);
    if (dist<minDist)
    {
      minDist = dist;
    }
  }

  return minDist;
}



void RS_Image::move(RS_Vector offset)
{
  data.insertionPoint.move(offset);
  calculateBorders();
}



void RS_Image::rotate(RS_Vector center, double angle)
{
  data.insertionPoint.rotate(center, angle);
  data.uVector.rotate(angle);
  data.vVector.rotate(angle);
  calculateBorders();
}



void RS_Image::scale(RS_Vector center, RS_Vector factor)
{
  data.insertionPoint.scale(center, factor);
  data.uVector.scale(factor);
  data.vVector.scale(factor);
  calculateBorders();
}



void RS_Image::mirror(RS_Vector axisPoint1, RS_Vector axisPoint2)
{
  data.insertionPoint.mirror(axisPoint1, axisPoint2);
  data.uVector.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
  data.vVector.mirror(RS_Vector(0.0,0.0), axisPoint2-axisPoint1);
  calculateBorders();
}



void RS_Image::draw(RS_Painter* painter, RS_GraphicView* view, double /*patternOffset*/)
{
  if (painter==NULL || view==NULL || img.isNull())
  {
    return;
  }

  // erase image:
  //if (painter->getPen().getColor()==view->getBackground()) {
  //	RS_VectorSolutions sol = getCorners();
  //
  //}

  int ox = 0;
  int oy = 0;
  int width = view->getWidth();
  int height = view->getHeight();

  RS_Vector scale = RS_Vector(view->toGuiDX(data.uVector.magnitude()),
                              view->toGuiDY(data.vVector.magnitude()));
  double angle = data.uVector.angle();

  int startX, stopX, startY, stopY;

  if (data.uVector.x>1.0e-6 && data.vVector.y>1.0e-6)
  {
    startX = (int)((view->toGraphX(ox)-data.insertionPoint.x) /
                   data.uVector.x) - 1;
    if (startX<0)
    {
      startX = 0;
    }
    stopX = (int)((view->toGraphX(width)-data.insertionPoint.x) /
                  data.uVector.x) + 1;
    if (stopX>(int)data.size.x)
    {
      stopX = (int)data.size.x;
    }
    startY = -(int)((view->toGraphY(oy) -
                     (data.insertionPoint.y+getImageHeight())) /
                    data.vVector.y) - 1;
    if (startY<0)
    {
      startY = 0;
    }
    stopY = -(int)((view->toGraphY(height) -
                    (data.insertionPoint.y+getImageHeight())) /
                   data.vVector.y) + 1;
    if (stopY>(int)data.size.y)
    {
      stopY = (int)data.size.y;
    }
  }
  else
  {
    startX = 0;
    startY = 0;
    stopX = 0;
    stopY = 0;
  }

  painter->drawImg(img,
                   view->toGui(data.insertionPoint),
                   angle, scale,
                   startX, startY, stopX-startX, stopY-startY);

  if (isSelected())
  {
    RS_VectorSolutions sol = getCorners();

    painter->drawLine(view->toGui(sol.get(0)), view->toGui(sol.get(1)));
    painter->drawLine(view->toGui(sol.get(1)), view->toGui(sol.get(2)));
    painter->drawLine(view->toGui(sol.get(2)), view->toGui(sol.get(3)));
    painter->drawLine(view->toGui(sol.get(3)), view->toGui(sol.get(0)));
  }
}



/**
 * Dumps the point's data to stdout.
 */
std::ostream& operator << (std::ostream& os, const RS_Image& i)
{
  os << " Image: " << i.getData() << "\n";
  return os;
}

