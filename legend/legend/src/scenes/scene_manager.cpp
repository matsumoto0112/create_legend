#include "src/scenes/scene_manager.h"
#include "src/scenes/title.h"

namespace legend {
namespace scenes {

SceneManager::SceneManager() : scene_() {}

SceneManager::~SceneManager() { 
	scene_.reset(); 
}

void SceneManager::Update() { 
	if (scene_ == nullptr) {
	    return;
	}

	std::unique_ptr<Scene> s;
    s = std::move(scene_);
    s->Update();
	if (s != scene_) {
		scene_.reset();
        scene_ = std::move(s);
	}
}

void SceneManager::Add() {}

void SceneManager::Change() { 
	if (scene_ != nullptr) {
	    return;
	}
}

void SceneManager::Draw() const { 
	if (scene_ == nullptr) {
	    return;
	}

	scene_->Draw(); 
}
}  // namespace scene
}  // namespace legend

