varying vec2 vUV;
varying vec3 vNormal;

uniform sampler2D uTexture;

void main(void){
	// lighting calculations
	vec3 lightDir = normalize(vec3(0, 1, -1));
	float ndl = dot(vNormal, lightDir);

	// color operations
	vec4 textel = texture2D(uTexture, vUV);
	vec3 color = textel.xyz;
	vec3 diffuse = color * ndl;

	gl_FragColor = vec4(diffuse, textel.a);
}