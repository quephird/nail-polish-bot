camera{
     location <0,5,-8>
     look_at <0,2,0> 
 }
 
 light_source{
      <20,50,-50>, color rgb<1,1,1>
 }
 
 background{
      color rgb<1,1,1>
 }
 
 plane{
   y,0 
   pigment{
        checker color rgb <1,1,0>
                color rgb <0,1,1>
   }       
 }
 
 sphere{
   <0,2,0>,2
   pigment{
        color rgb <1,0,0>
   }     
 }