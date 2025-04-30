#include "Waypoint.cpp"
#include <vector>

class WaypointMission : std::vector<Waypoint>
{
    private:
        std::vector<Waypoint> waypoints;

    public:
        WaypointMission(std::vector<std::pair<float, float>> data) {
            for(const auto& waypoint: data) {
                waypoints.push_back(Waypoint(waypoint));
            }
        }

        std::string toString() {
            std::string result = "[";
            for (size_t i = 0; i < waypoints.size(); ++i) {
                result += waypoints[i].toString();
                if (i < waypoints.size() - 1)
                    result += ", ";
            }
            result += "]";
            return result;
        }        
};