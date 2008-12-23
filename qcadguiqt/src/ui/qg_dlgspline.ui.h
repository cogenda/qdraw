/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update this file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

void QG_DlgSpline::setSpline(RS_Spline& e) {
    spline = &e;
    //pen = spline->getPen();
    wPen->setPen(spline->getPen(false), true, false, "Pen");
    RS_Graphic* graphic = spline->getGraphic();
    if (graphic!=NULL) {
        cbLayer->init(*(graphic->getLayerList()), false, false);
    }
    RS_Layer* lay = spline->getLayer(false);
    if (lay!=NULL) {
        cbLayer->setLayer(*lay);
    }

    QString s;
    s.setNum(spline->getDegree());
	cbDegree->setCurrentText(s);

    cbClosed->setChecked(spline->isClosed());

    // get spline division
    s.setNum(spline->getDivision());
    SplineDivisionEdit->setText(s);

    // get spline label
    SplineLabelEdit->setText(spline->getLabel());

}



void QG_DlgSpline::updateSpline() {
    spline->setDegree(RS_Math::round(RS_Math::eval(cbDegree->currentText())));
    spline->setClosed(cbClosed->isChecked());
    spline->setPen(wPen->getPen());
    spline->setLayer(cbLayer->currentText());
    spline->update();
    spline->setDivision(RS_Math::eval(SplineDivisionEdit->text()));
    spline->setLabel(SplineLabelEdit->text());
}

