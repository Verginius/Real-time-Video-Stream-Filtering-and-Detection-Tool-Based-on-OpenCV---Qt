#include "DetectionBackend.h"
#include "YOLODetector.h"

#ifdef HAVE_TENSORRT
#include "TensorRTBackend.h"
#endif

std::unique_ptr<DetectionBackend> createBackend(bool useTensorRT) {
#ifdef HAVE_TENSORRT
    if (useTensorRT) {
        return std::make_unique<TensorRTBackend>();
    }
#else
    (void)useTensorRT;
#endif
    return std::make_unique<YOLODetector>();
}
