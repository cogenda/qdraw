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

void ProfileEditor::SlotAdd( const QString & type )
{
    std::cout<<type<<std::endl;
}


void ProfileEditor::SlotEdit()
{

}


void ProfileEditor::SlotDelete()
{

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
}



