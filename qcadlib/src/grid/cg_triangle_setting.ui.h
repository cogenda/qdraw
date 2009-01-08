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
  if(AreaConstraint->isChecked())
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

  this->accept();
}


void TriangleSetting::init()
{
  _cmd_string = "pzV";

  MinimalAngleEdit->setText("20");
}
