/****************************************************************************
**
** Copyright (C) 2007-2008 Cogenda. All rights reserved.
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
**********************************************************************/
#ifndef __cg_pslg_h__
#define __cg_pslg_h__

#include <vector>
#include <map>

class RS_Vector;
class RS_Graphic;
class RS_String;

struct CG_Segment
{
  unsigned int p1, p2;
  int mark;
};


struct CG_Region
{
  double x;
  double y;
  double area_control;
  RS_String label;
  RS_String material;
};

struct CG_Hole
{
  double x;
  double y;
};

class CG_PSLG
{
public:
  CG_PSLG(RS_Graphic *);

  std::vector<RS_Vector> & get_points()
  { return _points; }

  std::vector<CG_Segment> & get_segments()
  { return _segments; }

  std::map<int, RS_String> & get_segments_info()
  { return _mark_to_label; }

  std::vector<CG_Region> & get_regions()
  { return _regions; }

  std::vector<CG_Hole> &   get_holes()
  { return _holes; }

  RS_String get_region_label(unsigned int r)
  { return _regions[r].label; }

  RS_String get_region_material(unsigned int r)
  { return _regions[r].material; }

  RS_String get_segment_label(unsigned int s)
  { return _mark_to_label[s]; }

private:
  /**
   * build PSLG from visitable Entities in current layer
   */
  void convert_cad_to_pslg(RS_Graphic *);

  /**
   * function to add point into PSLG, drop any overlaped points
   */
  unsigned int add_point(const RS_Vector &v);

  /**
   * PSLG points
   */
  std::vector<RS_Vector> _points;

  /**
   * PSLG segments
   */
  std::vector<CG_Segment> _segments;

  /**
   * map mark to segment label
   */
  std::map<RS_String, int> _label_to_mark;

  /**
   * map segment label to mark
   */
  std::map<int, RS_String> _mark_to_label;

  /**
   * PSLG region
   */
  std::vector<CG_Region> _regions;

  /**
   * PSLG hole
   */
  std::vector<CG_Hole> _holes;
};

#endif

