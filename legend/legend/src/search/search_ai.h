#ifndef LEGEND_SEARCH_SEARCH_AI_H_
#define LEGEND_SEARCH_SEARCH_AI_H_

#include <vector>
#include "src/math/vector_3.h"
#include "src/game/game_device.h"
#include "src/primitive/line.h"

namespace legend {
namespace search {

/**
 * @class SearchAI
 * @brief �����ێ��N���X
 */
class SearchAI {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SearchAI(math::Vector3 _position);
  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchAI();

  /**
   * @brief ���W�擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief ����擾
   */
  std::vector<SearchAI*> GetBranch();
  /**
   * @brief ����ݒ�
   */
  std::vector<SearchAI*> SetBranch(std::vector<SearchAI*> _branch);
  /**
   * @brief ����ǉ�
   */
  std::vector<SearchAI*> AddBranch(SearchAI* _branch);
  /**
   * @brief ����悩�烉���_���Ŏ擾
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);

  void DebugDraw(directx::device::CommandList& command_list);
 private:
  std::vector<SearchAI*> branch_;
  std::vector<std::unique_ptr<primitive::Line>> lines;
  math::Vector3 position_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_AI_H_