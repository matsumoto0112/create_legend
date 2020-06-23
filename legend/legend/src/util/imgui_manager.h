#ifndef LEGEND_UTIL_IMGUI_MANAGER_H_
#define LEGEND_UTIL_IMGUI_MANAGER_H_

/**
 * @file imgui_manager.h
 * @brief ImGUI�Ǘ��N���X��`
 */

namespace legend {
namespace util {
/**
 * @brief Imgui�Ǘ�
 */
class ImguiManager {
 public:
  /**
   * @brief �R���X�g���N�^
   */
  ImguiManager();
  /**
   * @brief �f�X�g���N�^
   */
  ~ImguiManager();
  /**
   * @brief ������
   * @param window_handle �E�B���h�E�n���h��
   * @param device DirectX12�f�o�C�X
   * @param back_buffer_format �o�b�N�o�b�t�@�̃t�H�[�}�b�g
   * @param frame_count �o�b�N�o�b�t�@�̖���
   * @param enable_gui GUI��L���ɂ��邩�ǂ���
   * @return �������ɐ���������true��Ԃ�
   */
  bool Init(HWND__* window_handle, ID3D12Device* device,
            DXGI_FORMAT back_buffer_format, u32 frame_count,
            bool enable_gui = true);
  /**
   * @brief GUI���\�ɂ��邩�ǂ����Z�b�g����
   * @param enable true�Ȃ�L���ɂ���
   */
  void SetEnableGui(bool enable) { enable_gui_ = enable; }
  /**
   * @brief �t���[���J�n��
   */
  void BeginFrame();
  /**
   * @brief �t���[���I����
   * @param command_list �R�}���h���X�g
   */
  void EndFrame(ID3D12GraphicsCommandList* command_list);

 private:
  //! ImGui�p�q�[�v
  ComPtr<ID3D12DescriptorHeap> heap_;
  //! gui���L�����ǂ���
  bool enable_gui_;
};

}  // namespace util
}  // namespace legend
#endif  //! LEGEND_UTIL_IMGUI_MANAGER_H_
