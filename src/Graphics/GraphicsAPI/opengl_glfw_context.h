#include "graphics_api_context.h"

GAPI_NAMESPACE_OPEN

class OpenGLGLFWContext : public IGraphicsAPIContext {
public:
    static OpenGLGLFWContext& Get();
    void Init(uint32_t major, uint32_t minor) override;

    uint32_t GetMajorV() override;
    uint32_t GetMinorV() override;
};

GAPI_NAMESPACE_CLOSE
