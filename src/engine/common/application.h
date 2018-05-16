#include "common.h"

//enum class 

class Application : public std::enable_shared_from_this<Application> 
{
public:
    virtual ~Application() {}

    // ray tracing properties
    virtual int GetMaxReflectionBounces() const = 0;
    virtual int GetMaxRefractionBounces() const = 0;

    // output 
    virtual glm::vec2 GetImageOutputResolution() const;

    // Sampling Properties
    virtual int GetSamplesPerPixel() const;

    // whether or not to continue sampling the scene from the camera.
    virtual bool NotifyNewPoxelSample(glm::vec3 inputSampleColor, int sampleIndex) = 0;

    // Postprocessing
    virtual void PerformImagePostprocessing(class ImageWriter& imageWriter);

    virtual std::string GetOutputFilename() const;

};