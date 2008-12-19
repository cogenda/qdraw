/**
 * Author: Andrew Mustun
 * $Id: pen01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Demonstrates the use of the color attribute of 
 * the Pen class.
 *
 * Please note that not the full range of colors that
 * can be generated is also supported by the DXF format.
 * When you save and reload the drawing created by this 
 * script, you won't get exactly the same colors.
 */
function main() {
    var doc = new Document;
    var view = new View;

    var pen;
    var line;
    var r, g, b;
    var x = 0;
    var y = 0;

    for (b=0; b<256; b+=16) {
        for (r=0; r<256; r+=16) {
            for (g=0; g<256; g+=16) {
                pen = new Pen(r,g,b,100);
                doc.setActivePen(pen);
                line = new Line(doc, x,y, x+1,y);
                doc.addEntity(line);
                y++;
            }
            x+=1;
            y=0;
        }
    }

    view.zoomAuto();
}
