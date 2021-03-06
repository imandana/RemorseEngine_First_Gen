uniform float wave_phase;
uniform vec2 wave_amplitude;

void main()
{
    vec4 vertex = gl_Vertex;
    vertex.x += cos(gl_Vertex.x ) * wave_phase;


    gl_Position = gl_ModelViewProjectionMatrix * vertex;
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}
