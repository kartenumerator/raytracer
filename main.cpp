#include <iostream>
#include "glad.h"
#include "shaders.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <bit>

#include "bvh.hpp"

#include <fstream>
#include <sstream>

#include <cstring>
#include <cstdint>


// using namespace std;

const int WINDOWHEIGHT = 1000;
const int WINDOWWIDTH = 1000;

const int scale = 2;

const int gridsize = 15;


int meshcount = 0;
double clickx = 0.0;
double clicky = 0.0;

//center, radius, color, reflectance

// float spheres[] = {
//     -4, -2, -3,   1, 1, 0, 0, 0.1,
//     0, -2, -5,   2, 0, 1, 1, 0.0,
//     0,  10, 0,  10, 0, 1, 1, 0.0,
//     -1, -2,-2.0, 1, 1, 1, 1, 0.1,
//      3, -2,-2.0, 1, 1, 0, 1, 0.0,
// };

// number of triangles, color, reflectance, triangles.
// float meshes[] = {
//     12, 1, 1, 1, 0.01,
//     // Front face (z = +1)
//     -1,-1, 1,   1,-1, 1,   1, -3, 1,
//     -1,-1, 1,   1, -3, 1,  -1, -3, 1,

//     // Back face (z = -1)
//      1,-1,-1,  -1,-1,-1,  -1, -3,-1,
//      1,-1,-1,  -1, -3,-1,   1, -3,-1,

//     // Left face (x = -1)
//     -1,-1,-1,  -1,-1, 1,  -1, -3, 1,
//     -1,-1,-1,  -1, -3, 1,  -1, -3,-1,

//     // Right face (x = +1)
//      1,-1, 1,   1,-1,-1,   1, -3,-1,
//      1,-1, 1,   1, -3,-1,   1, -3, 1,

//     // Top face (y = +1)
//     -1, -3, 1,   1, -3, 1,   1, -3,-1,
//     -1, -3, 1,   1, -3,-1,  -1, -3,-1,

//     // Bottom face (y = -1)
//     -1,-1,-1,   1,-1,-1,   1,-1, 1,
//     -1,-1,-1,   1,-1, 1,  -1,-1, 1
// };

std::vector<float> meshes;
std::vector<float> spheres;
std::vector<float> lights;

bool accumulate = false;

// pos, intensity, color, radius
// float lights[] = {
//     //  2, -10, 0, 5, 0, 1, 1,
//     // -5, -25, 10, 1, 1, 1, 0, 15,
//     -5, -25, -10, 40, 0.208, 0.94, 0.612, 10,
//     //  5, -15, 0, 30, 1, 0, 0, 5
// };

int fc = 0;

bool isMouseClicked = false;

glm::vec3 camera_pos = glm::vec3(8.09305, -6.7524, -5.23526);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec2 mouse_pos = glm::vec2(0.0f, 0.0f);
glm::vec3 camera_front = glm::vec3(-0.842474, 0.476902, 0.250721);

void rotateX(float angle, glm::vec3& vec) {
    float rad = glm::radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);
    float y = vec.y * cosA - vec.z * sinA;
    float z = vec.y * sinA + vec.z * cosA;
    vec.y = y;
    vec.z = z;
}

void rotateY(float angle, glm::vec3& vec) {
    float rad = glm::radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);
    float x = vec.x * cosA + vec.z * sinA;
    float z = -vec.x * sinA + vec.z * cosA;
    vec.x = x;
    vec.z = z;
}

