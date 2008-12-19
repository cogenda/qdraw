/**
 * Author: Andrew Mustun
 * $Id: layer01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Adds three layers 'red', 'green' and 'blue' to the
 * current drawing and a line on each layer.
 */
function main() {
    var doc = new Document;
    var view = new View;

    var pens = [ new Pen(255,0,0), new Pen(0,255,0), new Pen(0,0,255) ];
    var layers = [ "l_red", "l_green", "l_blue" ];

    for (var i = 0; i < layers.length; i++) {
        doc.addLayer(layers[i], pens[i]);
        doc.setActiveLayer(layers[i]);
        var line = new Line(doc, i*5.0,0.0, 50.0+i*5.0,50.0);
        doc.addEntity(line);
    }

    view.zoomAuto();
}
