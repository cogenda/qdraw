/**
 * Author: Andrew Mustun
 * $Id: zoom01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Moves around the current view port.
 */
function main() {
    var doc;       // current document
    var view;      // current document view
    var line;      // line to add
    var i;         // counter
    var k;         // counter
    var tools;     // tools
    var v1, v2;    // zoom window corners
    
    doc = new Document;
    view = new View;
    tools = new Tools;
    
    l = new Line(doc, 10,10, 90,10);
    doc.addEntity(l);
    l = new Line(doc, 50,140, 90,10);
    doc.addEntity(l);
    l = new Line(doc, 50,140, 10,10);
    doc.addEntity(l);

    for (k=0; k<3; k+=1) {
        for (i=0; i<3; i+=1) {
            tools.sleep(1);
            v1 = new Vector((i+1)*33,(k+1)*50);
            v2 = new Vector(i*33,k*50);
            view.zoomWindow(v1, v2);
        }
    }
    tools.sleep(1);
    view.zoomAuto();
    
    //view.redraw();
}
