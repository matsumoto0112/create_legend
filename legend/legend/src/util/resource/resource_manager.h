#ifndef LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_
#define LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_

/**
 * @file resource_manager.h
 * @brief ���\�[�X�Ǘ��V�X�e�����N���X��`
 */

#include "src/util/stl_extend.h"

namespace legend {
namespace util {
namespace resource {

/**
 * @class ResourceManager
 * @brief ���\�[�X�Ǘ��V�X�e�����N���X
 * @tparam TKey ���\�[�X����肷��L�[
 * @tparam TResource �Ǘ����郊�\�[�X
 */
template <typename TKey, typename TResource>
class ResourceManager {
 public:
  /**
   * @brief �f�X�g���N�^
   */
  virtual ~ResourceManager() = default;
  /**
   * @brief ������
   * @return �������ɐ���������true��Ԃ�
   */
  virtual bool Init();
  /**
   * @brief ���\�[�X��ǂݍ���
   * @param key ���\�[�X����肷��L�[
   * @param filepath �t�@�C���ւ̃p�X
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  virtual bool Load(TKey key, const std::filesystem::path& filepath) {
    return false;
  }
  /**
   * @brief ���\�[�X��o�^����
   * @param key ���\�[�X����肷��L�[
   * @param resource ���\�[�X
   * @return �o�^�ɐ���������true��Ԃ�
   */
  virtual bool Register(TKey key, TResource resource);
  /**
   * @brief ���\�[�X��j������
   * @param key ���\�[�X����肷��L�[
   * @return �j���ɐ���������true��Ԃ�
   */
  virtual bool Unload(TKey key);
  /**
   * @brief ���łɓǂݍ���ł��邩�ǂ���
   * @param key ���\�[�X����肷��L�[
   * @return �ǂݍ���ł�����true��Ԃ�
   */
  virtual bool IsLoaded(TKey key) const { return util::Exist(resources_, key); }
  /**
   * @brief ���\�[�X���擾����
   * @param key ���\�[�X����肷��L�[
   */
  virtual TResource Get(TKey key) const {
    MY_ASSERTION(IsLoaded(key), L"���ǂݍ��݂̃��\�[�X�L�[���I������܂����B");
    return resources_.at(key);
  }

 protected:
  //! �ǂݍ��񂾃��\�[�X�}�b�v
  std::unordered_map<TKey, TResource> resources_;
};

//������
template <typename TKey, typename TResource>
inline bool ResourceManager<TKey, TResource>::Init() {
  resources_.clear();
  return true;
}

//���\�[�X��o�^����
template <typename TKey, typename TResource>
inline bool ResourceManager<TKey, TResource>::Register(TKey key,
                                                       TResource resource) {
  MY_ASSERTION(!IsLoaded(key), L"�o�^�ς݂̃L�[���đI������܂����B");
  resources_.emplace(key, resource);
  return true;
}

//�j��
template <typename TKey, typename TResource>
inline bool ResourceManager<TKey, TResource>::Unload(TKey key) {
  MY_ASSERTION(IsLoaded(key), L"���o�^�̃L�[���폜����悤�Ƃ��Ă��܂��B");
  return resources_.erase(key) == 1;
}

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_