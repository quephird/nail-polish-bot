#version 3.7;
global_settings { assumed_gamma 1.0 }

#include "colors.inc"
#include "glass.inc"
#include "metals.inc"
#include "shapes.inc"
#include "stones.inc"
#include "textures.inc"

#include "./danie_bottle.inc"

// Random color generator with moving seed
#declare My_seed = seed(now * 100000);
#macro RandomColor()
    <rand(My_seed), rand(My_seed), rand(My_seed)>
#end

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

object {
    DanieBottleCapOn(
        RandomColor(),
        360*rand(My_seed)
        65)
    rotate    <0, 20-40*rand(My_seed), 0>
    translate <-3, 0.625, -3.0>
}

// Marble counter top
superellipsoid {
    <0.05, 0.05>
    scale <6.0, 0.5, 4.0>
    texture {
        T_Stone24
        scale 4
        finish {
            reflection {.01, .1}
        }
    }
    translate <0, 0, 0>
}

// Rear wall
plane {
    <0, 0, 1>, 0.1
    pigment {color <0.5, 0.5, 0.8>}
    translate <0, 0, 4.0>
}

// Mirror
box {
    <-4, 0.0, 0>, <4.0, 10, 0>
    texture {T_Silver_1C}
    translate <0, 3.0, 4>
}
