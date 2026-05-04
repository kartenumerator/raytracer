// #version 460 core

// in vec4 eyepos;
// out vec4 FragColor;

// uniform vec2 u_resolution;

// void main()
// {
//     vec2 screen_coords = gl_FragCoord.xy;
//     float depthValue = 1 - abs(eyepos.z)/25;
//     if(depthValue < 0.0f){
//         depthValue = 0.0f;
//     }
//     if(depthValue >1.0f){
//         depthValue = 1.0f;
//     }

//     vec2 normalized_screen_coords = screen_coords / u_resolution;
//     FragColor = vec4(normalized_screen_coords, 1.0f, depthValue);
// }
#version 460 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D computeTex;

void main() {
    FragColor = texture(computeTex, TexCoord);
}
