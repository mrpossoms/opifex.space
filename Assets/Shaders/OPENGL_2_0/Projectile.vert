attribute vec3 aPosition;
attribute vec2 aUV;

uniform mat4 uView;
uniform mat4 uProj;
uniform vec3 uPosition;

varying vec2 vUV;

void main(){
	vec4 cornerPos = uProj * vec4(aPosition * 0.25, 0.0);
    vec4 screenPos = uProj * uView * vec4(uPosition, 1.0);
    gl_Position = (screenPos + cornerPos) * screenPos.w;
    vUV = aUV;
}