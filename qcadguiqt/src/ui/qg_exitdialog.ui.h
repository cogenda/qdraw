/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

#include <qmessagebox.h>
#include <qaccel.h>

//sets aditional accel, eg. Key_A for ALT+Key_A
static void makeLetterAccell(QButton *btn)
{
    if (btn->accel().count()<1)
        return;
    QAccel *a = new QAccel( btn );
    a->connectItem( a->insertItem( btn->accel() & ~(Qt::MODIFIER_MASK|Qt::UNICODE_ACCEL) ),
                    btn, SLOT(animateClick()) );
}

void QG_ExitDialog::init()
{
    //set dlg icon
    QMessageBox mb("","",QMessageBox::Warning, QMessageBox::Ok, QMessageBox::NoButton, QMessageBox::NoButton);
    l_icon->setPixmap( *mb.iconPixmap() );
    bLeave->setIconSet(QPixmap::fromMimeSource("fileclose.png"));
    makeLetterAccell( bLeave );
    bSave->setIconSet(QPixmap::fromMimeSource("filesave2.png"));
    makeLetterAccell( bSave );
    bSaveAs->setIconSet(QPixmap::fromMimeSource("filesaveas.png"));
    makeLetterAccell( bSaveAs );
    makeLetterAccell( bCancel );
}

void QG_ExitDialog::setText(const QString& text) {
    lQuestion->setText(text);
    resize(lQuestion->sizeHint().width()+32,
           lQuestion->sizeHint().height() + layButtons->sizeHint().height()+32);
}

void QG_ExitDialog::setTitle(const QString& text) {
    setCaption(text);
}

void QG_ExitDialog::setForce(bool force) {
     bCancel->setDisabled(force);
}

void QG_ExitDialog::slotSaveAs() {
    done(3);
}

void QG_ExitDialog::slotSave() {
    done(2);
}

 
