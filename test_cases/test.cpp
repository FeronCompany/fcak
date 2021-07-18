

#include <iostream>
#include <test.hpp>

namespace fcak
{
namespace test
{

static void InitMapModel(std::shared_ptr<MapModel> map_model)
{
    for (int j = 0; j < 10; ++j)
    {
        for (int i = 1; i <= 10; ++i)
        {
            auto node = map_model->AddNode(i + 10 * j);
            node->x = i - 1.0;
            node->y = j - 1.0;
            if (i - 1 >= 1)
            {
                node->neighbor_node_list.push_back(i - 1 + 10 * j);
            }
            if (i + 1 <= 10)
            {
                node->neighbor_node_list.push_back(i + 1 + 10 * j);
            }
            if (j - 1 >= 0)
            {
                node->neighbor_node_list.push_back(i + 10 * (j - 1));
            }
            if (j + 1 <= 9)
            {
                node->neighbor_node_list.push_back(i + 10 * (j + 1));
            }
            if ((i == 4 && j == 1) || (i == 4 && j == 2) || (i == 2 && j == 3) ||
                (i == 3 && j == 3) || (i == 4 && j == 3) || (i == 4 && j == 4) ||
                (i == 5 && j == 4) || (i == 6 && j == 4) || (i == 7 && j == 4) ||
                (i == 7 && j == 5) || (i == 7 && j == 6) || (i == 7 && j == 7) ||
                (i == 5 && j == 8) || (i == 6 && j == 8) || (i == 7 && j == 8))
            {
                node->access = false;
            } 			else
            {
                node->access = true;
            }
        }
    }
}

void RunAllCases()
{
    AstarCase();
    KM_Case();
}

void AstarCase()
{
    std::cout << "AstarCase Begin:\n";
    std::shared_ptr<MapModel> map_model = std::make_shared<MapModel>();
    InitMapModel(map_model);
    AStarRouting astar(map_model);
    std::vector<RouteNode> route;
    astar.Routing(56, 36, route);
    for (auto& node : route)
    {
        std::cout << "id: " << node.map_node->id << "\n";
    }
    std::cout << "AstarCase End.\n\n";
}

void KM_Case()
{
    std::cout << "KM_Case Begin:\n";
    fcak::KM_Match match;
    match.SetSortMethod(fcak::KM_Match::SortMethod::LARGE);
    match.AddWeight("LiBing", "YangYi", 2);
    match.AddWeight("LiBing", "LuoMei", 5);
    match.AddWeight("ZhangHai", "ChenTing", 3);
    match.AddWeight("ZhangHai", "LiYun", 2);
    match.AddWeight("JiangWei", "ChenTing", 4);
    match.AddWeight("JiangWei", "HeYa", 6);
    match.AddWeight("LiuYi", "YangYi", 7);
    match.AddWeight("LiuYi", "LuoMei", 3);
    match.AddWeight("MaYun", "HeYa", 7);
    std::unordered_map<std::string, std::string> out;
    match.Match(out);
    for (auto pair : out)
    {
        std::cout << pair.first << "--" << pair.second << "\n";
    }
    std::cout << "KM_Case End.\n\n";
}

}
}
