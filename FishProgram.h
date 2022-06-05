#ifndef FISHPROGRAM_H_INCLUDED
#define FISHPROGRAM_H_INCLUDED

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shaderprogram.h"
#include <random>



class Fish {
//protected:
//	template<class T>
//		T random(T min, T max);
private:
	float x, y, z;			//aktualna pozycja
	float r_x, r_y, r_z;	//obrót
	float s_x, s_y, s_z;	//skalowanie modelu
	glm::vec3 aktualne_coord; 
	glm::vec3 aktualny_rotat;
	glm::vec3 aktualny_scale;
	glm::vec3 cel_coord;
	float predkosc_fish;
	GLuint tex_fish;
	int iterations;			//kroki
	float* vertices;
	float* normals;
	float* texCoords;
	float* colors;
	int vertexCount;
	int mnoznik_vertex;
	int mnoznik_predkosc;

public:

	Fish();
	Fish(glm::vec3 aktualne_coord,
		glm::vec3 aktualny_rotat, glm::vec3 aktualny_scale, glm::vec3 cel_coord, GLuint tex_fish,
		float* vertices, float* normals, float* texCoords, float* colors, int vertexCount, int mnoznik_vertex, int mnoznik_predkosc);
	void rysuj(ShaderProgram* sp);

	void generuj_cel();
	void ruch(glm::vec3 coords);
	glm::vec2 skrecanie(glm::vec3 vector_1, glm::vec3 vector_2);



};

//template<class T>
//T Fish::random(T min, T max) //random number generator
//{ 
//	std::mt19937 gen;
//	using dist = std::conditional_t <
//		std::is_integral<T>::value,
//		std::uniform_int_distribution<T>,
//		std::uniform_real_distribution<T>>;
//	return dist{ min, max }(gen);
//}
#endif