#macro KeflonCap ()
union {
    // Cap
    superellipsoid {
        <1.0, 0.05>
        scale <0.3, 0.3, 0.7>
        pigment {color Black}
        finish {
            phong 1.0
            diffuse 0.7
        }
        rotate <90, 0, 0>
    }

    // Brush stem
    superellipsoid {
        <1.0, 0.1>
        scale <0.05, 0.05, 0.8>
        rotate <90, 0, 0>
        translate <0.0, -0.2, 0.0>
        material { M_Glass3 }
        finish {
            phong 1.0
            diffuse 0.7
            reflection 0.3
        }
    }

    // Brush
    #declare new_seed = seed(now * 100000);
    #for (i, 0, 6.28, 0.375)
        #declare dr = 0.04*rand(new_seed);
        cylinder {
            <0.025*sin(i), -1.0, 0.025*cos(i)>,
            <(0.025+dr)*sin(i), -1.4, (0.025+dr)*cos(i)>,
            0.007
            pigment {color Black}
        }
    #end

    // Brand ring around cap
    object{
        Supertorus(
            0.30, 0.01,
            1.00, 0.45,
            0.001, 1.50)

        pigment {
            object {
                text {
                    ttf "aftasansthin-regular.ttf",
                    "KEFLON",
                    1, 0
                    scale <.08, 0.01, 1>
                    translate <0, -0.003, 0>
                }
                pigment { BrightGold }
                pigment { Black }
            }
            warp { cylindrical }
            rotate y*110
        }
        finish {
            ambient .1
            diffuse .1
            specular 1
            roughness .001
            reflection { 0.75 }
        }
        scale <1, 12, 1>
        translate <0, -0.55, 0>
    }
}
#end

#macro KeflonBottle (nailColor, capAngleY, percentFull)
union {
    // Main body
    object {
    Round_Cone(
        <0, 0, 0>, 0.6    // Center and radius of one end
        <0, 1.0, 0>, 0.4    // Center and radius of other end
        0.05, // rounding radius,
        1)
    material { M_Glass3 }
    }

    // Nail polish
    difference {
        object {
            Round_Cone(
                <0, 0.1, 0>, 0.5
                <0, 0.95, 0>, 0.38
                0.05, // rounding radius,
                1)
        }
        object {
            // We go through the hassle of computing the bottom radius
            // of the subtracting cone to try to produce a majuscus effect
            // by approximating the radius at the height of the polish
            // and rounding the resultant cone.
            Round_Cone(
                <0, 0.1, 0>, (0.51-0.39)*(1-percentFull*0.01)+0.39
                <0, 0.95, 0>, 0.4
                0.05, // rounding radius,
                1)
            translate <0, 0.85*percentFull*0.01, 0>
        }
        pigment {color nailColor}
    }
}
#end

#macro KeflonBottleCapOn (nailColor, capAngleY, percentFull)
union{
    object {
        KeflonCap ()
        translate <0, 1.7, 0>
        rotate <0, capAngleY, 0>
    }

    object {
        KeflonBottle (nailColor, capAngleY, percentFull)
    }
}
#end
