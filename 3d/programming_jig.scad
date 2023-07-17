difference() {
  cube([90, 26, 7.5]);
    
  translate([2.5, (26-20.5)/2, 7]) cube([39, 20.5, 0.5]);
  translate([7, (26-20.5)/2, 0]) cube([30, 20.5, 7.5]);
    
  translate([2.5, 19, 4]) cube([4.5, 4.5, 3]);
    
  translate([76, 16.5, 6])difference() {
    cylinder(d=8, h=1.5, $fn=20);
    cylinder(d=4, h=1.5, $fn=20);
  }
}

minkowski() {
  translate([5.25, 8.25, 7]) cube([1, 9.5, 0.5]);
  cylinder(d=1, h=0.5, $fn=10);
}

minkowski() {
  translate([5 + 32 + 0.75, 8.25, 7]) cube([1, 9.5, 0.5]);
  cylinder(d=1, h=0.5, $fn=10);
}

translate([53, 8.5, 7.5]) difference() {
  union() {
    translate([0, 0, 5]) rotate(a = [-90, 0, 0]) cylinder(d = 10, h = 12, $fn = 30);
    translate([-5, 0, 0]) cube([10, 12, 5]);
   
    // limit the rotation to 5 deg, to keep the spring retained.
    x = 5*(1 - tan(5));
    translate([0, 0, 5]) rotate(a = [-90, -180, 0])  linear_extrude(height = 12) polygon([[0, 0], [5, 0], [5, x], [0, 5]]);
  }
  
  translate([0, 0, 5]) rotate(a = [-90, 0, 0]) cylinder(d = 4, h = 2, $fn = 30);
  
  translate([0, 0, 5]) rotate(a = [-90, 0, 0]) cylinder(d = 2, h = 12, $fn = 16);
  translate([-5, 4, 0]) cube([10,4,10]);
}