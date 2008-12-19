/**
 * Author: Andrew Mustun
 * $Id: pen02.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Demonstrates the use of the width attribute 
 * of the Pen class.
 */
function main() {
    var doc = new Document;
    var view = new View;

    var pen;
    var line;
    var y = 0;
    var w = 0;

    // The width is given in hundredth Millimeter
	//  i.e. a width of 100 is used to create a 
	//  line that is 1mm wide
    for (w=0; w<210; w++) {
        pen = new Pen(255,255,255,w);
        doc.setActivePen(pen);
        line = new Line(doc, 0,y, 100,y);
        doc.addEntity(line);
        y+=2.5;
    }

    view.zoomAuto();
}
