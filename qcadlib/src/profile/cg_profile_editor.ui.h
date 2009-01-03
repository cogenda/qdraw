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
#include "cg_profile_editor_gauss.h"
#include "cg_profile_editor_erf.h"


void ProfileEditor::SlotAdd( const QString & type )
{
  if(type == "Add Uniform")
  {
    ProfileEditorUniform  profile_uniform;
    profile_uniform.set_profile(_pm, NULL);
    if (profile_uniform.exec() == QDialog::Accepted)
      ProfileList->insertItem(profile_uniform.get_profile()->label().c_str());
    return;
  }

  if(type == "Add Gauss")
  {
    ProfileEditorGauss  profile_gauss;
    profile_gauss.set_profile(_pm, NULL);
    if (profile_gauss.exec() == QDialog::Accepted)
      ProfileList->insertItem(profile_gauss.get_profile()->label().c_str());
    return;
  }

  if(type == "Add Erf")
  {
    ProfileEditorErf  profile_erf;
    profile_erf.set_profile(_pm, NULL);
    if (profile_erf.exec() == QDialog::Accepted)
      ProfileList->insertItem(profile_erf.get_profile()->label().c_str());
    return;
  }
}


void ProfileEditor::SlotEdit()
{
  int current_item = ProfileList->currentItem();

  if( current_item!= -1)
  {
    QString current_profile = ProfileList->currentText();
    const Profile * profile = _pm->get_profile(current_profile.ascii());

    if( profile->type() == "Uniform" )
    {
      ProfileEditorUniform  profile_uniform;
      profile_uniform.set_profile(_pm, profile);

      if( profile_uniform.exec() == QDialog::Accepted )
      {
        ProfileList->removeItem(ProfileList->currentItem());
        ProfileList->insertItem(profile_uniform.get_profile()->label().c_str());
      }
      return;
    }

    if( profile->type() == "Gauss" )
    {
      ProfileEditorGauss  profile_gauss;
      profile_gauss.set_profile(_pm, profile);

      if( profile_gauss.exec() == QDialog::Accepted )
      {
        ProfileList->removeItem(ProfileList->currentItem());
        ProfileList->insertItem(profile_gauss.get_profile()->label().c_str());
      }
      return;
    }

    if( profile->type() == "Erf" )
    {
      ProfileEditorErf  profile_erf;
      profile_erf.set_profile(_pm, profile);

      if( profile_erf.exec() == QDialog::Accepted )
      {
        ProfileList->removeItem(ProfileList->currentItem());
        ProfileList->insertItem(profile_erf.get_profile()->label().c_str());
      }
      return;
    }
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

  for(unsigned int n=0; n<_pm->n_profiles(); ++n)
    ProfileList->insertItem(_pm->get_profile(n)->label().c_str());
}



