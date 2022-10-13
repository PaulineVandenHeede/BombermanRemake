#include "stdafx.h"
#include "ModelAnimator.h"

ModelAnimator::ModelAnimator(MeshFilter* pMeshFilter):
	m_pMeshFilter{pMeshFilter}
{
	SetAnimation(0);
}

void ModelAnimator::Update(const SceneContext& sceneContext)
{
	//TODO_W7_();
	float deltaTime = sceneContext.pGameTime->GetElapsed();

	//We only update the transforms if the animation is running and the clip is set
	if (m_IsPlaying && m_ClipSet)
	{
		//1. 
		//Calculate the passedTicks (see the lab document)
		//auto passedTicks = ...
		//Make sure that the passedTicks stay between the m_CurrentClip.Duration bounds (fmod)
		float passedTicks = deltaTime * m_CurrentClip.ticksPerSecond * m_AnimationSpeed;
		passedTicks = std::fmod(passedTicks, m_CurrentClip.duration);

		//2. 
		//IF m_Reversed is true
		//	Subtract passedTicks from m_TickCount
		//	If m_TickCount is smaller than zero, add m_CurrentClip.Duration to m_TickCount
		//ELSE
		//	Add passedTicks to m_TickCount
		//	if m_TickCount is bigger than the clip duration, subtract the duration from m_TickCount
		if (m_Reversed)
		{
			m_TickCount -= passedTicks;
			if (m_TickCount < 0.f)
			{
				m_TickCount += m_CurrentClip.duration;
			}
		}
		else
		{
			m_TickCount += passedTicks;
			if (m_TickCount > m_CurrentClip.duration)
			{
				m_TickCount -= m_CurrentClip.duration;
			}
		}

		//3.
		//Find the enclosing keys
		//Iterate all the keys of the clip and find the following keys:
		//keyA > Closest Key with Tick before/smaller than m_TickCount
		//keyB > Closest Key with Tick after/bigger than m_TickCount
		std::vector<AnimationKey>::const_iterator it = std::find_if(m_CurrentClip.keys.cbegin(), m_CurrentClip.keys.cend(),
			[this](const AnimationKey& key) {
				return !(key.tick < m_TickCount);
			});
		const AnimationKey& keyB = *it;
		const AnimationKey& keyA = * (it - 1);


		//4.
		//Interpolate between keys
		//Figure out the BlendFactor (See lab document)
		//Clear the m_Transforms vector
		//FOR every boneTransform in a key (So for every bone)
		//	Retrieve the transform from keyA (transformA)
		//	auto transformA = ...
		// 	Retrieve the transform from keyB (transformB)
		//	auto transformB = ...
		//	Decompose both transforms
		//	Lerp between all the transformations (Position, Scale, Rotation)
		//	Compose a transformation matrix with the lerp-results
		//	Add the resulting matrix to the m_Transforms vector

		const float blendFactor = (m_TickCount - keyA.tick) / m_CurrentClip.duration;
		m_Transforms.clear();
		uint32_t boneCount = uint32_t(std::min(keyA.boneTransforms.size(), keyB.boneTransforms.size()));
		for (uint32_t i{ 0 }; i < boneCount; ++i)
		{
			XMMATRIX boneTransformA = XMLoadFloat4x4(&keyA.boneTransforms[i]);
			XMMATRIX boneTransformB = XMLoadFloat4x4(&keyB.boneTransforms[i]);

			XMVECTOR translationA, scaleA, rotationA;
			XMVECTOR translationB, scaleB, rotationB;
			XMVECTOR translationTot, scaleTot, rotationTot;
			XMMatrixDecompose(&scaleA, &rotationA, &translationA, boneTransformA);
			XMMatrixDecompose(&scaleB, &rotationB, &translationB, boneTransformB);

			scaleTot = XMVectorLerp(scaleA, scaleB, blendFactor);
			rotationTot = XMQuaternionSlerp(rotationA, rotationB, blendFactor);
			translationTot = XMVectorLerp(translationA, translationB, blendFactor);

			XMMATRIX transformation = XMMatrixAffineTransformation(scaleTot, XMVECTOR{}, rotationTot, translationTot);
			XMFLOAT4X4 transform;
			XMStoreFloat4x4(&transform, transformation);
			m_Transforms.emplace_back(transform);
		}
	}
}

void ModelAnimator::SetAnimation(const std::wstring& clipName)
{
	//TODO_W7_()
	//Set m_ClipSet to false
	//Iterate the m_AnimationClips vector and search for an AnimationClip with the given name (clipName)
	//If found,
	//	Call SetAnimation(Animation Clip) with the found clip
	//Else
	//	Call Reset
	//	Log a warning with an appropriate message

	m_ClipSet = false;
	std::vector<AnimationClip>::const_iterator it = std::find_if(m_pMeshFilter->m_AnimationClips.cbegin(), m_pMeshFilter->m_AnimationClips.cend(),
		[&clipName](const AnimationClip& clip) {
			return clip.name == clipName;
		});
	if (it == m_pMeshFilter->m_AnimationClips.cend())
	{
		Reset();
		Logger::LogWarning(L"Couldn't set Animation clip with name {}", clipName);
		return;
	}

	SetAnimation(*it);
}

void ModelAnimator::SetAnimation(UINT clipNumber)
{
	//TODO_W7_()
	//Set m_ClipSet to false
	//Check if clipNumber is smaller than the actual m_AnimationClips vector size
	//If not,
		//	Call Reset
		//	Log a warning with an appropriate message
		//	return
	//else
		//	Retrieve the AnimationClip from the m_AnimationClips vector based on the given clipNumber
		//	Call SetAnimation(AnimationClip clip)

	m_ClipSet = false;
	if (clipNumber >= m_pMeshFilter->m_AnimationClips.size())
	{
		Reset();
		Logger::LogWarning(L"Clip Number {} out of range", clipNumber);
		return;
	}

	SetAnimation(m_pMeshFilter->m_AnimationClips[clipNumber]);
}

void ModelAnimator::SetAnimation(const AnimationClip& clip)
{
	//TODO_W7_()
	//Set m_ClipSet to true
	//Set m_CurrentClip

	//Call Reset(false)

	m_ClipSet = true;
	m_CurrentClip = clip;
	Reset(false);
}

void ModelAnimator::Reset(bool pause)
{
	//TODO_W7_()
	//If pause is true, set m_IsPlaying to false
	if (pause)
		m_IsPlaying = false;

	//Set m_TickCount to zero
	//Set m_AnimationSpeed to 1.0f
	m_TickCount = 0;
	m_AnimationSpeed = 1.f;

	//If m_ClipSet is true
	//	Retrieve the BoneTransform from the first Key from the current clip (m_CurrentClip)
	//	Refill the m_Transforms vector with the new BoneTransforms (have a look at vector::assign)
	//Else
	//	Create an IdentityMatrix 
	//	Refill the m_Transforms vector with this IdenityMatrix (Amount = BoneCount) (have a look at vector::assign)
	if (m_ClipSet)
	{
		m_Transforms.assign(m_CurrentClip.keys.begin()->boneTransforms.cbegin(), m_CurrentClip.keys.begin()->boneTransforms.cend());
	}
	else
	{
		size_t size = m_Transforms.size();
		XMFLOAT4X4 identity{};
		XMStoreFloat4x4(&identity, XMMatrixIdentity());
		m_Transforms.assign(size, identity);
	}
}
