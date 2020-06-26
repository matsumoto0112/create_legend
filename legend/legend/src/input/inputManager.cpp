#include "src/input/inputManager.h"

namespace Framework::Input {
//�R���X�g���N�^
inputManager::inputManager(HWND hWnd)
    : mKeyboard(std::make_unique<keyboard>(hWnd)),
      mMouse(std::make_unique<mouse>(hWnd)),
      mGamepad(std::make_unique<gamePad>(hWnd)) {}
//�f�X�g���N�^
inputManager::~inputManager() {}
//���͏��̍X�V
void inputManager::update() {
  mKeyboard->update();
  mMouse->update();
  mGamepad->update();
}

}  // namespace Framework::Input
