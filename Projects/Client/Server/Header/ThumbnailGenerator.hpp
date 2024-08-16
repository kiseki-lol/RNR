#pragma once

#include <App/V8/Tree/Instance.hpp>

class ThumbnailGenerator : public RNR::Instance
{
public:
    ThumbnailGenerator();

    std::string click(int resX, int resY, char* format, bool clear);
};