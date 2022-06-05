/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "FishProgram.h"

float speed_x = 0; //[radiany/s]
float speed_y = 0; //[radiany/s]
float walk_speed = 0;

float box_x = 20; //szerokość szafki
float box_y = 3;  //wysokość szafki
float box_z = 20;  //długość szafki

float aqua_x = 20; //szerokość akwarium
float aqua_y = 10;  //wysokość akwarium
float aqua_z = 20;  //długość akwarium
float water_r = 0.3; //wartość Red koloru wody
float water_g = 0.48; //wartość Green koloru wody
float water_b = 0.5; //wartość Blue koloru wody	

float room_size = 30; //wielkość pokoju

float camera_tolerance = 0.5; //minimalna odległość kamery od ścian akwarium

//Światło 1
float light1_x = room_size/2 - 2;
float light1_y = room_size / 2;
float light1_z = 0;

float light1_r = 1;  
float light1_g = 1;
float light1_b = 1;

//Światło 2
float light2_x = 0;
float light2_y = room_size - 2;
float light2_z = 0;

float light2_r = 0;
float light2_g = 1;
float light2_b = 1;


ShaderProgram* sp;
GLuint tex_box, tex_room, tex_sand, tex_floor, tex_aqua, tex_roof, tex_fish1, tex_fish2, tex_fish3, tex_fish4, tex_fish5, tex_fish6, tex_fish7, tex_fish8, tex_fish9, tex_fish10,
tex_seashell, tex_seaweed;

Fish rybki[23] = { Fish(), Fish(), Fish(), Fish(), Fish(),Fish(),Fish(), Fish(),
Fish(),Fish(),Fish(),Fish(),Fish(),Fish(), Fish(), Fish(), Fish(), Fish(), Fish(), Fish(), Fish(), Fish(), Fish() };

//Tablice współrzędnych wierzchołków
float box_vertices[] = {						//Szafka
	-box_x / 2, 0, -box_z / 2, 1.0f,			//Front
	box_x / 2, 0, -box_z / 2, 1.0f,
	box_x / 2, box_y, -box_z / 2, 1.0f,
	-box_x / 2, 0, -box_z / 2, 1.0f,
	box_x / 2, box_y, -box_z / 2, 1.0f,
	-box_x / 2, box_y, -box_z / 2, 1.0f,

	-box_x / 2, 0, box_z / 2, 1.0f,				//Tył
	box_x / 2, 0, box_z / 2, 1.0f,
	box_x / 2, box_y, box_z / 2, 1.0f,
	-box_x / 2, 0, box_z / 2, 1.0f,
	box_x / 2, box_y, box_z / 2, 1.0f,
	-box_x / 2, box_y, box_z / 2, 1.0f,

	box_x / 2, 0, -box_z / 2, 1.0f,				//Lewo
	box_x / 2, 0, box_z / 2, 1.0f,
	box_x / 2, box_y, box_z / 2, 1.0f,
	box_x / 2, 0, -box_z / 2, 1.0f,
	box_x / 2, box_y, box_z / 2, 1.0f,
	box_x / 2, box_y, -box_z / 2, 1.0f,

	-box_x / 2, 0, -box_z / 2, 1.0f,			//Prawo
	-box_x / 2, 0, box_z / 2, 1.0f,
	-box_x / 2, box_y, box_z / 2, 1.0f,
	-box_x / 2, 0, -box_z / 2, 1.0f,
	-box_x / 2, box_y, box_z / 2, 1.0f,
	-box_x / 2, box_y, -box_z / 2, 1.0f,
};

