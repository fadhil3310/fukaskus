#pragma once

#include <string>


namespace FukaskusWriter
{    
    class IBundleWriter
    {
    public:
        virtual void write() = 0;
    };
}