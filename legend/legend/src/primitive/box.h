#ifndef LEGEND_PRIMITIVE_BOX_H_
#define LEGEND_PRIMITIVE_BOX_H_

/**
 * @file box.h
 */

#include "src/primitive/primitive.h"

namespace legend {
namespace primitive {
class Box : public PrimitiveBase {
 public:
  Box();
  ~Box();

  void SetTransform(const util::Transform& transform) {
    this->transform_ = transform;
  }
  bool Init() override;

 private:
};

}  // namespace primitive
}  // namespace legend
#endif  //! LEGEND_PRIMITIVE_BOX_H_