float room_vertices[] = {								//Pokój
	room_size / 2, 0, -room_size / 2, 1.0f,				//Tył
	-room_size / 2, room_size, -room_size / 2, 1.0f,
	-room_size / 2, 0, -room_size / 2, 1.0f,
	room_size / 2, 0, -room_size / 2, 1.0f,
	room_size / 2, room_size, -room_size / 2, 1.0f,
	-room_size / 2, room_size, -room_size / 2, 1.0f,

	-room_size / 2, 0, room_size / 2, 1.0f,				//Front
	room_size / 2, room_size, room_size / 2, 1.0f,
	room_size / 2, 0, room_size / 2, 1.0f,
	-room_size / 2, 0, room_size / 2, 1.0f,
	-room_size / 2, room_size, room_size / 2, 1.0f,
	room_size / 2, room_size, room_size / 2, 1.0f,

	room_size / 2, 0, room_size / 2, 1.0f,				//Lewo
	room_size / 2, room_size, -room_size / 2, 1.0f,
	room_size / 2, 0, -room_size / 2, 1.0f,
	room_size / 2, 0, room_size / 2, 1.0f,
	room_size / 2, room_size, room_size / 2, 1.0f,
	room_size / 2, room_size, -room_size / 2, 1.0f,

	-room_size / 2, 0, -room_size / 2, 1.0f,			//Prawo
	-room_size / 2, room_size, room_size / 2, 1.0f,
	-room_size / 2, 0, room_size / 2, 1.0f,
	-room_size / 2, 0, -room_size / 2, 1.0f,
	-room_size / 2, room_size, -room_size / 2, 1.0f,
	-room_size / 2, room_size, room_size / 2, 1.0f,
};

float roof_vertices[] = {								//Sufit
	-room_size / 2, room_size, -room_size / 2, 1.0f,
	room_size / 2, room_size, -room_size / 2, 1.0f,
	room_size / 2, room_size, room_size / 2, 1.0f,
	-room_size / 2, room_size, -room_size / 2, 1.0f,
	room_size / 2, room_size, room_size / 2, 1.0f,
	-room_size / 2, room_size, room_size / 2, 1.0f,
};

float floor_vertices[] = {						//Podłoga
	-room_size / 2, 0, -room_size / 2, 1.0f,
	room_size / 2, 0, -room_size / 2, 1.0f,
	room_size / 2, 0, room_size / 2, 1.0f,
	-room_size / 2, 0, -room_size / 2, 1.0f,
	room_size / 2, 0, room_size / 2, 1.0f,
	-room_size / 2, 0, room_size / 2, 1.0f,
};

float aqua_vertices[] = {							//Akwarium
	aqua_x / 2, box_y, -aqua_z / 2, 1.0f,				//Tył
	-aqua_x / 2, box_y + aqua_y, -aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y, -aqua_z / 2, 1.0f,
	aqua_x / 2, box_y, -aqua_z / 2, 1.0f,
	aqua_x / 2, box_y + aqua_y, -aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y + aqua_y, -aqua_z / 2, 1.0f,

	-aqua_x / 2, box_y, aqua_z / 2, 1.0f,				//Front
	aqua_x / 2, box_y + aqua_y, aqua_z / 2, 1.0f,
	aqua_x / 2, box_y, aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y, aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y + aqua_y, aqua_z / 2, 1.0f,
	aqua_x / 2, box_y + aqua_y, aqua_z / 2, 1.0f,

	aqua_x / 2, box_y, aqua_z / 2, 1.0f,				//Lewo
	aqua_x / 2, box_y + aqua_y, -aqua_z / 2, 1.0f,
	aqua_x / 2, box_y, -aqua_z / 2, 1.0f,
	aqua_x / 2, box_y, aqua_z / 2, 1.0f,
	aqua_x / 2, box_y + aqua_y, aqua_z / 2, 1.0f,
	aqua_x / 2, box_y + aqua_y, -aqua_z / 2, 1.0f,

	-aqua_x / 2, box_y, -aqua_z / 2, 1.0f,			//Prawo
	-aqua_x / 2, box_y + aqua_y, aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y, aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y, -aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y + aqua_y, -aqua_z / 2, 1.0f,
	-aqua_x / 2, box_y + aqua_y, aqua_z / 2, 1.0f,
};

float sand_vertices[] = {								//Piasek
	-box_x / 2, box_y, -box_z / 2, 1.0f,
	box_x / 2, box_y, -box_z / 2, 1.0f,
	box_x / 2, box_y, box_z / 2, 1.0f,
	-box_x / 2, box_y, -box_z / 2, 1.0f,
	box_x / 2, box_y, box_z / 2, 1.0f,
	-box_x / 2, box_y, box_z / 2, 1.0f,
};

//Tablice teksturowania
float one_tex_coords[] = {
	//Ściana 1
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
};

float four_tex_coords[] = {
	//Ściana 1
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

	//Ściana 2
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

	//Ściana 3
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,

	//Ściana 4
	1.0f,1.0f, 0.0f,0.0f, 0.0f,1.0f,
	1.0f,1.0f, 1.0f,0.0f, 0.0f,0.0f,
};

