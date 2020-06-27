#ifndef LEGEND_INPUT_GAMEPAD_H_
#define LEGEND_INPUT_GAMEPAD_H_

/**
 * @file gamePad.h
 * @brief ゲームパッド入力管理
 */

#pragma comment(lib, "winmm.lib")

#include <src/math/vector_2.h>
#include <stdio.h>
#include <windows.h>
#include "src/input/joyCode.h"

namespace legend {
namespace input {
class GamePad {
  static constexpr i32 PAD_MAX = 4;  //!< ゲームパッド最大数
  using JoyCode = joyCode::Enum;

 public:
  /**
   * @brief コンストラクタ
   * @param hWnd ウィンドウハンドル
   */
  GamePad(HWND hWnd);
  /**
   * @brief デストラクタ
   */
  ~GamePad();
  /**
   * @brief 更新
   */
  void Update();
  /**
   * @brief 接続数
   * @return 接続数
   */
  i32 GetCount();

  /**
  * @brief ゲームパッドの左スティックの移動値
  * @param index 対応するコントローラー
  * @return 左スティックの移動値
  動いていない、コントローラが存在しない場合は0
  */
  legend::math::Vector2 GetStickLeft(u32 index);
  /**
  * @brief ゲームパッドの右スティックの移動値
  * @param index 対応するコントローラー
  * @return 右スティックの移動値
  動いていない、コントローラが存在しない場合は0
  */
  legend::math::Vector2 GetStickRight(u32 index);
  /**
  * @brief ゲームパッドの十字キーの移動値
  * @param index 対応するコントローラー
  * @return 十字キーの移動値
  動いていない、コントローラが存在しない場合は0
  */
  legend::math::Vector2 GetCrossKey(u32 index);

  /**
  * @brief キーを押しているか
  * @param joy 判定するキー
  * @param index 判定するコントローラー
  * @return 今updateで押していたらtrue
  押されていない、コントローラが存在しない場合はfalse
  */
  bool GetButton(JoyCode joyCode, u32 index) const;
  /**
  * @brief キーを押したか（トリガー）
  * @param joy 判定するキー
  * @param index 判定するコントローラー
  * @return 前updateで押されていないかつ今updateで押されていたらtrue
  それ以外の場合はfalse
  */
  bool GetButtonDown(JoyCode joyCode, u32 index) const;
  /**
  * @brief キーを離したか（トリガー）
  * @param joy 判定するキー
  * @param index 判定するコントローラー
  * @return 前updateで押されていてかつ今updateで離されたらtrue
  それ以外の場合はfalse
  */
  bool GetButtonUp(JoyCode joyCode, u32 index) const;

  /**
  * @brief キーを押しているか
  * @param joy 判定するキー
  * @return 今updateで押していたらtrue
  押されていない、コントローラが存在しない場合はfalse
  */
  bool GetButton(JoyCode joyCode) const;
  /**
  * @brief キーを押したか（トリガー）
  * @param joy 判定するキー
  * @return 前updateで押されていないかつ今updateで押されていたらtrue
  それ以外の場合はfalse
  */
  bool GetButtonDown(JoyCode joyCode) const;
  /**
  * @brief キーを離したか（トリガー）
  * @param joy 判定するキー
  * @return 前updateで押されていてかつ今updateで離されたらtrue
  それ以外の場合はfalse
  */
  bool GetButtonUp(JoyCode joyCode) const;

 private:
  HWND mHWnd_;        //!< ウィンドウハンドル
  i32 padCount_ = 0;  //!< ゲームパッド数

  std::vector<JOYINFOEX> mCurrentJoys_;  //!< 現在のゲームパッドの状態
  std::vector<u64> mPreJoys_;  //!< 前回のゲームパッドのボタン情報取得
  std::vector<MMRESULT> mResultJoys_;  //!< ゲームパッドが取得できるかの判定

 private:
  /**
   * @brief ゲームパッドの状態更新
   */
  void SetState();
  /**
   * @brief 情報取得時、エラーが起きるか
   * @return 接続エラーが起きなければtrue
   */
  bool NoError(u32 index) const;

  /**
   * @brief ゲームパッドのスティックX軸の移動値取得
   * @return スティックのX軸
   */
  float Stick_X_Pos(u32 index);
  /**
   * @brief ゲームパッドのスティックY軸の移動値取得
   * @return スティックのY軸
   */
  float Stick_Y_Pos(u32 index);
  /**
   * @brief ゲームパッドのスティックZ軸の移動値取得
   * @return スティックのZ軸
   */
  float Stick_Z_Pos(u32 index);
  /**
   * @brief ゲームパッドのスティックU軸の移動値取得
   * @return スティックのU軸
   */
  float Stick_U_Pos(u32 index);
  /**
   * @brief ゲームパッドのスティックV軸の移動値取得
   * @return スティックのV軸
   */
  float Stick_V_Pos(u32 index);
  /**
   * @brief ゲームパッドのスティックR軸の移動値取得
   * @return スティックのR軸
   */
  float Stick_R_Pos(u32 index);

  /**
   * @brief ゲームパッドの十字キー横軸の移動値取得
   * @return 十字キーの横軸
   */
  float GetCrossHorizontal(u32 index);
  /**
   * @brief ゲームパッドの十字キー縦軸の移動値取得
   * @return 十字キーの縦軸
   */
  float GetCrossVertical(u32 index);
};

}  // namespace input
}  // namespace legend

#endif  // !LEGEND_INPUT_GAMEPAD_H_