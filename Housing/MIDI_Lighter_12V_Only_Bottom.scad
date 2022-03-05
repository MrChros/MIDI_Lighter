include <roundedcube.scad>

Board_Width                 = 47;
Board_Height                = 64;
Board_Thickness             = 2.5;

Board_Overlap               = 0.5;

Drill_Diameter              = 3.4;
Drill_Radius                = Drill_Diameter/2;

Drill_Offset_X              = Board_Width /2 - 4;
Drill_Offset_Y              = Board_Height/2    ;

Screw_Head_Rim_Thickness    = 1.25;
Screw_Head_Rim_Height       = 3.2;

Screw_Head_Diameter         = 6;
Screw_Head_Height           = 2.2;
Screw_Head_Extra_Height     = 0.1; // Only for visual purposes

Screw_Head_Offset_Z         =   Board_Thickness     / 2 +
                                Screw_Head_Rim_Height   -
                                Screw_Head_Height   / 2 +
                                Screw_Head_Extra_Height;

difference()
{
    difference()
    {
        union()
        {
            roundedcube([   Board_Width  + Board_Overlap * 2,
                            Board_Height + Board_Overlap * 2,
                            Board_Thickness], true, Board_Overlap * 2, "z");
            
            for(i = [   [+Drill_Offset_X, +(Drill_Offset_Y- 4), Board_Thickness/2 + Screw_Head_Rim_Height/2],
                        [+Drill_Offset_X, -(Drill_Offset_Y-19), Board_Thickness/2 + Screw_Head_Rim_Height/2],
                        [-Drill_Offset_X, +(Drill_Offset_Y- 4), Board_Thickness/2 + Screw_Head_Rim_Height/2],
                        [-Drill_Offset_X, -(Drill_Offset_Y-19), Board_Thickness/2 + Screw_Head_Rim_Height/2]])
            {
                translate(i)
                {
                    cylinder(h = Screw_Head_Rim_Height, r=Screw_Head_Diameter/2 + Screw_Head_Rim_Thickness, center = true, $fn = 100);
                }
            }
        }
        
        union()
        {
            for(i = [   [+Drill_Offset_X, +(Drill_Offset_Y- 4), 0],
                        [+Drill_Offset_X, -(Drill_Offset_Y-19), 0],
                        [-Drill_Offset_X, +(Drill_Offset_Y- 4), 0],
                        [-Drill_Offset_X, -(Drill_Offset_Y-19), 0]])
            {
                translate(i)
                cylinder(h = Board_Thickness * 20, r=Drill_Radius, center = true, $fn = 100);
            }
            
            for(i = [   [+Drill_Offset_X, +(Drill_Offset_Y- 4), Screw_Head_Offset_Z],
                        [+Drill_Offset_X, -(Drill_Offset_Y-19), Screw_Head_Offset_Z],
                        [-Drill_Offset_X, +(Drill_Offset_Y- 4), Screw_Head_Offset_Z],
                        [-Drill_Offset_X, -(Drill_Offset_Y-19), Screw_Head_Offset_Z]])
            {
                translate(i)
                cylinder(  h = Screw_Head_Height   +
                                Screw_Head_Extra_Height,
                            d = Screw_Head_Diameter, center = true, $fn = 100);
            }
        }
    }
    
    translate([+(Drill_Offset_X-3), (19-4)/2, 0])
    rotate(a=[0, 45, 0])
    cube([4, 10, Board_Thickness * 4], center = true, $fn = 100);
    
    translate([-(Drill_Offset_X-3), (19-4)/2, 0])
    rotate(a=[0, -45, 0])
    cube([4, 10, Board_Thickness * 4], center = true, $fn = 100);
}