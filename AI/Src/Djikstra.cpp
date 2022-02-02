#include "Precompiled.h"
#include "Djikstra.h"

using namespace AI;

bool Dijkstra::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost)
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
	bool found = false;
	while (!found && mOpenList.size() > 0)
	{
		auto node = mOpenList.front();
		if (node == endNode) found = true;
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
						std::list<GridBasedGraph::Node*>::iterator it = mOpenList.begin();
						for (int i = 0; i < mOpenList.size(); i++)
						{
							GridBasedGraph::Node* node3 = *it;
							if (node3->g > node2->g)
							{
								mOpenList.insert(it--, node2);
								break;
							}
							else if (node3 == mOpenList.back())
							{
								mOpenList.push_back(node2);
								break;
							}
							else
							{
								it++;
							}
						}
					}
					else if (node2->g > node->g + getCost(node, node2))
					{
						node2->parent = node;
						node2->g = node->g + getCost(node, node2);
						mOpenList.remove(node2); 
						std::list<GridBasedGraph::Node*>::iterator it = mOpenList.begin();
						for (int i = 0; i < mOpenList.size(); i++)
						{
							GridBasedGraph::Node* node3 = *it;
							if (node3->g > node2->g)
							{
								mOpenList.insert(it--, node2);
								break;
							}
							else if (node3 == mOpenList.back())
							{
								mOpenList.push_back(node2);
								break;
							}
							else
							{
								it++;
							}
						}
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