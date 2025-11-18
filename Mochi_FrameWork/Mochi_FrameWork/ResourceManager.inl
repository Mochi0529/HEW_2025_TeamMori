#pragma once
#include <iostream>

// ====== RegisterLoader =====================================

template<typename T>
inline void ResourceManager::RegisterLoader(std::function<T* (const std::string& path)> loader)
{
    std::type_index ti(typeid(T));

    LoaderEntry entry;
    entry.load = [loader](const std::string& path) -> void*
        {
            return loader(path);
        };
    entry.destroy = [](void* p)
        {
            delete static_cast<T*>(p);
        };

    mLoaders[ti] = std::move(entry);
}

// ====== FindExistingHandle =================================

template<typename T>
inline ResourceHandle ResourceManager::FindExistingHandle(const std::string& id) const
{
    ResourceKey key{ std::type_index(typeid(T)), id };
    auto it = mKeyToHandle.find(key);
    if (it != mKeyToHandle.end())
    {
        return it->second;
    }
    return ResourceHandle::Invalid();
}

// ====== GetFallback ========================================

template<typename T>
inline ResourcePtr<T> ResourceManager::GetFallback() const
{
    auto ti = std::type_index(typeid(T));
    auto it = mFallbackHandles.find(ti);
    if (it == mFallbackHandles.end())
    {
        return ResourcePtr<T>(); // 無効
    }
    return ResourcePtr<T>(const_cast<ResourceManager*>(this), it->second);
}

// ====== SetFallback ========================================

template<typename T>
inline void ResourceManager::SetFallback(ResourceHandle handle)
{
    auto ti = std::type_index(typeid(T));

    // 以前のフォールバックがあれば Release
    auto it = mFallbackHandles.find(ti);
    if (it != mFallbackHandles.end())
    {
        InternalRelease(it->second);
    }

    if (handle.IsValid())
    {
        InternalAddRef(handle); // フォールバックとして1参照
        mFallbackHandles[ti] = handle;
    }
}

// ====== Load ===============================================

template<typename T>
inline ResourcePtr<T> ResourceManager::Load(const std::string& id, int groupId)
{
    // すでにロード済みか？
    ResourceHandle handle = FindExistingHandle<T>(id);
    if (!handle.IsValid())
    {
        // 新規ロード
        auto ti = std::type_index(typeid(T));
        auto loaderIt = mLoaders.find(ti);
        if (loaderIt == mLoaders.end())
        {
            std::cerr << "[ResourceManager] Loader not found for type: "
                << ti.name() << std::endl;
            return GetFallback<T>();
        }

        const std::string path = ResolvePath(id);
        void* rawPtr = nullptr;
        try
        {
            rawPtr = loaderIt->second.load(path);
        }
        catch (const std::exception& e)
        {
            std::cerr << "[ResourceManager] Load exception id=" << id
                << " path=" << path << " : " << e.what() << std::endl;
            return GetFallback<T>();
        }

        if (!rawPtr)
        {
            std::cerr << "[ResourceManager] Loader returned nullptr id=" << id
                << " path=" << path << std::endl;
            return GetFallback<T>();
        }

        handle = AcquireSlot();
        Slot& slot = mSlots[handle.index];
        slot.ptr = rawPtr;
        slot.type = ti;
        slot.id = id;
        slot.path = path;
        slot.refCount = 0;
        slot.memoryBytes = 0;
        slot.alive = true;
        slot.generation = handle.gen;

        ResourceKey key{ ti, id };
        mKeyToHandle[key] = handle;
    }

    // 呼び出し元用 ResourcePtr（ここで refCount +1）
    ResourcePtr<T> ptr(this, handle);

    // グループに所属させる場合、グループも AddRef
    if (groupId != 0)
    {
        auto git = mGroups.find(groupId);
        if (git != mGroups.end())
        {
            git->second.handles.push_back(handle);
            InternalAddRef(handle); // グループの所有分
        }
    }

    return ptr;
}

// ====== Get ================================================

template<typename T>
inline T* ResourceManager::Get(ResourceHandle handle) const
{
    if (!handle.IsValid())
        return nullptr;
    if (handle.index >= mSlots.size())
        return nullptr;

    const Slot& slot = mSlots[handle.index];
    if (!slot.alive)
        return nullptr;
    if (slot.generation != handle.gen)
        return nullptr;
    if (slot.type != std::type_index(typeid(T)))
        return nullptr;

    return static_cast<T*>(slot.ptr);
}
