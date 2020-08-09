#ifndef LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_
#define LEGEND_STAGE_GENERATE_STAGE_GENERATOR_H_

namespace legend {
namespace stage_generate {

/**
 * @file stage_generator.h
 * @brief ステージデータを読み込み生成するクラス
 */

class StageGenerator {
 public:
  /**
   * @brief コンストラクタ
   */
  StageGenerator();
  /**
   * @brief デストラクタ
   */
  ~StageGenerator();
  /**
   * @brief 読み込み処理
   * @return 処理が正しく終了したらtrueを返す
   */
  std::vector<std::string> LoadStageData(std::wstring filepath);

 private:
};
}  // namespace stage_generate
}  // namespace legend

#endif  //! LEGEND_STAGE_GENERATE_STAGEGENERATOR_H_
