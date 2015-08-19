varying vec3 normal, tan, bitan;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;
varying float h1, h2;
attribute vec3 tangent, bitangent;


varying vec4 Color;
uniform sampler2D heightTexture;
uniform int control;

void main()
{	
	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy ;
	
	normal = gl_NormalMatrix * gl_Normal;
	// Building the matrix Eye Space -> Tangent Space
	vec3 n = normalize (gl_NormalMatrix * gl_Normal);
	vec3 t = normalize (gl_NormalMatrix * tangent);
	vec3 b = cross (n, t);
	
	vec3 vertexPosition = vec3(gl_ModelViewMatrix *  gl_Vertex);
	vec3 lightDir = normalize(gl_LightSource[0].position.xyz - vertexPosition);
		
		
	// transform light and half angle vectors by tangent basis
	vec3 v;
	v.x = dot (lightDir, t);
	v.y = dot (lightDir, b);
	v.z = dot (lightDir, n);
	lightVec = normalize (v);
	
	  
	v.x = dot (vertexPosition, t);
	v.y = dot (vertexPosition, b);
	v.z = dot (vertexPosition, n);
	eyeVec = normalize (v);
	
	
	vertexPosition = normalize(vertexPosition);
	
	/* Normalize the halfVector to pass it to the fragment shader */

	// No need to divide by two, the result is normalized anyway.
	// vec3 halfVector = normalize((vertexPosition + lightDir) / 2.0); 
	vec3 halfVector = normalize(vertexPosition + lightDir);
	v.x = dot (halfVector, t);
	v.y = dot (halfVector, b);
	v.z = dot (halfVector, n);

	// No need to normalize, t,b,n and halfVector are normal vectors.
	//normalize (v);
	halfVec = v ; 
	  

	if(control == 1)
	{
		vec4 a = gl_Vertex;

		vec3 hei_tex = texture2D(heightTexture, gl_TexCoord[0].xy).rbg;
		hei_tex *= 2;

		//a = a + texture2D(heightTexture, gl_TexCoord[0].xy) * vec4(gl_Normal, 0) ;
		a = a + vec4 (hei_tex  * gl_Normal, 0) ;
		//a = a + texture2D(heightTexture, gl_TexCoord[0].xy);
		gl_Vertex = a;
		gl_Position = ftransform();	
	}
	else if(control == 2)
	{
		
		float hei_tex = texture2D(heightTexture, gl_TexCoord[0].xy).r;
		float hei_tex1 = texture2D(heightTexture, gl_TexCoord[0].xy + vec2(0.01, 0.0)).r;
		float hei_tex2 = texture2D(heightTexture, gl_TexCoord[0].xy + vec2(0.0, 0.01)).r;

		h1 = (hei_tex1 - hei_tex) * 10;
		h2 = (hei_tex2 - hei_tex) * 10;

		//normal = normal - h1 * tangent - h2 * bitangent;
		tan = tangent;
		bitan = bitangent;
		//gl_Normal = normalize(cross(vec3(1.0,0.0,(hei_tex1 - hei_tex)), vec3(0.0,1.0,(hei_tex2 - hei_tex))));
		//gl_Normal = vec3(1,1,1);
		gl_Position = ftransform();	
	}
	/*vec4 t = gl_Vertex;
	t.y = gl_Vertex.y + 0.4*sin(0.1*time);
	
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * t;
	
	vec4 color = vec4((sin(0.25*time)+1)*0.5, 1.0, 0.0, 1.0);
	gl_FrontColor = color;

	// bump map test
	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy ;
 
	// Set the position of the current vertex
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;*/

}

