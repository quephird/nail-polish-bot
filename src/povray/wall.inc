#macro Wall()
union {
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
                translate <i*4, j*4, 0>
            }
        #end
    #end

    // Grout
    plane {
        <0, 0, 1>, 0.1
        pigment { color <1.0, 1.0, 0.9> }
        finish { ambient .4 diffuse 0.1 }
        translate <0, 0, 0.05>
    }
}
#end
