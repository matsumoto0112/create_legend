#ifndef LEGEND_BULLET_BOUNDING_SPHERE_H_
#define LEGEND_BULLET_BOUNDING_SPHERE_H_


#include "src/bullet/collider.h"
/**
 * @file bounding_box.h
 * @brief 立方体の物理判定クラス
 */
namespace legend {
    namespace bullet {

        class BoundingSphere : public Collider {
        public:
            /**
             * @brief 初期化パラメータ
             */
            struct InitializeParameter {
                btVector3 position;
                btQuaternion rotation;
                btScalar radius;
                btScalar mass;
                btScalar restitution;
            };

        public:
            BoundingSphere(const InitializeParameter& parameter);
            ~BoundingSphere();
            bool Update() override;
        };
    }  // namespace bullet
}  // namespace legend


#endif //! LEGEND_BULLET_BOUNDING_SPHERE_H_