void rotateZ(float angle, glm::vec3& vec) {
    float rad = glm::radians(angle);
    float cosA = cos(rad);
    float sinA = sin(rad);
    float x = vec.x * cosA - vec.y * sinA;
    float y = vec.x * sinA + vec.y * cosA;
    vec.x = x;
    vec.y = y;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true); // Close window on ESC
    }

    if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "W pressed" << std::endl;
        camera_pos += glm::normalize(camera_front)* 0.1f;
    }
    if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "S pressed" << std::endl;
        camera_pos -= glm::normalize(camera_front) * 0.1f;
    }
    if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "A pressed" << std::endl;
        camera_pos -= glm::normalize(glm::cross(camera_front, camera_up)) * 0.1f; // Move left
    }
    if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "D pressed" << std::endl;
        camera_pos += glm::normalize(glm::cross(camera_front, camera_up)) * 0.1f; // Move left
    }

    if (key == GLFW_KEY_E && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "D pressed" << std::endl;
        camera_pos += glm::normalize(camera_up) * 0.1f; // Move left
    }
    if (key == GLFW_KEY_Q && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "D pressed" << std::endl;
        camera_pos -= glm::normalize(camera_up) * 0.1f; // Move left
    }
    if (key == GLFW_KEY_P && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "D pressed" << std::endl;
        accumulate = !accumulate;
        fc = 0;
    }
    if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        // std::cout << "D pressed" << std::endl;
        std::ofstream outFile("store.txt");
        if (outFile.is_open()){
            outFile << camera_pos.x << " " << camera_pos.y << " " << camera_pos.z << std::endl;
            outFile << camera_front.x << " " << camera_front.y << " " << camera_front.z << std::endl;
            outFile.close();
        } else {
            std::cout << "Unable to open file\n";
        }
        // camera_pos -= glm::normalize(camera_up) * 0.1f; // Move left
    }
    if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        lights[3] += 1.0f;
        std::cout << "Light intensity increased to: " << lights[3] << std::endl;
        // camera_pos -= glm::normalize(camera_up) * 0.1f; // Move left
    }

    if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        lights[3] -= 1.0f;
        std::cout << "Light intensity decreased to: " << lights[3] << std::endl;
        // camera_pos -= glm::normalize(camera_up) * 0.1f; // Move left
    }

    std::cout << "Camera position updated to: (" << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << ")" << std::endl;
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &clickx, &clicky);
        std::cout << "Mouse clicked at: (" << clickx << ", " << clicky << ")" << std::endl;
        isMouseClicked = true;
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        std::cout << "Released" << std::endl;
        isMouseClicked = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    // std::cout << "Mouse moved to: " << xpos << ", " << ypos << std::endl;
    if(isMouseClicked) {
        // Update camera position based on mouse movement
        double deltaX = xpos - clickx;
        double deltaY = ypos - clicky;

        // Adjust camera position based on mouse movement
        double xangle = deltaX/800 * 90; // Adjust sensitivity as needed
        double yangle = deltaY/800 * 90; // Invert Y-axis for natural movement
        
        // camera_front.x = cos(glm::radians(mouse_pos.x));
        // camera_front.y = sin(glm::radians(mouse_pos.y));
        // camera_front.z = cos(glm::radians(mouse_pos.y)) + sin(glm::radians(mouse_pos.x)); // Keep the camera looking towards the negative
        // camera_front -= camera_pos; // Reset camera front to look at the camera position
        rotateX(yangle, camera_front);
        rotateY(-xangle, camera_front);

        // camera_front += camera_pos; // Update camera front to look at the new position
        // rotateX(-yangle, camera_pos);
        // rotateY(-xangle, camera_pos);


        clickx = xpos;
        clicky = ypos;

        std::cout << "Camera position updated to: (" << camera_front.x << ", " << camera_front.y << ", " << camera_front.z << ")" << std::endl;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    // Adjust camera position based on scroll input
    // camera_pos.y += yoffset * 0.1f; // Adjust sensitivity as needed
    std::cout << "Camera position after scroll: (" << camera_pos.x << ", " << camera_pos.y << ", " << camera_pos.z << ")" << std::endl;
}

uint hashh(uint x) {
    x += (x << 10u);
    x ^= (x >> 6u);
    x += (x << 3u);
    x ^= (x >> 11u);
    x += (x << 15u);
    return x;
}

