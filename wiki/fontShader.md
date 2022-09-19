# Quadratic Font Shader Ideas:
1. The function for the Quadratic Curve ```f(t) = (1-t)^2*p1+2t(1-t)*p2+t^2*p3```
2. f(0.5) is closest to p2, we know that because the quadratic formula tells us that ```x1 = 0```, ```x2 = 1``` and the parabola has a peak
3. We can check which three vertecies we should take into account based on their distance from the current pixel (fragCoord)

# Maybe a solution
1. Find the ```t``` that is the closest to the current pixel
2. Based on the ```t``` check if the pixel is inside or outside