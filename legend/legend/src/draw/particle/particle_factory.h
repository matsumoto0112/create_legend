#ifndef LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H
#define LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H

/**
 * @file particle_factory.h
 * @brief �p�[�e�B�N�������֐���`
 */

#include "src/draw/particle/particle_emitter.h"

namespace legend {
namespace draw {
namespace particle {
namespace particle_factory {
/**
 * @brief �v���C���[�̈ړ����ɔ�������p�[�e�B�N�����쐬����
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreatePlayerMoveParticle();
/**
 * @brief �G�̈ړ����ɔ�������p�[�e�B�N�����쐬����
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreateEnemyMoveParticle();
/**
 * @brief ���M�̔������ɔ�������p�[�e�B�N�����쐬����
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreateExplosionParticle();

/**
 * @brief �I�u�W�F�N�g�Փˎ��ɔ�������p�[�e�B�N�����쐬����
 */
std::shared_ptr<draw::particle::ParticleEmitter> CreateFireParticle();

}  // namespace particle_factory
}  // namespace particle
}  // namespace draw
}  // namespace legend
#endif  //! LEGEND_DRAW_PARTICLE_PARTICLE_FACTORY_H
