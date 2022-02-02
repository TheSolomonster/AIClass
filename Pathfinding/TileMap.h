#pragma once
#include <AI.h>
#include <XEngine.h>

class TileMap
{
public:
	void LoadTiles(const char* fileName);
	void LoadMap(const char* fileName);

	void Render();
	std::vector<X::Math::Vector2> FindPathBFS(int startX, int startY, int endX, int endY);
	std::vector<X::Math::Vector2> FindPathDFS(int startX, int startY, int endX, int endY);
	std::vector<X::Math::Vector2> FindPathDijkstra(int startX, int startY, int endX, int endY);
	std::vector<X::Math::Vector2> FindPathAStar(int startX, int startY, int endX, int endY);
	X::Math::Vector2 GetPixelPosition(int x, int y) const;
private:
	float GetCost(const AI::GridBasedGraph::Node* a, const AI::GridBasedGraph::Node* b) const;
	float GetHeuristic(const AI::GridBasedGraph::Node* a, const AI::GridBasedGraph::Node* b) const;
	std::list<AI::GridBasedGraph::Node*> mOpenListD;
	std::list<AI::GridBasedGraph::Node*> mOpenListB;
	AI::GridBasedGraph mGraph;
	std::vector<int> mMap;
	std::vector<X::TextureId> mTiles; // I forgot to add this in class
	std::vector<bool> mIsWalkable;
	int mColumns = 0;
	int mRows = 0;
	float mSize = 32.0f;
};