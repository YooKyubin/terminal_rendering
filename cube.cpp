#include <vector>
#include <string>
#include <cmath>
#include <iostream>
#include <unistd.h>

using std::vector, std::pair;

const int width = 160;
const int height = 44;
float zbuffer[width * height];
char buffer[width * height];
char background = ' ';
float speed = 0.5;
vector<float> theta {0,0,0};
float increment = 0.3;
float size = 15;
int cameraDist = size * 2;

struct Vertex{
    float x;
    float y;
    float z;
};

void scale(float size, vector<Vertex>& plane){
    for (auto& i: plane){
        i.x *= size;
        i.y *= size;
        i.z *= size;
    }
}

vector<float> rotate(vector<float> theta, vector<float> pos){
    float i = pos[0];
    float j = pos[1];
    float k = pos[2];

    float A = theta[0];
    float B = theta[1];
    float C = theta[2];

    float sinA = sin(A);
    float cosA = cos(A);
    float sinB = sin(B);
    float cosB = cos(B);
    float sinC = sin(C);
    float cosC = cos(C);

    // float calX = j * sinA * sinB * cosC - k * cosA * sinB * cosC +
    //                 j * cosA * sinC + k * sinA * sinC + i * cosB * cosC;
    // float calY = j * cosA * cosC + k * sinA * cosC -
    //                 j * sinA * sinB * sinC + k * cosA * sinB * sinC -
    //                 i * cosB * sinC;
    // float calZ = k * cosA * cosB - j * sinA * cosB + i * sinB;
    float calX = cosC * cosB * i + (cosC * sinB * sinA - sinC * cosA) * j + (sinC * sinA + cosC * sinB * cosA) * k;
    float calY = sinC * cosB * i + (cosC * cosA + sinC * sinB * sinA) * j + (sinC * sinB * cosA - cosC * sinA) * k;
    float calZ = -i * sinB + j * cosB * sinA + k * cosB * cosA;
    

    return {calX, calY, calZ};
}

void drawPlane(const vector<Vertex>& plane, char color){
    Vertex min = plane[0];
    Vertex max = plane[1];

    for (float x= min.x; x <= max.x; x += increment){
        for (float y = min.y; y <= max.y; y += increment){
            for (float z = min.z; z <= max.z; z += increment){
                vector<float> pos = rotate(theta, {x, y, z});
                int xp = (int)(width/2 + pos[0]);
                int yp = (int)(height / 2 + pos[1]);
                float ooz = 1 / (pos[2] + cameraDist);

                if (height > yp && yp > 0 && xp > 0 && width > xp){
                    int idx = xp + yp * width;
                    if (zbuffer[idx] < ooz){
                        zbuffer[idx] = ooz;
                        buffer[idx] = color;
                    }
                }
            }
        }
    }
}

int main(){
    printf("\x1b[2J");
    vector<Vertex> plane1 {
        {-size, -size, size},
        {size,size, size}
    };
    vector<Vertex> plane2 {
        {-size, -size, -size},
        {-size, size, size}
    };
    vector<Vertex> plane3 {
        {-size, -size, -size},
        {size,size, -size}
    };
    vector<Vertex> plane4 {
        {size, -size, -size},
        {size,size, size}
    };
    vector<Vertex> plane5 { //top
        {-size, size, -size},
        {size,size, size}
    };
    vector<Vertex> plane6 { //bottom
        {-size, -size, -size},
        {size, -size, size}
    };
    
    
    while(1){
        memset(zbuffer, 0, sizeof(float) * width * height);
        memset(buffer,background, sizeof(char) * width * height);
    
        drawPlane(plane1, '.');
        drawPlane(plane2, ';');
        drawPlane(plane3, ',');
        drawPlane(plane4, '*');
        drawPlane(plane5, '@');
        drawPlane(plane6, '#');
        
        for (int k = 0; k < width * height; k++) {
            putchar(k % width ? buffer[k] : '\n');
        }
    printf("\x1b[H");
    usleep(20000);
    theta[0] += 0.2;
    theta[1] += 0.13;
    theta[2] += 0.05;
    }
    return 0;
}