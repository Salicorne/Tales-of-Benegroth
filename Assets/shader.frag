uniform sampler2D texture;
uniform float time;

void main() {
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
    vec4 color = gl_Color * pixel;

    vec4 dawn  = vec4(0.6, 0.8, 1.0, 1.0);
    vec4 day   = vec4(1.0, 1.0, 0.95, 1.0);
    vec4 dusk  = vec4(1.0, 0.7, 0.4, 1.0);
    vec4 night = vec4(0.4, 0.5, 0.6, 1.0);
    
    vec4 light = night;

    /*if(time > 6.0 && time <= 7.0) {
        float factor = time - 6.0;
        light = dawn * factor + night * (1.0 - factor);
    }
    else if(time > 7.0 && time <= 9.0) {
        light = dawn;
    }
    else if(time > 9.0 && time <= 10.0) {
        float factor = time - 9.0;
        light = day * factor + dawn * (1.0 - factor);
    }
    else if(time > 10.0 && time <= 18.0) {
        light = day;
    }
    else if(time > 18.0 && time <= 19.0) {
        float factor = time - 18.0;
        light = dusk * factor + day * (1.0 - factor);
    }
    else if(time > 19.0 && time <= 21.0) {
        light = dusk;
    }
    else if(time > 21.0 && time <= 22.0) {
        float factor = time - 21.0;
        light = night * factor + dusk * (1.0 - factor);
    }*/

    if(time > 1.0 && time <= 6.0) {
        float factor = (time - 1.0) / 5.0;
        light = dawn * factor + night * (1.0 - factor);
    }
    else if(time > 6.0 && time <= 14.0) {
        float factor = (time - 6.0) / 8.0;
        light = day * factor + dawn * (1.0 - factor);
    }
    else if(time > 14.0 && time <= 19.0) {
        float factor = (time - 14.0) / 5.0;
        light = dusk * factor + day * (1.0 - factor);
    }
    else if(time > 19.0) {
        float factor = (time - 19.0) / 5.0;
        light = night * factor + dusk * (1.0 - factor);
    }

    gl_FragColor = color * light;
}
