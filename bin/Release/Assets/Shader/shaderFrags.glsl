uniform sampler2D texture;

void main()
{

    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 px2 = pixel + 20;
    
    
    gl_FragColor =  pixel * 0.5;
}
