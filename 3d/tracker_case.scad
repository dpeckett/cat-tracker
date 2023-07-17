thickness = 2;

case();
//lid();

module lid() {
  difference() {
    translate([0, 0, thickness/2]) cube([40 + 2*thickness, 21 + 2*thickness, thickness], center=true);
      
     // clearance for the screw ears.
    translate([-21.25, 12.25, 0]) cylinder(d = 3, h = thickness, $fn = 10);
    translate([-21.25, -12.25, 0]) cylinder(d = 3, h = thickness, $fn = 10);
    translate([21.25, 12.25, 0]) cylinder(d = 3, h = thickness, $fn = 10);
    translate([21.25, -12.25, 0]) cylinder(d = 3, h = thickness, $fn = 10);
  }
 
  // Screw ears
  translate([-19.5, 10.5, 0]) rotate(a = [0, 0, 45]) screw_ear(height = thickness, diameter = 3.5);
  translate([-19.5, -10.5, 0]) rotate(a = [0, 0, 135]) screw_ear(height = thickness, diameter = 3.5);
  translate([19.5, 10.5, 0]) rotate(a = [0, 0, -45]) screw_ear(height = thickness, diameter = 3.5);
  translate([19.5, -10.5, 0]) rotate(a = [0, 0, -135]) screw_ear(height = thickness, diameter = 3.5);
}

module case() {
  difference() {
    union() {
      minkowski() {
        union() {
          inner();
          translate([0, 0, 7.5 + (4+thickness)/2])        cube([40, 21, 4 + thickness], center = true);
        }
        sphere(r =  thickness);
      }
    }
  
    inner();
  
    // slice the top off clean.
    translate([0, 0, 7.5 + 2 * thickness + (11/2)]) cube([50, 50, 11], center = true);

    // clearance for the screw ears.
    translate([-21.25, 12.25, 7]) cylinder(d = 3, h = 5, $fn = 10);
    translate([-21.25, -12.25, 7]) cylinder(d = 3, h = 5, $fn = 10);
    translate([21.25, 12.25, 7]) cylinder(d = 3, h = 5, $fn = 10);
    translate([21.25, -12.25, 7]) cylinder(d = 3, h = 5, $fn = 10);
  
    // The antenna hole.
    translate([-21.75, 7.5, 9.5])rotate(a = [0, 90, 0]) cylinder(d = 1.9, h = thickness, $fn = 30);
  }

  // Screw ears
  translate([-19.5, 10.5, 6.5]) rotate(a = [0, 0, 45]) screw_ear();
  translate([-19.5, -10.5, 6.5]) rotate(a = [0, 0, 135]) screw_ear();
  translate([19.5, 10.5, 6.5]) rotate(a = [0, 0, -45]) screw_ear();
  translate([19.5, -10.5, 6.5]) rotate(a = [0, 0, -135]) screw_ear();

  translate([-23.5, -2, 9]) collar_ring(height=2.5);
  translate([23.5, -2, 9]) rotate(a = [0, 0, 180]) collar_ring(height=2.5);
}

module inner() {
  hull() {
    cube([30, 15, 0.1], center = true);
    translate([0, 0, 7.5 + 4/2]) cube([40, 21, 4], center = true);
  }
}

module screw_ear(height = 5, diameter = 2.5) {
    difference() {
      union() {
        translate([0, 6/2, 0]) cylinder(d = 6, h = height, $fn=30);
        translate([-6/2, 0, 0]) cube([6, 6/2, height]);
      }
      
      translate([0, 6/2, 0]) cylinder(d = diameter, h = height, $fn = 20);
    }
}

module collar_ring(height = 5) {
  linear_extrude(height = height) difference() {
    union() {
      minkowski() {
        square([3, 13], center=true);
        circle(r = thickness, $fn = 16);
      }
      translate([0, -(13 + 2*thickness)/2, 0]) square([3/2 + thickness, 13 + 2*thickness]);
    }
    square([3, 13], center=true);
  }
}