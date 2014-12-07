attribute vec3 aPosition;
attribute vec2 aUV;
attribute vec3 aNormal;

uniform mat4 uWorld; 
uniform mat4 uView;
uniform mat4 uProj; 

varying vec2 vUV; 
varying vec3 vNormal;

void main(void){
	vec4 worldPos = uWorld * vec4(aPosition, 1.0);
	mat3 rot = mat3(
		normalize(uWorld[0].xyz),
		normalize(uWorld[1].xyz),
		normalize(uWorld[2].xyz)
	);

	gl_Position = (uProj * uView) * worldPos; 
	
	vUV = aUV;
	vNormal = rot * aNormal;
}