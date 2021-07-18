

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <functional>

namespace fcak
{
    class KM_Match
    {
    private:
        using compare_func_ = std::function<bool(std::pair<std::string, double>, std::pair<std::string, double>)>;
    public:
        enum SortMethod
        {
            LARGE = 0,
            SMALL = 1,
        };

        void AddWeight(const std::string& positive_key, const std::string& negative_key, double weight);
        void SetSortMethod(SortMethod method);
        void Match(std::unordered_map<std::string, std::string>& match_result);
        
    private:
        bool Negotiate(
            std::pair<std::string, double> negative_pair,
            std::unordered_map<std::string, std::string>& match_result);

    private:
        std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> m_positive_units;
        std::unordered_map<std::string, std::string> m_negative_units;
        compare_func_ m_compare_func;
    };
}
