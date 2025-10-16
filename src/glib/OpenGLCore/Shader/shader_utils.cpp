#include <string>
#include "shader_utils.h"

int GlCore::ShaderCreator::GetMaxSlotsCount() {
    int maxTextureUnits = 0;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxTextureUnits);
    return maxTextureUnits;
}
