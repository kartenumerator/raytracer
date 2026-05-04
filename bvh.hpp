#include <iostream>
#include <vector>
#include "glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <climits>

bool inbounds(glm::vec3 v, float x, float y, float z, float len, float wid, float height){
    return (v.x >= x && v.y >= y && v.z >= z && v.x <= x+len  && v.y <= y+height && v.z <= z+wid);
}

void bvh(int id, float x, float y, float z, float length, float width, float height, int depth,
     int meshcount, std::vector<float> meshes, std::vector<float>& boundings, std::vector<float>& boundingcontent, int parent) {

    int bvhidx = boundingcontent.size();
    boundings[id] = bvhidx;

    boundingcontent.push_back(0); // Placeholder for number of primitives in this bounding box, will be updated later
    
    int primitivecount = 0;
    
    int idx = parent+1;

    if(parent == 0){
        idx = 0;
        for (int i = 0; i<meshcount; i++){
            int numtriangles = meshes[idx];
            float r = meshes[idx + 1];
            float g = meshes[idx + 2];
            float b = meshes[idx + 3];
            float reflectiveness = meshes[idx + 4];
            idx += 5;
            int meshincluded = 0;
            int numtrianglesincludedidx = boundingcontent.size();
            for (int j = 0; j<numtriangles; j++){
                glm::vec3 v1 = glm::vec3(meshes[idx + j*9 + 0], meshes[idx + j*9 + 1], meshes[idx + j*9 + 2]);
                glm::vec3 v2 = glm::vec3(meshes[idx + j*9 + 3], meshes[idx + j*9 + 4], meshes[idx + j*9 + 5]);
                glm::vec3 v3 = glm::vec3(meshes[idx + j*9 + 6], meshes[idx + j*9 + 7], meshes[idx + j*9 + 8]);

                glm::vec3 centroid = (v1 + v2 + v3) / 3.0f;

                if(!inbounds(centroid,x,y,z,length,width,height)){
                    continue;
                }
                primitivecount++;
                meshincluded++;
                if (meshincluded == 1) {
                    boundingcontent[bvhidx]++;
                    boundingcontent.push_back(numtriangles);
                    boundingcontent.push_back(r);
                    boundingcontent.push_back(g);
                    boundingcontent.push_back(b);
                    boundingcontent.push_back(reflectiveness);
                }
                boundingcontent.push_back(v1.x);
                boundingcontent.push_back(v1.y);
                boundingcontent.push_back(v1.z);
                boundingcontent.push_back(v2.x);
                boundingcontent.push_back(v2.y);
                boundingcontent.push_back(v2.z);
                boundingcontent.push_back(v3.x);
                boundingcontent.push_back(v3.y);
                boundingcontent.push_back(v3.z);
            }
            boundingcontent[numtrianglesincludedidx] = meshincluded;
            idx += numtriangles*9;
        }
    }else {

        for (int i = 0; i<meshcount; i++){
            int numtriangles = boundingcontent[idx];
            float r = boundingcontent[idx + 1];
            float g = boundingcontent[idx + 2];
            float b = boundingcontent[idx + 3];
            float reflectiveness = boundingcontent[idx + 4];
            idx += 5;
            int meshincluded = 0;
            int numtrianglesincludedidx = boundingcontent.size();
            for (int j = 0; j<numtriangles; j++){
                glm::vec3 v1 = glm::vec3(boundingcontent[idx + j*9 + 0], boundingcontent[idx + j*9 + 1], boundingcontent[idx + j*9 + 2]);
                glm::vec3 v2 = glm::vec3(boundingcontent[idx + j*9 + 3], boundingcontent[idx + j*9 + 4], boundingcontent[idx + j*9 + 5]);
                glm::vec3 v3 = glm::vec3(boundingcontent[idx + j*9 + 6], boundingcontent[idx + j*9 + 7], boundingcontent[idx + j*9 + 8]);

                glm::vec3 centroid = (v1 + v2 + v3) / 3.0f;

                if(!inbounds(centroid,x,y,z,length,width,height)){
                    continue;
                }
                primitivecount++;
                meshincluded++;
                if (meshincluded == 1) {
                    boundingcontent[bvhidx]++;
                    boundingcontent.push_back(numtriangles);
                    boundingcontent.push_back(r);
                    boundingcontent.push_back(g);
                    boundingcontent.push_back(b);
                    boundingcontent.push_back(reflectiveness);
                }
                boundingcontent.push_back(v1.x);
                boundingcontent.push_back(v1.y);
                boundingcontent.push_back(v1.z);
                boundingcontent.push_back(v2.x);
                boundingcontent.push_back(v2.y);
                boundingcontent.push_back(v2.z);
                boundingcontent.push_back(v3.x);
                boundingcontent.push_back(v3.y);
                boundingcontent.push_back(v3.z);
            }
            boundingcontent[numtrianglesincludedidx] = meshincluded;
            idx += numtriangles*9;
        }
    }

    float maxaxis = std::max(length, std::max(width, height));
    if (primitivecount < 4 || depth == 0 || maxaxis < 1.0f) {
        boundings[id+1] = 1;
        return;
    }

    if (maxaxis == length){

        int id1 = boundings.size();
        boundings[id+8] = float(id1/10);
        boundings[id+9] = float(id1/10 + 1);

        boundings.insert(boundings.end(), {0,0, x,y,z,length/2.0f,width,height, -1,-1});
        int id2 = boundings.size();
        boundings.insert(boundings.end(), {0,0, x+length/2.0f,y,z,length/2.0f,width,height,-1,-1});

        bvh(id1, x, y, z, length/2.0f, width, height, depth-1,boundingcontent[bvhidx], meshes, boundings, boundingcontent, bvhidx);
        bvh(id2, x+length/2, y, z, length/2.0f, width, height, depth-1,boundingcontent[bvhidx], meshes, boundings, boundingcontent, bvhidx);
    }else if (maxaxis == width){

        int id1 = boundings.size();
        boundings[id+8] = float(id1/10);
        boundings[id+9] = float(id1/10 +1);

        boundings.insert(boundings.end(), {0,0, x,y,z,length,width/2,height,-1,-1});
        int id2 = boundings.size();
        boundings.insert(boundings.end(), {0,0, x,y,z+width/2,length,width/2,height,-1,-1});

        bvh(id1, x, y, z, length, width/2, height, depth-1,boundingcontent[bvhidx], meshes, boundings, boundingcontent, bvhidx);
        bvh(id2, x, y, z+width/2, length, width/2, height, depth-1,boundingcontent[bvhidx], meshes, boundings, boundingcontent, bvhidx);
    } else {

        int id1 = boundings.size();
        boundings[id+8] = float(id1/10);
        boundings[id+9] = float(id1/10 + 1);

        boundings.insert(boundings.end(), {0,0, x,y,z,length,width,height/2,-1,-1});
        int id2 = boundings.size();
        boundings.insert(boundings.end(), {0,0, x,y+height/2,z,length,width,height/2,-1,-1});

        bvh(id1, x, y, z, length, width, height/2, depth-1,boundingcontent[bvhidx], meshes, boundings, boundingcontent, bvhidx);
        bvh(id2, x, y+height/2, z, length, width, height/2, depth-1,boundingcontent[bvhidx], meshes, boundings, boundingcontent, bvhidx);
    }
}

