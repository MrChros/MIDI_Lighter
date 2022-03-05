include <roundedcube.scad>

Board_Width                 = 47;
Board_Height                = 64;
Board_Thickness             = 2.5; //3.5;

Board_Overlap               = 0.5;

Drill_Diameter              = 3.4;
Drill_Radius                = Drill_Diameter/2;

Drill_Offset_X              = Board_Width /2 - 4;
Drill_Offset_Y              = Board_Height/2    ;

Screw_Head_Rim_Thickness    = 1.25;
Screw_Head_Rim_Height       = 2.5;
Screw_Head_Diameter         = 6;
Screw_Head_Extra_Height     = 0.0; // Only for visual purposes
Screw_Head_Sunk             = 0;

Screw_Head_Offset_Z         =   Board_Thickness         / 2 +
                                Screw_Head_Rim_Height   / 2 +
                                Screw_Head_Extra_Height     -
                                Screw_Head_Sunk         / 2;

difference()
{
    union()
    {
        roundedcube([   Board_Width  + Board_Overlap * 2,
                        Board_Height + Board_Overlap * 2,
                        Board_Thickness], true, Board_Overlap * 2, "z");
        
        for(i = [   [+Drill_Offset_X, +(Drill_Offset_Y- 4), Board_Thickness/2],
                    [+Drill_Offset_X, -(Drill_Offset_Y-19), Board_Thickness/2],
                    [-Drill_Offset_X, +(Drill_Offset_Y- 4), Board_Thickness/2],
                    [-Drill_Offset_X, -(Drill_Offset_Y-19), Board_Thickness/2],
        
                    [-(7+Screw_Head_Diameter/2), +(Drill_Offset_Y- 4), Board_Thickness/2],
                    [+(7+Screw_Head_Diameter/2), +(Drill_Offset_Y- 4), Board_Thickness/2]
                ])
        {
            translate(i)
            cylinder(h = Board_Thickness/2 + Screw_Head_Rim_Height, r=Screw_Head_Diameter/2 + Screw_Head_Rim_Thickness, center = true, $fn = 100);
        }
    }
    
    ////////////////////////////////////////////////
    // MIDI Plugs Main Cut
    translate([-(2.5+19/2), -(18+18/2), 0])
    cube([19, 18, Board_Thickness*2], center = true, $fn = 100);
    
    translate([+(2.5+19/2), -(18+18/2), 0])
    cube([19, 18, Board_Thickness*2], center = true, $fn = 100);
    
    
    translate([-(21+4/2), -(29+4/2), 0])
    cube([4, 4, Board_Thickness*2], center = true, $fn = 100);
    
    translate([+(21+4/2), -(29+4/2), 0])
    cube([4, 4, Board_Thickness*2], center = true, $fn = 100);
    
    
    translate([+(10+4/2), -(16.5+2/2), 0])
    cube([4, 2, Board_Thickness*2], center = true, $fn = 100);
    
    translate([-(10+4/2), -(16.5+2/2), 0])
    cube([4, 2, Board_Thickness*2], center = true, $fn = 100);
    ////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////
    // Removing small cube between MIDI Plugs
    translate([0, -(29+4/2), 0])
    cube([6, 4, Board_Thickness*2], center = true, $fn = 100);
    ////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////
    // LEDs Cut
    translate([-3.75, -8, 0])
    cube([10.5, 5, Board_Thickness*2], center = true, $fn = 100);
    ////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////
    // Energy Plug Cut
    translate([+(10+14/2+0.1), +(14.5+9/2), 0])
    cube([14+0.1, 9, Board_Thickness*2], center = true, $fn = 100);
    ////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////
    // Mode Switch Cut
    translate([19, 8, 0])
    cube([11, 14, Board_Thickness*2], center = true, $fn = 100);
    ////////////////////////////////////////////////
    
    
    ////////////////////////////////////////////////
    // Reset Switch Hole
    translate([6.98, 8.1325, 0])
    cylinder(h = Board_Thickness * 20, d=2.5, center = true, $fn = 100);
    ////////////////////////////////////////////////
    
    
    union()
    {
        for(i = [   [+Drill_Offset_X, +(Drill_Offset_Y- 4), 0],
                    [+Drill_Offset_X, -(Drill_Offset_Y-19), 0],
                    [-Drill_Offset_X, +(Drill_Offset_Y- 4), 0],
                    [-Drill_Offset_X, -(Drill_Offset_Y-19), 0],
                
                    [-(7+Screw_Head_Diameter/2), +(Drill_Offset_Y- 4), Board_Thickness/2],
                    [+(7+Screw_Head_Diameter/2), +(Drill_Offset_Y- 4), Board_Thickness/2]
                ])
        {
            translate(i)
            cylinder(h = Board_Thickness * 20, r=Drill_Radius, center = true, $fn = 100);
        }
        
        for(i = [   [+Drill_Offset_X, +(Drill_Offset_Y- 4), Screw_Head_Offset_Z],
                    [+Drill_Offset_X, -(Drill_Offset_Y-19), Screw_Head_Offset_Z],
                    [-Drill_Offset_X, +(Drill_Offset_Y- 4), Screw_Head_Offset_Z],
                    [-Drill_Offset_X, -(Drill_Offset_Y-19), Screw_Head_Offset_Z],
        
                    [-(7+Screw_Head_Diameter/2), +(Drill_Offset_Y- 4), Screw_Head_Offset_Z],
                    [+(7+Screw_Head_Diameter/2), +(Drill_Offset_Y- 4), Screw_Head_Offset_Z]
                ])
        {
            translate(i)
            cylinder(  h = Screw_Head_Rim_Height   +
                            Screw_Head_Extra_Height +
                            Screw_Head_Sunk,
                        d = Screw_Head_Diameter, center = true, $fn = 100);
        }
    }
}