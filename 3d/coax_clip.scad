thickness = 3;
teeth_engage=2;
major_dim = 20.1;
coax_hole = 5.5;

extendo = 0;

linear_extrude(height=6) union() {
    difference() {
      square([major_dim + 2*thickness, major_dim + thickness]);
      translate([thickness, 0, 0]) square([major_dim, major_dim]);
      translate([major_dim/2 + thickness - coax_hole/2, major_dim, 0]) square([coax_hole, thickness]);
    }

    polygon([[0, 0], [thickness + teeth_engage, 0], [thickness + teeth_engage, -0.5*thickness], [0, -2*thickness]]);

    polygon([[major_dim + 2*thickness, 0], [(major_dim + 2*thickness) - thickness - teeth_engage, 0], [(major_dim + 2*thickness) - thickness - teeth_engage, -0.5*thickness], [major_dim + 2*thickness, -2*thickness]]);

    translate([major_dim/2 + thickness, extendo + major_dim+thickness, 0]) difference() {
        circle(d=coax_hole + 2*thickness, $fn = 20);
        circle(d=coax_hole, $fn = 20);
        translate([-(coax_hole + 2*thickness)/2, -2 *thickness]) square([coax_hole + 2*thickness, 2*thickness]);
    }
    
    translate([major_dim/2 - coax_hole/2, major_dim+thickness, 0])square([thickness, extendo]);
    
    translate([major_dim/2 + coax_hole/2 + thickness, major_dim+thickness, 0])square([thickness, extendo]);
}