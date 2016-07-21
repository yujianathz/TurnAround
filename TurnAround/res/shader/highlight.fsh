#ifdef GL_ES
precision lowp float;
#endif

varying vec2 v_texCoord;   
varying vec4 v_fragmentColor;   
void main(void)   
{   
    vec4 normalColor = texture2D(CC_Texture0, v_texCoord);
    if (normalColor.a > 0.0)
    {
        normalColor.r += 0.2;
		    if (normalColor.r > 1.0) normalColor.r = 1.0;
		    normalColor.g += 0.2;
		    if (normalColor.g > 1.0) normalColor.g = 1.0;
		    normalColor.b += 0.2;
		    if (normalColor.b > 1.0) normalColor.b = 1.0; 
    }
    gl_FragColor = v_fragmentColor * normalColor;  
}  