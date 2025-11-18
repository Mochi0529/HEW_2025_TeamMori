#pragma once
#include "ResourceHandle.h"

class ResourceManager;

template<typename T>
class ResourcePtr
{
public:
    ResourcePtr() = default;
    ResourcePtr(ResourceManager* mgr, ResourceHandle handle);

    ResourcePtr(const ResourcePtr& rhs);
    ResourcePtr(ResourcePtr&& rhs) noexcept;

    ResourcePtr& operator=(const ResourcePtr& rhs);
    ResourcePtr& operator=(ResourcePtr&& rhs) noexcept;

    ~ResourcePtr();

    bool IsValid() const;

    ResourceHandle GetHandle() const { return mHandle; }

    T* Get() const;
    T* operator->() const { return Get(); }
    T& operator*()  const { return *Get(); }

private:
    void AddRef();
    void Release();

    ResourceManager* mManager = nullptr;
    ResourceHandle   mHandle = ResourceHandle::Invalid();

    template<typename U>
    friend class ResourcePtr;
};
