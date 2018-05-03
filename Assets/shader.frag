uniform sampler2D texture;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 color = gl_Color * pixel;
    // gl_FragColor.r = (color.r * 1.0 + 0.5) / 2.0;
    // gl_FragColor.g = (color.g * 1.0 + 0.5) / 2.0;
    // gl_FragColor.b = (color.b * 1.0 + 0.5) / 2.0;
    // gl_FragColor.a = color.a;
    gl_FragColor = color;
}
