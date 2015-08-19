varying vec3 normal, tan, bitan;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;
varying float h1, h2;
attribute vec3 tangent, bitangent;
uniform sampler2D colorTexture;

void main (void)
{
	// lookup normal from normal map, move from [0,1] to  [-1, 1] range, normalize
	normal =  normal - h1 * tan - h2 * bitan;;
	normal = normalize (normal);
	
	// compute diffuse lighting
	float lamberFactor= max (dot (lightVec, normal), 0.0) ;
	vec4 diffuseMaterial = 0.0;
	vec4 diffuseLight  = 0.0;
	
	// compute specular lighting
	vec4 specularMaterial ;
	vec4 specularLight ;
	float shininess ;
  
	// compute ambient
	vec4 ambientLight = gl_LightSource[0].ambient;	
	
	if (lamberFactor > 0.0)
	{
		diffuseMaterial = texture2D (colorTexture, gl_TexCoord[0].xy).rgba;
		diffuseLight  = gl_LightSource[0].diffuse;
		
		// In doom3, specular value comes from a texture 
		specularMaterial =  vec4(1.0)  ;
		specularLight = gl_LightSource[0].specular;
		shininess = pow (max (dot (halfVec, normal), 0.0), 2.0)  ;
		 
		gl_FragColor =	diffuseMaterial * diffuseLight * lamberFactor ;
		gl_FragColor +=	specularMaterial * specularLight * shininess ;				
	
	}
	
	gl_FragColor +=	ambientLight;

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