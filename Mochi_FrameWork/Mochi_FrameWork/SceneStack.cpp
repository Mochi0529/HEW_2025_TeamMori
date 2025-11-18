#include "SceneStack.h"
#include "EventQueue.h"
#include "Scene.h"
#include "InputManager.h"

// === –¼‘O‹óŠÔŽg—péŒ¾ === //
using namespace MochiFramework::InputSystem;

namespace MochiFramework::SceneSystem
{
    SceneStack::~SceneStack() {
        Shutdown();
    }

    void SceneStack::RegisterScene(const std::string& name, Factory factory) {
        factories[name] = std::move(factory);
    }

    void SceneStack::ChangeMainScene(const std::string& name, EventQueue* queue) {
        auto it = factories.find(name);
        if (it == factories.end()) return;

        if (mainScene) {
            mainScene->UnInit();
        }

        mainScene = it->second(queue);
        mainScene->Init();
    }

    void SceneStack::ReloadMainScene(EventQueue* queue) {
        if (mainScene) {
            mainScene->UnInit(); // GameObject‚È‚Ç‚ðˆê“x”jŠü
            mainScene->Init();   // •K—v‚È‚à‚Ì‚ðÄ\’ziCreateGameObject‚È‚Çj
        }
    }

    void SceneStack::PushOverlayScene(const std::string& name, EventQueue* queue) {
        auto it = factories.find(name);
        auto scene = it->second(queue);
        if (scene) {
            scene->Init();
            overlays.push_back(std::move(scene));
        }
    }

    void SceneStack::PopOverlayScene() {
        if (!overlays.empty()) {
            overlays.back()->UnInit();
            overlays.pop_back();
        }
    }

    void SceneStack::Input(InputManager* device) {
        if (!overlays.empty()) {
            overlays.back()->Input(); // Å‘O–Ê‚ÌƒI[ƒo[ƒŒƒCƒV[ƒ“‚É‚Ì‚Ý“ü—Í‚ð“n‚·
        }
        else if (mainScene) {

        }
        mainScene->Input();
    }

    void SceneStack::Update(const float deltaTime) {
        if (!overlays.empty()) {
            overlays.back()->Update(deltaTime);
        }
        else if (mainScene) {
            mainScene->Update(deltaTime);
        }
    }

    void SceneStack::Draw() {
        // •`‰æŽè‡F
        // 1. mainScene ‚ð‚Ü‚¸•`‰æi”wŒi‘wj
        // 2. overlay ‚ðã‚©‚ç‡‚É‘–¸‚µAAllowSceneBelow() ‚É‚æ‚Á‚Ä•`‰æ‘ÎÛ‚ðŒˆ’è
        // 3. Œˆ’è‚³‚ê‚½ overlays ‚ð‰º‚©‚ç‡‚É•`‰æiŽè‘O‚É‚È‚é‚Ù‚ÇŒã‚É•`‰æ‚³‚ê‚éj

        if (mainScene) mainScene->Draw();

        std::vector<Scene*> toDraw;
        for (int i = static_cast<int>(overlays.size()) - 1; i >= 0; --i) {
            Scene* scene = overlays[i].get();
            toDraw.push_back(scene);
            if (!scene->ShouldDrawSceneBelow()) break;
        }

        for (auto it = toDraw.rbegin(); it != toDraw.rend(); ++it) {
            (*it)->Draw();
        }
    }

    void SceneStack::Shutdown() {
        if (mainScene) {
            mainScene->UnInit();
            mainScene.reset();
        }

        for (auto& overlay : overlays) {
            overlay->UnInit();
        }
        overlays.clear();
    }
} // namespace MochiFramework::SceneSystem