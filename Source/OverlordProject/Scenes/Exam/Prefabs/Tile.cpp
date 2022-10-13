#include "stdafx.h"
#include "Tile.h"

#include "BomberMan.h"
#include "ColorMaterial.h"
#include "Shadow/DiffuseMaterial_Shadow.h"
#include "../Materials/Shadow/ToonTexMaterial_Shadow.h"
#include "../Components/GridComponent.h"

//Static Variables
uint16_t Tile::m_Size{ 20 };
//GameObject* Tile::m_pModelObject{ nullptr };

Tile::Tile(int32_t c, int32_t r, int light)
	: GridBasedObject{ {c,r}, 0.f }
	, m_pSlot{ nullptr }
{
	DiffuseMaterial_Shadow* const pColorMaterial = MaterialManager::Get()->CreateMaterial<DiffuseMaterial_Shadow>();
	//pColorMaterial->SetColor(m_Colour);
	pColorMaterial->SetDiffuseTexture(L"Textures/Tile_" + std::to_wstring(light + 1) + L".png");
	ModelComponent* const pModel = new ModelComponent{ L"Meshes/Tile.ovm", false };
	pModel->SetMaterial(pColorMaterial);
	AddComponent(pModel);
	GetTransform()->Scale(0.5f);
}


bool Tile::GetOccupied() const
{
	if (!m_pSlot)
		return false;
	else if (typeid(*m_pSlot) == typeid(BomberMan))
		return false;
	else
		return true;
}

void Tile::Initialize(const SceneContext&)
{
}
