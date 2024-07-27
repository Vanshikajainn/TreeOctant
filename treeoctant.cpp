#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <tuple>
#include <algorithm>

using namespace std;

// Define a geometric entity class for points and line segments
template <typename T>
class GeometricEntity {
public:
    T _x, _y, _z;
    T _x2, _y2, _z2;
    bool isLineSegment;

    // Point constructor
    GeometricEntity(T x, T y, T z) : _x(x), _y(y), _z(z), isLineSegment(false) {}

    // Line segment constructor
    GeometricEntity(T x1, T y1, T z1, T x2, T y2, T z2)
        : _x(x1), _y(y1), _z(z1), _x2(x2), _y2(y2), _z2(z2), isLineSegment(true) {}
};

// Node class for the octree structure
template <typename T>
class Node {
public:
    vector<GeometricEntity<T>> entities;
    vector<Node*> children;
    T xMin, yMin, zMin, xMax, yMax, zMax;
    int capacity;
    bool divided;

    Node(const vector<GeometricEntity<T>>& entities, int capacity, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax)
        : entities(entities), capacity(capacity), xMin(xMin), yMin(yMin), zMin(zMin), xMax(xMax), yMax(yMax), zMax(zMax), divided(false) {}

    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }

    void divide() {
        T xMid = (xMin + xMax) / 2;
        T yMid = (yMin + yMax) / 2;
        T zMid = (zMin + zMax) / 2;

        vector<Node*> newNodes = {
            new Node({}, capacity, xMin, yMin, zMin, xMid, yMid, zMid),
            new Node({}, capacity, xMid, yMin, zMin, xMax, yMid, zMid),
            new Node({}, capacity, xMin, yMid, zMin, xMid, yMax, zMid),
            new Node({}, capacity, xMid, yMid, zMin, xMax, yMax, zMid),
            new Node({}, capacity, xMin, yMin, zMid, xMid, yMid, zMax),
            new Node({}, capacity, xMid, yMin, zMid, xMax, yMid, zMax),
            new Node({}, capacity, xMin, yMid, zMid, xMid, yMax, zMax),
            new Node({}, capacity, xMid, yMid, zMid, xMax, yMax, zMax)
        };

        children = newNodes;
        divided = true;

        distributeEntities();
    }

    void distributeEntities() {
        for (const auto& entity : entities) {
            if (entity.isLineSegment) {
                distributeLineSegment(entity);
            } else {
                distributePoint(entity);
            }
        }
        entities.clear();
    }

    void distributePoint(const GeometricEntity<T>& point) {
        for (auto* child : children) {
            if (pointFitsWithinBox(point, child->xMin, child->yMin, child->zMin, child->xMax, child->yMax, child->zMax)) {
                child->entities.push_back(point);
                break; // Ensures the point is only added to one child
            }
        }
    }

   void distributeLineSegment(const GeometricEntity<T>& segment) {
    for (auto* child : children) {
        if (lineSegmentIntersectsBox(segment, child->xMin, child->yMin, child->zMin, child->xMax, child->yMax, child->zMax)) {
            vector<GeometricEntity<T>> splitSegments = splitLineSegment(segment, child->xMin, child->yMin, child->zMin, child->xMax, child->yMax, child->zMax);
            for (const auto& splitSegment : splitSegments) {
                if (lineSegmentIntersectsBox(splitSegment, child->xMin, child->yMin, child->zMin, child->xMax, child->yMax, child->zMax)) {
                    child->entities.push_back(splitSegment);
                }
            }
        }
    }
}


    vector<GeometricEntity<T>> splitLineSegment(const GeometricEntity<T>& segment, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax) {
        vector<GeometricEntity<T>> splitSegments;
        T x1 = segment._x, y1 = segment._y, z1 = segment._z;
        T x2 = segment._x2, y2 = segment._y2, z2 = segment._z2;

        vector<tuple<T, T, T>> intersectionPoints;
        calculateIntersectionPoints(x1, y1, z1, x2, y2, z2, xMin, yMin, zMin, xMax, yMax, zMax, intersectionPoints);

        intersectionPoints.push_back(make_tuple(x1, y1, z1));
        intersectionPoints.push_back(make_tuple(x2, y2, z2));
        sort(intersectionPoints.begin(), intersectionPoints.end(), [&](const tuple<T, T, T>& a, const tuple<T, T, T>& b) {
            T da = pow(get<0>(a) - x1, 2) + pow(get<1>(a) - y1, 2) + pow(get<2>(a) - z1, 2);
            T db = pow(get<0>(b) - x1, 2) + pow(get<1>(b) - y1, 2) + pow(get<2>(b) - z1, 2);
            return da < db;
        });

        for (size_t i = 0; i < intersectionPoints.size() - 1; ++i) {   //creation of new segments 
            T nx1, ny1, nz1, nx2, ny2, nz2;
            tie(nx1, ny1, nz1) = intersectionPoints[i];
            tie(nx2, ny2, nz2) = intersectionPoints[i + 1];
            if (nx1 != nx2 || ny1 != ny2 || nz1 != nz2) {
                splitSegments.emplace_back(nx1, ny1, nz1, nx2, ny2, nz2);
            }
        }

        return splitSegments;
    }

    void calculateIntersectionPoints(T x1, T y1, T z1, T x2, T y2, T z2, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax, vector<tuple<T, T, T>>& intersections) {
        T planes[6][4] = {
            {1, 0, 0, -xMin}, {-1, 0, 0, xMax},
            {0, 1, 0, -yMin}, {0, -1, 0, yMax},
            {0, 0, 1, -zMin}, {0, 0, -1, zMax}
        };

        for (const auto& plane : planes) {
         T a = plane[0], b = plane[1], c = plane[2], d = plane[3];
         T denom = a * (x2 - x1) + b * (y2 - y1) + c * (z2 - z1);
         if (abs(denom) > 1e-6) {
            T t = -(a * x1 + b * y1 + c * z1 + d) / denom;
            if (t > 0 && t < 1) {
                T nx = x1 + t * (x2 - x1);
                T ny = y1 + t * (y2 - y1);
                T nz = z1 + t * (z2 - z1);
                if (nx >= xMin && nx <= xMax && ny >= yMin && ny <= yMax && nz >= zMin && nz <= zMax) {
                    intersections.push_back(make_tuple(nx, ny, nz));
                }
            }
        }
    }
}
    bool pointFitsWithinBox(const GeometricEntity<T>& point, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax) {
        return (point._x >= xMin && point._x <= xMax &&
                point._y >= yMin && point._y <= yMax &&
                point._z >= zMin && point._z <= zMax);
    }

    bool lineSegmentIntersectsBox(const GeometricEntity<T>& seg, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax) {
        return (seg._x <= xMax && seg._x2 >= xMin &&
                seg._y <= yMax && seg._y2 >= yMin &&
                seg._z <= zMax && seg._z2 >= zMin);
    }

    int pointSize() const {
        int size = 0;
        for (const auto& entity : entities) {
            if (!entity.isLineSegment) {
                size++;
            }
        }
        for (const auto* child : children) {
            size += child->pointSize();
        }
        return size;
    }

    double segmentLength() const {
        double length = 0;
        for (const auto& entity : entities) {
            if (entity.isLineSegment) {
                length += calculateLength(entity);
            }
        }
        for (const auto* child : children) {
            length += child->segmentLength();
        }
        return length;
    }

    double calculateLength(const GeometricEntity<T>& seg) const {
        return sqrt(pow(seg._x2 - seg._x, 2) + pow(seg._y2 - seg._y, 2) + pow(seg._z2 - seg._z, 2));
    }

    void print(int level = 0) const {
        string indent(level * 2, '-');
        cout << indent << "Node(totalPoints: " << pointSize()
             << ", pointsSize: " << pointSize()
             << ", totalSegments: " << segmentCount()
             << ", totalSegmentLength: " << segmentLength()
             << ", capacity: " << capacity
             << ", bounds: [" << xMin << "," << yMin << "," << zMin << "]-[" << xMax << "," << yMax << "," << zMax << "])\n";

        for (const auto* child : children) {
            child->print(level + 1);
        }
    }

    int segmentCount() const {
        int count = 0;
        for (const auto& entity : entities) {
            if (entity.isLineSegment) {
                count++;
            }
        }
        for (const auto* child : children) {
            count += child->segmentCount();
        }
        return count;
    }
};

// Function to create the octree
template <typename T>
void createTree(Node<T>& node) {
    if (node.entities.size() > node.capacity) {
        node.divide();
        for (auto* child : node.children) {
            createTree(*child);
        }
    }
}

// Example usage in main
int main() {
    int numPoints, numSegments, capacity;

    cout << "Enter number of points for the root node: ";
    cin >> numPoints;

    cout << "Enter number of line segments for the root node: ";
    cin >> numSegments;

    cout << "Enter capacity for each node: ";
    cin >> capacity;

    vector<GeometricEntity<int>> entities;
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<int> dist(0, 100);

    for (int i = 0; i < numPoints; ++i) {
        entities.emplace_back(dist(rng), dist(rng), dist(rng));
    }

    for (int i = 0; i < numSegments; ++i) {
        entities.emplace_back(dist(rng), dist(rng), dist(rng), dist(rng), dist(rng), dist(rng));
    }

    Node<int> root(entities, capacity, 0, 0, 0, 100, 100, 100);
    createTree(root);
    root.print();

    return 0;
}

