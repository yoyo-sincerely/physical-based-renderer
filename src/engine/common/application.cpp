#include "common/Application.h"
#include "common/Acceleration/AccelerationCommon.h"
#include "common/Output/ImageWriter.h"

std::string Application::GetOutputFilename() const
{
    return "output.png";
}

int Application::GetSamplesPerPixel() const
{
    return 1;
}

glm::vec2 Application::GetImageOutputResolution() const
{
    return glm::vec2(512.f, 256.f);
}

void Application::PerformImagePostprocessing(class ImageWriter&)
{
}