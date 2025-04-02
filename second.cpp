#include <iostream>
#include <cmath>
#include <optional>

class Vector3D {
private:
    double x, y, z;

public:
    Vector3D(const double x, const double y, const double z) : x(x), y(y), z(z) {}

    Vector3D operator*(const double scalar) const { return {scalar * x, scalar * y, scalar * z}; }
    Vector3D operator+(const Vector3D &other) const { return {x + other.x, y + other.y, z + other.z}; }
    Vector3D operator-(const Vector3D &other) const { return {x - other.x, y - other.y, z - other.z}; }

    friend std::ostream &operator<<(std::ostream &os, const Vector3D &v) {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    [[nodiscard]] Vector3D cross(const Vector3D &other) const {
        return {y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x};
    }
    [[nodiscard]] double dot(const Vector3D &other) const { return x * other.x + y * other.y + z * other.z; }
};

struct Segment3D {
    Vector3D start, end;
};


std::optional<Vector3D> intersect(const Segment3D &a, const Segment3D &b) {
    const auto AB = a.end - a.start;
    const auto AC = b.start - a.start;
    if (std::fabs(AB.dot(AC.cross(b.end - a.start))) >= std::numeric_limits<double>::epsilon()) {
        return std::nullopt;
    }

    const auto CD = b.end - b.start;
    const auto crossProduct = AB.cross(CD);
    const auto det = crossProduct.dot(crossProduct);

    if (std::fabs(det) < std::numeric_limits<double>::epsilon()) {
        return std::nullopt;
    }

    const auto t = AC.cross(CD).dot(crossProduct) / det;
    const auto s = AC.cross(AB).dot(crossProduct) / det;

    if (t >= 0 && t <= 1 && s >= 0 && s <= 1) {
        return a.start + AB * t;
    }
    return std::nullopt;
}


int main() {
    const Segment3D segment1 = {{0, 0, 0}, {1, 1, 0}};
    const Segment3D segment2 = {{1, 0, 0}, {1, 1, 0}};

    if (const auto result = intersect(segment1, segment2)) {
        std::cout << *result << std::endl;
    } else {
        std::cout << "no intersection";
    }
    return 0;
}
