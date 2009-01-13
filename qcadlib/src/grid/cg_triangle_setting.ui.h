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
#include "rs_math.h"

const QString & TriangleSetting::get_cmd_string()
{
  return _cmd_string;
}


void TriangleSetting::set_cmd_string( const QString & cmd )
{
   _cmd_string = cmd;
}


void TriangleSetting::SlotCmdString()
{
  if(!_refine && AreaConstraint->isChecked())
    _cmd_string += "a";

  if(QualityControl->isChecked())
  {
    _cmd_string += "q" + MinimalAngleEdit->text();
  }

  if(ConformingDelaunay->isChecked())
    _cmd_string += "D";

  if(NoPointInsert->isChecked())
    _cmd_string += "Y";

  if(NoPointInsertMore->isChecked())
    _cmd_string += "Y";

  _max_dispersion = RS_Math::eval(MaxDispersion->text());
  _signed_log = SignedLog->isChecked();

  this->accept();
}


void TriangleSetting::init()
{

}


bool TriangleSetting::measure_with_signed_log()
{
   return _signed_log;
}


double TriangleSetting::get_dispersion()
{
  return _max_dispersion;
}


void TriangleSetting::init_mesh( const QString & cmd )
{
  _refine = false;
  _cmd_string = "pzAV";
  groupBox4->setEnabled(false);
  MinimalAngleEdit->setText("20");
}


void TriangleSetting::init_refine( const QString & cmd )
{
  _refine = true;
  _cmd_string = "pzraV";
  AreaConstraint->setChecked(true);
  groupBox1->setEnabled(false);
  MeshRefinement->setChecked(true);
  MinimalAngleEdit->setText("20");
  MaxDispersion->setText("3");
  SignedLog->setChecked(true);
}
