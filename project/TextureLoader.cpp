#include "TextureLoader.h"

auto texture = TextureLoader::LoadTextureFromFile(
    device,
    commandList,
    "path/to/texture.png");
