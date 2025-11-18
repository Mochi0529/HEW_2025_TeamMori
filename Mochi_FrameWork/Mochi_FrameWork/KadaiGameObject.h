    #pragma once
    #include "GameObjectBase.h"

    namespace MochiFramework::GameObjects {

        class KadaiGameObject : public GameObjectBase<KadaiGameObject>
        {
        public:
            explicit KadaiGameObject(MochiFramework::SceneSystem::Scene* scene)
                : GameObjectBase(scene)
            {
                mObjectCounter++;
            }

            virtual ~KadaiGameObject()
            {
                mObjectCounter--;
            }
            static void DisplayCounter(void);
            //

        private:
            static int mObjectCounter;
            //
        };

    } // namespace MochiFramework::GameObjects