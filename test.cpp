#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>

using namespace std;

int main(){
    vector<vector<glm::vec3>> list;
    vector<glm::vec3> t1;
    t1.push_back(glm::vec3(0,0,0));
    t1.push_back(glm::vec3(0,0,1));
    t1.push_back(glm::vec3(0,1,0));
    list.push_back(t1);

    vector<glm::vec3> t2;
    t2.push_back(glm::vec3(0,0,1));
    t2.push_back(glm::vec3(0,1,1));
    t2.push_back(glm::vec3(1,1,0));
    list.push_back(t2);

    
    return 0;
}