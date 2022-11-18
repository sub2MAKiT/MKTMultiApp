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

    // float nXt = -Xt;
    // float nYt = -Yt;

    // float nnewY = pow(1.0-nXt,2) *positions[0].y+2*nXt*(1-nXt)*positions[1].y+ pow(nXt,2) *positions[2].y;
    // float nnewX = pow(1.0-nYt,2) *positions[0].x+2*nYt*(1-nYt)*positions[1].x+ pow(nYt,2) *positions[2].x;

    // if((sqrt(pow(x-newX,2)+pow(y-newY,2))<0.01||sqrt(pow(nnewX-x,2)+pow(nnewY-y,2))<0.01))

    // outColor = vec4(x,y,1.0,1.0);
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

    if(positions[0].x == positions[2].x)
    {
        float a = (positions[2].x - positions[0].x)/(positions[2].y - positions[0].y);
        float b = -a*positions[0].y+positions[0].x;

        if(!((nnewX > x || newX < x) && (nnewY > y || newY < y)) && (missed == 0) && ((a*positions[1].y + b > positions[1].x && x < a*y+b) || (a*positions[1].y + b < positions[1].x && x > a*y+b)))
            outColor = vec4(0.5,0.5,0.5,1.0);
        else
            discard;
    } else {

        float a = (positions[2].y - positions[0].y)/(positions[2].x - positions[0].x);
        float b = -a*positions[0].x+positions[0].y;

        if(!((nnewX > x || newX < x) && (nnewY > y || newY < y)) && (missed == 0) && ((a*positions[1].x + b > positions[1].y && y < a*x+b) || (a*positions[1].x + b < positions[1].y && y > a*x+b)))
            outColor = vec4(0.5,0.5,0.5,1.0);
        else
            discard;

    }
    // if(sqrt(pow(abs(positions[0].x-x),2)+pow(positions[0].y-y,2)) < 0.01)
    // {
    //     outColor = vec4(1.0,1.0,1.0,1.0);
    // } else if(sqrt(pow(abs(positions[1].x-x),2)+pow(positions[1].y-y,2)) < 0.01)
    // {
    //     outColor = vec4(1.0,1.0,1.0,1.0);
    // } else if(sqrt(pow(abs(positions[2].x-x),2)+pow(positions[2].y-y,2)) < 0.01)
    // {
    //     outColor = vec4(1.0,1.0,1.0,1.0);
    // }
}



    // if(abs(nnewX-x)<size)//(abs(x-newX)<size||abs(y-newY)<size)||(abs(nnewY-y)<size||abs(nnewX-x)<size))
    // {
    //     outColor = vec4(1.0,1.0,1.0,0.5);
    // }

    // if(abs(y-newY)<size)//(abs(x-newX)<size||abs(y-newY)<size)||(abs(nnewY-y)<size||abs(nnewX-x)<size))
    // {
    //     outColor = vec4(0.0,1.0,0.0,0.5);
    // }

    // if(abs(nnewY-y)<size)//(abs(x-newX)<size||abs(y-newY)<size)||(abs(nnewY-y)<size||abs(nnewX-x)<size))
    // {
    //     outColor = vec4(0.0,0.0,1.0,0.5);
    // }

    // if(abs(x-newX)<size)//(abs(x-newX)<size||abs(y-newY)<size)||(abs(nnewY-y)<size||abs(nnewX-x)<size))
    // {
    //     outColor = vec4(1.0,0.0,0.0,0.5);
    // }



