#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <typeindex>

#include "ResourceHandle.h"
#include "ResourcePtr.h"

class ResourceManager
{
public:
    struct ResourceDebugInfo
    {
        ResourceHandle handle;
        std::string    id;          // エイリアス / ID
        std::string    path;        // 実パス
        std::string    typeName;    // typeid(T).name()
        int            refCount = 0;      // デフォルト初期化
        std::size_t    memoryBytes = 0;      // デフォルト初期化
    };

    ResourceManager();
    ~ResourceManager();

    //================ ローダ登録（型ごと） ====================

    template<typename T>
    void RegisterLoader(std::function<T* (const std::string& path)> loader);

    //================ エイリアス（ID → パス） =================

    void RegisterAlias(const std::string& id, const std::string& path);
    std::string ResolvePath(const std::string& id) const;

    //================ ロード / 取得 ============================

    template<typename T>
    ResourcePtr<T> Load(const std::string& id, int groupId = 0);

    template<typename T>
    T* Get(ResourceHandle handle) const;

    //================ フォールバック ==========================

    template<typename T>
    void SetFallback(ResourceHandle handle);

    template<typename T>
    ResourcePtr<T> GetFallback() const;

    //================ グループ管理 =============================

    int BeginGroup(const std::string& name);
    void UnloadGroup(int groupId);

    //================ デバッグ情報 =============================

    std::vector<ResourceDebugInfo> GetDebugInfo() const;
    void DumpDebugInfo() const;

    //================ ResourcePtr から呼ばれる内部API =========

    void InternalAddRef(ResourceHandle handle);
    void InternalRelease(ResourceHandle handle);

private:
    struct Slot
    {
        void* ptr = nullptr;
        std::type_index type = typeid(void);
        std::string    id;
        std::string    path;
        int            refCount = 0;
        std::size_t    memoryBytes = 0;
        uint32_t       generation = 0;
        bool           alive = false;
    };

    struct LoaderEntry
    {
        std::function<void* (const std::string&)> load;
        std::function<void(void*)>              destroy;
    };

    struct ResourceKey
    {
        std::type_index type;
        std::string     id;

        bool operator==(const ResourceKey& rhs) const noexcept
        {
            return type == rhs.type && id == rhs.id;
        }
    };

    struct ResourceKeyHasher
    {
        std::size_t operator()(const ResourceKey& key) const noexcept
        {
            std::size_t h1 = std::hash<std::string>()(key.id);
            std::size_t h2 = std::hash<std::size_t>()(key.type.hash_code());
            return h1 ^ (h2 << 1);
        }
    };

    struct Group
    {
        std::string                 name;
        std::vector<ResourceHandle> handles;
    };

private:
    ResourceHandle AcquireSlot();
    void           DestroySlot(ResourceHandle handle);

    template<typename T>
    ResourceHandle FindExistingHandle(const std::string& id) const;

private:
    std::vector<Slot>      mSlots;
    std::vector<uint32_t>  mFreeIndices;

    // (type, id) → handle
    std::unordered_map<ResourceKey, ResourceHandle, ResourceKeyHasher> mKeyToHandle;

    // type → loader
    std::unordered_map<std::type_index, LoaderEntry> mLoaders;

    // type → fallback handle
    std::unordered_map<std::type_index, ResourceHandle> mFallbackHandles;

    // alias → path
    std::unordered_map<std::string, std::string> mAliasToPath;

    // groupId → Group
    std::unordered_map<int, Group> mGroups;
    int mNextGroupId = 1;

    template<typename T>
    friend class ResourcePtr;
}; 

#include "ResourcePtr.inl"
#include "ResourceManager.inl"