void initiateBvh(std::vector<float> meshes, int meshcount, std::vector<float>& boundings, std::vector<float>& boundingcontent) {
    float mx = -1000000.0f;
    float my = -1000000.0f;
    float mz = -1000000.0f;
    float nx = 1000000.0f;
    float ny = 1000000.0f;
    float nz = 1000000.0f;
    int idx = 0;
    for (int i = 0; i<meshcount; i++){
        int numtriangles = meshes[idx];
        idx += 5;
        for (int j = 0; j<numtriangles; j++){
            glm::vec3 v1 = glm::vec3(meshes[idx + j*9 + 0], meshes[idx + j*9 + 1], meshes[idx + j*9 + 2]);
            glm::vec3 v2 = glm::vec3(meshes[idx + j*9 + 3], meshes[idx + j*9 + 4], meshes[idx + j*9 + 5]);
            glm::vec3 v3 = glm::vec3(meshes[idx + j*9 + 6], meshes[idx + j*9 + 7], meshes[idx + j*9 + 8]);

            mx = std::max(mx, std::max(v1.x, std::max(v2.x, v3.x)));
            my = std::max(my, std::max(v1.y, std::max(v2.y, v3.y)));
            mz = std::max(mz, std::max(v1.z, std::max(v2.z, v3.z)));
            nx = std::min(nx, std::min(v1.x, std::min(v2.x, v3.x)));
            ny = std::min(ny, std::min(v1.y, std::min(v2.y, v3.y)));
            nz = std::min(nz, std::min(v1.z, std::min(v2.z, v3.z)));
        }
    }

    std::cout << mx << ", " << my << ", " << mz << " | " << nx << ", " << ny << ", " << nz << std::endl;
    boundings.insert(boundings.end(), {0,0, nx,ny,nz,mx-nx,my-ny,mz-nz,-1,-1});
    bvh(0, nx, ny, nz, mx - nx, my - ny, mz - nz, 4, meshcount, meshes, boundings, boundingcontent,0);
}