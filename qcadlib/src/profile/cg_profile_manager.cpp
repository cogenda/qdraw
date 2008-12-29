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

#include "cg_profile.h"
#include "cg_profile_manager.h"
#include "cg_parameters.h"

ProfileManager::~ProfileManager()
{
  for(unsigned int n=0; n<_profiles.size(); ++n)
    delete _profiles[n];
  _profiles.clear();
}


int ProfileManager::add_profile(const Parameters & p, std::string &error_msg)
{
  std::string type = p.get<std::string>("type");
  if(type == "Unoform")
    return add_profile_uniform(p, error_msg);
  else if(type == "Gauss")
    return add_profile_gauss(p, error_msg);
  else if(type == "Erf")
    return add_profile_erf(p, error_msg);

  error_msg = "Unsupported profile type.";
  return 1;
}


double ProfileManager::property_to_real(const std::string &property) const
{
  if(property == "Na") return -1.0;
  if(property == "Nd") return  1.0;
  return 0.0;
}


int ProfileManager::add_profile_uniform(const Parameters & p, std::string &error_msg)
{

  std::string label    = p.get<std::string>("label");
  std::string property = p.get<std::string>("property");

  double xmax = p.get<double>("x.max");
  double xmin = p.get<double>("x.min");
  double ymax = p.get<double>("y.max");
  double ymin = p.get<double>("y.min");
  double peak = p.get<double>("n.peak");

  if(xmin-xmax>1e-8) {error_msg = "Xmin is larger than Xmax"; return 1;}
  if(ymin-ymax>1e-8) {error_msg = "Ymin is larger than Ymax"; return 1;}

  Profile * f = new UniformProfile(label, property, xmin, xmax, ymax, ymin, peak);
  _profiles.push_back(f);

  error_msg = "";
  return 0;
}


int ProfileManager::add_profile_gauss(const Parameters & p, std::string &error_msg)
{
  std::string label    = p.get<std::string>("label");
  std::string property = p.get<std::string>("property");

  double ion  = property_to_real(property);

  double xmax = p.get<double>("x.max");
  double xmin = p.get<double>("x.min");
  double ymax = p.get<double>("y.max");
  double ymin = p.get<double>("y.min");
  if(xmin-xmax>1e-8) {error_msg = "Xmin is larger than Xmax"; return 1;}
  if(ymin-ymax>1e-8) {error_msg = "Ymin is larger than Ymax"; return 1;}
  double slice = 0.5*(xmax+xmin);

  double peak = p.get<double>("n.peak");

  double YCHAR=0,XCHAR,YJUNC,dop=0;
  if(p.have_parameter<double>("y.char"))
    YCHAR = p.get<double>("y.char");
  else if(p.have_parameter<double>("y.junction"))
  {
    //Junction is an absolute location.
    YJUNC =  p.get<double>("y.junction");
    //get conc. of background doping
    for(unsigned int i=0;i<_profiles.size();i++)
      dop += property_to_real(_profiles[i]->property()) * _profiles[i]->profile(slice, YJUNC);

    //Can we even find a junction?
    if(dop*ion>0) {error_msg = "Junction can not be found"; return 1;}
    if(peak<=0.0) {error_msg = "Negtive peak value is not allowed"; return 1;}

    //Now convert junction depth into char. length
    YCHAR = (ymin-YJUNC)/sqrt(log(fabs(peak/dop)));
  }

  if(p.have_parameter<double>("x.char"))
    XCHAR = p.get<double>("x.char");
  else if(p.have_parameter<double>("xy.ratio"))
    XCHAR = YCHAR*p.get<double>("xy.ratio");
  else
    XCHAR = YCHAR;

  if(XCHAR<=0 || YCHAR<=0 ) {error_msg = "Negtive x.char or y.char value is not allowed"; return 1;}

  if(p.have_parameter<double>("dose"))
  {
    double dose = p.get<double>("dose");
    if(dose<0.0) {error_msg = "Negtive dose value is not allowed"; return 1;}
    peak=dose/(YCHAR*sqrt(3.14159265359));
  }
  if(peak<=0.0) {error_msg = "Negtive peak value is not allowed"; return 1;}

  Profile * f = new GaussProfile(label, property, xmin, xmax, ymax, ymin, peak, XCHAR, YCHAR);
  _profiles.push_back(f);

  error_msg = "";
  return 0;
}



int ProfileManager::add_profile_erf(const Parameters & p, std::string &error_msg)
{
  std::string label    = p.get<std::string>("label");
  std::string property = p.get<std::string>("property");

  double ion  = property_to_real(property);

  double xmax = p.get<double>("x.max");
  double xmin = p.get<double>("x.min");
  double ymax = p.get<double>("y.max");
  double ymin = p.get<double>("y.min");
  if(xmin-xmax>1e-8) {error_msg = "Xmin is larger than Xmax"; return 1;}
  if(ymin-ymax>1e-8) {error_msg = "Ymin is larger than Ymax"; return 1;}
  double slice = 0.5*(xmax+xmin);

  double peak = p.get<double>("n.peak");

  double YCHAR=0,XCHAR,YJUNC,dop=0;
  if(p.have_parameter<double>("y.char"))
    YCHAR = p.get<double>("y.char");
  else if(p.have_parameter<double>("y.junction"))
  {
    //Junction is an absolute location.
    YJUNC =  p.get<double>("y.junction");
    //get conc. of background doping
    for(unsigned int i=0;i<_profiles.size();i++)
      dop += property_to_real(_profiles[i]->property()) * _profiles[i]->profile(slice, YJUNC);

    //Can we even find a junction?
    if(dop*ion>0) {error_msg = "Junction can not be found"; return 1;}
    if(peak<=0.0) {error_msg = "Negtive peak value is not allowed"; return 1;}

    //Now convert junction depth into char. length
    YCHAR = (ymin-YJUNC)/sqrt(log(fabs(peak/dop)));
  }

  if(p.have_parameter<double>("x.char"))
    XCHAR = p.get<double>("x.char");
  else if(p.have_parameter<double>("xy.ratio"))
    XCHAR = YCHAR*p.get<double>("xy.ratio");
  else
    XCHAR = YCHAR;

  if(XCHAR<=0 || YCHAR<=0 ) {error_msg = "Negtive x.char or y.char value is not allowed"; return 1;}

  if(p.have_parameter<double>("dose"))
  {
    double dose = p.get<double>("dose");
    if(dose<0.0) {error_msg = "Negtive dose value is not allowed"; return 1;}
    peak=dose/(YCHAR*sqrt(3.14159265359));
  }
  if(peak<=0.0) {error_msg = "Negtive peak value is not allowed"; return 1;}


  Profile * f = new ErfProfile(label, property, xmin, xmax, ymax, ymin, peak, XCHAR, YCHAR);
  _profiles.push_back(f);

  error_msg = "";
  return 0;

}

void ProfileManager::delete_profile(const std::string &label)
{
  std::vector<const Profile * >::iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
    if ((*it)->label() == label)
    {
      delete (*it);
      _profiles.erase(it);
      return;
    }
}


double ProfileManager::profile(const std::string & property, double x, double y) const
{
  double q = 0;
  std::vector<const Profile * >::const_iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
  {
    if( (*it)->property() == property )
    q += property_to_real((*it)->property()) * (*it)->profile(x, y);
  }

  return q;
}


const Profile * ProfileManager::get_profile(unsigned int n) const
{ return _profiles[n]; }


const Profile * ProfileManager::get_profile(const std::string &label) const
{
  std::vector<const Profile * >::const_iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
  {
    if( (*it)->label() == label )
      return (*it);
  }
  return NULL;
}
