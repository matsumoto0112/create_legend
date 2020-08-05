#include "StageGenerator.h"

#include "src\\stdafx.h"

namespace legend {
namespace stage_generate {

//コンストラクタ
StageGenerator::StageGenerator() {}

//デストラクタ
StageGenerator::~StageGenerator() {}

//ファイルの読み込み処理
bool StageGenerator::LoadStageData(std::wstring filepath) {
  //ファイルパスからバイナリファイルを読み込む
  std::fstream fstream;
  fstream.open(filepath, std::ios::binary | std::ios::out);
  
  while (!fstream.eof()) {
      char* text;
      fstream.read((char*)&text, sizeof(char*));
  }

  return true;
}

}  // namespace stage_generate
}  // namespace legend