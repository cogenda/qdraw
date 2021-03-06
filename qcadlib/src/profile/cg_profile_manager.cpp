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
#include <fstream>
#include <sstream>

#include "cg_profile.h"
#include "cg_profile_manager.h"
#include "cg_parameters.h"

ProfileManager::~ProfileManager()
{
  clear();
}

void ProfileManager::clear()
{
  for(unsigned int n=0; n<_profiles.size(); ++n)
    delete _profiles[n];
  _profiles.clear();
}


int ProfileManager::add_profile(const Parameters & p, std::string &error_msg)
{
  std::string type = p.get<std::string>("type");
  if(type == "Uniform")
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
  if( get_profile(label) ) {error_msg = "Profile with label " + label + " already exist"; return 1;}

  std::string property = p.get<std::string>("property");

  double xmax = p.get<double>("x.max");
  double xmin = p.get<double>("x.min");
  double ymax = p.get<double>("y.max");
  double ymin = p.get<double>("y.min");
  double peak = p.get<double>("n.peak");


  if(xmin-xmax>1e-8) {error_msg = "Xmin is larger than Xmax"; return 1;}
  if(ymin-ymax>1e-8) {error_msg = "Ymin is larger than Ymax"; return 1;}

  Profile * f = new UniformProfile(label, property, xmin, xmax, ymin, ymax, peak);
  insert_profile(f);

  error_msg = "";
  return 0;
}


int ProfileManager::add_profile_gauss(const Parameters & p, std::string &error_msg)
{
  std::string label    = p.get<std::string>("label");
  if( get_profile(label) ) {error_msg = "Profile with label " + label + " already exist"; return 1;}

  std::string property = p.get<std::string>("property");

  double ion  = property_to_real(property);

  double xmax = p.get<double>("x.max");
  double xmin = p.get<double>("x.min");
  double ymax = p.get<double>("y.max");
  double ymin = p.get<double>("y.min");
  if(xmin-xmax>1e-8) {error_msg = "Xmin is larger than Xmax"; return 1;}
  if(ymin-ymax>1e-8) {error_msg = "Ymin is larger than Ymax"; return 1;}
  double slice = 0.5*(xmax+xmin);

  double peak=0;

  if(p.have_parameter<double>("n.peak"))
    peak = p.get<double>("n.peak");

  double YCHAR=0,XCHAR=0,YJUNC,dop=0;
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
    if(YJUNC<ymin)
      YCHAR = (ymin-YJUNC)/sqrt(log(fabs(peak/dop)));
    else if(YJUNC>ymax)
      YCHAR = (YJUNC-ymax)/sqrt(log(fabs(peak/dop)));
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

  Profile * f = new GaussProfile(label, property, xmin, xmax, ymin, ymax, peak, XCHAR, YCHAR);
  insert_profile(f);

  if(p.have_parameter<double>("xy.ratio"))
    f->set_xy_ratio(p.get<double>("xy.ratio"));

  if(p.have_parameter<double>("dose"))
    f->set_dose(p.get<double>("dose"));

  if(p.have_parameter<double>("y.junction"))
    f->set_y_junction(p.get<double>("y.junction"));

  error_msg = "";
  return 0;
}



int ProfileManager::add_profile_erf(const Parameters & p, std::string &error_msg)
{
  std::string label    = p.get<std::string>("label");
  if( get_profile(label) ) {error_msg = "Profile with label " + label + " already exist"; return 1;}

  std::string property = p.get<std::string>("property");

  double ion  = property_to_real(property);

  double xmax = p.get<double>("x.max");
  double xmin = p.get<double>("x.min");
  double ymax = p.get<double>("y.max");
  double ymin = p.get<double>("y.min");
  if(xmin-xmax>1e-8) {error_msg = "Xmin is larger than Xmax"; return 1;}
  if(ymin-ymax>1e-8) {error_msg = "Ymin is larger than Ymax"; return 1;}
  double slice = 0.5*(xmax+xmin);

  double peak=0;

  if(p.have_parameter<double>("n.peak"))
    peak = p.get<double>("n.peak");

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
    if(YJUNC<ymin)
      YCHAR = (ymin-YJUNC)/sqrt(log(fabs(peak/dop)));
    else if(YJUNC>ymax)
      YCHAR = (YJUNC-ymax)/sqrt(log(fabs(peak/dop)));
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


  Profile * f = new ErfProfile(label, property, xmin, xmax, ymin, ymax, peak, XCHAR, YCHAR);
  insert_profile(f);

  if(p.have_parameter<double>("xy.ratio"))
    f->set_xy_ratio(p.get<double>("xy.ratio"));

  if(p.have_parameter<double>("dose"))
    f->set_dose(p.get<double>("dose"));

  if(p.have_parameter<double>("y.junction"))
    f->set_y_junction(p.get<double>("y.junction"));

  error_msg = "";
  return 0;

}


void ProfileManager::insert_profile(const Profile * profile)
{
  if( _profile_map.find(profile->label()) != _profile_map.end() )
    delete_profile(profile->label());
  _profiles.push_back(profile);
  _profile_map[profile->label()] = profile;
}


void ProfileManager::delete_profile(const std::string &label)
{
  std::vector<const Profile * >::iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
    if ((*it)->label() == label)
    {
      delete (*it);
      _profiles.erase(it);
      break;
    }

  if(  _profile_map.find(label) != _profile_map.end() )
    _profile_map.erase(label);

}


const Profile * ProfileManager::remove_profile(const std::string &label)
{
  const Profile * profile = NULL;

  std::vector<const Profile * >::iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
    if ((*it)->label() == label)
    {
      profile = (*it);
      _profiles.erase(it);
      break;
    }

  if(  _profile_map.find(label) != _profile_map.end() )
    _profile_map.erase(label);

  return profile;
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


double ProfileManager::profile(double x, double y) const
{
  double q = 0;
  std::vector<const Profile * >::const_iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
  {
      q += property_to_real((*it)->property()) * (*it)->profile(x, y);
  }

  return q;
}


const Profile * ProfileManager::get_profile(unsigned int n) const
{ return _profiles[n]; }


const Profile * ProfileManager::get_profile(const std::string &label) const
{
  if(  _profile_map.find(label) != _profile_map.end() )
    return (*_profile_map.find(label)).second;

  return NULL;
}



void ProfileManager::load_profiles_from_file(const std::string &filename, bool append)
{
  if(!append) clear();

  std::ifstream  in(filename.c_str());
  while(!in.eof())
  {
    std::string line;
    std::string type;
    std::getline(in,line);
    std::stringstream ss;
    ss << line;
    ss >> type;
    if (type == "Uniform")
    {
      Profile * f = new UniformProfile(line);
      insert_profile(f);
    }

    if (type == "Gauss")
    {
      Profile * f = new GaussProfile(line);
      insert_profile(f);
    }

    if (type == "Erf")
    {
      Profile * f = new ErfProfile(line);
      insert_profile(f);
    }
  }
  in.close();
}


void ProfileManager::save_profiles_to_file(const std::string &filename)
{
  std::ofstream fout;
  fout.open(filename.c_str(), std::ofstream::trunc);

  std::vector<const Profile * >::iterator it = _profiles.begin();
  for(; it != _profiles.end(); ++it)
    fout<<(*it)->format_string();

  fout.close();
}
