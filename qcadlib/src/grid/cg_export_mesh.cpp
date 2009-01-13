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

#include "cg_export_mesh.h"

#include <qfiledialog.h>
#include <qmessagebox.h>

#include "rs_mesh.h"
#include "rs_string.h"
#include "rs_settings.h"
#include "rs_system.h"
#include "rs_graphic.h"
#include "rs_graphicview.h"



CG_MeshExport::CG_MeshExport(QWidget* parent, RS_GraphicView * gv)
    :_parent(parent), _gv(gv), _mesh(0)
{
}

void CG_MeshExport::write_mesh_to_tif()
{
  RS_Mesh * mesh = find_mesh_obj();
  if(mesh==NULL) return;

  RS_String file = getSaveFileName(_parent);

  if(file != "")
    mesh->export_mesh(file);
}


RS_Mesh * CG_MeshExport::find_mesh_obj()
{
  //find if mesh exist
  RS_Mesh * mesh = NULL;
  RS_Mesh * mesh_select = NULL;
  unsigned int n_mesh = 0;
  unsigned int n_mesh_select = 0;

  RS_Graphic * g = _gv->getGraphic();
  if(g==NULL) return NULL;

  //mesh all the entity on active layer
  for (RS_Entity* e=g->firstEntity(); e!=NULL; e=g->nextEntity())
    if(e->isOnActiveLayer() && e->isVisible() && e->rtti() == RS2::EntityMesh)
    {
      mesh = (RS_Mesh *)e;
      n_mesh++;
      if(e->isSelected())
      {
        mesh_select = (RS_Mesh *)e;
        n_mesh_select++;
      }
    }
  // only one mesh object exist
  if(mesh && n_mesh==1) return mesh;

  // return the select mesh
  if(mesh_select && n_mesh_select==1) return mesh_select;

  //errors
  if(mesh==NULL)
  {
    QMessageBox::critical( 0, "Mesh Export", "No mesh can be found");
    return NULL;
  }

  if(n_mesh_select>1)
  {
    QMessageBox::critical( 0, "Mesh Export", "More than one mesh select");
    return NULL;
  }

  return NULL;
}


/**
 * Shows a dialog for choosing a file name. Saving the file is up to
 * the caller.
 *
 *
 * @return File name with path and extension to determine the file type
 *         or an empty string if the dialog was cancelled.
 */
RS_String CG_MeshExport::getSaveFileName(QWidget* parent)
{
#ifndef QT_NO_FILEDIALOG
  // read default settings:
  RS_SETTINGS->beginGroup("/Paths");
  RS_String defDir = RS_SETTINGS->readEntry("/Save", RS_SYSTEM->getHomeDir());
  RS_SETTINGS->endGroup();

  // prepare file save as dialog:
  QFileDialog* fileDlg = new QFileDialog(parent,"Save as",true);
  QStringList filters;
  bool done = false;
  bool cancel = false;
  RS_String fn = "";

  filters.append("Technology interchange format (*.tif)");
  fileDlg->setFilters(filters);
  fileDlg->setMode(QFileDialog::AnyFile);
  fileDlg->setCaption(QObject::tr("Save Mesh As"));
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
        fn+=".tif";
      }

      // overwrite warning:
      if(QFileInfo(fn).exists())
      {
        int choice =
          QMessageBox::warning(parent, QObject::tr("Save Mesh As"),
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
    //RS_SETTINGS->writeEntry("/SaveFilter", fileDlg->selectedFilter());
    RS_SETTINGS->endGroup();
  }

  delete fileDlg;

  return fn;
#else
  return "";
#endif
}




