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
#include <qmessagebox.h>
#include <string>

#include "cg_parameters.h"
#include "rs_math.h"

void ProfileEditorUniform::init()
{
  _pm = NULL;
  _profile = NULL;

  profile_property_combobox->insertItem(tr("Na"));
  profile_property_combobox->insertItem(tr("Nd"));

}


void ProfileEditorUniform::set_profile(ProfileManager * pm,  const Profile * profile)
{
  _pm = pm;
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
    s.setNum(_profile->ymin());
    y0_edit->setText(s);

    s.setNum(_profile->xmax());
    x1_edit->setText(s);
    s.setNum(_profile->ymax());
    y1_edit->setText(s);
    s.setNum(_profile->peak());
    concentration_edit->setText(s);
  }
}


void ProfileEditorUniform::add_profile_to_profilemanager()
{
  Parameters p;
  p.set<std::string>( "type" ) = "Uniform";

  p.set<double>( "x.min" )  = RS_Math::eval(x0_edit->text()) ;
  p.set<double>( "y.min" )  = RS_Math::eval(y0_edit->text()) ;
  p.set<double>( "x.max" )  = RS_Math::eval(x1_edit->text()) ;
  p.set<double>( "y.max" )  = RS_Math::eval(y1_edit->text()) ;

  p.set<double>( "n.peak" ) = RS_Math::eval(concentration_edit->text()) ;

  p.set<std::string>( "label" )    = profile_label_edit->text().latin1() ;
  p.set<std::string>( "property" ) = profile_property_combobox->currentText().latin1() ;

  //remove old profile from pm if exist
  if(_profile) _pm->remove_profile(_profile->label());

  //create new profile
  std::string error_msg;
  if( _pm->add_profile(p, error_msg) )
  {
    // create new profile failed
    QMessageBox::critical( 0, tr("Set Uniform Profile"), error_msg.c_str());
    if(_profile) _pm->insert_profile(_profile);
  }
  else
  {
    if(_profile) delete _profile;
    _profile = _pm->get_profile(profile_label_edit->text().latin1());
    assert(_profile);
    this->accept();
  }
}



const Profile * ProfileEditorUniform::get_profile()
{
  return _profile;
}
