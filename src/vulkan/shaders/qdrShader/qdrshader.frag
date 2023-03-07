#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec4 fragColor;
layout(location = 1) in vec2 fragTexCoord;

vec2 positions[3] = vec2[](
    vec2(0.5, 0.5),
    vec2(0.6, 0.8),
    vec2(0.3, 0.6)
);

layout(location = 0) out vec4 outColor;

void main() {
    vec2 screenSize = {600,600};

    float x = gl_FragCoord.x/screenSize.x;
    float y = gl_FragCoord.y/screenSize.y;

    outColor = vec4(0.0,0.0,0.1,1.0);

    float Xt = (sqrt((-positions[0].x*positions[2].x+positions[0].x*x+pow(positions[1].x,2)-2*positions[1].x*x+positions[2].x*x))-positions[0].x+positions[1].x)/(-positions[0].x+2*positions[1].x-positions[2].x);
    float Yt = (sqrt((-positions[0].y*positions[2].y+positions[0].y*y+pow(positions[1].y,2)-2*positions[1].y*y+positions[2].y*y))-positions[0].y+positions[1].y)/(-positions[0].y+2*positions[1].y-positions[2].y);

    float newY = pow(1.0-Xt,2) *positions[0].y+2*Xt*(1-Xt)*positions[1].y+ pow(Xt,2) *positions[2].y;
    float newX = pow(1.0-Yt,2) *positions[0].x+2*Yt*(1-Yt)*positions[1].x+ pow(Yt,2) *positions[2].x;


    float nXt = (sqrt((-positions[2].x*positions[0].x+positions[2].x*x+pow(positions[1].x,2)-2*positions[1].x*x+positions[0].x*x))-positions[2].x+positions[1].x)/(-positions[2].x+2*positions[1].x-positions[0].x);
    float nYt = (sqrt((-positions[2].y*positions[0].y+positions[2].y*y+pow(positions[1].y,2)-2*positions[1].y*y+positions[0].y*y))-positions[2].y+positions[1].y)/(-positions[2].y+2*positions[1].y-positions[0].y);

    float nnewY = pow(1.0-nXt,2) *positions[2].y+2*nXt*(1-nXt)*positions[1].y+ pow(nXt,2) *positions[0].y;
    float nnewX = pow(1.0-nYt,2) *positions[2].x+2*nYt*(1-nYt)*positions[1].x+ pow(nYt,2) *positions[0].x;

    float xtester = -positions[2].x*positions[0].x+positions[2].x*x+pow(positions[1].x,2)-2*positions[1].x*x+positions[0].x*x;
    float ytester = -positions[2].y*positions[0].y+positions[2].y*y+pow(positions[1].y,2)-2*positions[1].y*y+positions[0].y*y;

    float size = 0.005;

    int missed = 0;
    

    if( (-positions[0].y*positions[2].y+positions[0].y*y+pow(positions[1].y,2)-2*positions[1].y*y+positions[2].y*y)<0)
        missed++;
    if((-positions[0].x*positions[2].x+positions[0].x*x+pow(positions[1].x,2)-2*positions[1].x*x+positions[2].x*x) < 0)
        missed++;
    if((-positions[2].y*positions[0].y+positions[2].y*y+pow(positions[1].y,2)-2*positions[1].y*y+positions[0].y*y) < 0)
        missed++;
    if((-positions[2].x*positions[0].x+positions[2].x*x+pow(positions[1].x,2)-2*positions[1].x*x+positions[0].x*x) < 0)
        missed++;

    if(missed == 0)
        outColor = vec4(1.0,0.0,0.0,1.0);
    else if(missed == 1)
        outColor = vec4(0.0,1.0,0.0,1.0);
    else if(missed == 2)
        outColor = vec4(0.0,0.0,1.0,1.0);

    outColor = vec4(0.5,0.5,0.5,1.0);
    // if(positions[0].x == positions[2].x)
    // {
    //     float a = (positions[2].x - positions[0].x)/(positions[2].y - positions[0].y);
    //     float b = -a*positions[0].y+positions[0].x;

    //     if(!((nnewX > x || newX < x) && (nnewY > y || newY < y)) && (missed == 0) && ((a*positions[1].y + b > positions[1].x && x < a*y+b) || (a*positions[1].y + b < positions[1].x && x > a*y+b)))
    //         outColor = vec4(0.5,0.5,0.5,1.0);
    //     else
    //         discard;
    // } else {

    //     float a = (positions[2].y - positions[0].y)/(positions[2].x - positions[0].x);
    //     float b = -a*positions[0].x+positions[0].y;

    //     if(!((nnewX > x || newX < x) && (nnewY > y || newY < y)) && (missed == 0) && ((a*positions[1].x + b > positions[1].y && y < a*x+b) || (a*positions[1].x + b < positions[1].y && y > a*x+b)))
    //         outColor = vec4(0.5,0.5,0.5,1.0);
    //     else
    //         discard;

    // }
}