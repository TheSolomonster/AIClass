#include "TileMap.h"
using namespace AI;

namespace
{
	inline int ToIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void TileMap::LoadTiles(const char* fileName)
{
    FILE* file = nullptr;
    fopen_s(&file, fileName, "r");
    if (file)
    {
        int count = 0;
        fscanf_s(file, "Tiles: %d\n", &count);

        mTiles.clear();
        for (int y = 0; y < count; ++y)
        {
            char buffer[256];
            bool* buffer2 = new bool{false};
            fscanf_s(file, "%s\n", buffer, static_cast<int>(std::size(buffer)));
            fscanf_s(file, "%i\n", buffer2);
            mIsWalkable.push_back(*buffer2);
            mTiles.push_back(X::LoadTexture(buffer));
        }
        fclose(file);
    }
}

void TileMap::LoadMap(const char* fileName)
{
    FILE* file = nullptr;
    fopen_s(&file, fileName, "r");
    if (file)
    {
        fscanf_s(file, "Columns: %d\n", &mColumns);
        fscanf_s(file, "Rows: %d\n", &mRows);

        mMap.clear();
        for (int y = 0; y < mRows; ++y)
        {
            for (int x = 0; x < mColumns; ++x)
            {
                const int value = fgetc(file) - '0';
                mMap.push_back(value);
            }
            fgetc(file);
        }
        mGraph.Initialize(mColumns, mRows);
        for (int y = 0; y < mRows; ++y)
        {
            for (int x = 0; x < mColumns; ++x)
            {
                if (mIsWalkable[mMap[ToIndex(x, y, mColumns)]] == 0)
                {
                    if (y != 0 && mIsWalkable[mMap[ToIndex(x, y - 1, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::North] = mGraph.GetNode(x, y - 1);
                    if (y != 0 && x < mColumns - 1 && mIsWalkable[mMap[ToIndex(x + 1, y - 1, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::NorthEast] = mGraph.GetNode(x + 1, y - 1);
                    if (x < mColumns - 1 && mIsWalkable[mMap[ToIndex(x + 1, y, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::East] = mGraph.GetNode(x + 1, y);
                    if (y < mRows - 1 && x < mColumns - 1 && mIsWalkable[mMap[ToIndex(x + 1, y + 1, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::SouthEast] = mGraph.GetNode(x + 1, y + 1);
                    if (y < mRows - 1 && mIsWalkable[mMap[ToIndex(x, y + 1, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::South] = mGraph.GetNode(x, y + 1);
                    if (y < mRows - 1 && x != 0 && mIsWalkable[mMap[ToIndex(x - 1, y + 1, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::SouthWest] = mGraph.GetNode(x - 1, y + 1);
                    if (x != 0 && mIsWalkable[mMap[ToIndex(x - 1, y, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::West] = mGraph.GetNode(x - 1, y);
                    if (x != 0 && y != 0 && mIsWalkable[mMap[ToIndex(x - 1, y - 1, mColumns)]] == 0) mGraph.GetNode(x, y)->neighbors[GridBasedGraph::NorthWest] = mGraph.GetNode(x - 1, y - 1);
                }
            }
        }
    }
    fclose(file);
}

void TileMap::Render()
{
    for (int y = 0; y < mRows; ++y)
    {
        for (int x = 0; x < mColumns; ++x)
        {
            const int mapIndex = ToIndex(x, y, mColumns);
            const int tileIndex = mMap[mapIndex];
            const X::TextureId textureId = mTiles[tileIndex];
            const X::Math::Vector2 position = { x * mSize, y * mSize};
            X::DrawSprite(textureId, position, X::Pivot::TopLeft);
            int num = mGraph.GetNode(5, 5)->neighbors.size();
            for (int i = 0; i < mGraph.GetNode(x, y)->neighbors.size(); i++)
            {
                float buffer = mSize / 2;
                if (mGraph.GetNode(x, y)->neighbors[i] != nullptr)
                    X::DrawScreenLine(GetPixelPosition(mGraph.GetNode(x, y)->neighbors[i]->columns, mGraph.GetNode(x, y)->neighbors[i]->row), GetPixelPosition(mGraph.GetNode(x, y)->columns, mGraph.GetNode(x, y)->row), X::Colors::Black);
            }
        }
    }
}

std::vector<X::Math::Vector2> TileMap::FindPathBFS(int startX, int startY, int endX, int endY)
{
    if (startX < 0 || startX>29 || startY < 0 || startY>19)return std::vector<X::Math::Vector2>{-10,-10};
    std::vector<X::Math::Vector2> path;
    BFS bfs;
    if (bfs.Run(mGraph, startX, startY, endX, endY))
    {
        const auto& openList = bfs.GetOpenList();
        auto node = openList.front();
        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->columns, node->row));
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());

        // Cache the closed list for visualization
        mOpenListB = openList;
    }

    return path;
}
std::vector<X::Math::Vector2> TileMap::FindPathDFS(int startX, int startY, int endX, int endY)
{
    if (startX < 0 || startX>29 || startY < 0 || startY>19)return std::vector<X::Math::Vector2>{-10, -10};
    std::vector<X::Math::Vector2> path;
    DFS dfs;
    if (dfs.Run(mGraph, startX, startY, endX, endY))
    {
        const auto& openList = dfs.GetOpenList();
        auto node = openList.front();
        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->columns, node->row));
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());
        // Cache the closed list for visualization
        mOpenListD = openList;
    }
    return path;
}

std::vector<X::Math::Vector2> TileMap::FindPathDijkstra(int startX, int startY, int endX, int endY)
{
    if (startX < 0 || startX>29 || startY < 0 || startY>19)return std::vector<X::Math::Vector2>{-10, -10};
    std::vector<X::Math::Vector2> path;
    Dijkstra dijkstra;
    auto getCost = [&](auto a, auto b)
    {
        return GetCost(a, b);
    };
    if (dijkstra.Run(mGraph, startX, startY, endX, endY, getCost))
    {
        const auto& openList = dijkstra.GetOpenList();
        auto node = openList.front();
        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->columns, node->row));
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());
        // Cache the closed list for visualization
        mOpenListD = openList;
    }
    return path;
}

std::vector<X::Math::Vector2> TileMap::FindPathAStar(int startX, int startY, int endX, int endY)
{
    if (startX < 0 || startX>29 || startY < 0 || startY>19)return std::vector<X::Math::Vector2>{-10, -10};
    std::vector<X::Math::Vector2> path;
    AStar aStar;
    auto getCost = [&](auto a, auto b)
    {
        return GetCost(a, b);
    };
    auto getHeuristic = [&](auto a, auto b)
    {
        return GetHeuristic(a, b);
    };
    if (aStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
    {
        const auto& openList = aStar.GetOpenList();
        auto node = openList.front();
        while (node != nullptr)
        {
            path.push_back(GetPixelPosition(node->columns, node->row));
            node = node->parent;
        }
        std::reverse(path.begin(), path.end());
        // Cache the closed list for visualization
        mOpenListD = openList;
    }
    return path;
}


X::Math::Vector2 TileMap::GetPixelPosition(int x, int y) const
{
    return { (x + 0.5f) * mSize, (y + 0.5f) * mSize, };
}

float TileMap::GetCost(const AI::GridBasedGraph::Node* a, const AI::GridBasedGraph::Node* b) const
{
    const int fromTileIndex = ToIndex(a->columns, a->row, mColumns);
    const int toTileIndex = ToIndex(b->columns, b->row, mColumns);
    const int tileType = mMap[toTileIndex];
    if (tileType == 1) return 5.0f;
    return 1.0f;
}

float TileMap::GetHeuristic(const AI::GridBasedGraph::Node* a, const AI::GridBasedGraph::Node* b) const
{
    float length = b->columns - a->columns;
    float height = b->row - a->row;
    float num = (length * length) + (height * height);
    if (num < 0)num *= -1;
    return X::Math::Sqrt(num)/25.0f;
}

