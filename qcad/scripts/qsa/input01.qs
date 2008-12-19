/**
 * Author: Andrew Mustun
 * $Id: input01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Draws a sine curve from parameters given by the user.
 */
function main() {
    var doc;       // current document
    var view;      // current document view
    var line;        // line to add
    var i;             // counter
    var factor;    // scaling factor
    var lx = -1.0;
    var ly = -1.0;
    var x;
    var y;

    factor = inputData();

    doc = new Document;
    view = new View;

    for (x=0.0; x<2*Math.PI; x+=Math.PI/50) {
        print("x: " + x);

        y = Math.sin(x);

        print("y: " + y);
        if (lx>=0.0) {
            var line = new Line(doc, lx*factor.x, ly*factor.y,
                                x*factor.x, y*factor.y);
            doc.addEntity(line);
        }
        lx = x;
        ly = y;
    }

    view.redraw();
}



/**
 * Presents a dialog to input an X and Y factor for the sine.
 */
function inputData() {
    var dialog = new Dialog;
    dialog.caption = "Factor reading dialog";
    dialog.okButtonText = "Done";
    dialog.cancelButtonText = "Abort";

    var xfact = new LineEdit;
    xfact.label = "X Factor: ";
    xfact.text = "1";
    dialog.add(xfact);

    var yfact = new LineEdit;
    yfact.label = "Y Factor: ";
    yfact.text = "1";
    dialog.add(yfact);

    if (dialog.exec()) {
        print("Factors: " + xfact.text + "/" + yfact.text);
    }

    var ret = new Vector(xfact.text, yfact.text);

    return ret;
}

