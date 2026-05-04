#include <iostream>
#include "glad.h"
#include <cmath>
#include <fstream>
#include <GLFW/glfw3.h>

using namespace std;

class Shader {
    private:
        string src;
    public :

        GLuint shaderID;
        Shader(string filepath){
            string myText;

            // Read from the text file
            ifstream MyReadFile(filepath);

            // Use a while loop together with the getline() function to read the file line by line
            while (getline (MyReadFile, myText)) {
              // Output the text from the file
              src += myText +"\n";
            }

            // cout << src;

            // Close the file
            MyReadFile.close();

        }

        void init(GLenum shaderType) {

            shaderID = glCreateShader(shaderType);

            const char* shaderSource = src.c_str();
            glShaderSource(shaderID, 1, &shaderSource, NULL);
            glCompileShader(shaderID);
        }
};


struct initout{
    GLuint shaderprogram;
    GLuint computeShaderProgram;
};

initout shadersInit(){

    
    Shader fragmentShader("shaders/fragment.frag");
    fragmentShader.init(GL_FRAGMENT_SHADER);

    Shader vertexShader("shaders/default.vert");
    vertexShader.init(GL_VERTEX_SHADER);

    Shader compshader("shaders/raytracer.comp");
    compshader.init(GL_COMPUTE_SHADER);

    GLuint computeShaderProgram = glCreateProgram();
    glAttachShader(computeShaderProgram, compshader.shaderID);
    glLinkProgram(computeShaderProgram);
    glDeleteShader(compshader.shaderID);


    GLuint shaderprogram = glCreateProgram();
    glAttachShader(shaderprogram, vertexShader.shaderID);
    glAttachShader(shaderprogram, fragmentShader.shaderID);
    glLinkProgram(shaderprogram);

    glDeleteShader(vertexShader.shaderID);
    glDeleteShader(fragmentShader.shaderID);
    return {shaderprogram,computeShaderProgram};
}
