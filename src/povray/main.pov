#version 3.7;
global_settings { assumed_gamma 1.0 }

#include "colors.inc"
#include "glass.inc"
#include "metals.inc"
#include "shapes.inc"
#include "stones.inc"
#include "textures.inc"

#include "bottles/danie.inc"
#include "bottles/keflon.inc"
#include "bottles/mimani.inc"
#include "bottles/twisted_danie.inc"
#include "scene/wall_mirror.inc"
#include "scene/sink.inc"
#include "scene/wall.inc"
#include "polish_color.inc"

// Seed for all random number generation below
#declare My_seed = seed(now * 100000);

#declare CameraX = -2.5+rand(My_seed);
#declare CameraY = 2.0+rand(My_seed);
#declare CameraZ = -7.5+rand(My_seed);

#declare BottleRotateY = 20-40*rand(My_seed);
#declare CapRotateY = 360*rand(My_seed);

/*
// Uncomment overrides in order to run from within the POV-Ray GUI
#declare BottleNumber = 3;
#declare R = 0.9;
#declare G = 0.4;
#declare B = 0.2;
#declare PolishType = 1;
#declare PercentFull = 20;
#declare BottleRotateY = 10;
*/

// Main camera
camera {
    perspective angle 75
    right     x*image_width/image_height
    location  <CameraX, CameraY, CameraZ>
    look_at   <-4.0, 1.0, 0.0>
}

// Main light
light_source{
	  <10, 10, -10>
	  color White
}

// NOTA BENE: BottleNumber, R, G, B, PolishType, and PercentFull all
// get passed in from the command line
object {
    #switch (BottleNumber)
    #case (0)
        DanieBottleCapOn(
            <R, G, B>,
            PolishType,
            CapRotateY,
            PercentFull)
        #break
    #case (1)
        KeflonBottleCapOn(
            <R, G, B>,
            PolishType,
            CapRotateY,
            PercentFull)
        #break
    #case (2)
        MimaniBottleCapOn(
            <R, G, B>,
            PolishType,
            PercentFull)
        #break
    #case (3)
        TwistedDanieBottleCapOn(
            <R, G, B>,
            PolishType,
            CapRotateY,
            PercentFull)
        #break
    #end
    rotate    <0, BottleRotateY, 0>
    translate <-3, 0.625, -3.0>
}

object {
    Sink()
    translate <0, 0, 0>
}

object {
    Wall()
    translate <-17.5, -10.5, 4.0>
}

object {
    WallMirror()
    translate <0, 3.0, 3.75>
}