int get_int_from_string(const std::string& str) {
    int ret = 0;
    for(char c : str) {
        if (isdigit(c)) {
            ret = ret * 10 + (c - '0');
        }
        if(c == '/')
            break;
    }
    return ret; // Return the converted integer or an error code if no digit is found
}

int get_back_int_from_string(const std::string& str) {
    int ret = 0;
    bool start = false;
    for(char c : str) {
        if(c == '/')
            start = true;
        if (start && isdigit(c)) {
            ret = ret * 10 + (c - '0');
        }
    }
    return ret; // Return the converted integer or an error code if no digit is found
}

// idx, x, y, z, length, width, height
std::vector<float> boundings;
std::vector<float> boundingcontent;



void load_obj(std::string filename, float cx, float cy, float cz, float rx, float ry, float rz, float scale, float r, float g, float b, float reflectance) {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> meshess;
    std::ifstream inFile(filename);

    if (inFile.is_open()) {
        std::string line;
        while(getline(inFile, line)) {
            // std::cout << line << std::endl;
            std::istringstream iss(line);
            std::string prefix;
            iss >> prefix;
            if (prefix == "v") {
                float x, y, z;
                iss >> x >> y >> z;
                glm::vec3 vert = glm::vec3(x + cx, -y + cy, z + cz)*scale;
                
                // glm::mat4 model = glm::mat4(1.0f); // Identity matrix
                // float angle = glm::radians(90.0f); // Convert degrees to radians
                // glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis
                
                // rotateY = glm::rotate(model, angle, axis);

                // vert = glm::vec3(model * glm::vec4(vert, 1.0f)); // Apply rotation to the vertex
                rotateX(rx, vert);
                rotateY(ry,vert);
                rotateZ(rz, vert);

                vertices.push_back(vert.x);
                vertices.push_back(vert.y);
                vertices.push_back(vert.z);
            }else if (prefix == "vn") {
                float x, y, z;
                iss >> x >> y >> z;
                glm::vec3 vert = glm::vec3(x, y, z);
                
                // glm::mat4 model = glm::mat4(1.0f); // Identity matrix
                // float angle = glm::radians(90.0f); // Convert degrees to radians
                // glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis
                
                // rotateY = glm::rotate(model, angle, axis);

                // vert = glm::vec3(model * glm::vec4(vert, 1.0f)); // Apply rotation to the vertex
                rotateX(rx, vert);
                rotateY(ry,vert);
                rotateZ(rz, vert);

                glm::normalize(vert);

                // normals.push_back(vert.x);
                // normals.push_back(vert.y);
                // normals.push_back(vert.z);
            } else if (prefix == "f") {
                std::string vs1, vs2, vs3;
                iss >> vs1 >> vs2 >> vs3;
                int v1 = get_int_from_string(vs1);
                int v2 = get_int_from_string(vs2);
                int v3 = get_int_from_string(vs3);
                // OBJ files are 1-indexed, so we need to subtract 1
                meshess.push_back(vertices[(v1 - 1) * 3 + 0]);
                meshess.push_back(vertices[(v1 - 1) * 3 + 1]);
                meshess.push_back(vertices[(v1 - 1) * 3 + 2]);
                
                
                meshess.push_back(vertices[(v2 - 1) * 3 + 0]);
                meshess.push_back(vertices[(v2 - 1) * 3 + 1]);
                meshess.push_back(vertices[(v2 - 1) * 3 + 2]);
                
                meshess.push_back(vertices[(v3 - 1) * 3 + 0]);
                meshess.push_back(vertices[(v3 - 1) * 3 + 1]);
                meshess.push_back(vertices[(v3 - 1) * 3 + 2]);
                
                //////////////////////////////////////////////
                
                if (normals.size() > 0){
                    int n1 = get_back_int_from_string(vs1);
                    int n2 = get_back_int_from_string(vs2);
                    int n3 = get_back_int_from_string(vs3);

                    meshess.push_back(normals[(n1 - 1) * 3 + 0]);
                    meshess.push_back(normals[(n1 - 1) * 3 + 1]);
                    meshess.push_back(normals[(n1 - 1) * 3 + 2]);

                    meshess.push_back(normals[(n2 - 1) * 3 + 0]);
                    meshess.push_back(normals[(n2 - 1) * 3 + 1]);
                    meshess.push_back(normals[(n2 - 1) * 3 + 2]);
                    
                    meshess.push_back(normals[(n3 - 1) * 3 + 0]);
                    meshess.push_back(normals[(n3 - 1) * 3 + 1]);
                    meshess.push_back(normals[(n3 - 1) * 3 + 2]);
                }
            }

            iss.clear();
        }
    }else {
        std::cout << "Unable to open file\n";
        return;
    }

    meshes.push_back(meshess.size() / 9);
    meshes.push_back(r);
    meshes.push_back(g);
    meshes.push_back(b);
    meshes.push_back(reflectance);
    if (normals.size() > 0){
        meshes.push_back(1.0f);
    }else {
        meshes.push_back(0.0f);
    }
    meshes.reserve(meshes.size() + meshess.size());
    meshes.insert(meshes.end(), meshess.begin(), meshess.end());

    meshcount++;
    std::cout << "Total faces: " << meshess.size()/9 << std::endl;
}