//Tablice wektorów normalnych
float room_normals[] = {
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,
	0.0f, 0.0f, 1.0f,0.0f,

	0.0f, 0.0f,-1.0f,0.0f,
	0.0f, 0.0f,-1.0f,0.0f,
	0.0f, 0.0f,-1.0f,0.0f,
	0.0f, 0.0f,-1.0f,0.0f,
	0.0f, 0.0f,-1.0f,0.0f,
	0.0f, 0.0f,-1.0f,0.0f,

	-1.0f, 0.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,0.0f,
	-1.0f, 0.0f, 0.0f,0.0f,

	1.0f, 0.0f, 0.0f,0.0f,
	1.0f, 0.0f, 0.0f,0.0f,
	1.0f, 0.0f, 0.0f,0.0f,
	1.0f, 0.0f, 0.0f,0.0f,
	1.0f, 0.0f, 0.0f,0.0f,
	1.0f, 0.0f, 0.0f,0.0f,
};

float sand_normals[] = {
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
	0.0f, 1.0f, 0.0f,0.0f,
};

float roof_normals[] = {
	0.0f, -1.0f, 0.0f,0.0f,
	0.0f, -1.0f, 0.0f,0.0f,
	0.0f, -1.0f, 0.0f,0.0f,
	0.0f, -1.0f, 0.0f,0.0f,
	0.0f, -1.0f, 0.0f,0.0f,
	0.0f, -1.0f, 0.0f,0.0f,
};


glm::vec3 pos = glm::vec3(-room_size, room_size , -room_size); //Ustalenie pozycji początkowej


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

//Funkcja obsługi kamery
glm::vec3 calcDir(float kat_x, float kat_y) {
	glm::vec4 dir = glm::vec4(0, 0, 1, 0);
	glm::mat4 M = glm::rotate(glm::mat4(1.0f), kat_y, glm::vec3(0, 1, 0));
	M = glm::rotate(M, kat_x, glm::vec3(1, 0, 0));
	dir = M * dir;
	return glm::vec3(dir);
}

