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

#include "rs_graphic.h"
#include "rs_spline.h"
#include "rs_hatch.h"
#include "cg_pslg.h"


CG_PSLG::CG_PSLG(RS_Graphic * g)
{
  convert_cad_to_pslg(g);
}


void CG_PSLG::convert_cad_to_pslg(RS_Graphic * g)
{
  if(g==NULL) return;

  //mesh all the entity on active layer
  for (RS_Entity* e=g->firstEntity(); e!=NULL; e=g->nextEntity())
    if(e->isOnActiveLayer() && e->isVisible())
    {
      switch (e->rtti())
      {
      case RS2::EntityPoint :
        {
          const RS_Point * p = (const RS_Point *)e;
          add_point(p->getData().pos);
          break;
        }

      case RS2::EntityLine :
        {
          const RS_Line * line = (const RS_Line *)e;
          int mark;
          if(_label_to_mark.find(line->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[line->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[line->getLabel()] = mark;
          }

          unsigned int division = line->getDivision();
          RS_Vector  start = line->getStartpoint();
          RS_Vector  end = line->getEndpoint();
          RS_Vector  p1 = start;
          RS_Vector  p2 = p1 + (end - start)/division;

          if(line->isPointSet())
          {
            CG_Constrain constrain;
            constrain.p1 = start;
            constrain.p2 = end;
            constrain.char_length = (end - start).magnitude()/division;
            _constrains.push_back(constrain);

            for(unsigned int n=0; n<division; ++n)
            {
              add_aux_point(p1);
              add_aux_point(p2);
              p1 = p2;
              p2 = p1 + (end - start)/division;
            }
          }
          else
          {
            for(unsigned int n=0; n<division; ++n)
            {
              CG_Segment segment;
              segment.p1 = add_point(p1);
              segment.p2 = add_point(p2);
              segment.mark = mark;
              _segments.push_back(segment);

              p1 = p2;
              p2 = p1 + (end - start)/division;
            }
          }
          break;
        }

      case RS2::EntityArc:
        {
          const RS_Arc * arc = (const RS_Arc *)e;
          int mark;
          if(_label_to_mark.find(arc->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[arc->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[arc->getLabel()] = mark;
          }

          unsigned int division = arc->getDivision();
          RS_Vector  center = arc->getCenter();
          double     r = arc->getRadius();
          double     a1, a2;
          if(arc->isReversed())
          {
            // Arc Clockwise:
            a2 = arc->getAngle1();
            a1 = arc->getAngle2();
          }
          else
          {
            // Arc Counterclockwise:
            a1 = arc->getAngle1();
            a2 = arc->getAngle2();
          }
          if (a1>a2-0.01)  a2+=2*M_PI;

          RS_Vector  p1 = center + RS_Vector(r*cos(a1), r*sin(a1));
          RS_Vector  p2 = center + RS_Vector(r*cos(a1+(a2-a1)/division), r*sin(a1+(a2-a1)/division));
          for(unsigned int n=1; n<=division; ++n)
          {
            CG_Segment segment;
            segment.p1 = add_point(p1);
            segment.p2 = add_point(p2);
            segment.mark = mark;
            _segments.push_back(segment);
            p1 = p2;
            p2 = center + RS_Vector(r*cos(a1+(n+1)*(a2-a1)/division), r*sin(a1+(n+1)*(a2-a1)/division));
          }

          break;
        }

      case RS2::EntityCircle:
        {
          const RS_Circle * circle = (const RS_Circle *)e;
          int mark;
          if(_label_to_mark.find(circle->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[circle->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[circle->getLabel()] = mark;
          }

          unsigned int division = circle->getDivision();
          RS_Vector  center = circle->getCenter();
          RS_Vector  start = circle->getStartpoint();
          double     r = circle->getRadius();
          RS_Vector  p1 = start;
          RS_Vector  p2 = center + RS_Vector(r*cos(2*M_PI/division), r*sin(2*M_PI/division));
          for(unsigned int n=1; n<=division; ++n)
          {
            CG_Segment segment;
            segment.p1 = add_point(p1);
            segment.p2 = add_point(p2);
            segment.mark = mark;
            _segments.push_back(segment);
            p1 = p2;
            p2 = center + RS_Vector(r*cos((n+1)*2*M_PI/division), r*sin((n+1)*2*M_PI/division));
            if(n==division) p2 = start;
          }
          break;
        }

      case RS2::EntityEllipse :
        {
          const RS_Ellipse * ellipse = (const RS_Ellipse *)e;
          int mark;
          if(_label_to_mark.find(ellipse->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[ellipse->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[ellipse->getLabel()] = mark;
          }

          unsigned int division = ellipse->getDivision();
          RS_Vector  center = ellipse->getCenter();
          double     a = ellipse->getMajorRadius();
          double     b = ellipse->getMinorRadius();
          double     angel = ellipse->getAngle1();
          RS_Vector  start = center + RS_Vector(a*cos(angel), b*sin(angel));
          start.rotate(center, ellipse->getAngle());

          RS_Vector  p1 = start;
          RS_Vector  p2 = center + RS_Vector(a*cos(angel+2*M_PI/division), b*sin(angel+2*M_PI/division));
          p2.rotate(center, ellipse->getAngle());

          for(unsigned int n=1; n<=division; ++n)
          {
            CG_Segment segment;
            segment.p1 = add_point(p1);
            segment.p2 = add_point(p2);
            segment.mark = mark;
            _segments.push_back(segment);
            p1 = p2;
            p2 = center + RS_Vector(a*cos(angel+(n+1)*2*M_PI/division), b*sin(angel+(n+1)*2*M_PI/division));
            p2.rotate(center, ellipse->getAngle());
            if(n==division) p2 = start;
          }
          break;
        }

      case RS2::EntitySpline :
        {
          RS_Spline * spline = (RS_Spline *)e;
          int mark;
          if(_label_to_mark.find(spline->getLabel())!=_label_to_mark.end())
            mark = _label_to_mark[spline->getLabel()];
          else
          {
            mark = _label_to_mark.size()+1;
            _label_to_mark[spline->getLabel()] = mark;
          }

          for (RS_Entity* l=spline->firstEntity(); l!=NULL; l=spline->nextEntity())
            if(l->rtti() == RS2::EntityLine )
            {
              const RS_Line * line = (const RS_Line *)l;
              CG_Segment segment;
              segment.p1 = add_point(line->getStartpoint());
              segment.p2 = add_point(line->getEndpoint());
              segment.mark = mark;
              _segments.push_back(segment);
            }

          break;
        }

      case RS2::EntityHatch :
        {
          RS_Hatch * hatch = (RS_Hatch *)e;
          RS_HatchData data = hatch->getData();

          if(data.hole)
          {
            CG_Hole hole;
            hole.x    = data.internal_point.x;
            hole.y    = data.internal_point.y;
            _holes.push_back(hole);
          }
          else
          {
            CG_Region region;
            region.x    = data.internal_point.x;
            region.y    = data.internal_point.y;
            region.area_control = data.area_control > 0? data.area_control : RS_MAXDOUBLE;
            region.label = data.label;
            region.material = data.material;

            for (RS_Entity* c = hatch->firstEntity(RS2::ResolveAll);
                 c!=NULL;
                 c = hatch->nextEntity(RS2::ResolveAll))
            {
              if (c->rtti()==RS2::EntityLine)
              {
                RS_Line* line = (RS_Line*)c;
                region.add_point(line->getStartpoint());
                region.add_point(line->getEndpoint());
              }

              if (c->rtti()==RS2::EntityArc)
              {
                RS_Arc* arc = (RS_Arc*)c;
                unsigned int division = arc->getDivision();
                RS_Vector  center = arc->getCenter();
                double     r = arc->getRadius();
                double     a1, a2;
                if(arc->isReversed())
                {
                  // Arc Clockwise:
                  a2 = arc->getAngle1();
                  a1 = arc->getAngle2();
                }
                else
                {
                  // Arc Counterclockwise:
                  a1 = arc->getAngle1();
                  a2 = arc->getAngle2();
                }
                if (a1>a2-0.01)  a2+=2*M_PI;

                RS_Vector  p1 = center + RS_Vector(r*cos(a1), r*sin(a1));
                RS_Vector  p2 = center + RS_Vector(r*cos(a1+(a2-a1)/division), r*sin(a1+(a2-a1)/division));
                for(unsigned int n=1; n<=division; ++n)
                {
                  region.add_point(p1);
                  region.add_point(p2);
                  p1 = p2;
                  p2 = center + RS_Vector(r*cos(a1+(n+1)*(a2-a1)/division), r*sin(a1+(n+1)*(a2-a1)/division));
                }
              }

              if (c->rtti()==RS2::EntityCircle)
              {
                RS_Circle * circle = (RS_Circle *)c;
                unsigned int division = circle->getDivision();
                RS_Vector  center = circle->getCenter();
                RS_Vector  start = circle->getStartpoint();
                double     r = circle->getRadius();
                RS_Vector  p1 = start;
                RS_Vector  p2 = center + RS_Vector(r*cos(2*M_PI/division), r*sin(2*M_PI/division));
                for(unsigned int n=1; n<=division; ++n)
                {
                  region.add_point(p1);
                  region.add_point(p2);
                  p1 = p2;
                  p2 = center + RS_Vector(r*cos((n+1)*2*M_PI/division), r*sin((n+1)*2*M_PI/division));
                  if(n==division) p2 = start;
                }
              }

              if (c->rtti()==RS2::EntityEllipse)
              {
                RS_Ellipse * ellipse = (RS_Ellipse *)c;
                unsigned int division = ellipse->getDivision();
                RS_Vector  center = ellipse->getCenter();
                double     a = ellipse->getMajorRadius();
                double     b = ellipse->getMinorRadius();
                double     angel = ellipse->getAngle1();
                RS_Vector  start = center + RS_Vector(a*cos(angel), b*sin(angel));
                start.rotate(center, ellipse->getAngle());

                RS_Vector  p1 = start;
                RS_Vector  p2 = center + RS_Vector(a*cos(angel+2*M_PI/division), b*sin(angel+2*M_PI/division));
                p2.rotate(center, ellipse->getAngle());

                for(unsigned int n=1; n<=division; ++n)
                {
                  region.add_point(p1);
                  region.add_point(p2);
                  p1 = p2;
                  p2 = center + RS_Vector(a*cos(angel+(n+1)*2*M_PI/division), b*sin(angel+(n+1)*2*M_PI/division));
                  p2.rotate(center, ellipse->getAngle());
                  if(n==division) p2 = start;
                }
              }
            }
            _regions.push_back(region);
          }
          break;
        }

      default: break;
      }
    }

  std::map<RS_String, int>::iterator it = _label_to_mark.begin();
  for(; it != _label_to_mark.end(); ++it)
    _mark_to_label[it->second] = it->first;

}

unsigned int CG_PSLG::add_point(const RS_Vector &v)
{
  //this point already exist
  for(unsigned int n=0; n<_points.size(); ++n)
    if( v.distanceTo(_points[n])<1e-4 ) return n;

  _points.push_back(v);
  return _points.size()-1;
}

bool CG_PSLG::add_aux_point(const RS_Vector &v, double point_dist, double segment_dist)
{
  //this point already exist
  for(unsigned int n=0; n<_points.size(); ++n)
    if( v.distanceTo(_points[n])<point_dist ) return false;

  //the aux point lies on any segment?
  for(unsigned int n=0; n<_segments.size(); ++n)
  {
    RS_Vector p1 = _points[_segments[n].p1];
    RS_Vector p2 = _points[_segments[n].p2];

    double dist = fabs(RS_Vector::crossP((p2-p1).unit(), v-p1).z);
    RS_Vector project = p1+(p2-p1).unit()*RS_Vector::dotP((p2-p1).unit(), v-p1);
    bool in_p1p2 = ((project-p1).magnitude() + (project-p2).magnitude() - (p1-p2).magnitude()) < 1e-4;

    if(dist<segment_dist && in_p1p2)
    {
      return false;
    }
  }

  _aux_points.push_back(v);
  return true;
}

