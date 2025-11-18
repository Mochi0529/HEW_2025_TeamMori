#pragma once
#include <cstdint>

struct ResourceHandle
{
    uint32_t index = 0xffffffffu; // –³Œø’l
    uint32_t gen   = 0xffffffffu; // –³Œø’l

    bool IsValid() const noexcept
    {
        return index != 0xffffffffu && gen != 0xffffffffu;
    }

    static ResourceHandle Invalid()
    {
        return ResourceHandle{};
    }

    bool operator==(const ResourceHandle& rhs) const noexcept
    {
        return index == rhs.index && gen == rhs.gen;
    }
};
