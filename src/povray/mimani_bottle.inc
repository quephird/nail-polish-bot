#macro MimaniCap ()
union {
    // Body
    // TODO: Make this a union of a cone and a cylinder;
    //       that makes it easier to wrap text around the cap.
    object{
        Round_Cone(
            <0, 0, 0>, 0.25
            <0, 1.1, 0>, 0.2
            0.02, // rounding radius,
            1)
        pigment { BrightGold }
        finish {
            ambient .1
            diffuse 0.1
            specular 0.5
            roughness .1
            reflection { 0.75 }
        }
    }

    // Brush stem
    superellipsoid {
        <1.0, 0.1>
        scale <0.05, 0.05, 0.8>
        rotate <90, 0, 0>
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
            <0.025*sin(i), -0.8, 0.025*cos(i)>,
            <(0.025+dr)*sin(i), -1.2, (0.025+dr)*cos(i)>,
            0.007
            pigment {color Black}
        }
    #end
}
#end

#macro MimaniBottleCapOn (nailColor, polishType, percentFull)
union {
    object {
        MimaniCap ()
        translate <0, 1.2, 0>
    }

    // Bottle
    difference {
        object {
            Round_Cone(
                <0, 0, 0>, 0.4
                <0, 1.0, 0>, 0.5
                0.15,
                1)
            material { M_Glass3 }
            scale <1.1, 1.3, 0.8>
            translate <0, -0.1, 0>
        }
        object {
            Round_Cone(
                <0, 0, 0>, 0.36
                <0, 1.0, 0>, 0.46
                0.15,
                1)
            material { M_Glass3 }
            scale <1.1, 1.3, 0.8>
            translate <0, -0.1, 0>
        }
    }

    // Label
    // TODO: Think about making a macro that wraps text around a curved surface,
    //       parameterized by font, size, cylindrical radius, etc.
		object {
				cylinder {
            // TODO: Look into what y actually refers to
            // The z parameter is quite important here, needs to be
            // close to the z-value for the bottle and obviously
            // greater than the polish to avoid clipping.
				    -y, y, 0.39
				    pigment {
				        object {
				        	  union {
						            text {
						                ttf "senator_thin.ttf",
						                "_________________",
						                2.1,
						                0
						                scale <.08, 0.3, 1.0>
		    			    	        translate <-0.05, 0.25, 0.0>
						            }
						            text {
						                ttf "senator_thin.ttf",
						                "M I M A N I",
						                2.1,
						                0
						                scale <.08, 0.3, 1.0>
						            }
						            text {
						                ttf "senator_thin.ttf",
						                "_________________",
						                2.1,
						                0
						                scale <.08, 0.3, 1.0>
		    			    	        translate <-0.05, -0.05, 0.0>
						            }
						        }
    				        pigment { rgbt <1.0, 1.0, 1.0, 1.0> }
    				        pigment { BrightGold }
                }
                warp {
				            cylindrical
				            orientation <0, 0, 1>
				        }
				        rotate <0, 135, 0>
				        translate <0, 0.7, 0.0>
				    }
		    }
		}

    // Polish
    difference {
		    object {
		        Round_Cone(
		            <0, 0, 0>, 0.35
		            <0, 0.9, 0>, 0.45
		            0.15,
		            1)
		        translate <0, 0, 0>
		    }
		    object {
		        Round_Cone(
		            <0, 0, 0>, 0.45
		            <0, 0.9, 0>, 0.45
		            0.15,
		            1)
		        translate <0, 0.9*percentFull*0.01, 0>
		    }
        PolishColor(nailColor, polishType)
        scale <1, 1.2, 0.7>
    }
}
#end
