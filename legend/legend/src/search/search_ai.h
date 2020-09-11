#ifndef LEGEND_SEARCH_SEARCH_AI_H_
#define LEGEND_SEARCH_SEARCH_AI_H_

#include <vector>
#include "src/bullet/physics_field.h"
#include "src/math/vector_3.h"
#include "src/game/game_device.h"
#include "src/primitive/line.h"
#include "src/primitive/box.h"

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
   * @param _position ���W
   */
  SearchAI(math::Vector3 _position);
  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchAI();

  /**
   * @brief ���W�擾
   * @return ���W�擾
   */
  math::Vector3 GetPosition();
  /**
   * @brief ���򃊃X�g�擾
   * @return ���򃊃X�g�擾
   */
  std::vector<SearchAI*> GetBranch();
  /**
   * @brief ����ݒ�
   * @param _branch �����ݒ�
   * @return ����ݒ�
   */
  std::vector<SearchAI*> SetBranch(std::vector<SearchAI*> _branch);
  /**
   * @brief ����ǉ�
   * @param _branch �����ǉ�
   * @return �ǉ��㕪�򃊃X�g
   */
  std::vector<SearchAI*> AddBranch(SearchAI* _branch);
  /**
   * @brief ����悩�烉���_���Ŏ擾
   * @param remove �����ΏۊO���X�g
   * @return ����悩�烉���_���Ŏ擾
   */
  SearchAI* GetRandomSearch(std::vector<SearchAI*> remove);
  /**
   * @brief �f�o�b�O�`��
   * @param physics_field_ �������Z�N���X
   */
  void DebugDraw(bullet::PhysicsField* physics_field_);
 private:
  //! �����
  std::vector<SearchAI*> branch_;
  //! �f�o�b�O�`��p���C��
  std::vector<std::unique_ptr<primitive::Line>> lines_;
  //! �f�o�b�O�`��p�{�b�N�X
  std::unique_ptr<primitive::Box> box_;
  //! ���W
  math::Vector3 position_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_AI_H_