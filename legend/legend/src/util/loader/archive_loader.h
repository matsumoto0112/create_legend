#ifndef LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H
#define LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H

/**
 * @file archive_loader.h
 * @brief archiveファイルからデータを復元する
 */
namespace legend {
namespace util {
namespace loader {

/**
 * @brief アーカイブの各項目のデータ
 */
struct Entity {
 public:
  //! 先頭からの読み取り位置
  i32 position;
  //! 1項目のデータの大きさ
  i32 size;
  /**
   * @brief コンストラクタ
   */
  Entity(i32 position, i32 size) : position(position), size(size) {}
  /**
   * @brief デストラクタ
   */
  ~Entity() {}
};

/**
 * @brief アーカイブからデータを読み取るクラス
 */
class ArchiveLoader {
 public:
  /**
   * @brief コンストラクタ
   */
  ArchiveLoader();
  /**
   * @brief デストラクタ
   */
  ~ArchiveLoader();
  /**
   * @brief 初期化
   * @param filepath アーカイブファイルへのパス
   */
  bool Init(const std::filesystem::path& filepath);
  /**
   * @brief アーカイブの各項目を読み取り、バイナリデータとして返す
   * @param filename 項目名となっているファイル名(assetsからの相対パス)
   */
  std::vector<u8> Load(const std::filesystem::path& filename);
  /**
   * @brief 終了処理
   */
  void Finalize();

 private:
  //! 読み込んだファイルマップ
  //! 読み込み時にpathを受け取るが、pathは同じ表現をするものでもhash値が一致しないため、wstringに変換して保存する
  std::unordered_map<std::wstring, Entity> load_files_;
  //! アーカイブのファイルストリーム
  std::ifstream ifs_;
};

}  // namespace loader
}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_LOADER_ARCHIVE_LOADER_H
