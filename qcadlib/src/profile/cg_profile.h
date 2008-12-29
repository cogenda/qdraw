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

#ifndef __cg_profile_h__
#define __cg_profile_h__

#include <cmath>
#include <string>

/**
 * the base class for profile function
 */
class Profile
{
public:
  /**
   * constructor, do nothing
   */
  Profile(const std::string &label, const std::string &property, double xmin, double xmax, double ymin, double ymax)
      : _label(label), _property(property), _xmin(xmin), _xmax(xmax), _ymin(ymin), _ymax(ymax)
  {}

  /**
   * destructor, do nothing
   */
  virtual ~Profile() {}

  /**
   * virtual profile concentration computation function
   */
  virtual double profile(double x, double y)const=0;

  /**
   * @return profle label
   */
  const std::string & label() const
  { return _label; }

  /**
   * @return profle property
   */
  const std::string & property() const
  { return _property; }

protected:

  /**
   * the label of this profile
   */
  std::string _label;

  /**
   * profile property
   */
  std::string _property;

  /**
   * bound box of doping region
   */
  double _xmin;    // bound box

  /**
   * bound box of doping region
   */
  double _xmax;

  /**
   * bound box of doping region
   */
  double _ymin;

  /**
   * bound box of doping region
   */
  double _ymax;
};



/**
 * uniform profile
 */
class UniformProfile : public Profile
{
public:

  /**
   * constructor
   */
  UniformProfile(const std::string &label, const std::string &property, double xmin, double xmax, double ymin, double ymax, double N)
      : Profile(label, property, xmin, xmax, ymin, ymax), _PEAK(N)
  {}

  /**
   * compute doping concentration by point location
   */
  double profile(double x,double y) const
  {
    if( x >= _xmin-1e-6 && x <= _xmax+1e-6 &&
        y >= _ymin-1e-6 && y <= _ymax+1e-6 )
      return _PEAK;
    else
      return 0.0;
  }

private:
  /**
   * the peak value of profile
   */
  double _PEAK;
};


/**
 * profile with gauss distribution in x-y direction
 */
class GaussProfile : public Profile
{
public:
  GaussProfile(const std::string &label, const std::string &property,
               double xmin, double xmax, double ymin, double ymax,
               double N, double XCHAR, double YCHAR)
    : Profile(label, property, xmin, xmax, ymin, ymax), _PEAK(N),  _XCHAR(XCHAR),  _YCHAR(YCHAR)
  {
  }

  double profile(double x, double y) const
  {
    double dx,dy;
    if(x<_xmin)
      dx = exp(-(x-_xmin)*(x-_xmin)/(_XCHAR*_XCHAR));
    else if(x>=_xmin&&x<=_xmax)
      dx = 1.0;
    else
      dx = exp(-(x-_xmax)*(x-_xmax)/(_XCHAR*_XCHAR));

    if(y<_ymin)
      dy = exp(-(y-_ymin)*(y-_ymin)/(_YCHAR*_YCHAR));
    else if(y>=_ymin&&y<=_ymax)
      dy = 1.0;
    else
      dy = exp(-(y-_ymax)*(y-_ymax)/(_YCHAR*_YCHAR));

    return _PEAK*dx*dy;
  }

private:

  /**
   * the peak value of doping concentration
   */
  double _PEAK;

  /**
   * characteristic length along x direction
   */
  double _XCHAR;

  /**
   * characteristic length along y direction
   */
  double _YCHAR;
};




class ErfProfile : public Profile
{
public:
  ErfProfile(const std::string &label, const std::string &property,
               double xmin, double xmax, double ymin, double ymax,
           double N, double XCHAR, double YCHAR)
    : Profile(label, property, xmin, xmax, ymin, ymax), _PEAK(N),  _XCHAR(XCHAR),  _YCHAR(YCHAR)
  {
  }

  double profile(double x,double y) const
  {
    double dx,dy;
    dx = (erfc((x-_xmax)/_XCHAR)-erfc((x-_xmin)/_XCHAR))/2;
    if(y<_ymin)
      dy = exp(-(y-_ymin)*(y-_ymin)/(_YCHAR*_YCHAR));
    else if(y>=_ymin&&y<=_ymax)
      dy = 1.0;
    else
      dy = exp(-(y-_ymax)*(y-_ymax)/(_YCHAR*_YCHAR));
    return _PEAK*dx*dy;
  }

private:

  /**
   * the peak value of doping concentration
   */
  double _PEAK;

  /**
   * characteristic length along x direction
   */
  double _XCHAR;

  /**
   * characteristic length along y direction
   */
  double _YCHAR;

};




#endif // #define __cg_profile_h__
