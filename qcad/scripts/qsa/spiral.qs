/**
 * Author: John K. Emmons
 * $Id: spiral.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Draws a logarithmical spiral.
 */
function main() {
    var doc;        // current document
    var view;       // current document view
    var line;       // line to add
    var i;          // counter
    var x;          // x starting point
    var y;          // y starting point y
    var x1=71;      // x ending point
    var y1=70;      // y ending point
    var r;          // radius
    var theta=0.10; // angle in radians

    doc = new Document;
    view = new View;

    for (i=0; i<=2500; i++) {
        with(Math) {
            r=exp(theta*0.185);

            // numerical values are for moditying a reference
            //   position and scaling
            x=r*cos(theta)+70;
            y=r*sin(theta)+70;
        }

        l = new Line(doc,x,y,x1,y1);
        doc.addEntity(l);

        x1=x;
        y1=y;

        // angle increment
        theta=theta+0.01;
    }

    view.redraw();
}

