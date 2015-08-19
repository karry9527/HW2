varying vec3 normal, tan, bitan;
varying vec3 lightVec;
varying vec3 halfVec;
varying vec3 eyeVec;
varying float h1, h2;
attribute vec3 tangent, bitangent;

varying vec3 N, v;

varying vec4 Color;
uniform sampler2D heightTexture;
uniform int control;

void main()
{	
	gl_TexCoord[0].xy = gl_MultiTexCoord0.xy ;
	v = vec3(gl_ModelViewMatrix * gl_Vertex);       
	N = normalize(gl_NormalMatrix * gl_Normal);
	  

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

		h1 = (hei_tex1 - hei_tex) * 100;
		h2 = (hei_tex2 - hei_tex) * 100;

		//normal = normal - h1 * tangent - h2 * bitangent;
		tan = tangent;
		bitan = bitangent;
		//gl_Normal = normalize(cross(vec3(1.0,0.0,(hei_tex1 - hei_tex)), vec3(0.0,1.0,(hei_tex2 - hei_tex))));
		//gl_Normal = vec3(1,1,1);
		//gl_Position = ftransform();	
		
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

