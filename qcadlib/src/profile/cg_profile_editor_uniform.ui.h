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

#include <string>

#include "cg_parameters.h"
#include "rs_math.h"

void ProfileEditorUniform::init()
{
  _profile = NULL;

  profile_property_combobox->insertItem(tr("Na"));
  profile_property_combobox->insertItem(tr("Nd"));

}


void ProfileEditorUniform::set_profile(const Profile * profile)
{
  _profile = profile;
  if(profile!=NULL)
  {
    profile_label_edit->setText(_profile->label().c_str());

    int item = 0;
    QString property = _profile->property().c_str();
    for(int n=0; n<profile_property_combobox->count(); n++)
      if(property==profile_property_combobox->text(n)) item=n;
    profile_property_combobox->setCurrentItem(item);

    QString s;
    s.setNum(_profile->xmin());
    x0_edit->setText(s);
    s.setNum(_profile->xmax());
    y0_edit->setText(s);
    s.setNum(_profile->ymin());
    x1_edit->setText(s);
    s.setNum(_profile->xmax());
    y1_edit->setText(s);
    s.setNum(_profile->peak());
    concentration_edit->setText(s);
  }
}

void ProfileEditorUniform::add_profile_to_profilemanager(ProfileManager * pm)
{
  Parameters p;
  p.set<std::string>( "type" ) = "Unoform";

  p.set<double>( "x.min" )  = RS_Math::eval(x0_edit->text()) ;
  p.set<double>( "y.min" )  = RS_Math::eval(y0_edit->text()) ;
  p.set<double>( "x.max" )  = RS_Math::eval(x1_edit->text()) ;
  p.set<double>( "y.max" )  = RS_Math::eval(y1_edit->text()) ;

  p.set<double>( "n.peak" ) = RS_Math::eval(concentration_edit->text()) ;

  p.set<std::string>( "label" )    = profile_label_edit->text().ascii() ;
  p.set<std::string>( "property" ) = profile_property_combobox->currentText().ascii() ;

  //delete old profile if exist
  pm->delete_profile(profile_label_edit->text().ascii());

  //create new profile
  std::string error_msg;
  int error_code = pm->add_profile(p, error_msg);

  _profile = pm->get_profile(profile_label_edit->text().ascii());
}



const Profile * ProfileEditorUniform::get_profile()
{
  return _profile;
}
