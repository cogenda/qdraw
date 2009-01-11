/****************************************************************************
** Form interface generated from reading ui file 'grid/cg_triangle_setting.ui'
**
** Created by: The User Interface Compiler ($Id: qt/main.cpp   3.3.6   edited Aug 31 2005 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef TRIANGLESETTING_H
#define TRIANGLESETTING_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QGroupBox;
class QCheckBox;
class QLineEdit;
class QLabel;
class QPushButton;

class TriangleSetting : public QDialog
{
    Q_OBJECT

public:
    TriangleSetting( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TriangleSetting();

    QGroupBox* groupBox1;
    QCheckBox* AreaConstraint;
    QGroupBox* groupBox2;
    QCheckBox* ConformingDelaunay;
    QCheckBox* QualityControl;
    QLineEdit* MinimalAngleEdit;
    QLabel* textLabel1;
    QGroupBox* groupBox3;
    QCheckBox* NoPointInsert;
    QCheckBox* NoPointInsertMore;
    QPushButton* BOK;
    QPushButton* BCancel;

    const QString & get_cmd_string();
    void set_cmd_string( const QString & cmd );

public slots:
    void SlotCmdString();

protected:
    QString _cmd_string;

    QGridLayout* TriangleSettingLayout;
    QVBoxLayout* layout2;
    QGridLayout* groupBox1Layout;
    QSpacerItem* spacer1;
    QGridLayout* groupBox2Layout;
    QSpacerItem* spacer5;
    QGridLayout* groupBox3Layout;
    QSpacerItem* spacer6;
    QHBoxLayout* layout1;
    QSpacerItem* spacer2;
    QSpacerItem* spacer3;
    QSpacerItem* spacer4;

protected slots:
    virtual void languageChange();

private:
    void init();

};

#endif // TRIANGLESETTING_H
