/****************************************************************************
** Form implementation generated from reading ui file 'grid/cg_triangle_setting.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "cg_triangle_setting.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "cg_triangle_setting.ui.h"

/*
 *  Constructs a TriangleSetting as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
TriangleSetting::TriangleSetting( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "TriangleSetting" );
    TriangleSettingLayout = new QGridLayout( this, 1, 1, 11, 6, "TriangleSettingLayout"); 

    layout2 = new QVBoxLayout( 0, 0, 6, "layout2"); 

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 11 );
    groupBox1Layout = new QGridLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    AreaConstraint = new QCheckBox( groupBox1, "AreaConstraint" );

    groupBox1Layout->addWidget( AreaConstraint, 0, 0 );
    spacer1 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer1, 0, 1 );
    layout2->addWidget( groupBox1 );

    groupBox2 = new QGroupBox( this, "groupBox2" );
    groupBox2->setColumnLayout(0, Qt::Vertical );
    groupBox2->layout()->setSpacing( 6 );
    groupBox2->layout()->setMargin( 11 );
    groupBox2Layout = new QGridLayout( groupBox2->layout() );
    groupBox2Layout->setAlignment( Qt::AlignTop );

    ConformingDelaunay = new QCheckBox( groupBox2, "ConformingDelaunay" );

    groupBox2Layout->addMultiCellWidget( ConformingDelaunay, 1, 1, 0, 1 );

    QualityControl = new QCheckBox( groupBox2, "QualityControl" );

    groupBox2Layout->addWidget( QualityControl, 0, 0 );

    MinimalAngleEdit = new QLineEdit( groupBox2, "MinimalAngleEdit" );

    groupBox2Layout->addWidget( MinimalAngleEdit, 0, 3 );
    spacer5 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox2Layout->addItem( spacer5, 0, 1 );

    textLabel1 = new QLabel( groupBox2, "textLabel1" );

    groupBox2Layout->addWidget( textLabel1, 0, 2 );
    layout2->addWidget( groupBox2 );

    groupBox3 = new QGroupBox( this, "groupBox3" );
    groupBox3->setColumnLayout(0, Qt::Vertical );
    groupBox3->layout()->setSpacing( 6 );
    groupBox3->layout()->setMargin( 11 );
    groupBox3Layout = new QGridLayout( groupBox3->layout() );
    groupBox3Layout->setAlignment( Qt::AlignTop );
    spacer6 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox3Layout->addItem( spacer6, 0, 1 );

    NoPointInsert = new QCheckBox( groupBox3, "NoPointInsert" );

    groupBox3Layout->addWidget( NoPointInsert, 0, 0 );

    NoPointInsertMore = new QCheckBox( groupBox3, "NoPointInsertMore" );

    groupBox3Layout->addWidget( NoPointInsertMore, 0, 2 );
    layout2->addWidget( groupBox3 );

    layout1 = new QHBoxLayout( 0, 0, 6, "layout1"); 
    spacer2 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer2 );

    BOK = new QPushButton( this, "BOK" );
    layout1->addWidget( BOK );
    spacer3 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer3 );

    BCancel = new QPushButton( this, "BCancel" );
    layout1->addWidget( BCancel );
    spacer4 = new QSpacerItem( 40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout1->addItem( spacer4 );
    layout2->addLayout( layout1 );

    TriangleSettingLayout->addLayout( layout2, 0, 0 );
    languageChange();
    resize( QSize(518, 268).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( BCancel, SIGNAL( clicked() ), this, SLOT( reject() ) );
    connect( BOK, SIGNAL( clicked() ), this, SLOT( SlotCmdString() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
TriangleSetting::~TriangleSetting()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void TriangleSetting::languageChange()
{
    setCaption( tr( "Mesh Setting" ) );
    groupBox1->setTitle( tr( "Triangle Area Constraint" ) );
    AreaConstraint->setText( tr( "Enable Triangle Area Constraint" ) );
    groupBox2->setTitle( tr( "Quality Delaunay Mesh" ) );
    ConformingDelaunay->setText( tr( "Generate Conforming Delaunay Mesh" ) );
    QualityControl->setText( tr( "Generate Quality Mesh" ) );
    textLabel1->setText( tr( "Minimal Angle:" ) );
    groupBox3->setTitle( tr( "Constrained Delaunay Mesh" ) );
    NoPointInsert->setText( tr( "No Point insertion on Boundary" ) );
    NoPointInsertMore->setText( tr( "No Point Insert on any Segment" ) );
    BOK->setText( tr( "OK" ) );
    BCancel->setText( tr( "Cancel" ) );
}

