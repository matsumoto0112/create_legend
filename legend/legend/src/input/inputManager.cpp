#include "src/input/inputManager.h"

namespace Framework::Input {
//コンストラクタ
inputManager::inputManager(HWND hWnd)
    : mKeyboard(std::make_unique<keyboard>(hWnd)),
      mMouse(std::make_unique<mouse>(hWnd)),
      mGamepad(std::make_unique<gamePad>(hWnd)) {}
//デストラクタ
inputManager::~inputManager() {}
//入力情報の更新
void inputManager::update() {
  mKeyboard->update();
  mMouse->update();
  mGamepad->update();
}

}  // namespace Framework::Input
