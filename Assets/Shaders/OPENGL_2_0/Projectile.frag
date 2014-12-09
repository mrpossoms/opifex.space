uniform vec4 uColor;

varying vec2 vUV;

void main(){
    float alpha = clamp(1.0 - length(vUV - vec2(0.5, 0.5)) * 2.0, 0.0, 1.0);
    if(alpha == 0.5) discard;
    
   	float aa = alpha * uColor.a;
    
    gl_FragColor = vec4(uColor.rgb * aa + vec3(alpha * alpha), aa);
}