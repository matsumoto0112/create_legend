#ifndef LEGEND_MATH_RECT_H_
#define LEGEND_MATH_RECT_H_

/**
 * @file rect.h
 */

namespace legend {
namespace math {
struct Rect {
 public:
  Rect() : left(0.0f), top(0.0f), width(0.0f), height(0.0f) {}
  Rect(float left, float top, float width, float height)
      : left(left), top(top), width(width), height(height) {}
  ~Rect() {}
  inline float GetLeft() const { return left; }
  inline void SetLeft(float left) { this->left = left; }
  inline float GetTop() const { return top; }
  inline void SetTop(float top) { this->top = top; }
  inline float GetWidth() const { return width; }
  inline void SetWidth(float width) { this->width = width; }
  inline float GetHeight() const { return height; }
  inline void SetHeight(float height) { this->height = height; }

 private:
  float left;
  float top;
  float width;
  float height;
};

}  // namespace math
}  // namespace legend
#endif  //! LEGEND_MATH_RECT_H_
