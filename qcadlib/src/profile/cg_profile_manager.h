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

#ifndef __cg_profile_manager_h__
#define __cg_profile_manager_h__

#include <vector>

class Profile;
class Parameters;


class ProfileManager
{
public:
  /**
   *  constructor, do nothing
   */
  ProfileManager() {}

  /**
   * destructor, free the doping functions
   */
  ~ProfileManager();

  /**
   * create profile with user specified parameters
   */
  int add_profile(const Parameters &, std::string &error_msg);

  /**
   * remove existing profile by its label
   */
  void delete_profile(const std::string &label);

  /**
   * @return how many profiles exist
   */
  unsigned int n_profiles() const
  { return _profiles.size(); }

  /**
   *@return nth profile
   */
  const Profile * get_profile(unsigned int n) const;

  /**
   *@return profile by its label
   */
  const Profile * get_profile(const std::string &label) const;

  /**
   * get the profile quatity at given point (x,y)
   */
  double profile(const std::string &property, double x, double y) const;

  /**
   * load profile information from file
   */
  void load_profiles_from_file(const std::string &filename);

  /**
   * save profile information to external files
   */
  void save_profiles_to_file(const std::string &filename);


private:

 double property_to_real(const std::string &property) const;

 int add_profile_uniform(const Parameters & p, std::string &error_msg);

 int add_profile_gauss(const Parameters & p, std::string &error_msg);

 int add_profile_erf(const Parameters & p, std::string &error_msg);

 /**
  * the pointer vector to Profile
  */
 std::vector<const Profile * >  _profiles;

};


#endif // #define __cg_profile_manager_h__
