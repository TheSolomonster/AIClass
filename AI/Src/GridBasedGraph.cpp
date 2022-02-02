#include "Precompiled.h"
#include "GridBasedGraph.h"

using namespace AI;

void GridBasedGraph::Initialize(int columns, int rows)
{
	mNodes.clear();
	mRows = rows;
	mColumns = columns;
	int c = 0;
	int r = 0;
	for (int i = 0; i < rows*columns; i++)
	{
		mNodes.emplace_back(Node());
		mNodes[i].columns = c;
		mNodes[i].row = r;
		c++;
		if (c == columns)
		{
			c = 0;
			r++;
		}
	}
}

GridBasedGraph::Node* GridBasedGraph::GetNode(int x, int y)
{
	if (x < 0 || x >= mColumns || y < 0 || y >= mRows) return nullptr;
	return &mNodes[GetIndex(x, y)];
}

const GridBasedGraph::Node* GridBasedGraph::GetNode(int x, int y) const
{
	if (x < 0 || x >= mColumns || y < 0 || y >= mRows) return nullptr;
	return &mNodes[GetIndex(x, y)];
}

void AI::GridBasedGraph::ResetSearchParams()
{
	for (auto& node : mNodes)
	{
		node.parent = nullptr;
		node.opened = false;
		node.closed = false;
		node.g = 0.0f;
		node.h = 0.0f;
	}
}

int GridBasedGraph::GetIndex(int x, int y) const
{
	return x + (y * mColumns);
}