// #version 460 core
// layout (location = 0) in vec3 aPos;

// uniform mat4 projection;
// uniform mat4 view;

// out vec4 eyepos;

// void main()
// {
//     eyepos = view * vec4(aPos, 1.0);
//     gl_Position = projection * eyepos;
//     gl_PointSize = 5.0f; // Set point size for rendering

//     // gl_ClipDistance[0] = eyepos.z+ 1;
// }
#version 460 core
out vec2 TexCoord;

const vec2 verts[4] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 1.0, -1.0),
    vec2(-1.0,  1.0),
    vec2( 1.0,  1.0)
);

const vec2 uvs[4] = vec2[](
    vec2(0.0, 0.0),
    vec2(1.0, 0.0),
    vec2(0.0, 1.0),
    vec2(1.0, 1.0)
);

void main() {
    gl_Position = vec4(verts[gl_VertexID], 0.0, 1.0);
    TexCoord = uvs[gl_VertexID];
}
