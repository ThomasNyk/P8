#include <vector>
#include "WaypointMission.cpp"
#include <string>
#include "lib/json.hpp"

using json = nlohmann::json;

class WaypointMissionList : std::vector<WaypointMission>
{
    private:
        std::vector<WaypointMission> WaypointMissions;
    
    public:
        WaypointMissionList(std::string data)
        {
            json jsonData = json::parse(data);
            
            std::vector<std::vector<std::pair<float, float>>> parsed;
            for (const auto& group : jsonData) {
                std::vector<std::pair<float, float>> inner;
                for (const auto& pair : group) {
                    inner.emplace_back(pair[0], pair[1]);
                }
                WaypointMissions.push_back(WaypointMission(inner));
            }
        }

        std::string toString() {
            std::string result = "[";
            for (size_t i = 0; i < WaypointMissions.size(); ++i) {
                result += WaypointMissions[i].toString();
                if (i < WaypointMissions.size() - 1)
                    result += ", ";
            }
            result += "]";
            return result;
        }
        

        
};