#pragma once
#include <cstdint>

namespace Graphics {

    enum class DeviceMemoryProperty : size_t {
        DeviceLocal = 0,
        HostVisible,
        HostCoherent,
        HostCached,
        LazilyAllocated,
        Protected,
        DeviceCoherentAMD,
        DeviceUncachedAMD,
        RdmaCapableNV,

        Num
    };

    template<DeviceMemoryProperty F>
    struct DeviceMemoryPropertyTypeTrait;

    // Shader Pipeline Features
    template<> struct DeviceMemoryPropertyTypeTrait<DeviceMemoryProperty::Num> { using Type = bool; };
}