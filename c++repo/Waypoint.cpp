#include "Vector3.cpp"

class Waypoint {
    private:
        Vector3 _destination;

    public:
        Waypoint(float x, float y, float z, bool homeAfter = true)
        {
            _destination = Vector3(x, y, z);
        }
        Waypoint(Vector3 destination, bool homeAfter = true) : _destination(destination)
        {

        }

        Waypoint(float x, float z, bool homeAfter = true)
        {
            _destination = Vector3(x, 10, z);
        }

        Waypoint(std::pair<float, float> destination, bool homeAfter = true)
        {
            _destination = Vector3(destination.first, 10, destination.second);
        }

        Vector3 getDestination() const { return _destination; };

        std::string toString()
        {
            return "{" + std::to_string(_destination.getX()) + ", " + std::to_string(_destination.getZ()) + "}";
        }
};