#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Importuje color z Vertex Shadera
in vec3 color;
//Importuje kordyynaty textur z Vertex Shadera
in vec2 texCoord;
//Importuje normalsy z Vertex Shadera
in vec3 Normal;
//Importuje obecn¹ pozycje z Vertex Shadera
in vec3 crntPos;

//Pobiera texture z g³ównej funkcji
uniform sampler2D tex0;
uniform sampler2D tex1;

//Pobiera color swiatla z glownej funkcji
uniform vec4 lightColor;
//Pobiera pozycje swiatla z glownej funkcji
uniform vec3 lightPos;
//Pobiera pozycje kamery z glownej funkcji
uniform vec3 camPos;

void main()
{
	//ambient lighting
	float ambient = 0.2f;

	//diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	float diffuse = max(dot(normal, lightDirection),0.0f);

	//specular lighting
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection),0.0f),8);
	float specular = specAmount * specularLight;

	//wynik
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient) + texture(tex1,texCoord).r * specular;

}