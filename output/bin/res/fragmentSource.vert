#version 330 core
in vec4 vertexColor; 
in vec2 texCoord;
uniform sampler2D tex0;

 vec2 u_resolution=vec2(400,300);


 void main()
 {
     //FragColor =ourColor+vertexColor;
     //FragColor=vec4(gl_PointCoord,0,1);
    // vec2 st=gl_FragCoord.xy;
    //  gl_FragColor=vec4(st.y/10,0,,1);
    //  vec2 st = gl_FragCoord.xy / u_resolution.xy;
    //  gl_FragColor = vec4(st, 0.f, 1.f);
     vec2 st = (gl_FragCoord.xy-500)/u_resolution.xy;
    st.x *= u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.);
    color = vec3(st.x,st.y,0.4);

    // gl_FragColor = vec4((gl_FragCoord.x-500)2300,0,0,1.0);
    gl_FragColor = texture2D(tex0,vec2(gl_FragCoord.x-500)/300);
     
 }

// #version 330 core

// uniform vec2 u_resolution;
// uniform vec2 u_mouse;
// uniform float u_time;

// void main() {
//     vec2 st = gl_FragCoord.xy/u_resolution.xy;
//     st.x *= u_resolution.x/u_resolution.y;
	
//     vec3 color = vec3(0.);
//     color = vec3(st.x,st.y,abs(sin(u_time)));
//     gl_FragColor = vec4(color,1.0);
// }