#pragma once
#include "Components/GridComponent.h"
#include "Prefabs/Bomb.h"
#include "Prefabs/BomberMan.h"

template<typename T, typename = std::enable_if<std::is_base_of_v<GridBasedObject, T>, T>>
class Pool
{
public:
	Pool(GameScene* const pScene, uint32_t size)
	{
		m_pPool.reserve(size);
		InitializePool(pScene, size);
	}
	~Pool() = default;

	T* const ActivateElement(const XMFLOAT3& pos)
	{
		if(!m_pFirstAvailable)
		{
#ifdef _DEBUG
			Logger::LogWarning(L"Pool fully occupied!");
#endif
			return nullptr;
		}

		T* const pTemp = m_pFirstAvailable;
		m_pFirstAvailable = pTemp->GetNext();
		pTemp->GetTransform()->Translate(pos);
		pTemp->Activate(true);
		pTemp->SetNext(nullptr);
		return pTemp;
	}
	void DeactivateElement(T* pElement)
	{
		pElement->Deactivate(true);
		pElement->SetNext(m_pFirstAvailable);
		m_pFirstAvailable = pElement;
	}

private:
	std::vector<T*> m_pPool;
	T* m_pFirstAvailable;

	void InitializePool(GameScene* const pScene, uint32_t size)
	{
		for(uint32_t i { 0 }; i < size; ++i)
		{
			m_pPool.push_back(pScene->AddChild(new T{ {-3, -3}, 0 }));
			m_pPool.back()->Deactivate(true);
		}
		m_pFirstAvailable = m_pPool[0];

		for(uint32_t i{ 0 }; i < size - 1; ++i)
		{
			m_pPool[i]->SetNext(m_pPool[i + 1]);
		}

		m_pPool[size - 1]->SetNext(nullptr);
	}
};
