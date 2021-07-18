

#include <iostream>

#include <astar.hpp>

namespace fcak
{

MapModel::~MapModel()
{
    for (auto pair : m_node_map)
    {
        if (pair.second)
        {
            delete pair.second;
        }
    }
}

MapNode* MapModel::AddNode(int id)
{
    auto iter = m_node_map.find(id);
    if (iter == m_node_map.end())
    {
        MapNode* node = new MapNode;
        m_node_map.insert(std::make_pair(id, node));
        node->id = id;
        return node;
    } 	else
    {
        std::cout << "Already exist node " << id << "\n";
        return iter->second;
    }
}

MapNode* MapModel::GetNode(int id)
{
    auto iter = m_node_map.find(id);
    if (iter == m_node_map.end())
    {
        std::cout << "Cant find node " << id << "\n";
        return nullptr;
    } 	else
    {
        return iter->second;
    }
}

void MapModel::GetNeighborNodes(int id, std::vector<MapNode*>& neighbor_nodes)
{
    auto iter_from = m_node_map.find(id);
    if (iter_from == m_node_map.end())
    {
        std::cout << "Cant find node " << id << "\n";
        return;
    }
    if (!iter_from->second)
    {
        std::cout << "find node but nullptr " << id << "\n";
        return;
    }
    for (auto& to_id : iter_from->second->neighbor_node_list)
    {
        auto iter_to = m_node_map.find(to_id);
        if (iter_to == m_node_map.end())
        {
            std::cout << "Cant find node " << to_id << "\n";
            return;
        }
        if (!iter_to->second)
        {
            std::cout << "find node but nullptr " << to_id << "\n";
            return;
        }
        neighbor_nodes.push_back(iter_to->second);
    }
}


AStarRouting::AStarRouting(std::shared_ptr<MapModel> node_map)
{
    if (node_map.get())
    {
        m_node_map = node_map;
    }
}

AStarRouting::~AStarRouting()
{

}

RouteResult AStarRouting::Routing(int start_id, int end_id, std::vector<RouteNode>& route)
{
    auto start_map_node = m_node_map->GetNode(start_id);
    if (nullptr == start_map_node)
    {
        std::cout << "cant find start_map_node " << start_id << "\n";
        return RouteResult::FAILED;
    }
    RouteNode start_route_node(start_map_node);
    auto end_map_node = m_node_map->GetNode(end_id);
    if (nullptr == end_map_node)
    {
        std::cout << "cant find end_map_node " << end_id << "\n";
        return RouteResult::FAILED;
    }
    RouteNode end_route_node(end_map_node);

    m_open_list.push(start_route_node);
    m_route_map.insert(std::make_pair(start_id, start_route_node));
    for (; !m_open_list.empty(); )
    {
        auto& top_node = m_open_list.top();
        auto& curr_node = m_route_map[top_node.map_node->id];
        m_open_list.pop();
        m_close_list.insert(curr_node.map_node->id);
        std::vector<MapNode*> neighbors;
        m_node_map->GetNeighborNodes(curr_node.map_node->id, neighbors);
        for (auto map_node : neighbors)
        {
            if (curr_node.map_node->id == end_id)
            {
                std::cout << "found destination.\n";
                end_route_node.from_node_id = curr_node.from_node_id;
                break;
            }
            if (!map_node->access)
            {
                continue;
            }
            auto iter = m_route_map.find(map_node->id);
            if (iter != m_route_map.end())
            {
                RouteNode& route_neighbor = iter->second;
                if (m_close_list.count(route_neighbor.map_node->id))
                {
                    continue;
                }
                double distance = GetDistance(
                    curr_node.map_node->x,
                    curr_node.map_node->x,
                    map_node->x,
                    map_node->y);
                double pre_calc_1 = route_neighbor.g + distance;
                double pre_calc_2 = curr_node.g + distance;
                if (pre_calc_1 < curr_node.g)
                {
                    curr_node.from_node_id = map_node->id;
                    curr_node.g = pre_calc_1;
                    double heu_distance = GetDistance(
                        curr_node.map_node->x,
                        curr_node.map_node->x,
                        end_map_node->x,
                        end_map_node->y);
                    curr_node.f += curr_node.g + heu_distance;
                } 				else if (pre_calc_2 < route_neighbor.g)
                {
                    route_neighbor.from_node_id = curr_node.map_node->id;
                    route_neighbor.g = pre_calc_2;
                    double heu_distance = GetDistance(
                        route_neighbor.map_node->x,
                        route_neighbor.map_node->x,
                        end_map_node->x,
                        end_map_node->y);
                    route_neighbor.f += route_neighbor.g + heu_distance;
                }
            } 			else
            {
                RouteNode route_neighbor(map_node);
                route_neighbor.g = curr_node.g + GetDistance(
                    curr_node.map_node->x,
                    curr_node.map_node->x,
                    map_node->x,
                    map_node->y);
                route_neighbor.f = route_neighbor.g + GetDistance(
                    map_node->x,
                    map_node->y,
                    end_map_node->x,
                    end_map_node->y);
                route_neighbor.from_node_id = curr_node.map_node->id;
                m_open_list.push(route_neighbor);
                m_route_map.insert(std::make_pair(map_node->id, route_neighbor));
            }
        }
    }

    BuildLane(end_route_node, route);
    return RouteResult::SUCCESS;
}

double AStarRouting::GetDistance(double src_x, double src_y, double dst_x, double dst_y)
{
    return std::abs(src_x - dst_x) + std::abs(src_y - dst_y);
}

void AStarRouting::BuildLane(const RouteNode& dst_node, std::vector<RouteNode>& route)
{
    route.push_back(dst_node);
    int id = dst_node.from_node_id;
    do
    {
        auto& node = m_route_map[id];
        route.push_back(node);
        id = node.from_node_id;
    }
    while (id != 0);
    if (!route.empty())
    {
        std::reverse(route.begin(), route.end());
    }
}


}