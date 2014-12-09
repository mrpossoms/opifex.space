varying vec2 vUV;
varying vec3 vNormal;

uniform sampler2D uTexture;
uniform sampler2D uEngineMask;

uniform float uEnginePower;

void main(void){
	// lighting calculations
	vec3 lightDir = normalize(vec3(0, 1, -1));
	float ndl = dot(vNormal, lightDir);

	// color operations
	vec4 textel = texture2D(uTexture, vUV);
	vec4 engine = texture2D(uEngineMask, vUV);
	vec3 color = textel.xyz;
	vec3 diffuse = color * min(max(0.0, ndl * 0.8) + engine.a * uEnginePower, 0.8);
	vec3 ambient = color * vec3(0.2, 0.2, 0.2);

	gl_FragColor = vec4(diffuse + ambient, textel.a);
}