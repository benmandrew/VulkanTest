#ifndef __MODEL_H_INCLUDED__
#define __MODEL_H_INCLUDED__

#include "texture.h"


struct Vertex;

struct Model {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    Texture* texture;

    Model();
    void create(Instance* instance, std::string modelPath, std::string texPath);

private:
    void load(std::string modelPath);
};

#endif