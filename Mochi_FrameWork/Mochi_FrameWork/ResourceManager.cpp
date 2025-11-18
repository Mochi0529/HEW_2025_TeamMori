#include "ResourceManager.h"
#include <iostream>

ResourceManager::ResourceManager()
    : mNextGroupId(1)
{
}

ResourceManager::~ResourceManager()
{
    // 全スロット破棄
    for (auto& slot : mSlots)
    {
        if (slot.alive && slot.ptr)
        {
            auto loaderIt = mLoaders.find(slot.type);
            if (loaderIt != mLoaders.end())
            {
                loaderIt->second.destroy(slot.ptr);
            }
            slot.ptr = nullptr;
        }
    }
}

// 以下、RegisterAlias / ResolvePath / AcquireSlot / DestroySlot / InternalAddRef /
// InternalRelease / BeginGroup / UnloadGroup / GetDebugInfo / DumpDebugInfo
// は前回渡したものと同じでOK
