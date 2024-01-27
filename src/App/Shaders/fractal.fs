#version 330 core

uniform int iters;
uniform float threshold;
uniform vec2 windowSize;
uniform vec2 sizes;
uniform vec2 fromPos;
uniform int cx;
uniform int cy;
uniform int r0;
uniform int g0;
uniform int b0;

out vec4 out_col;

void main() {
    float threshold2 = threshold * threshold;

    vec2 xy0 = fromPos + ((gl_FragCoord.xy / windowSize) - 0.5) * sizes;
    vec2 p = xy0;
    //vec2 c = vec2(-0.8 ,0.156);
    vec2 c = vec2(float(cx / 1000.0), float(cy/ 1000.0));

    vec3 color=vec3(0.0,0.0,0.0);
    for (int i = 0; i < iters; ++i) {
        p = vec2(p.x*p.x-p.y*p.y, 2.0*p.x*p.y) + c;
        if (dot(p,p) > 4){
            //The point, c, is not part of the set, so smoothly color it. colorRegulator increases linearly by 1 for every extra step it takes to break free.
        	float colorRegulator = float(i-1)-log(((log(dot(p,p)))/log(2.0)))/log(2.0);
        	//This is a coloring algorithm I found to be appealing. Written in HSV, many functions will work.
        	color = vec3((r0 / 100.0) + .012*colorRegulator , (g0 / 100.0), (b0 / 100.0) +.4*(1.0+sin(.2*colorRegulator)));
        	break;
        }
    }

    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
    color = color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y);
    out_col = vec4(color.rgb, 1.0);
}