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
    #switch (BottleNumber)
    #case (0)
        DanieBottleCapOn(
            // NOTA BENE: R, G, B, and PercentFull all
            // get passed in from the command line
            <R, G, B>,
            360*rand(My_seed)
            PercentFull)
        #break
    #case (1)
        KeflonBottleCapOn(
            // NOTA BENE: R, G, B, and PercentFull all
            // get passed in from the command line
            <R, G, B>,
            360*rand(My_seed)
            PercentFull)
        #break
    #end
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
// Ceramic tiles
#for (i, 0, 5)
    #for (j, 0, 4)
        superellipsoid {
            <0.05, 0.5>
            scale <1.95, 1.95, 0.1>
            normal { bumps 0.1 scale 1.0 }
            pigment { color <0.5, 0.6, 0.95> }
            finish {
                phong 1.0
                diffuse 0.7
                reflection 0.3
            }
            translate <-17.5+i*4, -10+j*4, 4>
        }
    #end
#end
// Grout
plane {
    <0, 0, 1>, 0.1
    pigment { color <1.0, 1.0, 0.9> }
    finish { ambient .4 diffuse 0.1 }
    translate <0, 0, 4.05>
}

// Mirror
box {
    <-4, 0.0, 0>, <4.0, 10, 0>
    texture {T_Silver_1C}
    translate <0, 3.0, 3.75>
}
