#declare CREAM = 0;
#declare METALLIC = 1;

#macro PolishColor (polishColor, polishType)
    pigment {color polishColor}
    #switch (polishType)
    #case (CREAM)
        #break
    #case (METALLIC)
        finish {
            diffuse 0.001   // Spread of reflection
            specular 5      // Brightness of reflectivity
            roughness 0.1   // Dullness of surface
            metallic        // Reflected light takes on colour of surface
            reflection {
              1.0           // Reflects white light
            }
        }
        #break
    #end
#end
