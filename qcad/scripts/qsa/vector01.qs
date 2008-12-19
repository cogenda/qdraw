/**
 * Author: Andrew Mustun
 * $Id: vector01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Demonstrates basic vector functions.
 */
function main() {
    var v1;
    var v2;

    v1 = new Vector(50,10);

    print("v1: " + v1.x + "/" + v1.y);

    v1.setPolar(50,30);

    print("v1: " + v1.x + "/" + v1.y);

    v2 = new Vector;
    v2.setPolar(100, 30);
    var dist = v1.distanceTo(v2);

    print("distance from: " + v1.toString() + " to " + v2.toString() +
          " is " + dist);
}
