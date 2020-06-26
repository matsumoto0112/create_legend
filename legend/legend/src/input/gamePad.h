/**
 * @file gamePad.h
 * @brief ゲームパッド入力管理
 */

#pragma once
#pragma comment(lib, "winmm.lib")

#include <stdio.h>
#include <windows.h>
#include <src/math/vector_2.h>
#include "src/input/joyCode.h"

namespace Framework::Input {
class gamePad {
  static constexpr int PAD_MAX = 4;  //!< ゲームパッド最大数
  using JoyCode = joyCode::Enum;
  using Vector2 = legend::math::Vector2;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  gamePad(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~gamePad();
  /**
   * @brief 更新
   */
  void update();
  /**
   * @brief 接続数
   * @return 接続数
   */
  int joyCount();

  /**
  * @brief ゲームパッドの左スティックの移動値
  * @param index 対応するコントローラー
  * @return 左スティックの移動値
  動いていない、コントローラが存在しない場合は0
  */
  Vector2 getStickLeft(unsigned int index);
  /**
  * @brief ゲームパッドの右スティックの移動値
  * @param index 対応するコントローラー
  * @return 右スティックの移動値
  動いていない、コントローラが存在しない場合は0
  */
  Vector2 getStickRight(unsigned int index);
  /**
  * @brief ゲームパッドの十字キーの移動値
  * @param index 対応するコントローラー
  * @return 十字キーの移動値
  動いていない、コントローラが存在しない場合は0
  */
  Vector2 getCrossKey(unsigned int index);

  /**
  * @brief キーを押しているか
  * @param joy 判定するキー
  * @return 今updateで押していたらtrue
  押されていない、コントローラが存在しない場合はfalse
  */
  bool getButton(JoyCode joyCode) const;
  /**
  * @brief キーを押したか（トリガー）
  * @param joy 判定するキー
  * @return 前updateで押されていないかつ今updateで押されていたらtrue
  それ以外の場合はfalse
  */
  bool getButtonDown(JoyCode joyCode) const;
  /**
  * @brief キーを離したか（トリガー）
  * @param joy 判定するキー
  * @return 前updateで押されていてかつ今updateで離されたらtrue
  それ以外の場合はfalse
  */
  bool getButtonUp(JoyCode joyCode) const;

 private:
  /**
   * @brief ゲームパッドの状態更新
   */
  void setState();
  /**
   * @brief 情報取得時、エラーが起きるか
   * @return 接続エラーが起きなければtrue
   */
  bool noError(unsigned int index) const;

  /**
   * @brief ゲームパッドのスティックX軸の移動値取得
   * @return スティックのX軸
   */
  float xPos(unsigned int index);
  /**
   * @brief ゲームパッドのスティックY軸の移動値取得
   * @return スティックのY軸
   */
  float yPos(unsigned int index);
  /**
   * @brief ゲームパッドのスティックZ軸の移動値取得
   * @return スティックのZ軸
   */
  float zPos(unsigned int index);
  /**
   * @brief ゲームパッドのスティックU軸の移動値取得
   * @return スティックのU軸
   */
  float uPos(unsigned int index);
  /**
   * @brief ゲームパッドのスティックV軸の移動値取得
   * @return スティックのV軸
   */
  float vPos(unsigned int index);
  /**
   * @brief ゲームパッドのスティックR軸の移動値取得
   * @return スティックのR軸
   */
  float rPos(unsigned int index);

  /**
   * @brief ゲームパッドの十字キー横軸の移動値取得
   * @return 十字キーの横軸
   */
  float getCrossHorizontal(unsigned int index);
  /**
   * @brief ゲームパッドの十字キー縦軸の移動値取得
   * @return 十字キーの縦軸
   */
  float getCrossVertical(unsigned int index);

 private:
  HWND mHWnd;        //!< ウィンドウハンドル
  int padCount = 0;  //!< ゲームパッド数

  std::vector<JOYINFOEX> mCurrentJoys;  //!< 現在のゲームパッドの状態
  std::vector<unsigned long> mPreJoys;  //!< 前回のゲームパッドのボタン情報取得
  std::vector<MMRESULT> mResultJoys;  //!< ゲームパッドが取得できるかの判定
};

}  // namespace Framework::Input
