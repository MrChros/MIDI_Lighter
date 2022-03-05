Board_Width                 = 8.5;
Board_Height                = 64;
Board_Thickness             = 2;

Drill_Diameter              = 3.4;
Drill_Radius                = Drill_Diameter/2;

Screw_Head_Rim_Thickness    = 1.25;
Screw_Head_Rim_Height       = 1.5;
Screw_Head_Diameter         = 6;

Screw_Head_Offset_Z         =   Board_Thickness         / 2 +
                                Screw_Head_Rim_Height   / 2;
                                
difference()
{
    union()
    {
        cube([14+Screw_Head_Diameter, Board_Width, Board_Thickness], center = true, $fn = 100);
        
        for(i = [   [-(7+Screw_Head_Diameter/2), 0, Screw_Head_Rim_Height/2],
                    [+(7+Screw_Head_Diameter/2), 0, Screw_Head_Rim_Height/2]
                ])
        {
            translate(i)
            cylinder(h = Board_Thickness + Screw_Head_Rim_Height, r=Screw_Head_Diameter/2 + Screw_Head_Rim_Thickness, center = true, $fn = 100);
        }
    }
    
    
     for(i =    [   [-(7+Screw_Head_Diameter/2), 0, Board_Thickness/2],
                    [+(7+Screw_Head_Diameter/2), 0, Board_Thickness/2]
                ])
        {
            translate(i)
            cylinder(h = Board_Thickness * 20, r=Drill_Radius, center = true, $fn = 100);
        }
}