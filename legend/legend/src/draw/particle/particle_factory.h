#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H
#define LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H

/**
 * @file particle_factory.h
 * @brief パーティクル生成関数定義
 */

#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {
namespace particle_factory {
/**
 * @brief プレイヤーの移動時に発生するパーティクルを作成する
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreatePlayerMoveParticle();
/**
 * @brief 敵の移動時に発生するパーティクルを作成する
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreateEnemyMoveParticle();
/**
 * @brief 鉛筆の爆発時に発生するパーティクルを作成する
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreateExplosionParticle();

/**
 * @brief オブジェクト衝突時に発生するパーティクルを作成する
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreateFireParticle();

}  // namespace particle_factory
}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H
