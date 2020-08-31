#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H
#define LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H

/**
 * @file particle_factory.h
 * @brief
 */

#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {
namespace particle_factory {
std::shared_ptr<draw::particle::ParticleEmitter> CreatePlayerMoveParticle();
std::shared_ptr<draw::particle::ParticleEmitter> CreateEnemyMoveParticle();

std::shared_ptr<draw::particle::ParticleEmitter> CreateFireParticle();

}  // namespace particle_factory
}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H