void add_sphere(float x, float y, float z, float radius, float r, float g, float b, float reflectance){
    spheres.push_back(x);
    spheres.push_back(y);
    spheres.push_back(z);
    spheres.push_back(radius);
    spheres.push_back(r);
    spheres.push_back(g);
    spheres.push_back(b);
    spheres.push_back(reflectance);
}

void add_cuboid(float x, float y, float z, float length, float width, float height, float r, float g, float b, float reflectiveness, int flag){
    float mesh[] = {
        12, r, g, b, reflectiveness, 0,
        // Front face (z = +1)
        -1,-1, 1,   1,-1, 1,   1, 1, 1,
        -1,-1, 1,   1, 1, 1,  -1, 1, 1,
        
        // Back face (z = -1)
        1,-1,-1,  -1,-1,-1,  -1, 1,-1,
        1,-1,-1,  -1, 1,-1,   1, 1,-1,
        
        // Left face (x = -1)
        -1,-1,-1,  -1,-1, 1,  -1, 1, 1,
        -1,-1,-1,  -1, 1, 1,  -1, 1,-1,
        
        // Right face (x = +1)
        1,-1, 1,   1,-1,-1,   1, 1,-1,
        1,-1, 1,   1, 1,-1,   1, 1, 1,
        
        // Top face (y = +1)
        -1, 1, 1,   1, 1, 1,   1, 1,-1,
        -1, 1, 1,   1, 1,-1,  -1, 1,-1,
        
        // Bottom face (y = -1)
        -1,-1,-1,   1,-1,-1,   1,-1, 1,
        -1,-1,-1,   1,-1, 1,  -1,-1, 1
    };
    int idxnumtriangles = meshes.size();
    for(int i = 0; i<6; i++){
        meshes.push_back(mesh[i]);
    }
    int numtriangles = 0;
    for(int i = 0; i<mesh[0]; i++){
        if ((flag & (1 << i)) == 0) {
            std::cout << "Skipping triangle " << i << " due to flag" << std::endl;
            continue; // Skip this triangle
        }
        for(int j = 0; j<3; j++){
            meshes.push_back(mesh[6 + i*9 + j*3 + 0] * length/2 + x);
            meshes.push_back(mesh[6 + i*9 + j*3 + 1] * height/2 + y);
            meshes.push_back(mesh[6 + i*9 + j*3 + 2] * width/2 + z);
        }
        numtriangles ++;
    }
    meshes[idxnumtriangles] = numtriangles;
    // std::cout << meshes[idxnumtriangles] << std::endl;
    meshcount ++;

}

