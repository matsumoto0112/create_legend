#include "StageGenerator.h"

#include "src\\stdafx.h"

namespace legend {
namespace stage_generate {

//�R���X�g���N�^
StageGenerator::StageGenerator() {}

//�f�X�g���N�^
StageGenerator::~StageGenerator() {}

//�t�@�C���̓ǂݍ��ݏ���
bool StageGenerator::LoadStageData(std::wstring filepath) {
  //�t�@�C���p�X����o�C�i���t�@�C����ǂݍ���
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