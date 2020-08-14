#ifndef LEGEND_SEARCH_SEARCH_AI_H_
#define LEGEND_SEARCH_SEARCH_AI_H_

#include <vector>
#include "src/math/vector_3.h"

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
   * @brief ����悩�烉���_���Ŏ擾
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);
 private:
  std::vector<SearchAI*> branch_;
  math::Vector3 position_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_AI_H_