#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;



//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=2) in vec2 texCoord; //wspó³rzêdne teksturowania
layout (location=3) in vec3 anormal; //wspó³rzêdne wektorów normalnych

//Zmienne interpolowane
out vec2 i_tc;
out float visibility;
out vec3 frag_position;
out vec3 normal;


const float density = 0.08; //gêstoœæ wody
const float gradient = 0.3; //szybkoœæ zmniejszania widzenia

void main(void) {
    gl_Position=P*V*M*vertex;
    i_tc=texCoord;

    //mg³a
    vec4 position_relative_to_camera = P*M*vertex;  //odleg³oœci piksela od kamery
    float distance = length(position_relative_to_camera.xyz);   //liczenie odleg³oœci piksela od kamery
    visibility = exp(-pow((distance*density), gradient));   //liczenie widocznoœci
    visibility = clamp (visibility, 0.0, 1.0);  //ustawienie limitu widocznoœci od 0 do 1

    //oœwietlenie
    frag_position = vec3(M * vertex);   //wierzcho³ek w przestrzeni œwiata
    normal = inverse(transpose(mat3(M))) * anormal; //wektor normalny w przestrzeni œwiata
}
