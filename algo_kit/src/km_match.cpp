

#include <algorithm>
#include <iostream>

#include <km_match.hpp>

namespace fcak
{

    void KM_Match::AddWeight(const std::string& positive_key, const std::string& negative_key, double weight)
    {
        m_positive_units[positive_key].push_back({ negative_key, weight });
        m_negative_units[negative_key] = "";
    }

    void KM_Match::SetSortMethod(SortMethod method)
    {
        if (method == SortMethod::LARGE)
        {
            m_compare_func = [](std::pair<std::string, double> a, std::pair<std::string, double> b) {
                return a.second > b.second;
            };
        }
        else        
        {
            m_compare_func = [](std::pair<std::string, double> a, std::pair<std::string, double> b) {
                return a.second < b.second;
            };
        }
    }

    void KM_Match::Match(std::unordered_map<std::string, std::string>& match_result)
    {
        for (auto& positive_pair : m_positive_units)
        {
            auto& negative_weights = positive_pair.second;
            if (negative_weights.empty())
            {
                std::cout << "Error: " << positive_pair.first << " has empty relations.\n";
                continue;
            }
            if (m_compare_func)
            {
                std::sort(negative_weights.begin(), negative_weights.end(), m_compare_func);
            }
            else
            {
                std::sort(negative_weights.begin(), negative_weights.end());
            }
            for (auto negative_iter = negative_weights.begin(); negative_iter != negative_weights.end();)
            {
                std::string negative_key = negative_iter->first;
                auto search_iter = m_negative_units.find(negative_key);
                if (search_iter == m_negative_units.end())
                {
                    std::cout << "Invalid data.\n";
                    return;
                }
                if (!m_negative_units[negative_key].empty())
                {
                    if (Negotiate(*negative_iter, match_result))
                    {
                        m_negative_units[negative_key] = positive_pair.first;
                        match_result[positive_pair.first] = negative_key;
                        break;
                    }
                    else
                    {
                        negative_iter = negative_weights.erase(negative_iter);
                        continue;
                    }
                }
                else
                {
                    m_negative_units[negative_key] = positive_pair.first;
                    match_result[positive_pair.first] = negative_key;
                    break;
                }
            }
        }
    }

    bool KM_Match::Negotiate(
        std::pair<std::string, double> negative_pair,
        std::unordered_map<std::string, std::string>& match_result)
    {
        std::cout << "conflict about: " << negative_pair.first << "\n";
        std::string conflict_negative_key = negative_pair.first;
        std::string positive_key = m_negative_units[conflict_negative_key];
        auto& negotiate_negative_weights = m_positive_units[positive_key];
        if (negotiate_negative_weights.empty() || negotiate_negative_weights[0].first != conflict_negative_key)
        {
            std::cout << "negotiate_negative_weights data invalid.\n";
            return false;
        }
        if (m_compare_func(negotiate_negative_weights[0], negative_pair))
        {
            std::cout << "current is larger.\n";
            return false;
        }
        negotiate_negative_weights.erase(negotiate_negative_weights.begin());
        match_result.erase(positive_key);
        for (auto negative_iter = negotiate_negative_weights.begin(); negative_iter != negotiate_negative_weights.end();)
        {
            std::string negative_key = negative_iter->first;
            auto search_iter = m_negative_units.find(negative_key);
            if (search_iter == m_negative_units.end())
            {
                std::cout << "Invalid data.\n";
                return false;
            }
            if (!m_negative_units[negative_key].empty())
            {
                if (Negotiate(*negative_iter, match_result))
                {
                    m_negative_units[negative_key] = positive_key;
                    match_result[positive_key] = negative_key;
                    break;
                }
                else
                {
                    negative_iter = negotiate_negative_weights.erase(negative_iter);
                    continue;
                }
            }
            else
            {
                m_negative_units[negative_key] = positive_key;
                match_result[positive_key] = negative_key;
                break;
            }
        }
        return true;
    }
}
