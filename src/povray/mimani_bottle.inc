#macro MimaniCap ()
object{
    Round_Cone(
        <0, 0, 0>, 0.25
        <0, 1.2, 0>, 0.2
        0.02, // rounding radius,
        1)
    pigment { BrightGold }
    finish {
        ambient .1
        diffuse .1
        specular 1
        roughness .1
        reflection { 0.75 }
    }
}
#end

#macro MimaniBottleCapOn (nailColor, percentFull)
union {
    object {
        MimaniCap ()
        translate <0, 1.2, 0>
    }
    // Bottle
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

    // Label
		object {
				cylinder {
				    -y, y, 0.3
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
				        rotate <0, 125, 0>
				        translate <0, 0.7, 0.0>
				    }
		    }
		}

    // Polish
    difference {
		    object {
		        Round_Cone(
		            <0, 0, 0>, 0.3
		            <0, 0.9, 0>, 0.4
		            0.15,
		            1)
		        pigment {color nailColor}
		        scale <1, 1, 0.6>
		        translate <0, 0, 0>
		    }
		    object {
		        Round_Cone(
		            <0, 0, 0>, 0.4
		            <0, 0.9, 0>, 0.4
		            0.15,
		            1)
		        pigment {color nailColor}
		        scale <1, 1, 0.6>
		        translate <0, 0.9*percentFull*0.01, 0>
		    }
    }
}
#end
