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
        scale <1,10,1>
        translate <0, -0.4, 0>
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
                <0, 0.1, 0>, 0.5    // Center and radius of one end
                <0, 0.95, 0>, 0.38    // Center and radius of other end
                0.05, // rounding radius,
                1)
        }
        object {
            Round_Cone(
                <0, 0.1, 0>, 0.5    // Center and radius of one end
                <0, 0.95, 0>, 0.38    // Center and radius of other end
                0.05, // rounding radius,
                1)
            translate <0, percentFull*0.01-0.05, 0>
        }
        pigment {color nailColor}
    }
}
#end

#macro KeflonBottleCapOn (nailColor, capAngleY, percentFull)
union{
    object {
        KeflonCap ()
        translate <0, 1.5, 0>
        rotate <0, capAngleY, 0>
    }

    object {
        KeflonBottle (nailColor, capAngleY, percentFull)
    }
}
#end