void load_meshes() {
    std::ifstream inFile("localstore/mesh.txt");
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::cout<< "Processing line: " << line << std::endl;
            std::stringstream iss(line);
            std::string cmd = "";
            iss >> cmd;
            if (cmd == "cuboid") {
                float x, y, z, length, width, height, r, g, b, reflectiveness;
                
                iss >> x >> y >> z >> length >> width >> height >> r >> g >> b >> reflectiveness;
                std::string bin = "";
                iss >> bin;
                if (bin.empty()) {
                    std::cout << "Error: Missing binary flag for cuboid. Defaulting to 1." << std::endl;
                    bin = "111111111111";
                }
                int binval = std::stoi(bin, nullptr, 2);
                std::cout << "Loaded cuboid with center: (" << x << ", " << y << ", " << z << ") and dimensions: (" << length << ", " << width << ", " << height << ") " << binval << std::endl;
                add_cuboid(x, y, z, length, width, height, r, g, b, reflectiveness, binval);
            }else if(cmd == "sphere") {
                float x, y, z, radius, r, g, b, reflectiveness;
                iss >> x >> y >> z >> radius >> r >> g >> b >> reflectiveness;
                std::cout << "Loaded sphere with center: (" << x << ", " << y << ", " << z << "), radius: " << radius << ", color: (" << r << ", " << g << ", " << b << ") and reflectiveness: " << reflectiveness << std::endl;
                add_sphere(x, y, z, radius, r, g, b, reflectiveness);
            }else if(cmd == "light") {
                float x, y, z, intensity, r, g, b, radius;
                iss >> x >> y >> z >> intensity >> r >> g >> b >> radius;
                std::cout << "Loaded light with position: (" << x << ", " << y << ", " << z << "), intensity: " << intensity << ", color: (" << r << ", " << g << ", " << b << ") and radius: " << radius << std::endl;
                lights.push_back(x);
                lights.push_back(y);
                lights.push_back(z);
                lights.push_back(intensity);
                lights.push_back(r);
                lights.push_back(g);
                lights.push_back(b);
                lights.push_back(radius);
            }else if(cmd == "obj"){
                std::string filename;
                iss >> filename;
                std::cout << "Loading OBJ file: " << filename << std::endl;
                float x,y,z,rx,ry,rz,scale, r,g,b,reflectance;
                iss >> x >> y >> z >> rx >> ry >> rz >> scale >> r >> g >> b >> reflectance;
                
                load_obj(filename,x,y,z,rx,ry,rz,scale,r,g,b,reflectance);
            }
            iss.clear();
        }

    } else {
        std::cout << "Unable to open file\n";
    }
}

