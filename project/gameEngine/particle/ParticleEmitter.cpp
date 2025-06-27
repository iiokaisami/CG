#include "ParticleEmitter.h"

void ParticleEmitter::Emit(const std::string& groupName, const Vector3& position, uint32_t count, uint32_t interval)
{
	if (auto manager = ParticleManager::GetInstance())
	{
		manager->Emit(groupName, position, count, interval);
	}
}

void ParticleEmitter::StartLoop(const std::string& groupName, const std::string& motionName, const Vector3& position, uint32_t count, float interval)
{
	if (auto manager = ParticleManager::GetInstance())
	{
		EmitSetting setting;
		setting.groupName = groupName;
		setting.motionName = motionName;
		setting.emitPosition = position;
		setting.interval = interval;
		setting.emitCount = count;
		setting.isLooping = true;
		manager->AddEmitterSetting(setting);
	}
}

void ParticleEmitter::EmitOnce(const Vector3& position, uint32_t count, float interval)
{
	if (manager_) 
	{
		manager_->Emit(groupName_, position, count, static_cast<uint32_t>(interval));
	}
}

void ParticleEmitter::StartLoopEmit(const Vector3& position, uint32_t count, float interval)
{
	if (!manager_)
	{
		return;
	}

	EmitSetting setting;
	setting.groupName = groupName_;
	setting.motionName = motionName_;
	setting.emitPosition = position;
	setting.interval = interval;
	setting.emitCount = count;
	setting.isLooping = true;

	manager_->AddEmitterSetting(setting);
}
