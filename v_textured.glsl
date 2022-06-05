#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=2) in vec2 texCoord; //wsp�rz�dne teksturowania
layout (location=3) in vec3 anormal; //wsp�rz�dne wektor�w normalnych

//Zmienne interpolowane
out vec2 i_tc;
out float visibility;
out vec3 frag_position;
out vec3 normal;


const float density = 0.08; //g�sto�� wody
const float gradient = 0.3; //szybko�� zmniejszania widzenia

void main(void) {
    gl_Position=P*V*M*vertex;
    i_tc=texCoord;

    //mg�a
    vec4 position_relative_to_camera = P*M*vertex;  //odleg�o�ci piksela od kamery
    float distance = length(position_relative_to_camera.xyz);   //liczenie odleg�o�ci piksela od kamery
    visibility = exp(-pow((distance*density), gradient));   //liczenie widoczno�ci
    visibility = clamp (visibility, 0.0, 1.0);  //ustawienie limitu widoczno�ci od 0 do 1

    //o�wietlenie
    frag_position = vec3(M * vertex);   //wierzcho�ek w przestrzeni �wiata
    normal = inverse(transpose(mat3(M))) * anormal; //wektor normalny w przestrzeni �wiata
}
