#version 3.7;
global_settings { assumed_gamma 1.0 }

#include "colors.inc"
#include "glass.inc"
#include "metals.inc"
#include "shapes.inc"
#include "stones.inc"
#include "textures.inc"

#include "danie_bottle.inc"
#include "keflon_bottle.inc"
#include "mimani_bottle.inc"
#include "polish_color.inc"
#include "wall_mirror.inc"
#include "sink.inc"
#include "wall.inc"

// Main camera
camera {
    perspective angle 75
    right     x*image_width/image_height
    location  <-2.0, 2.5, -7.0>
    look_at   <-4.0, 1.0, 0.0>
}

// Main light
light_source{
	  <10, 10, -10>
	  color White
}

#declare My_seed = seed(now * 100000);
#declare BottleNumber = 0;
#declare R = 0.50;
#declare G = 1.0;
#declare B = 0.0;
#declare PercentFull = 70;
#declare PolishType = 0;

// NOTA BENE: BottleNumber, R, G, B, PolishType, and PercentFull all
// get passed in from the command line
object {
    #switch (BottleNumber)
    #case (0)
        DanieBottleCapOn(
            <R, G, B>,
            PolishType,
            360*rand(My_seed)
            PercentFull)
        #break
    #case (1)
        KeflonBottleCapOn(
            <R, G, B>,
            PolishType,
            360*rand(My_seed)
            PercentFull)
        #break
    #case (2)
        MimaniBottleCapOn(
            <R, G, B>,
            PercentFull)
        #break
    #end
    rotate    <0, 20-40*rand(My_seed), 0>
    translate <-3, 0.625, -3.0>
}

object {
    Sink()
    translate <0, 0, 0>
}

object {
    Wall()
    translate <-17.0, -10.0, 4.0>
}

object {
    WallMirror()
    translate <0, 3.0, 3.75>
}
