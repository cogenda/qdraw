/**
 * Author: Andrew Mustun
 * $Id: math01.qs 921 2004-12-19 00:31:22Z andrew $
 *
 * Math calculations.
 */
function main() {
    for (var v=-10.0; v<10.0; v+=0.5) {
        with (Math) {
            print("value: " + v);
            print("sin: " + sin(v));
            print("cos: " + cos(v));
            print("tan: " + tan(v));
            print("asin: " + asin(v));
            print("acos: " + acos(v));
            print("atan: " + atan(v));
            print("exp: " + exp(v));
            print("log: " + log(v));
            print("pow (2): " + pow(v, 2.0));
            print("sqrt: " + sqrt(v));
        }
    }
}
