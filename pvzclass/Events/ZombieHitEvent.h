#pragma once
#include "BaseEvent.h"

// ��ʬ�����¼�
// �����������¼��Ľ�ʬ���˺����ͣ��˺���ֵ
// ����ֵ�����º���˺�ֵ
// ����¼�֮���˺��ᴮ���޸ģ���������˺�20
// ��һ��������������40���ڶ����¼����������˺���ֵ����40
// �粻�������޸ģ���ʬ�����ܵ�40���˺�
class ZombieHitEvent : public BaseEvent<std::function<
	int(std::shared_ptr<PVZ::Zombie>, DamageType::DamageType, int)>>
{
public:
	ZombieHitEvent();
	bool handle(EventHandler handler);
};

ZombieHitEvent::ZombieHitEvent()
{
	address = 0x5317C0;
}

bool ZombieHitEvent::handle(EventHandler handler)
{
	if (handler.context.Eip != address) return false;
	auto zombie = std::make_shared<PVZ::Zombie>(handler.context.Esi);
	DamageType::DamageType type = (DamageType::DamageType)(handler.context.Eax);
	int amount = PVZ::Memory::ReadMemory<DWORD>(handler.context.Esp + 4);
	for (int i = 0; i < listeners.size(); i++)
	{
		amount = listeners[i](zombie, type, amount);
	}
	PVZ::Memory::WriteMemory<DWORD>(handler.context.Esp + 4, amount);
	afterHandle(handler);
	return true;
}