// this is the greatest fixXxxxxxxxxxxxxxxxxxxxxxxxxxx
    // if(xtester<size&&ytester<size)
    // {
        // if(abs(xtester)<size/2&&abs(ytester)<size/2&&abs(x-(pow(1.0-0.5,2) *positions[0].x+2*0.5*(1-0.5)*positions[1].x+ pow(0.5,2) *positions[2].x))<size&&abs(y-(pow(1.0-0.5,2) *positions[0].y+2*0.5*(1-0.5)*positions[1].y+ pow(0.5,2) *positions[2].y))<size)
        // {
        //     outColor = vec4(0.0,0.0,0.0,1.0);
        // }
        // if(sqrt(pow(x-(pow(1.0-0.5,2) *positions[0].x+2*0.5*(1-0.5)*positions[1].x+ pow(0.5,2) *positions[2].x),2)+pow(y-(pow(1.0-0.5,2) *positions[0].y+2*0.5*(1-0.5)*positions[1].y+ pow(0.5,2) *positions[2].y),2))<size)
        // {
            // outColor = vec4(0.0,0.0,0.0,1.0);
        // }

        // if(x-(pow(1.0-0.5,2) *positions[0].x+2*0.5*(1-0.5)*positions[1].x+ pow(0.5,2) *positions[2].x)<abs(xtester))
        // {
        //     outColor = vec4(0.0,0.0,0.0,1.0);
        // }

        // if(y-(pow(1.0-0.5,2) *positions[0].y+2*0.5*(1-0.5)*positions[1].y+ pow(0.5,2) *positions[2].y)<abs(ytester))
        // {
        //     outColor = vec4(0.0,0.0,0.0,1.0);
        // }
    // }
    // if((sqrt(pow(x-nnewX,2)+pow(y-nnewY,2))<0.01))
    // {
        // outColor = vec4(1.0,1.0,1.0,1.0);
    // }

    // (math.sqrt(-p0y*p2y+p0y*y+math.pow(p1y,2)-2*p1y*y+p2y*y)-p0y+p1y)/(-p0y+2*p1y-p2y);


    // if(bX||bY)
    // {
        // float t = (tY+tX)/2;
        // t = abs(tY-tX);
        // float PY = (1.0-t)*positions[0].y + 2.0*t*(1.0-t)*positions[1].y + pow(t,2)*positions[2].y;
        // float PX = (1.0-t)*positions[0].x + 2.0*t*(1.0-t)*positions[1].x + pow(t,2)*positions[2].x;
        // if(abs(tY-tX)<0.01)
            // outColor = vec4(tY,0.0,0.0,1.0);

        // if((tY+tX)/2<0.1)
        // outColor = vec4(tY,tX,(tY+tX)/2,1.0);
    // }

    // if(sqrt(pow(x-positions[0].x,2)+pow(y-positions[0].y,2))<0.01)
    //     outColor = vec4(0.0,0.2,0.8,1.0);
    // else if(sqrt(pow(x-positions[1].x,2)+pow(y-positions[1].y,2))<0.01)
    //     outColor = vec4(0.0,0.1,0.9,1.0);
    // else if(sqrt(pow(x-positions[2].x,2)+pow(y-positions[2].y,2))<0.01)
    //     outColor = vec4(0.0,0.0,1.0,1.0);
    // else
    // {
    //     float PY = pow(1-x,2)*positions[0].y+2*x*(1-x)*positions[1].y+pow(x,2)*positions[2].y;
    //     if(PY > 0.6)
    //         outColor = vec4(0.5,0.0,0.0,1.0);
    //     else
    //         outColor = vec4(0.0,0.0,0.1,1.0);
    // }

// #version 450

// layout(binding = 1) uniform sampler2D texSampler;

// layout(location = 0) in vec4 fragColor;
// layout(location = 1) in vec2 fragTexCoord;

// layout(location = 0) out vec4 outColor;

// //gl_FragCoord = the exact pixel coordinate

// vec2 positions[3] = vec2[](
//     vec2(0.7, 0.5),
//     vec2(0.6, 0.8),
//     vec2(0.3, 0.6)
// );

// void main() {
//     // step 1, normalize the pixel position
//     vec2 screenSize = {600,600};
//     float x = gl_FragCoord.x/screenSize.x;
//     float y = gl_FragCoord.y/screenSize.y;
//     // step 2, my lovely quadratic equation

