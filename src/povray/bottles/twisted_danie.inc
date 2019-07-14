#declare twisted_superellipsoid = function(x, y, z, E, N) {
    -f_superellipsoid(
        z*sin(y*2*pi/12) + x*cos(y*2*pi/12),
        y,
        z*cos(y*2*pi/12) - x*sin(y*2*pi/12),
        E, N)
}

#macro TwistedDanieBottle (polishColor, polishType, capAngleY, percentFull)
union {
    // Main body of bottle
    isosurface {
        function { twisted_superellipsoid(x, y, z, 0.15, 0.15) }
        contained_by { box { -2, 2 } } // This is to insure no clipping
        scale <0.45, 0.625, 0.45>
        material { M_Glass3 }
        finish {
            phong 1.0
            diffuse 0.7
            reflection 0.3
        }
        translate <0, 0.5, 0>
    }

    // Inside of bottle
    isosurface {
        function { twisted_superellipsoid(x, y, z, 0.25, 0.25) }
        scale <0.35, 0.5, 0.35>
        material { M_Glass3 }
        translate <0, 0.5, 0>
    }

    // Neck
    difference {
        superellipsoid {
            <1.0, 0.1>
		        scale <0.2, 0.2, 0.3>
		    }
        superellipsoid {
            <1.0, 0.1>
		        scale <0.15, 0.15, 0.3>
		    }
        rotate <90,0,0>
        translate <0, 1.2, 0>
        material { M_Glass3 }
        finish {
            phong 1.0
            diffuse 0.7
            reflection 0.3
        }
    }

    // Ring around neck
    object{
        Supertorus(
            0.22, 0.02,
            1.00, 0.45,
            0.001, 1.50)

        material { M_Glass3 }
        finish {
            phong 1.0
            diffuse 0.7
            reflection 0.3
        }
        translate <0, 1.1, 0>
    }

    // Threading
    #for (i, 0, 0.1, 0.1)
		    object{
		        Supertorus(
		            0.21, 0.02,
		            1.00, 0.45,
		            0.001, 1.50)

		        material { M_Glass3 }
		        finish {
		            phong 1.0
		            diffuse 0.7
		            reflection 0.3
		        }
		        rotate <10, 0, 0>
		        translate <0, 1.3+i, 0>
		    }
    #end

    // Nail polish itself
    // We use a difference here to fake a majuscus effect
    difference {
        isosurface {
        	  function { twisted_superellipsoid(x, y, z, 0.25, 0.25) }
            contained_by { box { -2, 2 } } // This is to insure no clipping
            scale <0.37, 0.52, 0.37>
        }
        // We use the superellipsoid macro here to make sure that
        // the intersection face from the difference is rendered too.
        superellipsoid {
        	  <0.25, 0.25>
            // Note that the scales here have to be just a tiny bit
            // larger than those above to avoid strange intersection/
            // clipping issues
            scale <0.5, 0.5, 0.5>
            translate <0, percentFull*0.01-0.05, 0>
            // We rotate this shape as much as the twist angle of the
            // the primary shape.
            rotate <0, percentFull*pi/600.0, 0>
        }
        PolishColor(polishColor, polishType)
        translate <0, 0.5, 0>
    }

    // Side label
    object {
        Bevelled_Text (
            "texgyreadventor-regular.ttf", // font file
            "danie",  // text
            10 ,      // slice
            35,       // taper
            0.045,    // point chamfer
            0.2,      // character depth along z-axis
            0.00,     // increase per character
            0)        // 1 = "merge"
        material { M_Glass3 }
        finish {
            phong 1.0
            diffuse 1.0
            reflection 0.1
        }
        rotate <0, 0, 75>
        scale <0.27, 0.27, 0.27>
        translate <0.0, 0.1, -0.48>
    }
}
#end

#macro DanieCap (polishColor, polishType, onBottle)
union {
    // Main body
    difference {
		    superellipsoid {
		        <1.0, 0.1>
		        scale <0.3, 0.3, 0.5>
		    }
		    superellipsoid {
		        <1.0, 0.1>
		        scale <0.25, 0.25, 0.45>
		        translate <0.0, 0.0, 0.1>
		    }
        rotate <90,0,0>
        pigment {color White}
        finish {
            phong 1.0
            diffuse 0.7
        }
    }

    // Brush stem
    superellipsoid {
        <1.0, 0.1>
        scale <0.05, 0.05, 0.8>
        translate <0.0, 0.0, 0.35>
        rotate <90,0,0>
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
            <0.025*sin(i), -1.15, 0.025*cos(i)>,
            <(0.025+dr)*sin(i), -1.55, (0.025+dr)*cos(i)>,
            0.007
            pigment {color Black}
        }
    #end

    #if(!onBottle)
        // Polish with same color at end of brush
        // but only if not on the bottle
        blob {
          	threshold 0.1
        		sphere {
    		        <0.0, -1.25, 0.0>, 0.03, 1
        		}
            cylinder {
                <0, -1.25, 0>,
                <0, -1.55, 0>,
                0.05,
                1
            }
        		sphere {
    		        <0.0, -1.55, 0.0>, 0.05, 1
        		}
            pigment {
            	  color polishColor
            }
    		}
    #end

    // Brand initial on top of cap
    object {
        Bevelled_Text (
            "texgyreadventor-bold.ttf", // font file
            "d",      // text
            30 ,      // slice
            35,       // taper
            0.045,    // point chamfer
            0.1,      // character depth along z-axis
            0.00,     // increase per character
            0)        // 1 = "merge"
        finish {
            phong   1.0
            diffuse 0.7
        }
        pigment {color White}
        rotate <90, 0, 0>
        scale <0.5, 0.5, 0.5>
        translate <-0.15, 0.53, -0.16>
    }
}
#end

#macro TwistedDanieBottleCapOn (polishColor, polishType, capAngleY, percentFull)
union{
    object {
        DanieCap (polishColor, polishType, true)
        translate <0, 1.625, 0>
        rotate <0, capAngleY, 0>
    }
    object {
        TwistedDanieBottle (polishColor, polishType, capAngleY, percentFull)
    }
}
#end
