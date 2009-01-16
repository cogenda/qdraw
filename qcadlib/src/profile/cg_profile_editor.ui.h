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

#include <qfiledialog.h>
#include <qmessagebox.h>

#include "rs_settings.h"
#include "rs_system.h"
#include "cg_profile_editor_uniform.h"
#include "cg_profile_editor_gauss.h"
#include "cg_profile_editor_erf.h"


void ProfileEditor::SlotAdd( const QString & type )
{
  if(type == tr("Add Uniform"))
  {
    ProfileEditorUniform  profile_uniform;
    profile_uniform.set_profile(_pm, NULL);
    if (profile_uniform.exec() == QDialog::Accepted)
      ProfileList->insertItem(profile_uniform.get_profile()->label().c_str());
    return;
  }

  if(type == tr("Add Gauss"))
  {
    ProfileEditorGauss  profile_gauss;
    profile_gauss.set_profile(_pm, NULL);
    if (profile_gauss.exec() == QDialog::Accepted)
      ProfileList->insertItem(profile_gauss.get_profile()->label().c_str());
    return;
  }

  if(type == tr("Add Erf"))
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
  QString file = getLoadFileName();
  if(file != "")
  {
    bool append=false;
    if(_pm->n_profiles())
    {
      int choice =
          QMessageBox::warning(this, QObject::tr("Load Profile"),
                               QObject::tr("Replace or Append to existing profile list"),
                               QObject::tr("Replace"), QObject::tr("Append"),
                               QObject::tr("Cancel"), 0, 1 );
      if(choice == 0) append=false;
      if(choice == 1) append=true;
      if(choice == 2) return;
    }
    _pm->load_profiles_from_file(file.ascii(), append);

    ProfileList->clear();
    for(unsigned int n=0; n<_pm->n_profiles(); ++n )
      ProfileList->insertItem(_pm->get_profile(n)->label().c_str());
  }
}


void ProfileEditor::SlotSave()
{
  QString file = getSaveFileName();
  if(file!="") _pm->save_profiles_to_file(file.ascii());
}


void ProfileEditor::init()
{}


void ProfileEditor::set_profile_maneger( ProfileManager * pm )
{
  _pm = pm;

  for(unsigned int n=0; n<_pm->n_profiles(); ++n)
    ProfileList->insertItem(_pm->get_profile(n)->label().c_str());
}





QString ProfileEditor::getLoadFileName()
{
#ifndef QT_NO_FILEDIALOG

  // read default settings:
  RS_SETTINGS->beginGroup("/Paths");
  RS_String defDir = RS_SETTINGS->readEntry("/Open",  RS_SYSTEM->getHomeDir());
  RS_SETTINGS->endGroup();

  RS_String fn = "";
  bool cancel = false;

  QFileDialog* fileDlg = new QFileDialog(this, tr("File Dialog"), true);

  QStringList filters;
  filters.append("Profile Data (*.pf)");

  fileDlg->setFilters(filters);
  fileDlg->setMode(QFileDialog::ExistingFile);
  fileDlg->setCaption(QObject::tr("Load Profile"));
  fileDlg->setDir(defDir);

  if (fileDlg->exec()==QDialog::Accepted)
  {
    fn = fileDlg->selectedFile();
    fn = QDir::convertSeparators( QFileInfo(fn).absFilePath() );
    cancel = false;
  }
  else
  {
    cancel = true;
  }

  // store new default settings:
  if (!cancel)
  {
    RS_SETTINGS->beginGroup("/Paths");
    RS_SETTINGS->writeEntry("/Open", QFileInfo(fn).dirPath(true));
    RS_SETTINGS->endGroup();
  }

  delete fileDlg;

  return fn;
#else
  return "";
#endif

}


QString ProfileEditor::getSaveFileName()
{
#ifndef QT_NO_FILEDIALOG
  // read default settings:
  RS_SETTINGS->beginGroup("/Paths");
  RS_String defDir = RS_SETTINGS->readEntry("/Save", RS_SYSTEM->getHomeDir());
  RS_SETTINGS->endGroup();

  // prepare file save as dialog:
  QFileDialog* fileDlg = new QFileDialog(this,tr("Save as"),true);
  QStringList filters;
  bool done = false;
  bool cancel = false;
  RS_String fn = "";

  filters.append("Profile Data (*.pf)");
  fileDlg->setFilters(filters);
  fileDlg->setMode(QFileDialog::AnyFile);
  fileDlg->setCaption(QObject::tr("Save Profile As"));
  fileDlg->setDir(defDir);

  // run dialog:
  do
  {
    // accepted:
    if (fileDlg->exec()==QDialog::Accepted)
    {
      fn = fileDlg->selectedFile();
      fn = QDir::convertSeparators( QFileInfo(fn).absFilePath() );
      cancel = false;

      // append default extension:
      if (QFileInfo(fn).fileName().find('.')==-1)
      {
        fn+=".pf";
      }

      // overwrite warning:
      if(QFileInfo(fn).exists())
      {
        int choice =
          QMessageBox::warning(this, QObject::tr("Save Profile As"),
                               QObject::tr("%1 already exists.\n"
                                           "Do you want to replace it?")
                               .arg(fn),
                               QObject::tr("Yes"), QObject::tr("No"),
                               QObject::tr("Cancel"), 0, 1 );

        switch (choice)
        {
        case 0:
          done = true;
          break;
        case 1:
        case 2:
        default:
          done = false;
          break;
        }
      }
      else
      {
        done = true;
      }
    }
    else
    {
      done = true;
      cancel = true;
      fn = "";
    }
  }
  while(!done);


  // store new default settings:
  if (!cancel)
  {
    RS_SETTINGS->beginGroup("/Paths");
    RS_SETTINGS->writeEntry("/Save", QFileInfo(fn).dirPath(true));
    RS_SETTINGS->endGroup();
  }

  delete fileDlg;

  return fn;
#else
  return "";
#endif

}
