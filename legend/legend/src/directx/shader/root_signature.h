#ifndef LEGEND_DIRECTX_SHADER_ROOT_SIGNATURE_H_
#define LEGEND_DIRECTX_SHADER_ROOT_SIGNATURE_H_

/**
 * @file root_signature.h
 * @brief ルートシグネチャクラス定義
 * @details
 このクラスで定義するルートシグネチャはすべてのシェーダーで使いまわせるようにディスクリプタを最大限に確保する。
 決して速度的な意味で効率的な構造とは言えないが、シェーダーごとに作成するよりメモリ効率が良い。
 最大限の速度を出すためならば、シェーダーごとにチューニングしたルートシグネチャを作るほうが良いが、その分作成時間・メモリを消費する。
 */

#include "src/directx/directx12_device.h"

namespace legend {
namespace directx {
namespace shader {
/**
 * @brief ルートシグネチャクラス
 */
class RootSignature {
 public:
  /**
   * @brief コンストラクタ
   */
  RootSignature();
  /**
   * @brief デストラクタ
   */
  ~RootSignature();
  /**
   * @brief 初期化
   * @param device DirectX12デバイス
   * @param name リソース名
   * @return 初期化に成功したらtrueを返す
   */
  bool Init(DirectX12Device& device, const std::wstring& name);
  /**
   * @brief ルートシグネチャを取得する
   */
  inline ID3D12RootSignature* GetRootSignature() const {
    return root_signature_.Get();
  }

  /**
   * @brief コマンドリストにセットする
   * @param device DirectX12デバイス
   */
  void SetGraphicsCommandList(DirectX12Device& device);

 private:
  //! ルートシグネチャ
  ComPtr<ID3D12RootSignature> root_signature_;
};

}  // namespace shader
}  // namespace directx
}  // namespace legend
#endif  //! LEGEND_DIRECTX_SHADER_ROOT_SIGNATURE_H_
