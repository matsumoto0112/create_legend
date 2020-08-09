#include "stage_generator.h"

#include "src\\stdafx.h"

namespace legend {
namespace stage_generate {

//コンストラクタ
StageGenerator::StageGenerator() {}

//デストラクタ
StageGenerator::~StageGenerator() {}

//ファイルの読み込み処理
std::vector<std::string> StageGenerator::LoadStageData(std::wstring filepath) {
  ////ファイルパスからバイナリファイルを読み込む
  // std::fstream fstream;
  // fstream.open(filepath, std::ios::binary | std::ios::out);
  //
  // while (!fstream.eof()) {
  //    char* text;
  //    fstream.read((char*)&text, sizeof(char*));
  //}

  //ファイルパスからテキストデータを読み込み
  std::ifstream ifstream(filepath);
  std::vector<std::string> indexs;
  if (ifstream.fail()) {
    MY_LOG(L"ステージデータを開けませんでした。");
    indexs[0] = "error";
    return indexs;
  }
  std::string index;
  i32 counter = 0;
  while (std::getline(ifstream, index)) {
    indexs.push_back(index);
  }

  return indexs;
}

}  // namespace stage_generate
}  // namespace legend