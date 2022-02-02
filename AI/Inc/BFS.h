#pragma once
#include "GridBasedGraph.h"
namespace AI
{
	class BFS
	{
		using nodeList = std::list<GridBasedGraph::Node*>;
	public:
		bool Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY);
		const std::list<GridBasedGraph::Node*>& GetOpenList() const { return mOpenList; }
	private:
		std::list<GridBasedGraph::Node*> mOpenList;
		std::list<GridBasedGraph::Node*> mClosedList;
	};
}