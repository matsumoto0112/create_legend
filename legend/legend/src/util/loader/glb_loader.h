#ifndef LEGEND_UTIL_LOADER_GLB_LOADER_H_
#define LEGEND_UTIL_LOADER_GLB_LOADER_H_

#include <GLTFSDK/GLBResourceReader.h>
#include <GLTFSDK/GLTF.h>

/**
 * @file glb_loader.h
 * @brief .glbファイルを読み込む
 */

namespace legend {
namespace util {
namespace loader {

/**
 * @brief 読み込み
 */
class StreamReader final : public Microsoft::glTF::IStreamReader {
 public:
  StreamReader();
  /**
   * @brief filenameをバイナリ形式で読み込む
   * @param filename 読み込むファイル名
   * @return ストリームを構築して返す
   */
  std::shared_ptr<std::istream> GetInputStream(
      const std::string& filename) const override;
};

/**
 * @brief GLBモデルを読み込む
 */
class GLBLoader {
 public:
  GLBLoader();
  /**
   * @brief デストラクタ
   */
  ~GLBLoader();
  /**
   * @brief モデルを読み込む
   * @param filename ファイル名
   * @return 成功したらtrueを返す
   */
  bool Load(const std::filesystem::path& filename);
  /**
   * @brief 頂点数を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  u32 GetVertexNum(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief 頂点座標配列を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  std::vector<float> GetPosition(u32 mesh_index = 0,
                                 u32 primitive_index = 0) const;
  /**
   * @brief 一頂点座標の要素数を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  u32 GetPositionComponentSize(u32 mesh_index = 0,
                               u32 primitive_index = 0) const;
  /**
   * @brief 法線配列を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  std::vector<float> GetNormal(u32 mesh_index = 0,
                               u32 primitive_index = 0) const;
  /**
   * @brief 一法線の要素数を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  u32 GetNormalComponentSize(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief UV座標配列を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  std::vector<float> GetUV(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief 一座標の要素数を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  u32 GetUVComponentSize(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief 接線配列を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  std::vector<float> GetTangent(u32 mesh_index = 0,
                                u32 primitive_index = 0) const;
  /**
   * @brief 一接線の要素数を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  u32 GetTangentComponentSize(u32 mesh_index = 0,
                              u32 primitive_index = 0) const;
  /**
   * @brief インデックス配列を取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  std::vector<u16> GetIndex(u32 mesh_index = 0, u32 primitive_index = 0) const;
  /**
   * @brief アルベドテクスチャを取得する
   */
  std::vector<u8> GetAlbedo() const;
  std::vector<u8> GetNormalMap() const;

 private:
  /**
   * @brief メッシュを取得する
   * @param mesh_index メッシュ番号
   * @param primitive_index プリミティブ番号
   */
  const Microsoft::glTF::MeshPrimitive& GetMesh(u32 mesh_index,
                                                u32 primitive_index) const;
  /**
   * @brief アクセサが存在すればそれを取得する
   * @param mesh 対象メッシュ
   * @param accessor_name アクセサ名
   * @param accessor 取得したアクセサ（戻り値）
   * @return アクセサが存在したらtrueを返す
   */
  bool GetAccessor(const Microsoft::glTF::MeshPrimitive& mesh,
                   const char* accessor_name,
                   Microsoft::glTF::Accessor* accessor) const;
  /**
   * @brief アクセサ名に対応したアトリビュートを取得する
   * @param mesh 対象メッシュ
   * @param accessor_name アクセサ名
   * @return アクセサ名に対応した要素を、存在しなければ空の配列を返す
   */
  std::vector<float> GetAttribute(const Microsoft::glTF::MeshPrimitive& mesh,
                                  const char* accessor_name) const;
  /**
   * @brief アクセサ名に対応したアトリビュートの要素数を取得する
   * @param mesh 対象メッシュ
   * @param accessor_name アクセサ名
   * @return アクセサ名に対応した要素を、存在しなければ0を返す
   */
  u32 GetAttributeComponentSize(const Microsoft::glTF::MeshPrimitive& mesh,
                                const char* accessor_name) const;

 private:
  //! ストリーム読み込み器
  std::unique_ptr<StreamReader> stream_reader_;
  //! バイナリ読み込み
  std::unique_ptr<Microsoft::glTF::GLBResourceReader> glb_resource_reader_;
  //! デシリアライズされたデータ
  Microsoft::glTF::Document document_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_GLB_LOADER_H_
