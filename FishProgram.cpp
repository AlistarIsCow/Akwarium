#include "FishProgram.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <random>
#include "shaderprogram.h"
#include "constants.h"

Fish::Fish() {}
Fish::Fish(glm::vec3 aktualne_coord_f, glm::vec3 aktualny_rotat_f, glm::vec3 aktualny_scale_f, glm::vec3 cel_coord_f,
	GLuint tex_fish_f, float* vertices_, float* normals_, float* texCoords_, float* colors_, int vertexCount_, int mnoznik_vertex_, int mnoznik_predkosc_)
{
	aktualne_coord = aktualne_coord_f;
	aktualny_rotat = aktualny_rotat_f;
	aktualny_scale = aktualny_scale_f;
	cel_coord = cel_coord_f;
	tex_fish = tex_fish_f;
	vertices = vertices_;
	normals = normals_;
	texCoords = texCoords_;
	colors = colors_;
	vertexCount = vertexCount_;
	iterations = 0;
	x = aktualne_coord.x; y = aktualne_coord.y; z = aktualne_coord.z;
	r_x = aktualny_rotat.x; r_y = aktualny_rotat.y; r_z = aktualny_rotat.z;
	s_x = aktualny_scale.x; s_y = aktualny_scale.y; s_z = aktualny_scale.z;
	mnoznik_predkosc = mnoznik_predkosc_;
	mnoznik_vertex = mnoznik_vertex_;
	predkosc_fish = 0.01f;
}

float random(float min, float max)
{
	std::random_device rd;
	std::mt19937 e2(rd());
	std::uniform_real_distribution<> dist(min, max);

	return dist(e2);
}

void Fish::rysuj(ShaderProgram* sp) 
{
	
	//std::cout << random(1.78, 3.19) << "\n";
	//std::cout << "tujestem rysuje\n";
	//sp->use();
	glm::mat4 M = glm::mat4(1.0f);

	M = glm::translate(M, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::scale(M, glm::vec3(s_x, s_y, s_z));
	M = glm::translate(M, glm::vec3(x, y, z));
	M = glm::rotate(M, glm::radians(r_x), glm::vec3(1.0f, 0.0f,0.0f));
	M = glm::rotate(M, glm::radians(r_y), glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, glm::radians(r_z), glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glEnableVertexAttribArray(sp->a("vertex"));
	glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(sp->a("anormal"));
	glVertexAttribPointer(sp->a("anormal"), 4, GL_FLOAT, false, 0, normals);
	glEnableVertexAttribArray(sp->a("texCoord"));
	glVertexAttribPointer(sp->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_fish);

	glUniform1i(sp->u("textureMap"), 0);
	glDrawArrays(GL_TRIANGLES, 0, vertexCount);

	glDisableVertexAttribArray(sp->a("vertex"));
	//glDisableVertexAttribArray(sp->a("normal"));
	glDisableVertexAttribArray(sp->a("texCoord"));
}



void Fish::generuj_cel()
{
	//std::cout << iterations << "\n";
	if (iterations == 0)
	{
		predkosc_fish = random(0.05f* mnoznik_predkosc, 0.1f* mnoznik_predkosc);
		cel_coord = glm::vec3(random(-MAX_X* mnoznik_vertex, MAX_X* mnoznik_vertex), random(MIN_Y* mnoznik_vertex, MAX_Y* mnoznik_vertex), random(-MAX_Z* mnoznik_vertex, MAX_Z* mnoznik_vertex));
		//std::cout << cel_coord.y << "\n";
		iterations = round(distance(cel_coord, glm::vec3(x, y, z)) / predkosc_fish);

		r_x = glm::degrees(skrecanie(glm::vec3(x, y, z), cel_coord).x);
		r_y = glm::degrees(skrecanie(glm::vec3(x, y, z), cel_coord).y);
	}
	else
	{
		//std::cout << cel_coord.x <<"\n";
		ruch(cel_coord);
		iterations--;
	}
}

void Fish::ruch(glm::vec3 coords)
{
	//std::cout << "tujestem ruszam sie"<<x<<"\n";
	float odleglosc = distance(coords, glm::vec3(x, y, z));
	x += (coords.x - x) / odleglosc * predkosc_fish;
	y += (coords.y - y) / odleglosc * predkosc_fish;
	z += (coords.z - z) / odleglosc * predkosc_fish;
}

glm::vec2 Fish::skrecanie(glm::vec3 vector_1, glm::vec3 vector_2)
{
	float odleglosc = glm::distance(vector_1, vector_2);
	float odleglosc_X_Z = glm::distance(glm::vec2(vector_1.x, vector_1.z), glm::vec2(vector_2.x, vector_2.z));
	float r_x = glm::acos(odleglosc_X_Z / odleglosc);
	if (vector_2.y < vector_1.y)
		r_x = -r_x;

	float r_y = glm::acos((vector_2.z - vector_1.z)/ 
		glm::distance(glm::vec2(vector_1.x, vector_1.z), glm::vec2(vector_2.x, vector_2.z)));
	if (vector_2.x < vector_1.x)
		r_y = -r_y;

	return glm::vec2(r_x, r_y);
}