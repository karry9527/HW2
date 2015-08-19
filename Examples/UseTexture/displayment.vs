varying vec3 normal, lightDir, eyeDir;

uniform sampler2D heightTexture;
varying vec4 Color;
uniform int control;

void main()
{	/*
	//phong shading test
	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeDir = -vVertex;
	//end
	*/


	vec4 a = gl_Vertex;
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy ;

	
	vec3 hei_tex = texture2D(heightTexture, gl_TexCoord[0].xy).rbg;
	hei_tex *= 2;

	a = a + vec4 (hei_tex  * gl_Normal, 0) ;
	gl_Vertex = a;
	gl_Position = ftransform();	
	
	
}

