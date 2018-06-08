#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "stb_image.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "model.h"
#include "mesh.h"

namespace PBR {

    Model::Model()
    {

    }


    Model::~Model()
    {

    }


    void Model::loadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
            return;
        }

        this->directory = path.substr(0, path.find_last_of('/'));
        this->processNode(scene->mRootNode, scene);
    }

    void Model::Draw()
    {
        for (GLuint i = 0; i < this->meshes.size(); i++)
            this->meshes[i].Draw();
    }


    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        for (GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            this->meshes.push_back(this->processMesh(mesh, scene));
        }

        for (GLuint i = 0; i < node->mNumChildren; i++)
        {
            this->processNode(node->mChildren[i], scene);
        }
    }


    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        for (GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector;

            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if (mesh->mTextureCoords[0])
            {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }

        for (GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];

            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }

        return Mesh(vertices, indices);
    }

//    void Model::mergeModel(const Model &other, const glm::mat4x4 transform) {
//        aiMesh ** new_meshes = new aiMesh*[scene->mNumMeshes + other.scene->mNumMeshes];
//        memcpy(new_meshes, scene->mMeshes, scene->mNumMeshes * sizeof(aiMesh*));
//        delete[] scene->mMeshes;
//        scene->mMeshes = new_meshes;
//        for (int i = 0, offset = scene->mNumMeshes; i < other.scene->mNumMeshes; i++, offset++) {
//            aiMesh *mesh = other.scene->mMeshes[i];
//            aiMesh *copy_mesh = scene->mMeshes[offset] = new aiMesh;

//            copy_mesh->mNumVertices = mesh->mNumVertices;
//            copy_mesh->mVertices = new aiVector3D[mesh->mNumVertices];
//            copy_mesh->mPrimitiveTypes = mesh->mPrimitiveTypes;
//            for (int j = 0; j < mesh->mNumVertices; j++) {
//                glm::vec4 v(mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z, 1);
//                v = transform * v;
//                copy_mesh->mVertices[j] = aiVector3D(v.x, v.y, v.z);
//            }
//            // copy_mesh->mMaterialIndex = mesh->mMaterialIndex; // FIXME
//            copy_mesh->mName = mesh->mName;
//            copy_mesh->mNumFaces = mesh->mNumFaces;
//            if (mesh->mNormals) {
//                copy_mesh->mNormals = new aiVector3D[mesh->mNumVertices];
//                for (int j = 0; j < mesh->mNumVertices; j++) {
//                    glm::vec4 v(mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z, 0);
//                    v = transform * v;
//                    copy_mesh->mNormals[j] = aiVector3D(v.x, v.y, v.z);
//                }
//            }
//            copy_mesh->mFaces = new aiFace[mesh->mNumFaces];
//            for (int j = 0; j < mesh->mNumFaces; j++) {
//                copy_mesh->mFaces[j].mNumIndices = mesh->mFaces[j].mNumIndices;
//                copy_mesh->mFaces[j].mIndices = new unsigned[mesh->mFaces[j].mNumIndices];
//                memcpy(copy_mesh->mFaces[j].mIndices, mesh->mFaces[j].mIndices,
//                       mesh->mFaces[j].mNumIndices * sizeof(unsigned));
//            }
//        }
//        scene->mNumMeshes += other.scene->mNumMeshes;
//    }
}
