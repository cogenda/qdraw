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
#include "rs_settings.h"

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


  RS_SETTINGS->beginGroup("/Mesh");
  if(!_refine)
    RS_SETTINGS->writeEntry("/AreaConstraint", AreaConstraint->isChecked());
  RS_SETTINGS->writeEntry("/Quality", QualityControl->isChecked());
  RS_SETTINGS->writeEntry("/Delaunay", ConformingDelaunay->isChecked());
  RS_SETTINGS->writeEntry("/MinimalAngle", MinimalAngleEdit->text());
  RS_SETTINGS->writeEntry("/NoPointInsert", NoPointInsert->isChecked());
  RS_SETTINGS->writeEntry("/NoPointInsertMore", NoPointInsertMore->isChecked());
  if(_refine)
  {
    RS_SETTINGS->writeEntry("/MaxDispersion", MaxDispersion->text());
    RS_SETTINGS->writeEntry("/DispersionWithSignedLog", SignedLog->isChecked());
  }
  RS_SETTINGS->endGroup();

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
  _cmd_string = "pzAQ";
  groupBox4->setEnabled(false);

  RS_SETTINGS->beginGroup("/Mesh");
  QString area = RS_SETTINGS->readEntry("/AreaConstraint", "F");
  QString quality = RS_SETTINGS->readEntry("/Quality", "T");
  QString delaunay = RS_SETTINGS->readEntry("/Delaunay", "T");
  QString minimal_angle = RS_SETTINGS->readEntry("/MinimalAngle", "20");
  QString no_point_insert1 = RS_SETTINGS->readEntry("/NoPointInsert", "F");
  QString no_point_insert2 = RS_SETTINGS->readEntry("/NoPointInsertMore", "F");
  RS_SETTINGS->endGroup();

  AreaConstraint->setChecked(area=="T");
  ConformingDelaunay->setChecked(delaunay=="T");
  QualityControl->setChecked(quality=="T");
  MinimalAngleEdit->setText(minimal_angle);
  NoPointInsert->setChecked(no_point_insert1=="T");
  NoPointInsertMore->setChecked(no_point_insert2=="T");
}


void TriangleSetting::init_refine( const QString & cmd )
{
  _refine = true;
  _cmd_string = "pzraQ";
  AreaConstraint->setChecked(true);
  groupBox1->setEnabled(false);
  MeshRefinement->setChecked(true);
  MeshRefinement->setEnabled(false);

  RS_SETTINGS->beginGroup("/Mesh");
  QString quality = RS_SETTINGS->readEntry("/Quality", "T");
  QString delaunay = RS_SETTINGS->readEntry("/Delaunay", "T");
  QString minimal_angle = RS_SETTINGS->readEntry("/MinimalAngle", "20");
  QString no_point_insert1 = RS_SETTINGS->readEntry("/NoPointInsert", "F");
  QString no_point_insert2 = RS_SETTINGS->readEntry("/NoPointInsertMore", "F");
  QString max_dispersion = RS_SETTINGS->readEntry("/MaxDispersion", "3");
  QString signed_log = RS_SETTINGS->readEntry("/DispersionWithSignedLog", "T");
  RS_SETTINGS->endGroup();

  ConformingDelaunay->setChecked(delaunay=="T");
  QualityControl->setChecked(quality=="T");
  MinimalAngleEdit->setText(minimal_angle);
  NoPointInsert->setChecked(no_point_insert1=="T");
  NoPointInsertMore->setChecked(no_point_insert2=="T");
  MaxDispersion->setText(max_dispersion);
  SignedLog->setChecked(signed_log=="T");

}
