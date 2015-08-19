varying vec3 normal, lightDir, eyeDir, tan, bitan;
varying float h1, h2;
attribute vec3 tangent, bitangent;
uniform sampler2D colorTexture;

void main (void)
{
	//tan = normalize(tan);
	bitan = normalize(bitan);
	normal = normal - h1 * tan - h2 * bitan;
	vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
							
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += gl_LightSource[0].diffuse * 
		               gl_FrontMaterial.diffuse * 
					   lambertTerm;	
		
		vec3 E = normalize(eyeDir);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * 
		               gl_FrontMaterial.specular * 
					   specular;	
	}
	
	//gl_FragColor = vec4(normal, 1) * texture2D(colorTexture, gl_TexCoord[0].xy).rgba;
	gl_FragColor = final_color * texture2D(colorTexture, gl_TexCoord[0].xy).rgba;	
	//gl_FragColor = vec4(tan, 1.0);
	//gl_FragColor = texture2D(colorTexture, gl_TexCoord[0].xy).rgba;	

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