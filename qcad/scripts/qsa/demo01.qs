/**
 * Author: Andrew Mustun
 * $Id: demo01.qs 922 2004-12-19 03:35:52Z andrew $
 *
 * Draws a simple net with lines.
 */
function main() {
    var doc;       // current document
    var view;      // current document view
    var line;      // line to add
    var i;         // counter

    doc = new Document;
    view = new View;

    for (i=0; i<=100; i+=5) {
        print(i);
        l = new Line(doc, i,0, 0,100-i);
        doc.addEntity(l);
    }

    view.redraw();
    view.zoomAuto();
}
