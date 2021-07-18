

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <queue>
#include <unordered_map>
#include <unordered_set>

namespace fcak
{


struct MapNode
{
    double x;
    double y;
    bool access;
    int id;
    std::vector<int> neighbor_node_list;

    MapNode()
    {
        x = 0;
        y = 0;
        access = false;
        id = 0;
    }
};

class MapModel
{
public:
    ~MapModel();

    MapNode* AddNode(int id);
    MapNode* GetNode(int id);
    void GetNeighborNodes(int id, std::vector<MapNode*>& neighbor_nodes);

private:

private:
    std::unordered_map<int, MapNode*> m_node_map;
};

enum class RouteResult
{
    SUCCESS = 0,
    FAILED = 1,
};

struct RouteNode
{
    MapNode* map_node;
    int from_node_id;
    double g;
    double f;

    RouteNode() = default;
    RouteNode(const RouteNode& node) = default;
    RouteNode(MapNode* map_node_in)
    {
        map_node = map_node_in;
        from_node_id = 0;
        g = 0;
        f = 0;
    }

    bool operator==(const RouteNode& node) const
    {
        return map_node->id == node.map_node->id;
    }
    bool operator<(const RouteNode& node) const
    {
        return f > node.f;// smallest
    }
};

class AStarRouting
{
public:
    AStarRouting(std::shared_ptr<MapModel> node_map);
    ~AStarRouting();

    RouteResult Routing(int start_id, int end_id, std::vector<RouteNode>& route);

private:
    double GetDistance(double src_x, double src_y, double dst_x, double dst_y);
    void BuildLane(const RouteNode& dst_node, std::vector<RouteNode>& route);

private:
    std::shared_ptr<MapModel> m_node_map;
    std::priority_queue<RouteNode> m_open_list;
    std::unordered_map<int, RouteNode> m_route_map;
    std::unordered_set<int> m_close_list;
};


}