#pragma once

#define GAPI_NAMESPACE_OPEN  namespace GAPI {
#define GAPI_NAMESPACE_CLOSE }
#define GAPI_NAMESPACE_USING using namespace GAPI;

GAPI_NAMESPACE_OPEN

class IGraphicsAPIContext {
public:
    virtual void Init(uint32_t major, uint32_t minor) = 0;

    virtual uint32_t GetMajorV() = 0;
    virtual uint32_t GetMinorV() = 0;
protected:
    uint32_t m_MajorV = 0;
    uint32_t m_MinorV = 0;
};

GAPI_NAMESPACE_CLOSE
