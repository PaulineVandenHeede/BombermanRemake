#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	//TODO_W9(L"Implement Destructor")
	SafeDelete(m_ParticlesArray);
	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Initialize")
	//STATIC VAR -> create instance of particle material
	if(m_pParticleMaterial == nullptr)
	{
		m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();
	}

	CreateVertexBuffer(sceneContext);

	//Load particle texture
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement CreateVertexBuffer")
	//If exists -> safe release
	SafeRelease(m_pVertexBuffer);

	//create dynamic vertex buffer
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = m_ParticleCount * sizeof(VertexParticle);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;

	//create buffer
	HANDLE_ERROR(sceneContext.d3dContext.pDevice->CreateBuffer(&desc, nullptr, &m_pVertexBuffer));
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement Update")
	//average particle emit threshold
	const float deltaTime = sceneContext.pGameTime->GetElapsed();
	const float particleInterval = ((m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / 2.f) / m_MaxParticles;

	m_LastParticleSpawn += deltaTime;

	//validate particles
	m_ActiveParticles = 0;
	D3D11_MAPPED_SUBRESOURCE subResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	VertexParticle* pBuffer = reinterpret_cast<VertexParticle*>(subResource.pData);

	for(UINT i{ 0 }; i < m_ParticleCount; ++i)
	{
		UpdateParticle(m_ParticlesArray[i], deltaTime);

		if(!m_ParticlesArray[i].isActive && m_LastParticleSpawn > particleInterval)
		{
			SpawnParticle(m_ParticlesArray[i]);
		}

		if(m_ParticlesArray[i].isActive)
		{
			pBuffer[m_ActiveParticles++] = m_ParticlesArray[i].vertexInfo;
		}
	}

	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	//TODO_W9(L"Implement UpdateParticle")

	if (!p.isActive)
		return;

	p.currentEnergy -= elapsedTime;
	if(p.currentEnergy < 0.f)
	{
		//deactivate particle
		p.isActive = false;
		return;
	}

	//update vertex
	VertexParticle& vertex = p.vertexInfo;
	const float lifePercent = p.currentEnergy / p.totalEnergy;

	//position
	XMVECTOR vel = DirectX::XMLoadFloat3(&m_EmitterSettings.velocity);
	vel = DirectX::XMVectorScale(vel, elapsedTime);
	XMVECTOR pos = DirectX::XMLoadFloat3(&vertex.Position);
	pos = DirectX::XMVectorAdd(pos, vel);
	DirectX::XMStoreFloat3(&vertex.Position, pos);

	//colour
	vertex.Color = m_EmitterSettings.color;
	vertex.Color.w *= lifePercent;

	//size
	float startSize = p.initialSize;
	float endSize = p.initialSize * p.sizeChange;
	//float sizeMultiplier = p.sizeChange < 1.f ? 1.f - (1.f - p.sizeChange) * (1.f - lifePercent) : 1.f + (p.sizeChange - 1.f) * (1.f - lifePercent);
	//vertex.Size = sizeMultiplier * p.initialSize;
	vertex.Size = std::lerp(startSize, endSize, (1.f - lifePercent));
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	//TODO_W9(L"Implement SpawnParticle")
	p.isActive = true;

	//energy initialisation
	p.totalEnergy = p.currentEnergy =MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);

	//position initialisation
	//random direction
	XMVECTOR randomDirection{ 1.f, 0.f, 0.f };
	XMMATRIX randomRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI), MathHelper::randF(-XM_PI, XM_PI));
	randomDirection = DirectX::XMVector3TransformNormal(randomDirection, randomRotationMatrix);
	randomDirection = DirectX::XMVectorScale(randomDirection, MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius));
	XMVECTOR pos = DirectX::XMVectorAdd(randomDirection, XMLoadFloat3(&GetGameObject()->GetTransform()->GetPosition()));
	DirectX::XMStoreFloat3(&p.vertexInfo.Position, pos);

	//size initialisation
	p.vertexInfo.Size = p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	//rotation intialisation
	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	//colour initialisation
	p.vertexInfo.Color = m_EmitterSettings.color;
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	//TODO_W9(L"Implement PostDraw")
	//set shader variables
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture);

	const MaterialTechniqueContext& technique = m_pParticleMaterial->GetTechniqueContext();
	ID3D11DeviceContext* const pDeviceContext = sceneContext.d3dContext.pDeviceContext;
	pDeviceContext->IASetInputLayout(technique.pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	UINT stride = sizeof(VertexParticle);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	technique.pTechnique->GetDesc(&techDesc);
	for (unsigned int i = 0; i < techDesc.Passes; ++i)
	{
		technique.pTechnique->GetPassByIndex(i)->Apply(0, pDeviceContext);
		pDeviceContext->Draw(m_ActiveParticles, 0);
	}
}

void ParticleEmitterComponent::DrawImGui()
{
	if(ImGui::CollapsingHeader("Particle System"))
	{
		ImGui::SliderUInt("Count", &m_ParticleCount, 0, m_MaxParticles);
		ImGui::InputFloatRange("Energy Bounds", &m_EmitterSettings.minEnergy, &m_EmitterSettings.maxEnergy);
		ImGui::InputFloatRange("Size Bounds", &m_EmitterSettings.minSize, &m_EmitterSettings.maxSize);
		ImGui::InputFloatRange("Scale Bounds", &m_EmitterSettings.minScale, &m_EmitterSettings.maxScale);
		ImGui::InputFloatRange("Radius Bounds", &m_EmitterSettings.minEmitterRadius, &m_EmitterSettings.maxEmitterRadius);
		ImGui::InputFloat3("Velocity", &m_EmitterSettings.velocity.x);
		ImGui::ColorEdit4("Color", &m_EmitterSettings.color.x, ImGuiColorEditFlags_NoInputs);
	}
}