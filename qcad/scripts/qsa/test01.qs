function main() {
    var d, l, p;

    d = new Document;
    p = new Pen(0,0,255);
    d.addLayer("blah", p);
    d.setActiveLayer("blah");
    l = new Line(d, 20,20,100,20);
    d.addEntity(l);

    p = new Pen(255,0,0);
    d.setActivePen(p);

    l = new Line(d, 30,30,100,30);
    d.addEntity(l);
}
