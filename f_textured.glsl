#version 330


uniform sampler2D tex;
uniform vec3 water_color;
uniform vec3 light_position1;
uniform vec3 light_color1;
uniform vec3 light_position2;
uniform vec3 light_color2;

out vec4 pixel_color; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 i_tc;
in float visibility;
in vec3 normal;
in vec3 frag_position;



void main(void) {

	vec3 norm = normalize(normal);	//znormalizowanie wektor�w normalnych

	//ambient 1 (o�wietlenie proste)
	float ambient_strength1 = 0.1;	//ustawienie mno�nika o�wietlenia
	vec3 ambient1 = ambient_strength1 * light_color1;	//ustawienie o�wietlenia "ambient"
	
	//diffuse 1 (o�wietlenie rozproszone)
	vec3 light_direction1 = normalize(light_position1 - frag_position);	//znormalizowanie wektora kierunku �wiat�a
	float diffuse_strength1 = max(dot(norm, light_direction1), 0.0);	//wyliczenie rozproszenia
	vec3 diffuse1 = diffuse_strength1 * light_color1;	//ustawienie o�wietlenia "diffuse"

	//ambient 2 (o�wietlenie proste)
	float ambient_strength2 = 0.1;	//ustawienie mno�nika o�wietlenia
	vec3 ambient2 = ambient_strength2 * light_color2;	//ustawienie o�wietlenia "ambient"
	
	//diffuse 2 (o�wietlenie rozproszone)
	vec3 light_direction2 = normalize(light_position2 - frag_position);	//znormalizowanie wektora kierunku �wiat�a
	float diffuse_strength2 = max(dot(norm, light_direction2), 0.0);	//wyliczenie rozproszenia
	vec3 diffuse2 = diffuse_strength2 * light_color2;	//ustawienie o�wietlenia "diffuse"

	//wynik
	pixel_color = texture(tex,i_tc);	//ustawienie tekstur
	pixel_color = pixel_color * vec4((ambient1 + diffuse1 + ambient2 + diffuse2), 1.0);	//dodanie o�wietlenia
	pixel_color = mix(vec4(water_color, 1.0), pixel_color, visibility);	//dodanie koloru wody

}