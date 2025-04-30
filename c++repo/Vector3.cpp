#include <complex>
class Vector3
{
    private:
        float _x, _y, _z;
    public:
        Vector3() : _x(0), _y(0), _z(0) {}

        Vector3(float x, float y, float z) : _x(x), _y(y), _z(z) {
        }
        
        float CalcDistance(const Vector3* other) {
            float dx = _x - other->getX();
            float dy = _y - other->getY();
            float dz = _z - other->getZ();
            return std::sqrt(dx * dx + dy * dy + dz * dz);
        }

        float getX() const { return _x; };
        float getY() const { return _y; };
        float getZ() const { return _z; };
};