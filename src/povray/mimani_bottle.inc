#macro MimaniCap ()
object{
    Round_Cone(
        <0, 0, 0>, 0.22
        <0, 1.0, 0>, 0.17 
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
    scale <1, 1, 1>                          
}
#end

#macro MimaniBottleCapOn (nailColor, percentFull)
union {
    object {
        MimaniCap ()
        translate <0, 0.9, 0>
    }
    // Bottle
    object {
        Round_Cone(
            <0, 0, 0>, 0.4
            <0, 1.0, 0>, 0.5
            0.15,
            1)
        material { M_Glass3 }
        scale <1.0, 1.0, 0.7>
        translate <0, -0.1, 0>
    }

		object {
				cylinder {
				    -y, y, 0.3
				    pigment {
				        object {
				            text {
				                ttf "senator_thin.ttf",
				                "M I M A N I",
				                2.1,
				                0
				                scale <.08, 0.25, 1.0>
				            }
				        pigment { rgbt <1.0, 1.0, 1.0, 1.0> }
				        pigment { BrightGold }
				    }
				    warp {
				        cylindrical
				        orientation <0, 0, 1>
				    }
				    rotate <0, 125, 0>
				    translate <0, 0.45, 0.0>
				}
		}}

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
