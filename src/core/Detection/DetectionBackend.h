#pragma once
#include "Detection.h"
#include "DetectorBase.h"
#include <memory>
#include <string>

class DetectionBackend : public DetectorBase {
public:
    virtual ~DetectionBackend() = default;

    virtual double lastInferenceMsec() const = 0;
};

std::unique_ptr<DetectionBackend> createBackend(bool useTensorRT);
