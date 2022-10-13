#pragma once

class GridComponent;

enum class ObjectState
{
	Active,
	DestroyAnim,
	Destroy,
	Won,
	Inactive //out of scene
};


class GridBasedObject : public GameObject
{
public:
	GridBasedObject(XMINT2 gridPos, float y);
	virtual ~GridBasedObject() override = default;

	GridBasedObject(const GridBasedObject&) = delete;
	GridBasedObject(GridBasedObject&&) noexcept = delete;

	GridBasedObject& operator=(const GridBasedObject&) = delete;
	GridBasedObject& operator=(GridBasedObject&&) noexcept = delete;

	void Activate(bool children) override;
	void Deactivate(bool children) override;

	//GETTERS
	GridComponent* GetGridComponent() const { return m_pGridComponent; }
	ObjectState GetState() const { return m_State; }

	//SETTERS
	void SetState(ObjectState state) { m_State = state; }

protected:
	virtual void Initialize(const SceneContext&) override;
	void Update(const SceneContext&) override final;
	virtual void OnSceneAttach(GameScene*) override;
	virtual void OnSceneDetach(GameScene*) override;


	virtual void UpdateActive(const SceneContext&) {};
	virtual void UpdateDestroyAnim(const SceneContext&){};
	virtual void UpdateDestroy(const SceneContext&) {};

private:
	GridComponent* m_pGridComponent;

	ObjectState m_State;
};

