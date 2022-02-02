#pragma once
#include "GridBasedGraph.h"
namespace AI
{
	class Dijkstra
	{
		using nodeList = std::list<GridBasedGraph::Node*>;
		using GetCost = std::function<float(const GridBasedGraph::Node*, const GridBasedGraph::Node*)>;
	public:
		bool Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost);
		const std::list<GridBasedGraph::Node*>& GetOpenList() const { return mOpenList; }
	private:
		std::list<GridBasedGraph::Node*> mOpenList;
		std::list<GridBasedGraph::Node*> mClosedList;
	};
}