//     // float PY = (1.0-y)*positions[0].y + 2.0*y*(1.0-y)*positions[1].y + pow(y,2)*positions[2].y;?
//     // float PX = (1.0-x)*positions[0].x + 2.0*x*(1.0-x)*positions[1].x + pow(x,2)*positions[2].x;

//     if(sqrt(pow(x-positions[0].x,2)+pow(y-positions[0].y,2))<0.01)
//         outColor = vec4(0.0,0.2,0.8,1.0);
//     else if(sqrt(pow(x-positions[1].x,2)+pow(y-positions[1].y,2))<0.01)
//         outColor = vec4(0.0,0.1,0.9,1.0);
//     else if(sqrt(pow(x-positions[2].x,2)+pow(y-positions[2].y,2))<0.01)
//         outColor = vec4(0.0,0.0,1.0,1.0);
//     else
//         outColor = vec4(0.0,0.0,0.3,1.0);//texture(texSampler, fragTexCoord)*fragColor;

//     bool bX = false;
//     bool bY = false;
//     float tX = 0.0;
//     float tY = 0.0;

//     if((positions[2].x==2*positions[1].x-positions[0].x) && (positions[0].x-positions[1].x!=0))
//     {
//         tX = (positions[0].x-x)/(2*(positions[0].x-positions[1].x));

//         if(tX>=0&&tX<=1)
//             bX=true;
//     } else if(positions[0].x-2*positions[1].x+positions[2].x!=0)
//     {
//         tX = (sqrt(-positions[0].x*positions[2].x+positions[0].x*x+pow(positions[1].x,2)-2*positions[1].x*x+positions[2].x*x)-positions[0].x+positions[1].x)/(-positions[0].x+2*positions[1].x-positions[2].x);

//         if(abs(tX)>=0&&abs(tX)<=1)
//             bX=true;
//     }

//     if((positions[2].y==2*positions[1].y-positions[0].y) && (positions[0].y-positions[1].y!=0))
//     {
//         tY = (positions[0].y-y)/(2*(positions[0].y-positions[1].y));

//         if(tY>=0&&tY<=1)
//             bY=true;
//     } else if(positions[0].y-2*positions[1].y+positions[2].y!=0)
//     {
//         tY = (sqrt(-positions[0].y*positions[2].y+positions[0].y*y+pow(positions[1].y,2)-2*positions[1].y*y+positions[2].y*y)-positions[0].y+positions[1].y)/(-positions[0].y+2*positions[1].y-positions[2].y);

//         if(abs(tY)>=0&&abs(tY)<=1)
//             bY=true;
//     }

//     // if(bX||bY)
//     {
//         // float t = (tY+tX)/2;
//         // t = abs(tY-tX);
//         // float PY = (1.0-t)*positions[0].y + 2.0*t*(1.0-t)*positions[1].y + pow(t,2)*positions[2].y;
//         // float PX = (1.0-t)*positions[0].x + 2.0*t*(1.0-t)*positions[1].x + pow(t,2)*positions[2].x;
//         // if(abs(tY-tX)<0.01)
//             // outColor = vec4(tY,0.0,0.0,1.0);

//         // if((tY+tX)/2<0.1)
//         outColor = vec4(tY,tX,(tY+tX)/2,1.0);
//     }

//         if(sqrt(pow(x-positions[0].x,2)+pow(y-positions[0].y,2))<0.01)
//         outColor = vec4(0.0,0.2,0.8,1.0);
//     else if(sqrt(pow(x-positions[1].x,2)+pow(y-positions[1].y,2))<0.01)
//         outColor = vec4(0.0,0.1,0.9,1.0);
//     else if(sqrt(pow(x-positions[2].x,2)+pow(y-positions[2].y,2))<0.01)
//         outColor = vec4(0.0,0.0,1.0,1.0);
// }