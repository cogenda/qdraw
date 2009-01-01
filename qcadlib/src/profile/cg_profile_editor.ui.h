/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <iostream>
#include "cg_profile_editor_uniform.h"

void ProfileEditor::SlotAdd( const QString & type )
{
    Profile * profile = NULL;

    if(type == "Add Uniform")
    {
      ProfileEditorUniform  profile_uniform;
      profile_uniform.set_profile(profile);
      if(profile_uniform.exec() == QDialog::Accepted)
        profile_uniform.add_profile_to_profilemanager(_pm);
      ProfileList->insertItem(profile_uniform.get_profile()->label().c_str());
    }

    if(type == "Add Gauss");
    if(type == "Add Erf");
}


void ProfileEditor::SlotEdit()
{
  QString current_profile = ProfileList->currentText();
  const Profile * profile = _pm->get_profile(current_profile.ascii());

  if( profile->type() == "Uniform" )
  {
      ProfileEditorUniform  profile_uniform;
      profile_uniform.set_profile(profile);
      if(profile_uniform.exec() == QDialog::Accepted)
        profile_uniform.add_profile_to_profilemanager(_pm);
  }

}


void ProfileEditor::SlotDelete()
{
  QString current_profile = ProfileList->currentText();
  _pm->delete_profile(current_profile.ascii());
  ProfileList->removeItem(ProfileList->currentItem());
}


void ProfileEditor::SlotLoad()
{

}


void ProfileEditor::SlotSave()
{

}


void ProfileEditor::init()
{

}


void ProfileEditor::set_profile_maneger( ProfileManager * pm )
{
    _pm = pm;

    for(int n=0; n<_pm->n_profiles(); ++n)
      ProfileList->insertItem(_pm->get_profile(n)->label().c_str());
}



