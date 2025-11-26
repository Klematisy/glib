#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Graphics/RendererCore/renderer.h"
#include "Graphics/RendererCore/window.h"
#include "FontGenerator/font_generator.h"

#include "texture_manager.h"
#include "Geometry/mesh.h"
#include "Geometry/text.h"
#include "frame_buffer.h"
#include "Utils/camera.h"
#include "environment.h"
#include "structs.h"
#include "batch.h"
#include "shader.h"

GLIB_NAMESPACE_OPEN

class BufferDrawer {
public:
    BufferDrawer() = default;

    void Start();
    void End();

    void FlushBuffer();

    void BatchText(const Geom::Text2D& text2D);
    void BatchMesh(const Geom::Mesh& mesh, const Texture* texture);

    //--------------------------------WARNING---------------------------------//
    /* Be careful with the Use methods because they use raw pointers that's   */
    /* why your provided objects need to be exists at all time of use         */
    //------------------------------------------------------------------------//

    void UseShader(Shader* shader);
    void UseBuffer(DrawBuffer* drawResources);
    void UseTextureManager(TextureManager* textureManager);
    void SetProjMatrix(const glm::mat4& proj);
    void SetViewMatrix(const glm::mat4& view);

    const TextureManager* GetBoundTexManager() const;
    const Shader* GetBoundShader() const;
    const DrawBuffer* GetDrawBuffer() const;
private:
    Batch m_Batch = Batch(GLIB_MAX_BATCH_COUNT);

    TextureManager* m_BoundTexManager = nullptr;
    Shader* m_BoundShader = nullptr;
    DrawBuffer* m_BoundDrawBuffer = nullptr;

    glm::mat4 m_Proj {1.0f};
    const glm::mat4* m_View = nullptr;
};

GLIB_NAMESPACE_CLOSE