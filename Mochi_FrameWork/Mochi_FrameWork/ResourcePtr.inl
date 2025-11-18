#include "ResourcePtr.h"
#pragma once

// ResourceManager.h からインクルードされる前提。
// ここでは ResourceManager クラスの完全な宣言が見えている。



template<typename T>
inline ResourcePtr<T>::ResourcePtr(ResourceManager* mgr, ResourceHandle handle)
    : mManager(mgr)
    , mHandle(handle)
{
    AddRef();
}

template<typename T>
inline ResourcePtr<T>::ResourcePtr(const ResourcePtr& rhs)
    : mManager(rhs.mManager)
    , mHandle(rhs.mHandle)
{
    AddRef();
}

template<typename T>
inline ResourcePtr<T>::ResourcePtr(ResourcePtr&& rhs) noexcept
    : mManager(rhs.mManager)
    , mHandle(rhs.mHandle)
{
    rhs.mManager = nullptr;
    rhs.mHandle  = ResourceHandle::Invalid();
}

template<typename T>
inline ResourcePtr<T>& ResourcePtr<T>::operator=(const ResourcePtr& rhs)
{
    if (this == &rhs) return *this;
    Release();
    mManager = rhs.mManager;
    mHandle  = rhs.mHandle;
    AddRef();
    return *this;
}

template<typename T>
inline ResourcePtr<T>& ResourcePtr<T>::operator=(ResourcePtr&& rhs) noexcept
{
    if (this == &rhs) return *this;
    Release();
    mManager     = rhs.mManager;
    mHandle      = rhs.mHandle;
    rhs.mManager = nullptr;
    rhs.mHandle  = ResourceHandle::Invalid();
    return *this;
}

template<typename T>
inline ResourcePtr<T>::~ResourcePtr()
{
    Release();
}

template<typename T>
inline bool ResourcePtr<T>::IsValid() const
{
    return mManager != nullptr && mHandle.IsValid();
}

template<typename T>
inline void ResourcePtr<T>::AddRef()
{
    if (mManager && mHandle.IsValid())
    {
        mManager->InternalAddRef(mHandle);
    }
}

template<typename T>
inline void ResourcePtr<T>::Release()
{
    if (mManager && mHandle.IsValid())
    {
        mManager->InternalRelease(mHandle);
    }
    mManager = nullptr;
    mHandle  = ResourceHandle::Invalid();
}

template<typename T>
inline T* ResourcePtr<T>::Get() const
{
    if (!mManager || !mHandle.IsValid())
        return nullptr;

    return mManager->template Get<T>(mHandle);
}

