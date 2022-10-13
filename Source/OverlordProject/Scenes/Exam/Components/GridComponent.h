#pragma once
class GridComponent final : public BaseComponent
{
public:
	GridComponent(int32_t col, int32_t row) : m_GridPos{ col, row }, m_PreviousGridPos{ col, row } {}
	GridComponent(XMINT2 gridPos) : m_GridPos{ gridPos }, m_PreviousGridPos{ gridPos }{}
	virtual ~GridComponent() = default;

	GridComponent(const GridComponent&) = delete;
	GridComponent(GridComponent&&) noexcept = delete;

	GridComponent& operator=(const GridComponent&) = delete;
	GridComponent& operator= (GridComponent&&) noexcept = delete;

	//GETTERS
	XMINT2 GetGridPos() const { return m_GridPos; }
	XMINT2 GetPreviousGridPos() const { return m_PreviousGridPos; }
	static XMFLOAT2 GetSpawnPosition() { return m_SpawnPosition; }
	static XMINT2 GetGridDimensions() { return m_GridDimensions; }
	static uint32_t GetGridSize() { return m_GridSize; }

	//SETTERS
	static void SetSpawnPosition(XMFLOAT2 spawnPosition) { m_SpawnPosition = spawnPosition; }
	static void SetGridDimensions(XMINT2 gridDimensions) {  m_GridDimensions = gridDimensions; }
	static void SetGridSize(uint32_t gridSize) { m_GridSize = gridSize; }
	void SetGridPos(int32_t col, int32_t row) { SetGridPos(XMINT2{ col, row }); }
	void SetGridPos(XMINT2 pos) { m_GridPos = pos; }
	
protected:
	void Initialize(const SceneContext&) override{};
	void Update(const SceneContext&) override;

	void OnSceneAttach(GameScene*) override;

private:
	XMINT2 m_GridPos;
	XMINT2 m_PreviousGridPos;

	static XMFLOAT2 m_SpawnPosition;
	static XMINT2 m_GridDimensions;
	static uint32_t m_GridSize;
};

