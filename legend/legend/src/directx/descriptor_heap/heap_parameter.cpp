#include "src/directx/descriptor_heap/heap_parameter.h"

#include "src/util/stl_extend.h"

namespace legend {
namespace directx {
namespace descriptor_heap {
namespace heap_parameter {
namespace local {

u32 GetDefinedLocalDescriptorNum(local_heap_id::Enum id) {
  static const std::unordered_map<local_heap_id::Enum, u32>
      LOCAL_DESCRIPTOR_NUM = {
          {local_heap_id::STATIC, 20000},
          {local_heap_id::MULTI_RENDER_TARGET_TEST_SCENE, 100},
      };

  MY_ASSERTION(util::Exist(LOCAL_DESCRIPTOR_NUM, id),
               L"ñ¢íËã`ÇÃIDÇ™ëIëÇ≥ÇÍÇ‹ÇµÇΩÅB");
  return LOCAL_DESCRIPTOR_NUM.at(id);
}

}  // namespace local
}  // namespace heap_parameter
}  // namespace descriptor_heap
}  // namespace directx
}  // namespace legend