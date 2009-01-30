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


void QG_DlgMesh::setMesh( RS_Mesh & e )
{
  mesh = &e;

  rb_outline->setChecked(mesh->draw_outline());
  rb_mesh->setChecked(mesh->draw_mesh());
  rb_material->setChecked(mesh->draw_material());

  Contour->setChecked(mesh->draw_contour());

  QString s;
  s.setNum(mesh->contour_number());
  ContourLine->setText(s);
  ContourLine->setEnabled(false);
  ContorWithSignedLog->setChecked(mesh->contour_with_signed_log());
}


void QG_DlgMesh::updateMesh()
{
  mesh->draw_outline() = rb_outline->isChecked();
  mesh->draw_mesh() = rb_mesh->isChecked();
  mesh->draw_material() = rb_material->isChecked();
  mesh->draw_contour() = Contour->isChecked();
  mesh->contour_number() = RS_Math::eval(ContourLine->text());
  mesh->contour_with_signed_log() = ContorWithSignedLog->isChecked();
}
