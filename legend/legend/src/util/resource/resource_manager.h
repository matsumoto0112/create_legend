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
 * @tparam TResource �Ǘ����郊�\�[�X
 */
template <typename TResource>
class ResourceManager {
 protected:
  using ResourceMap = std::unordered_map<std::wstring, TResource>;

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
   * @brief ���\�[�X�����[�h����
   * @param name ���\�[�X��
   * @return �ǂݍ��݂ɐ���������true��Ԃ�
   */
  virtual bool Load(const std::wstring& name);
  /**
   * @brief ���\�[�X��o�^����
   * @param name ���\�[�X��
   * @param resource �o�^���郊�\�[�X
   */
  virtual bool Register(const std::wstring& name, TResource resource);
  /**
   * @brief ���\�[�X���擾����
   * @param name ���\�[�X��
   */
  virtual TResource Get(const std::wstring& name);
  /**
   * @brief ���łɓǂݍ���ł��邩�ǂ���
   * @param name ���\�[�X��
   * @return �ǂݍ���ł�����true��Ԃ�
   */
  virtual bool IsLoaded(const std::wstring& name) const;

 protected:
  //! �ǂݍ��񂾃��\�[�X�}�b�v
  ResourceMap resources_;
};

template <typename TResource>
inline bool ResourceManager<TResource>::Init() {
  return true;
}

template <typename TResource>
inline bool ResourceManager<TResource>::Load(const std::wstring& name) {
  return false;
}

template <typename TResource>
inline bool ResourceManager<TResource>::Register(const std::wstring& name,
                                                 TResource resource) {
  if (IsLoaded(name)) {
    MY_LOG(L"�o�^�ς݂̃��\�[�X���㏑�����悤�Ƃ��܂����B");
    resources_[name] = resource;
    return true;
  }
  resources_.emplace(name, resource);
  return true;
}

template <typename TResource>
inline TResource ResourceManager<TResource>::Get(const std::wstring& name) {
  //�ǂݍ��܂�Ă����炻�̂܂ܕԂ�
  if (IsLoaded(name)) {
    return resources_.at(name);
  }

  //�ǂݍ��܂�Ă��Ȃ���Γǂݍ���
  if (Load(name)) {
    return resources_.at(name);
  }
  return TResource();
}

template <typename TResource>
inline bool ResourceManager<TResource>::IsLoaded(
    const std::wstring& name) const {
  return util::Exist(resources_, name);
}

}  // namespace resource
}  // namespace util
}  // namespace legend

#endif  //! LEGEND_UTIL_RESOURCE_RESOURCE_MANAGER_H_