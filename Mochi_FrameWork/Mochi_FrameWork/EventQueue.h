    #pragma once
    #include "SceneEvent.h"
    #include <queue>
    #include <optional>

    // === 名前空間 === //
    namespace MochiFramework::SceneSystem
    {

        // シーンイベントを一時的に蓄積して、順に処理するためのキュークラス
        // イベントはFIFO（先入れ先出し）で処理される
        class EventQueue {
        public:
            void Push(const MochiFramework::SceneSystem::SceneEvent& e);    // コピーしてキューに追加
            void Push(MochiFramework::SceneSystem::SceneEvent&& e);         // ムーブしてキューに追加

            bool IsEmpty() const;

            // キューの先頭からイベントを1つ取り出す（ムーブ）
            // 空の場合は std::nullopt を返す
            std::optional<MochiFramework::SceneSystem::SceneEvent> TryPop();

        private:
            std::queue<MochiFramework::SceneSystem::SceneEvent> events;
        };
    } // namespace MochiFramework