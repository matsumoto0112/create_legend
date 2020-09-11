#ifndef LEGEND_SEARCH_SEARCH_MANAGER_H_
#define LEGEND_SEARCH_SEARCH_MANAGER_H_

#include "src/bullet/physics_field.h"
#include "src/player/player.h"
#include "src/search/search_course.h"

namespace legend {
namespace search {

/**
 * @class SearchManager
 * @brief �T���Ǘ��N���X
 */
class SearchManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  SearchManager();
  /**
   * @brief �f�X�N�g���N�^
   */
  ~SearchManager();

  /**
   * @brief �����ݒ�
   * @param mediator ����҂�ǉ�
   */
  void Initialize(actor::IActorMediator* mediator);

  /**
   * @brief ����
   * @param filepath �t�@�C���p�X�����
   */
  void Make(std::filesystem::path filepath);
  /**
   * @brief �e�L�X�g�̓ǂݍ��ݏ���
   * @param filepath �t�@�C���p�X�����
   * @return �������������I��������ǂݍ��܂ꂽ���e(string)��Ԃ�
   */
  std::vector<std::string> LoadStringStageData(std::filesystem::path filepath);
  /**
   * @brief String��String���X�g�֕ϊ�
   * @param string ����String
   * @param border �܂�Ԃ��ӏ�
   * @return �ϊ����ꂽString�̃��X�g
   */
  std::vector<std::string> StringSplit(const std::string& string, char border);

  /**
   * @brief ���W���番��_�ǉ�
   * @param _position ���W
   */
  void Add(math::Vector3 _position);
  /**
   * @brief ���W���X�g���番��_�ǉ�
   * @param _positions ���W
   */
  void Add(std::vector<math::Vector3> _positions);
  /**
   * @brief ���W���番��_�ǉ�
   * @param index ��ԍ�
   * @param branch �ǉ����镪���̗�ԍ�
   */
  void SetBranch(i32 index, std::vector<i32> branch);

  /**
   * @brief ���̌o�H���W
   * @param _enemy ���݉ғ�������G�̃R���C�_
   * @param _enemys �S�̂̓G�̃R���C�_
   * @return ���̈ړ���擾
   */
  math::Vector3 NextSearch(bullet::Collider* _enemy,
                           std::vector<bullet::Collider*> _enemys);
  /**
   * @brief �o�H�T��
   * @param sStart �J�n�n�_
   * @param sEnd �I���n�_
   */
  void SetCourse(SearchAI* sStart, SearchAI* sEnd);
  /**
   * @brief �f�o�b�O�`��
   * @param physics_field_ �������Z�N���X
   */
  void DebugDraw(bullet::PhysicsField* physics_field_);

 private:
  /**
   * @brief �ǐՌo�H
   */
  void ChaseCourse();
  /**
   * @brief ���W����ߏ�̒T���ӏ����擾
   * @param _position ���W
   * @return ���W����ߏ�̒T���ӏ����擾
   */
  SearchAI* NearSearch(math::Vector3 _position);
  /**
   * @brief ���_�̃|�C���^���擾
   * @param _search �T�����̌o�H�����擾
   * @return ���_�̃|�C���^���擾
   */
  SearchAI* GetBaseSearch(SearchAI* _search);
  /**
   * @brief �Փˏ����擾
   * @param start �n�_
   * @param direction ����
   * @return �Փ˂��Ă��邩����
   */
  bool OnCollision(math::Vector3 start, math::Vector3 direction);

 private:
  //! �T���ӏ����X�g
  std::vector<std::unique_ptr<SearchAI>> search_list_;
  //! �o�H
  std::vector<SearchAI*> course_list_;

  //! �S�̂̓G�ꗗ
  std::vector<bullet::Collider*> enemys_;
  //! ���s���̓G
  bullet::Collider* ignore_enemy_;

  //! �����
  actor::IActorMediator* mediator_;
};
}  // namespace search
}  // namespace legend

#endif  //! LEGEND_SEARCH_SEARCH_MANAGER_H_