//Procedura obsługi kamery
void key_callback(GLFWwindow* window, int key,int scancode,int action,int mod) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) speed_y = 2;
		if (key == GLFW_KEY_RIGHT) speed_y = -2;
		if (key == GLFW_KEY_DOWN) speed_x = 2;
		if (key == GLFW_KEY_UP) speed_x = -2;
		if (key == GLFW_KEY_W) walk_speed = 5;
		if (key == GLFW_KEY_S) walk_speed = -5;
		
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT) speed_y = 0;
		if (key == GLFW_KEY_RIGHT) speed_y = 0;		
		if (key == GLFW_KEY_DOWN) speed_x = 0;
		if (key == GLFW_KEY_UP) speed_x = 0;
		if (key == GLFW_KEY_W) walk_speed = 0;
		if (key == GLFW_KEY_S) walk_speed = 0;
	}
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	initShaders();
	glClearColor(0, 0, 0, 1); //Ustaw kolor czyszczenia bufora kolorów
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //Ustawienie funkcji blendującej
	glfwSetKeyCallback(window, key_callback);

	//Ustawianie tekstur
	tex_box = readTexture("stone.png");
	tex_room = readTexture("wall.png");
	tex_sand = readTexture("sand.png");
	tex_floor = readTexture("floor.png");
	tex_aqua = readTexture("window.png");
	tex_roof = readTexture("roof.png");
	tex_fish1 = readTexture("Fish1.png");
	tex_fish2 = readTexture("Fish2.png");
	tex_fish3 = readTexture("Fish3.png");
	tex_fish4 = readTexture("Fish4.png");
	tex_fish5 = readTexture("Fish5.png");
	tex_fish6 = readTexture("Fish6.png");
	tex_fish7 = readTexture("Fish7.png");
	tex_fish8 = readTexture("Fish8.png");
	tex_fish9 = readTexture("Fish9.png");
	tex_fish10 = readTexture("Fish10.png");
	tex_seashell = readTexture("SeaShell0.png");
	tex_seaweed = readTexture("SeaWeed1.png");

	rybki[0] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish1, fish1Vertices, fish1Normals, fish1TexCoords, fish1Colors, fish1VertexCount, 5, 2);
	rybki[1] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish2, fish2Vertices, fish2Normals, fish2TexCoords, fish2Colors, fish2VertexCount, 5, 2);
	rybki[2] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish3, fish3Vertices, fish3Normals, fish3TexCoords, fish3Colors, fish3VertexCount, 5, 2);
	rybki[3] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish5, fish5Vertices, fish5Normals, fish5TexCoords, fish5Colors, fish5VertexCount, 200, 100);
	rybki[4] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish6, fish6Vertices, fish6Normals, fish6TexCoords, fish6Colors, fish6VertexCount, 200, 100);
	rybki[5] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish7, fish7Vertices, fish7Normals, fish7TexCoords, fish7Colors, fish7VertexCount, 200, 100);
	rybki[6] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish8, fish8Vertices, fish8Normals, fish8TexCoords, fish8Colors, fish8VertexCount, 200, 100);
	rybki[7] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish9, fish9Vertices, fish9Normals, fish9TexCoords, fish9Colors, fish9VertexCount, 200, 100);
	rybki[8] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish10, fish10Vertices, fish10Normals, fish10TexCoords, fish10Colors, fish10VertexCount, 200, 100);
	rybki[9] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish7, fish7Vertices, fish7Normals, fish7TexCoords, fish7Colors, fish7VertexCount, 200, 100);
	rybki[10] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish8, fish8Vertices, fish8Normals, fish8TexCoords, fish8Colors, fish8VertexCount, 200, 100);
	rybki[11] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish9, fish9Vertices, fish9Normals, fish9TexCoords, fish9Colors, fish9VertexCount, 200, 100);
	rybki[12] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish10, fish10Vertices, fish10Normals, fish10TexCoords, fish10Colors, fish10VertexCount, 200, 100);
	rybki[13] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish5, fish5Vertices, fish5Normals, fish5TexCoords, fish5Colors, fish5VertexCount, 200, 100);
	rybki[14] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish6, fish6Vertices, fish6Normals, fish6TexCoords, fish6Colors, fish6VertexCount, 200, 100);
	rybki[15] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 180), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish7, fish7Vertices, fish7Normals, fish7TexCoords, fish7Colors, fish7VertexCount, 200, 100);
	rybki[16] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish2, fish2Vertices, fish2Normals, fish2TexCoords, fish2Colors, fish2VertexCount, 5, 2);
	rybki[17] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish9, fish9Vertices, fish9Normals, fish9TexCoords, fish9Colors, fish9VertexCount, 200, 100);
	rybki[18] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish10, fish10Vertices, fish10Normals, fish10TexCoords, fish10Colors, fish10VertexCount, 200, 100);
	rybki[19] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish1, fish1Vertices, fish1Normals, fish1TexCoords, fish1Colors, fish1VertexCount, 5, 2);
	rybki[20] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.20f, 0.20f, 0.20f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish1, fish1Vertices, fish1Normals, fish1TexCoords, fish1Colors, fish1VertexCount, 5, 2);
	rybki[21] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish9, fish9Vertices, fish9Normals, fish9TexCoords, fish9Colors, fish9VertexCount, 200, 100);
	rybki[22] = Fish(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.005f, 0.005f, 0.005f),
		glm::vec3(0.0f, 0.0f, 0.0f), tex_fish10, fish10Vertices, fish10Normals, fish10TexCoords, fish10Colors, fish10VertexCount, 200, 100);

}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window, float kat_x, float kat_y, float angle_x) {
	glClearColor(0, 0, 0, 0.5f); //Dodaj kolor czyszczenia
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Wyczyść bufor koloru i bufor głębokości
	glUniform3f(spTextured ->u("water_color"), water_r, water_g, water_b); //Załaduj do programu cieniującego kolor wody
	glUniform3f(spTextured->u("light_position1"), light1_x, light1_y, light1_z); //Załaduj do programu cieniującego pozycję światła 1
	glUniform3f(spTextured->u("light_color1"), light1_r, light1_g, light1_b); //Załaduj do programu cieniującego kolor światła 1
	glUniform3f(spTextured->u("light_position2"), light2_x, light2_y, light2_z); //Załaduj do programu cieniującego pozycję światła 2
	glUniform3f(spTextured->u("light_color2"), light2_r, light2_g, light2_b); //Załaduj do programu cieniującego kolor światła 2

	glm::mat4 M = glm::mat4(1.0f); //Zainicjuj macierz modelu macierzą jednostkową
	glm::mat4 V = glm::lookAt(pos, pos + calcDir(kat_x, kat_y), glm::vec3(0.0f, 1.0f, 0.0f)); //Wylicz macierz widoku
	glm::mat4 P = glm::perspective(glm::radians(50.0f), 1.0f, 0.1f, 50.0f); //Wylicz macierz rzutowania

	spTextured->use(); //Aktywuj program cieniujący

	glUniformMatrix4fv(spTextured->u("P"), 1, false, glm::value_ptr(P)); //Załaduj do programu cieniującego macierz rzutowania
	glUniformMatrix4fv(spTextured->u("V"), 1, false, glm::value_ptr(V)); //Załaduj do programu cieniującego macierz widoku
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M)); //Załaduj do programu cieniującego macierz modelu
	
	//Pokój
	glEnableVertexAttribArray(spTextured->a("vertex")); //Załaduj wierzchołki pokoju
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, room_vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));  //Załaduj współrzędne teksturowania pokoju
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, four_tex_coords);

	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne pokoju
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, room_normals);

	glUniform1i(spTextured->u("tex"), 0); //Załaduj teksturę pokoju
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_room);

	glDrawArrays(GL_TRIANGLES, 0, 24); //Narysuj pokój

	glDisableVertexAttribArray(spTextured->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(spTextured->a("texCoord"));  //Wyłącz przesyłanie danych do atrybutu texCoord
	glDisableVertexAttribArray(spTextured->a("anormal"));  //Wyłącz przesyłanie danych do atrybutu anormal

	//Podłoga
	glEnableVertexAttribArray(spTextured->a("vertex")); //Załaduj wierzchołki podłogi
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, floor_vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));  //Załaduj współrzędne teksturowania podłogi
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, one_tex_coords);

	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne pokoju
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, sand_normals);

	glUniform1i(spTextured->u("tex"), 0); //Załaduj teksturę podłogi
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_floor);

	glDrawArrays(GL_TRIANGLES, 0, 6); //Narysuj podłogę

	glDisableVertexAttribArray(spTextured->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(spTextured->a("texCoord"));  //Wyłącz przesyłanie danych do atrybutu texCoord
	glDisableVertexAttribArray(spTextured->a("anormal"));  //Wyłącz przesyłanie danych do atrybutu anormal


	//Sufit
	glEnableVertexAttribArray(spTextured->a("vertex")); //Załaduj wierzchołki sufitu
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, roof_vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));  //Załaduj współrzędne teksturowania sufitu
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, one_tex_coords);

	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne sufitu
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, roof_normals);

	glUniform1i(spTextured->u("tex"), 0); //Załaduj teksturę podłogi
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_roof);

	glDrawArrays(GL_TRIANGLES, 0, 6); //Narysuj sufit

	glDisableVertexAttribArray(spTextured->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(spTextured->a("texCoord"));  //Wyłącz przesyłanie danych do atrybutu texCoord
	glDisableVertexAttribArray(spTextured->a("anormal"));  //Wyłącz przesyłanie danych do atrybutu anormal

	//Akwarium
	glEnableVertexAttribArray(spTextured->a("vertex")); //Załaduj wierzchołki akwarium
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, aqua_vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));  //Załaduj współrzędne teksturowania akwarium
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, four_tex_coords);

	glUniform1i(spTextured->u("tex"), 0); //Załaduj teksturę akwarium
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_aqua);

	glEnable(GL_BLEND);
	glDrawArrays(GL_TRIANGLES, 0, 24); //Narysuj akwarium
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(spTextured->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(spTextured->a("texCoord"));  //Wyłącz przesyłanie danych do atrybutu texCoord

	//Piasek
	glEnableVertexAttribArray(spTextured->a("vertex")); //Załaduj wierzchołki piasku
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, sand_vertices);

	glEnableVertexAttribArray(spTextured->a("texCoord"));  //Załaduj współrzędne teksturowania piasku
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, one_tex_coords);

	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne piasku
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, sand_normals);

	glUniform1i(spTextured->u("tex"), 0); //Załaduj teksturę piasku
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_sand);

	glDrawArrays(GL_TRIANGLES, 0, 6); //Narysuj piasek

	glDisableVertexAttribArray(spTextured->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(spTextured->a("texCoord"));  //Wyłącz przesyłanie danych do atrybutu texCoord
	glDisableVertexAttribArray(spTextured->a("anormal"));  //Wyłącz przesyłanie danych do atrybutu anormal


	//Roslinka1
	glm::mat4 M1 = glm::mat4(1.0f);
	M1 = glm::translate(M1, glm::vec3(3.0f,2.5f,-5.0f));
	M1 = glm::scale(M1, glm::vec3(2.4f, 2.0f, 3.0f));
	M1 = glm::rotate(M1,2* angle_x, glm::vec3(0.0f, 0.0f, 1.0f));
	
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M1));
	glEnableVertexAttribArray(spTextured->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, seaWeed2Vertices); //Wskaż tablicę z danymi dla atrybutu vertex


	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne piasku
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, seaWeed2Normals);


	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, seaWeed2TexCoords);
	glUniform1i(spTextured->u("tex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_seaweed);

	glDrawArrays(GL_TRIANGLES, 0, seaWeed2VertexCount);
	
	//Roslinka2
	glm::mat4 M2 = glm::mat4(1.0f);
	M2 = glm::translate(M2, glm::vec3(4.0f, 2.5f, 0.0f));
	M2 = glm::scale(M2, glm::vec3(1.0f, 5.0f, 1.0f));
	M2 = glm::rotate(M2, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M2 = glm::rotate(M2, -9/4 * angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M2));

	glDrawArrays(GL_TRIANGLES, 0, seaWeed2VertexCount);

	//Roslinka3
	glm::mat4 M3 = glm::mat4(1.0f);
	M3 = glm::translate(M3, glm::vec3(4.0f, 2.4f, 0.0f));
	M3 = glm::scale(M3, glm::vec3(1.0f, 5.0f, 1.0f));
	M3 = glm::rotate(M3, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M3 = glm::rotate(M3, angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M3));

	glDrawArrays(GL_TRIANGLES, 0, seaWeed2VertexCount);

	//Roslinka 4

	glm::mat4 M4 = glm::mat4(1.0f);
	M4 = glm::translate(M4, glm::vec3(-2.0f, 2.5f, 4.4f));
	M4 = glm::scale(M4, glm::vec3(3.0f, 3.0f, 2.0f));
	M4 = glm::rotate(M4, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M4 = glm::rotate(M4, -2 * angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M4));

	glDrawArrays(GL_TRIANGLES, 0, seaWeed2VertexCount);



	//Roslinka 5

	glm::mat4 M5 = glm::mat4(1.0f);
	M5 = glm::translate(M5, glm::vec3(4.0f, 2.4f, 0.0f));
	M5 = glm::scale(M5, glm::vec3(1.0f, 4.4f, 1.0f));
	M5 = glm::rotate(M5, 90.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M5 = glm::rotate(M5, -10/6 * angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M5));

	glDrawArrays(GL_TRIANGLES, 0, seaWeed2VertexCount);



	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
	glDisableVertexAttribArray(spTextured->a("anormal"));
	//Roslinka 6

	glm::mat4 M6 = glm::mat4(1.0f);
	M6 = glm::translate(M6, glm::vec3(-4.0f, 2.6f, 6.5f));
	M6 = glm::scale(M6, glm::vec3(1.0f, 3.0f, 1.0f));
	M6 = glm::rotate(M6, 200.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M6 = glm::rotate(M6, -2 * angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M6));
	glEnableVertexAttribArray(spTextured->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, seaWeed1Vertices); //Wskaż tablicę z danymi dla atrybutu vertex


	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne piasku
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, seaWeed1Normals);


	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, seaWeed1TexCoords);

	glDrawArrays(GL_TRIANGLES, 0, seaWeed1VertexCount);


	//Roslinka 7
	glm::mat4 M7 = glm::mat4(1.0f);
	M7 = glm::translate(M7, glm::vec3(-6.0f, 2.6f, -4.0f));
	M7 = glm::scale(M7, glm::vec3(2.0f, 3.0f, 1.5f));
	M7 = glm::rotate(M7, 132.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M7 = glm::rotate(M7, 20/9 * angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M7));

	glDrawArrays(GL_TRIANGLES, 0, seaWeed1VertexCount);


	//Roslinka 8

	glm::mat4 M8 = glm::mat4(1.0f);
	M8 = glm::translate(M8, glm::vec3(-4.0f, 2.6f, 6.9f));
	M8 = glm::scale(M8, glm::vec3(3.0f, 2.0f, 1.0f));
	M8 = glm::rotate(M8, 343.0f * PI / 180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	M8 = glm::rotate(M8, -2 * angle_x, glm::vec3(0.0f, 0.0f, 1.0f));

	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M8));

	glDrawArrays(GL_TRIANGLES, 0, seaWeed1VertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
	glDisableVertexAttribArray(spTextured->a("anormal"));

	//Muszla1
	glm::mat4 M9 = glm::mat4(1.0f);
	M9 = glm::translate(M9, glm::vec3(5.7f, 3.0f, -3.5f));
	M9 = glm::scale(M9, glm::vec3(0.09f, 0.09f, 0.09f));
	M9 = glm::rotate(M9, 1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M9));
	glEnableVertexAttribArray(spTextured->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glVertexAttribPointer(spTextured->a("vertex"), 4, GL_FLOAT, false, 0, seaShell0Vertices); //Wskaż tablicę z danymi dla atrybutu vertex


	glEnableVertexAttribArray(spTextured->a("anormal"));  //Załaduj wektory normalne piasku
	glVertexAttribPointer(spTextured->a("anormal"), 4, GL_FLOAT, false, 0, seaShell0Normals);


	glEnableVertexAttribArray(spTextured->a("texCoord"));
	glVertexAttribPointer(spTextured->a("texCoord"), 2, GL_FLOAT, false, 0, seaShell0TexCoords);
	glUniform1i(spTextured->u("tex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_seashell);

	glDrawArrays(GL_TRIANGLES, 0, seaShell0VertexCount);

	//Muszelka 2
	glm::mat4 M10 = glm::mat4(1.0f);
	M10 = glm::translate(M10, glm::vec3(-2.7f, 3.0f, -5.0f));
	M10 = glm::scale(M10, glm::vec3(0.14f, 0.14f, 0.14f));
	M10 = glm::rotate(M10, -1.5708f, glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(spTextured->u("M"), 1, false, glm::value_ptr(M10));

	glDrawArrays(GL_TRIANGLES, 0, seaShell0VertexCount);

	glDisableVertexAttribArray(spTextured->a("vertex"));
	glDisableVertexAttribArray(spTextured->a("texCoord"));
	glDisableVertexAttribArray(spTextured->a("anormal"));

	for (int i = 0; i < 23; i++)
	{
		rybki[i].rysuj(spTextured);
		rybki[i].generuj_cel();
	}


	glfwSwapBuffers(window); //Skopiuj bufor tylny do bufora przedniego
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(900, 900, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla
	float angle_x = 0;// do roslinek
	int switching = -1; //zmiena pomocnicza do roslinek
	float angle = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float kat_x = 0;
	float kat_y = 0;
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		if (switching == -1)
		{
			angle_x -= PI / 16 * glfwGetTime();
			if (angle_x < -0.11f)
			{
				switching = 1;
			}
		}
		if (switching == 1)
		{
			angle_x += PI / 16 * glfwGetTime();
			if (angle_x > 0.15f)
			{
				switching = -1;
			}
		}
		kat_x += speed_x * glfwGetTime();
		kat_y += speed_y * glfwGetTime();
		pos += (float)(walk_speed * glfwGetTime()) * calcDir(kat_x, kat_y); //Policz pozycję kamery
		pos.x = glm::clamp(pos.x, -aqua_x / 2 + camera_tolerance, aqua_x / 2 - camera_tolerance); //Ograniczenia pozycji kamery
		pos.y = glm::clamp(pos.y, box_y + camera_tolerance, box_y + aqua_y - camera_tolerance);
		pos.z = glm::clamp(pos.z, -aqua_z / 2 + camera_tolerance, aqua_z / 2 - camera_tolerance);
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window, kat_x, kat_y, angle_x); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
