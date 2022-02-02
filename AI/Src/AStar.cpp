#include "Precompiled.h"
#include "AStar.h"

using namespace AI;

void AStar::sortedAdd(GridBasedGraph::Node* node)
{
	std::list<GridBasedGraph::Node*>::iterator it = mOpenList.begin();
	for (int i = 0; i < mOpenList.size(); i++)
	{
		GridBasedGraph::Node* node3 = *it;
		if (node3->g + node3->h > node->g + node->h)
		{
			mOpenList.insert(it--, node);
			break;
		}
		else if (node3 == mOpenList.back())
		{
			mOpenList.push_back(node);
			break;
		}
		else
		{
			it++;
		}
	}
}
bool AStar::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost, GetHeuristic getHeuristic)
{
	//reset everything
	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();
	//add start node to open list
	auto node = graph.GetNode(startX, startY);
	auto endNode = graph.GetNode(endX, endY);
	mOpenList.push_back(node);
	node->opened = true;
	node->h = getHeuristic(node, endNode);
	bool found = false;
	while (!found && mOpenList.size() > 0)
	{
		auto node = mOpenList.front();
		if (node == endNode)
		{
			found = true;
		}
		else
		{
			for (auto& node2 : node->neighbors)
			{
				if (node2 != nullptr && !node2->closed)
				{

					if (!node2->opened)
					{
						node2->opened = true;
						node2->parent = node;
						node2->g = node->g + getCost(node, node2);
						node2->h = getHeuristic(node2, endNode);
						sortedAdd(node2);
					}
					else if (node2->g > node->g + getCost(node, node2))
					{
						node2->parent = node;
						node2->g = node->g + getCost(node, node2);
						mOpenList.remove(node2);
						sortedAdd(node2);
					}
				}
			}
			mClosedList.push_back(node);
			node->closed = true;
			mOpenList.pop_front();
		}
	}
	return found;
}