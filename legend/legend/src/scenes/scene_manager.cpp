#include "src/scenes/scene_manager.h"
#include "src/scenes/title.h"

namespace legend {
namespace scenes {

SceneManager::SceneManager() : scene_() {}

SceneManager::~SceneManager() { 
	delete scene_; 
}

void SceneManager::Update() { 
	Scene *s = scene_->Update();
	if (s != scene_) {
        delete scene_;
        scene_ = s;
	}
}

void SceneManager::Add() {}

void SceneManager::Change() {}

void SceneManager::Draw() const { 
	scene_->Draw(); 
}
}  // namespace scene
}  // namespace legend

