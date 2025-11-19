#pragma once

#include <memory>

#include "batch.h"
#include "structs.h"

GLIB_NAMESPACE_OPEN

struct Buffer {
public:
    DrawResources m_LocalDR;
    std::shared_ptr<Batch> m_LocalBatch;
};

GLIB_NAMESPACE_CLOSE