int main(int, char**){

    load_meshes();
    initiateBvh(meshes, meshcount, boundings, boundingcontent);
    std::cout << boundings.size()/10 << std::endl;
    for(int i = 0; i<boundings.size()/10; i++){
        int idx = boundings[i*10 + 0];
        int isleaf = boundings[i*10 + 1];
        
        float x = boundings[i*10 + 2];
        float y = boundings[i*10 + 3];
        float z = boundings[i*10 + 4];
        
        float length = boundings[i*10 + 5];
        float width = boundings[i*10 + 6];
        float height = boundings[i*10 + 7];
        
        int c1 = boundings[i*10 + 8];
        int c2 = boundings[i*10 + 9];

        int nummesh = boundingcontent[idx];
        int id = idx+1;
        std::cout<<"Bounding box, "<< isleaf <<" with "<<nummesh<<" meshes with x :"<< x << " y: " << y << " z: " << z << " length: " << length << " width: " << width << " height: " << height << " Children : "<<c1 << ", " << c2 << std::endl;

        // for(int j = 0; j<nummesh; j++){
        //     int numtriangles = boundingcontent[id + 0];
        //     std::cout << boundingcontent[id + 0] << " | " << boundingcontent[id + 1] << " " << boundingcontent[id + 2] << " " << boundingcontent[id + 3]<< " | " << boundingcontent[id + 4] << std::endl;
        //     for (int k = 0; k<numtriangles; k++){
        //         std::cout << boundingcontent[id + 5 + k*9 + 0] << " " << boundingcontent[id + 5 + k*9 + 1] << " " << boundingcontent[id + 5 + k*9 + 2] << std::endl;
        //         std::cout << boundingcontent[id + 5 + k*9 + 3] << " " << boundingcontent[id + 5 + k*9 + 4] << " " << boundingcontent[id + 5 + k*9 + 5] << std::endl;
        //         std::cout << boundingcontent[id + 5 + k*9 + 6] << " " << boundingcontent[id + 5 + k*9 + 7] << " " << boundingcontent[id + 5 + k*9 + 8] << std::endl;
        //     }
        //     id += numtriangles*9 + 5;
        // }
    }
    // add_sphere(-4, -2, -3, 1, 1, 0, 0, 0.1);
    // add_sphere(0, -2, -5, 2, 0, 1, 1, 0.0);
    // add_sphere(0, 10, 0, 10, 0, 1, 1, 0.0);
    // add_sphere(-1, -2, -2.0, 1, 1, 1, 1, 0.1);
    // add_sphere(3, -2, -2.0, 1, 1, 0, 1, 0.0);

    // add_cuboid(0,-2,0,5,5,2);
    std::ifstream inFile("localstore/store.txt");

    if (inFile.is_open()) {
        // Read file line by line
        inFile >> camera_pos.x >> camera_pos.y >> camera_pos.z;
        inFile >> camera_front.x >> camera_front.y >> camera_front.z;
        inFile.close();
    } else {
        std::cout << "Unable to open file\n";
    }

    

    glm::mat4 projection = glm::perspective(
        glm::radians(90.0f), // Field of view in radians
        800.0f / 800.0f,    // Aspect ratio
        0.1f,               // Near plane
        100.0f              // Far plane
    );


    int width, height, nrChannels;
    // Flip textures vertically because OpenGL expects 0,0 at bottom-left
    stbi_set_flip_vertically_on_load(true); 
    unsigned char *data = stbi_load("localstore/skybox.jpg", &width, &height, &nrChannels, 0);

    if (!data) {
        std::cout << "Failed to load image" << std::endl;
        return -1;
    }

    std::cout << "Image loaded with width: " << width << ", height: " << height << ", channels: " << nrChannels << std::endl;

    glm::mat4 viewMatrix = glm::lookAt(camera_pos,camera_pos + camera_front,camera_up);
    glm::mat4 invViewMatrix = glm::inverse(viewMatrix);

    std::cout << "Projection matrix: " << glm::to_string(projection) << std::endl;
    //Initialise glfw
    glfwInit();
    
    //Set properties
    // Set the opengl version being used.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    //Creating the window
    GLFWwindow* window = glfwCreateWindow(WINDOWWIDTH,WINDOWHEIGHT,"test window",NULL, NULL);

    //Error handling
    if (window == NULL){
        std::cout << "could not do that amigo." << std::endl;
        glfwTerminate();
        return -1;
    }

    // Making the window the current context in order to do all the processes on the window
    glfwMakeContextCurrent(window);

    // Load all the opengl processes on the gpu which right now is the window.
    gladLoadGL();
    
    glEnable(GL_CLIP_DISTANCE0);
    glEnable(GL_DEPTH_TEST);

    
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    //Define the viewport area which can be displayed upon.
    glViewport(0,0,WINDOWWIDTH,WINDOWHEIGHT);

    initout shaders = shadersInit();
    GLuint shaderprogram = shaders.shaderprogram;
    GLuint computeShaderProgram = shaders.computeShaderProgram;
    
    // GLuint ssbo;
    // glGenBuffers(1, &ssbo);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    // glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*boundingcontent.size(), boundingcontent.data(), GL_DYNAMIC_COPY);
    // glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    // glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    GLuint ssbo;
    glGenBuffers(1, &ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*meshes.size(), meshes.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    
    GLuint ssbolight;
    glGenBuffers(1, &ssbolight);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbolight);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * lights.size(), lights.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbolight);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 1);
    
    
    GLuint sphere;
    glGenBuffers(1, &sphere);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, sphere);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*spheres.size(), spheres.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, sphere);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 2);
    
    GLuint boundingsssbo;
    glGenBuffers(1, &boundingsssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, boundingsssbo);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float)*boundings.size(), boundings.data(), GL_DYNAMIC_COPY);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, boundingsssbo);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 3);
    
    
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,WINDOWWIDTH/scale,WINDOWHEIGHT/scale,0,GL_RGBA,GL_FLOAT,NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    GLuint skybox;
    glGenTextures(1, &skybox);
    glBindTexture(GL_TEXTURE_2D, skybox);

    // Allocate immutable storage:
    // Target: GL_TEXTURE_2D
    // Levels: 4 (1 base level + 3 mipmap levels)
    // Internal Format: GL_RGBA8 (8-bit RGBA)
    // Width: 512, Height: 512

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    std::cout << "Image loaded with " << nrChannels << " channels, using format: " << ((format == GL_RGBA) ? "GL_RGBA" : "GL_RGB") << std::endl;
    
    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
    //          GL_RGBA, GL_UNSIGNED_BYTE, nullptr); // allocate
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, data);

    // glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glBindImageTexture(1, skybox, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
    // glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);


    glUseProgram(shaderprogram);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLint uniformLocation = glGetUniformLocation(computeShaderProgram, "u_resolution");
    if (uniformLocation != -1) {
        std::cout << "Uniform location found: " << uniformLocation << std::endl;
        // Set the uniform value
        glUseProgram(computeShaderProgram);
        glUniform2f(uniformLocation, WINDOWWIDTH/scale, WINDOWHEIGHT/scale); // Assuming a resolution of 800x600
    }

    GLint skyres = glGetUniformLocation(computeShaderProgram, "sky_resolution");
    if (skyres != -1) {
        std::cout << "Uniform location found: " << skyres << std::endl;
        // Set the uniform value
        glUseProgram(computeShaderProgram);
        glUniform2f(skyres, width, height); // Assuming a resolution of 800x600
    }

    GLint framecount = glGetUniformLocation(computeShaderProgram, "frame");
    if (framecount != -1) {
        std::cout << "Uniform location found: " << framecount << std::endl;
        // Set the uniform value
        glUseProgram(computeShaderProgram);
        glUniform1i(framecount, 0); // Assuming a resolution of 800x600
    
    }
    GLint mc = glGetUniformLocation(computeShaderProgram, "meshcount");
    if (mc != -1) {
        std::cout << "Uniform location found: " << mc << std::endl;
        // Set the uniform value
        glUseProgram(computeShaderProgram);
        glUniform1i(mc, meshcount); // Assuming a resolution of 800x600
    
    }

    glUniform3f(glGetUniformLocation(computeShaderProgram, "cameraPos"), camera_pos.x, camera_pos.y, camera_pos.z);

    glUniformMatrix4fv(glGetUniformLocation(computeShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(computeShaderProgram, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(computeShaderProgram, "invView"), 1, GL_FALSE, &invViewMatrix[0][0]);

    vector<glm::vec3> vertices;

    for(float i = -gridsize/2; i<gridsize/2; i++) {
        for(float j = -gridsize/2; j<gridsize/2; j++) {
            glm::vec3 pt(i, -0.8f, j);
            vertices.push_back(pt);
            // std::cout << "Point: " << glm::to_string(pt) << std::endl;
        }
    }


    // GLfloat vertices[] = {
    //     // positions     
    //      0.5f, -0.8f,  -0.5f,  // top right
    //      0.5f, -0.8f, 0.5f,  // bottom right
    //     -0.5f, -0.8f, 0.5f,  // bottom left
    //     -0.5f, -0.8f,  -0.5f   // top left 
    // };


    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };


    GLuint VBO, VAO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),vertices.data(),GL_DYNAMIC_DRAW);   


    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, 3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_PROGRAM_POINT_SIZE);
    // glPointSize(10.0f);

    std::vector<float> pixels(WINDOWWIDTH/scale * WINDOWHEIGHT/scale * 4);

    std::vector<float> prevpixels(WINDOWWIDTH/scale * WINDOWHEIGHT/scale * 4);

    double lasttime = glfwGetTime();
    double fps = 0.0;
    // int fc = 0;
    glfwSwapInterval(0);
    while (!glfwWindowShouldClose(window))
	{
        glUseProgram(computeShaderProgram);
        
        viewMatrix = glm::lookAt(camera_pos,camera_pos + camera_front,camera_up);
        glUniformMatrix4fv(glGetUniformLocation(computeShaderProgram, "view"), 1, GL_FALSE, &viewMatrix[0][0]);
        invViewMatrix = glm::inverse(viewMatrix);
        glUniformMatrix4fv(glGetUniformLocation(computeShaderProgram, "invView"), 1, GL_FALSE, &invViewMatrix[0][0]);

        // glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbolight);
        // glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * lights.size(), lights.data(), GL_DYNAMIC_COPY);

        // glBindImageTexture(0, tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glUniform1i(framecount, fc); // Assuming a resolution of 800x600
    
        // glBindImageTexture(1, skybox, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA8);
        glDispatchCompute((WINDOWWIDTH/scale)/8, (WINDOWHEIGHT/scale)/8, 1);
        // glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        glBindTexture(GL_TEXTURE_2D, tex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels.data());

        if (fc > 1 && accumulate) {
            for (int i = 0; i < pixels.size(); i++) {
                pixels[i] = (pixels[i]/fc + prevpixels[i]*(fc-1)/fc);
                // if (pixels[i] != prevpixels[i]) {
                //     pixels[i] = (pixels[i]/fc + prevpixels[i]*(fc-1)/fc);
                // }else{
                //     pixels[i] = prevpixels[i];
                // }
            
            }

        }
        // std::cout<<pixels[WINDOWHEIGHT*WINDOWWIDTH*4/2 + WINDOWWIDTH/2] << " " << pixels[1] << " " << pixels[2] << " " << pixels[3] << std::endl;
        
		glClearColor(0.0f,0.0f,0.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glUseProgram(shaderprogram);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER,VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),vertices.data(),GL_DYNAMIC_DRAW);   
        
        
        
        glBindVertexArray(VAO);
        
        glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA32F,          // internal format (GPU)
            WINDOWWIDTH/scale,
            WINDOWHEIGHT/scale,
            0,
            GL_RGBA,             // format (CPU data)
            GL_FLOAT,            // type
            pixels.data()          // pointer to vector<float>
        );
        glUniform1i(glGetUniformLocation(shaderprogram, "computeTex"), 0);
        
        
        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
        // glDrawElements(GL_LINE_LOOP, (sizeof(indices)/sizeof(uint)), GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
        
        fc++;
        double currentTime = glfwGetTime();
        fps++;
        if(currentTime - lasttime >= 1.0) {
            glfwSetWindowTitle(window,("FPS: "+std::to_string(fps)+" | Frame: " + std::to_string(fc)).c_str());
            fps = 0.0;
            lasttime = currentTime;
        }
        prevpixels = pixels;
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // Destroy the window.
    glfwDestroyWindow(window);

    //Terminate glfw.
    glfwTerminate();
    return 0;
}
