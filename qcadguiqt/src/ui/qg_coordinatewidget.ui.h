/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename slots use Qt Designer which will
** update this file, preserving your code. Create an init() slot in place of
** a constructor, and a destroy() slot in place of a destructor.
*****************************************************************************/

void QG_CoordinateWidget::init() {
    lCoord1->setText("");
    lCoord2->setText("");
    lCoord1b->setText("");
    lCoord2b->setText("");
    
    int fsize;
#ifdef __APPLE__
    fsize = 9;
#else
    fsize = 7;
#endif
    
    RS_SETTINGS->beginGroup("/Appearance");
    fsize = RS_SETTINGS->readNumEntry("/StatusBarFontSize", fsize);
    RS_SETTINGS->endGroup();
    
    lCoord1->setFont(QFont("Helvetica", fsize));
    lCoord1b->setFont(QFont("Helvetica", fsize));
    lCoord2->setFont(QFont("Helvetica", fsize));
    lCoord2b->setFont(QFont("Helvetica", fsize));
    
    graphic = NULL;
    prec = 4;
    format = RS2::Decimal;
    aprec = 2;
    aformat = RS2::DegreesDecimal;
}

void QG_CoordinateWidget::setGraphic(RS_Graphic* graphic) {
    this->graphic = graphic;
    
    setCoordinates(RS_Vector(0.0,0.0), RS_Vector(0.0,0.0), true);
}

/*void QG_CoordinateWidget::setAbsCoordinates(double x, double y) {
    setAbsCoordinates(RS_Vector(x, y));
}

void QG_CoordinateWidget::setAbsCoordinates(const RS_Vector& v) {
    QString str;
    
    str.sprintf("%.4f / %.4f", v.x, v.y);
    lCoord1->setText(str);
    
    double ang = RS_Math::rad2deg(v.angle());
    double rad = v.magnitude();
    str.sprintf("%.4f < %.4f", rad, ang);
    lCoord1b->setText(str);
}

void QG_CoordinateWidget::setAbsCoordinates(const QString& x, const QString& y) {
    lCoord1->setText(x);
    lCoord1b->setText(y);
}

void QG_CoordinateWidget::setRelCoordinates(double x, double y) {
    setRelCoordinates(RS_Vector(x, y));
}

void QG_CoordinateWidget::setRelCoordinates(const RS_Vector& v) {
    QString str;
    str.sprintf("@%.4f / %.4f", v.x, v.y);
    lCoord2->setText(str);
    
    double ang = RS_Math::rad2deg(v.angle());
    double rad = v.magnitude();
    str.sprintf("@%.4f < %.4f", rad, ang);
    lCoord2b->setText(str);
}

void QG_CoordinateWidget::setRelCoordinates(const QString& x, const QString& y) {
    lCoord2->setText(x);
    lCoord2b->setText(y);
}

void QG_CoordinateWidget::setCoordinates(double x, double y,
        double rx, double ry) {
    setAbsCoordinates(x, y);
    setRelCoordinates(rx, ry);
}*/

/*void QG_CoordinateWidget::setAbsCoordinates(const QString& x, 
                                            const QString& y,
                                            const QString& rx,
                                            const QString& ry) {
    lCoord1->setText(x);
    lCoord1b->setText(y);
    lCoord2->setText(rx);
    lCoord2b->setText(ry);
}*/



void QG_CoordinateWidget::setCoordinates(const RS_Vector& abs,
                                         const RS_Vector& rel, bool updateFormat) {
    setCoordinates(abs.x, abs.y, rel.x, rel.y, updateFormat);
}



void QG_CoordinateWidget::setCoordinates(double x, double y,
        double rx, double ry, bool updateFormat) {
    
    if (graphic!=NULL) {
        if (updateFormat) {
            format = graphic->getLinearFormat();
            prec = graphic->getLinearPrecision();
            aformat = graphic->getAngleFormat();
            aprec = graphic->getAnglePrecision();
        }
    
        // abs / rel coordinates:
        RS_String absX = RS_Units::formatLinear(x,
                                               graphic->getUnit(),
                                               format, prec);
        RS_String absY = RS_Units::formatLinear(y,
                                               graphic->getUnit(),
                                               format, prec);
        RS_String relX = RS_Units::formatLinear(rx,
                                               graphic->getUnit(),
                                               format, prec);
        RS_String relY = RS_Units::formatLinear(ry,
                                               graphic->getUnit(),
                                               format, prec);
        
        lCoord1->setText(absX + " , " + absY);
        lCoord2->setText(relX + " , " + relY);        
        
        // polar coordinates:
        RS_Vector v;
        v = RS_Vector(x, y);
        QString str;
        QString rStr = RS_Units::formatLinear(v.magnitude(),
                                               graphic->getUnit(),
                                               format, prec);
        QString aStr = RS_Units::formatAngle(v.angle(),
                                               aformat, aprec);
        
        str = rStr + " < " + aStr;
        lCoord1b->setText(str);
    
        v = RS_Vector(rx, ry);
        rStr = RS_Units::formatLinear(v.magnitude(),
                                               graphic->getUnit(),
                                               format, prec);
        aStr = RS_Units::formatAngle(v.angle(),
                                               aformat, aprec);
        str = rStr + " < " + aStr;
        lCoord2b->setText(str);
    }
}
