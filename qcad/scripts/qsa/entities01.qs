/**
 * Demonstrates the entity classes.
 * $Id: entities01.qs 934 2005-02-16 22:22:56Z andrew $
 *
 * Author: Andrew Mustun
 */
var doc;       // current document
var view;      // current document view

/**
 * Starts the demo.
 */
function main() {
    doc = new Document;
    view = new View;

    arcs();
    lines();
    points();
    view.zoomAuto();
}



/**
 * Arc demo.
 */
function arcs() {
    var angle, arc;
    var radius = 1;
    for (angle = 0.0; angle<Math.PI*2; angle+=Math.PI/16) {
        arc = new Arc(doc,
                      50.0,50.0,                 // center
                      radius,                    // radius
                      angle, angle+Math.PI/2,    // start- / end angle
                      false);                    // counter clock wise
        doc.addEntity(arc);
        radius+=1;
    }
}



/**
 * Line demo.
 */
function lines() {
    var line, v1, v2, v3;
    v1 = new Vector(200,50);
    v2 = new Vector(200,50);
    v3 = new Vector(200,50);
    v2.rotate(150,50, Math.PI*2/3);
    v3.rotate(150,50, Math.PI*2/3*2);

    for (var i=0; i<10; i++) {
        line = new Line(doc, v1, v2);
        doc.addEntity(line);
        line = new Line(doc, v2, v3);
        doc.addEntity(line);
        line = new Line(doc, v3, v1);
        doc.addEntity(line);

        v1.rotate(150,50, Math.PI/16);
        v2.rotate(150,50, Math.PI/16);
        v3.rotate(150,50, Math.PI/16);

        v1.scale(150,50, 0.758);
        v2.scale(150,50, 0.758);
        v3.scale(150,50, 0.758);
    }
}



/**
 * Point demo
 */
function points() {
/* Incomp. with QSA 1.1.2
    var p11 = new Vector(20, 100);
    var p12 = new Vector(90, 110);
    var p21 = new Vector(40, 130);
    var p22 = new Vector(60, 125);
    var point;

    for (var i=0; i<=10; i++) {
        var p1 = divide(p11, p12, 10, i);
        var p2 = divide(p21, p22, 10, i);

        print("p1: " + p1.toString());
        print("p2: " + p2.toString());
        for (var k=0; k<=10; k++) {
            var p = divide(p1, p2, 10, k);
            point = new Point(doc, p);
            doc.addEntity(point);
        }
    }
*/
}



function divide(p1, p2, steps, step) {
    var p = new Vector;
    p.x = p1.x + (p2.x - p1.x)/steps*step;
    p.y = p1.y + (p2.y - p1.y)/steps*step;
    return p;
}
