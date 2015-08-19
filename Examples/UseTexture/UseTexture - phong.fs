varying vec3 normal, tan, bitan;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;
varying float h1, h2;
attribute vec3 tangent, bitangent;
uniform sampler2D colorTexture;


varying vec3 N, v;

void main (void)
{	
	tan = normalize(tan);
	bitan = normalize(bitan);
	N = N - h1 * tan - h2 * bitan;
	//N = vec3(-5.33276,  0,  -10.4662);
	vec3 L = normalize(gl_LightSource[0].position.xyz - v);   
	vec3 E = normalize(-v); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N));  
 
	//calculate Ambient Term:  
	vec4 Iamb = gl_FrontLightProduct[0].ambient;    

	//calculate Diffuse Term:  
	vec4 Idiff = texture2D(colorTexture, gl_TexCoord[0].xy).rgba * max(dot(N,L), 0.0);
	Idiff = clamp(Idiff, 0.0, 1.0);     
   
	// calculate Specular Term:
	vec4 Ispec = gl_FrontLightProduct[0].specular 
				* pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
	Ispec = clamp(Ispec, 0.0, 1.0); 
	// write Total Color:  
	gl_FragColor = gl_FrontLightModelProduct.sceneColor + Iamb + Idiff + Ispec;
	//gl_FragColor = vec4(bitan, 0);
}
/*void main() {
 
// Extract the normal from the normal map
vec3 normal = normalize(texture2D(normal_texture, gl_TexCoord[0].xy).rgb * 2.0 - 1.0);
 
// Determine where the light is positioned
vec3 light_pos = normalize(vec3(1.0, 1.0, 1.5));
 
// Calculate the lighting diffuse value
float diffuse = max(dot(normal, light_pos), 0.0);
 
vec3 color = diffuse * texture2D(colorTexture, gl_TexCoord[0].xy).rgb;
 
// Set the output color of our current pixel
gl_FragColor = vec4(color, 1.0);
}*/