// #include <iostream>
// #include <vector>
// #include <random>
// #include <algorithm>
// #include <cmath>

// using namespace std;

// template <typename T>
// class GeometricEntity {
// public:
//     T _x, _y, _z;
//     T _x2, _y2, _z2; // Coordinates for the end point of the line segment
//     bool isLineSegment;

//     // Constructor for a point
//     GeometricEntity(T x, T y, T z) 
//         : _x(x), _y(y), _z(z), isLineSegment(false) {}

//     // Constructor for a line segment
//     GeometricEntity(T x1, T y1, T z1, T x2, T y2, T z2) 
//         : _x(x1), _y(y1), _z(z1), _x2(x2), _y2(y2), _z2(z2), isLineSegment(true) {}

//     double length() const {
//         if (isLineSegment) {
//             return sqrt(pow(_x2 - _x, 2) + pow(_y2 - _y, 2) + pow(_z2 - _z, 2));
//         }
//         return 0;
//     }
// };

// template <typename T>
// class Node {
// public:
//     vector<GeometricEntity<T>> entities;
//     int totalPoints;  // Total number of points in the node.
//     int totalSegments; // Total number of line segments in the node.
//     double totalSegmentLength; // Total length of line segments in the node.
//     int capacity;
//     T _xmin, _ymin, _zmin, _xmax, _ymax, _zmax;
//     vector<Node*> children; // A vector of pointers to child nodes.

//     Node(const vector<GeometricEntity<T>>& ents, int cap, T xmin, T ymin, T zmin, T xmax, T ymax, T zmax) 
//         : entities(ents), capacity(cap), _xmin(xmin), _ymin(ymin), _zmin(zmin), _xmax(xmax), _ymax(ymax), _zmax(zmax), totalPoints(0), totalSegments(0), totalSegmentLength(0) {
//             for (const auto& ent : entities) {
//                 if (ent.isLineSegment) {
//                     totalSegments++;
//                     totalSegmentLength += ent.length();
//                 } else {
//                     totalPoints++;
//                 }
//             }
//     }

//     void divide() {
//         if (entities.size() > capacity) {
//             vector<vector<GeometricEntity<T>>> entityDistribution = distributeEntities(entities); // Distribute entities among child nodes.
//             for (int i = 0; i < 8; ++i) { // Create child nodes.
//                 T newXMin = _xmin + (i & 1) * (_xmax - _xmin) / 2; // Lower or upper half of x-axis.
//                 T newYMin = _ymin + ((i >> 1) & 1) * (_ymax - _ymin) / 2;
//                 T newZMin = _zmin + ((i >> 2) & 1) * (_zmax - _zmin) / 2;
//                 T newXMax = newXMin + (_xmax - _xmin) / 2;
//                 T newYMax = newYMin + (_ymax - _ymin) / 2;
//                 T newZMax = newZMin + (_zmax - _zmin) / 2;
//                 children.push_back(new Node(entityDistribution[i], capacity, newXMin, newYMin, newZMin, newXMax, newYMax, newZMax));
//             }
//             entities.clear();
//         }
//     }

//     size_t pointSize() const {
//         return totalPoints;
//     }

//     double segmentLength() const {
//         return totalSegmentLength;
//     }

//     void print(int level = 0) const {
//         for (int i = 0; i < level; ++i) cout << "--";
//         cout << "Node(totalPoints: " << totalPoints 
//              << ", pointsSize: " << pointSize() << ", totalSegments: " << totalSegments << ", totalSegmentLength: " << totalSegmentLength << ", capacity: " << capacity 
//              << ", bounds: [" << _xmin << "," << _ymin << "," << _zmin 
//              << "]-[" << _xmax << "," << _ymax << "," << _zmax << "])\n";
//         for (const auto* child : children) {
//             child->print(level + 1);
//         }
//     }

// private:
//     vector<vector<GeometricEntity<T>>> distributeEntities(const vector<GeometricEntity<T>>& ents) {
//         vector<vector<GeometricEntity<T>>> distribution(8);
//         for (const auto& ent : ents) {
//             int index = (ent._x > (_xmin + _xmax) / 2) + ((ent._y > (_ymin + _ymax) / 2) << 1) + ((ent._z > (_zmin + _zmax) / 2) << 2);
//             distribution[index].push_back(ent);
//         }
//         return distribution;
//     }
// };

// template <typename T>
// void createTree(Node<T>& node) {
//     if (node.entities.size() > node.capacity) {
//         node.divide();
//         for (auto* child : node.children) {
//             createTree(*child);
//         }
//     }
// }

// int main() {
//     int numPoints, numSegments, capacity;

//     cout << "Enter number of points for the root node: ";
//     cin >> numPoints;

//     cout << "Enter number of line segments for the root node: ";
//     cin >> numSegments;

//     cout << "Enter capacity for each node: ";
//     cin >> capacity;

//     vector<GeometricEntity<int>> entities;
//     random_device rd;
//     mt19937 rng(rd());
//     uniform_int_distribution<int> dist(0, 100);

//     for (int i = 0; i < numPoints; ++i) {
//         entities.emplace_back(dist(rng), dist(rng), dist(rng));
//     }

//     for (int i = 0; i < numSegments; ++i) {
//         entities.emplace_back(dist(rng), dist(rng), dist(rng), dist(rng), dist(rng), dist(rng));
//     }

//     Node<int> root(entities, capacity, 0, 0, 0, 100, 100, 100);

//     createTree(root);

//     cout << "\nOctree Structure:\n";
//     root.print();

//     cout << "\nRoot Node Point Size: " << root.pointSize() << endl;
//     cout << "Root Node Total Segment Length: " << root.segmentLength() << endl;

//     return 0;
// }


//g++ -std=c++11 "/Users/apple/octrees/linesegment.cpp" -o "/Users/apple/octrees/Program"
//"/Users/apple/octrees/Program"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

using namespace std;

template <typename T>
class GeometricEntity {
public:
    T _x, _y, _z;
    T _x2, _y2, _z2; // Coordinates for the end point of the line segment
    bool isLineSegment;

    // Constructor for a point
    GeometricEntity(T x, T y, T z) 
        : _x(x), _y(y), _z(z), isLineSegment(false) {}

    // Constructor for a line segment
    GeometricEntity(T x1, T y1, T z1, T x2, T y2, T z2) 
        : _x(x1), _y(y1), _z(z1), _x2(x2), _y2(y2), _z2(z2), isLineSegment(true) {}

    double length() const {
        if (isLineSegment) {
            return sqrt(pow(_x2 - _x, 2) + pow(_y2 - _y, 2) + pow(_z2 - _z, 2));
        }
        return 0;
    }
};

template <typename T>
class Node {
public:
    vector<GeometricEntity<T>> entities;
    int totalPoints;  // Total number of points in the node.
    int totalSegments; // Total number of line segments in the node.
    double totalSegmentLength; // Total length of line segments in the node.
    int capacity;
    T _xmin, _ymin, _zmin, _xmax, _ymax, _zmax;
    vector<Node*> children; // A vector of pointers to child nodes.

    Node(const vector<GeometricEntity<T>>& ents, int cap, T xmin, T ymin, T zmin, T xmax, T ymax, T zmax) 
        : entities(ents), capacity(cap), _xmin(xmin), _ymin(ymin), _zmin(zmin), _xmax(xmax), _ymax(ymax), _zmax(zmax), totalPoints(0), totalSegments(0), totalSegmentLength(0) {
            for (const auto& ent : entities) {
                if (ent.isLineSegment) {
                    totalSegments++;
                    totalSegmentLength += ent.length();
                } else {
                    totalPoints++;
                }
            }
    }

    void divide() {
        if (entities.size() > capacity) {
            vector<vector<GeometricEntity<T>>> entityDistribution = distributeEntities(entities); // Distribute entities among child nodes.
            for (int i = 0; i < 8; ++i) { // Create child nodes.
                T newXMin = _xmin + (i & 1) * (_xmax - _xmin) / 2; // Lower or upper half of x-axis.
                T newYMin = _ymin + ((i >> 1) & 1) * (_ymax - _ymin) / 2;
                T newZMin = _zmin + ((i >> 2) & 1) * (_zmax - _zmin) / 2;
                T newXMax = newXMin + (_xmax - _xmin) / 2;
                T newYMax = newYMin + (_ymax - _ymin) / 2;
                T newZMax = newZMin + (_zmax - _zmin) / 2;
                children.push_back(new Node(entityDistribution[i], capacity, newXMin, newYMin, newZMin, newXMax, newYMax, newZMax));
            }
            entities.clear();
        }
    }

    size_t pointSize() const {
        return totalPoints;
    }

    double segmentLength() const {
        return totalSegmentLength;
    }

    void print(int level = 0) const {
        for (int i = 0; i < level; ++i) cout << "--";
        cout << "Node(totalPoints: " << totalPoints 
             << ", pointsSize: " << pointSize() << ", totalSegments: " << totalSegments << ", totalSegmentLength: " << totalSegmentLength << ", capacity: " << capacity 
             << ", bounds: [" << _xmin << "," << _ymin << "," << _zmin 
             << "]-[" << _xmax << "," << _ymax << "," << _zmax << "])\n";
        for (const auto* child : children) {
            child->print(level + 1);
        }
    }

private:
    vector<vector<GeometricEntity<T>>> distributeEntities(const vector<GeometricEntity<T>>& ents) {
        vector<vector<GeometricEntity<T>>> distribution(8);
        for (const auto& ent : ents) {
            if (!ent.isLineSegment) {
                // For points, distribute based on their coordinates.
                int index = (ent._x > (_xmin + _xmax) / 2) + ((ent._y > (_ymin + _ymax) / 2) << 1) + ((ent._z > (_zmin + _zmax) / 2) << 2);
                distribution[index].push_back(ent);
            } 
            else {
                // For line segments, split and distribute based on intersection points.
                auto segments = splitLineSegment(ent);
                for (const auto& seg : segments) {
                    for (int i = 0; i < 8; ++i) {
                        T xMin = _xmin + (i & 1) * (_xmax - _xmin) / 2;
                        T yMin = _ymin + ((i >> 1) & 1) * (_ymax - _ymin) / 2;
                        T zMin = _zmin + ((i >> 2) & 1) * (_zmax - _zmin) / 2;
                        T xMax = xMin + (_xmax - _xmin) / 2;
                        T yMax = yMin + (_ymax - _ymin) / 2;
                        T zMax = zMin + (_zmax - _zmin) / 2;

                        // Check if the split line segment fits within this child node's bounds.
                        if (segmentFitsWithinBox(seg, xMin, yMin, zMin, xMax, yMax, zMax)) {
                            distribution[i].push_back(seg);
                        }
                    }
                }
            }
        }
        return distribution;
    }

    // Function to split a line segment based on child node boundaries.
    vector<GeometricEntity<T>> splitLineSegment(const GeometricEntity<T>& seg) {
        vector<GeometricEntity<T>> splitSegments;

        // Check intersection with each child node's bounding box
        for (int i = 0; i < 8; ++i) {
            T xMin = _xmin + (i & 1) * (_xmax - _xmin) / 2;
            T yMin = _ymin + ((i >> 1) & 1) * (_ymax - _ymin) / 2;
            T zMin = _zmin + ((i >> 2) & 1) * (_zmax - _zmin) / 2;
            T xMax = xMin + (_xmax - _xmin) / 2;
            T yMax = yMin + (_ymax - _ymin) / 2;
            T zMax = zMin + (_zmax - _zmin) / 2;

            // Check if the segment intersects with this child node's bounds
            if (lineSegmentIntersectsBox(seg, xMin, yMin, zMin, xMax, yMax, zMax)) {
                // Calculate intersection points (in a real implementation)
                // For now, we'll assume the segment should be split at midpoint
                auto midpoint = seg.midpoint();

                // Create new segments based on the intersection points
                GeometricEntity<T> newSeg1(seg._x, seg._y, seg._z, midpoint.first, midpoint.second, seg._z2);
                GeometricEntity<T> newSeg2(midpoint.first, midpoint.second, seg._z, seg._x2, seg._y2, seg._z2);

                // Add new segments to the splitSegments vector
                splitSegments.push_back(newSeg1);
                splitSegments.push_back(newSeg2);

                // Break early if necessary, depending on actual intersection logic
                break;
            }
        }

        return splitSegments;
    }

    // Function to check if a line segment fits within a given box
    bool segmentFitsWithinBox(const GeometricEntity<T>& seg, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax) {
        // For demonstration, assuming segment endpoints must be within box bounds
        return (seg._x >= xMin && seg._x <= xMax &&
                seg._y >= yMin && seg._y <= yMax &&
                seg._z >= zMin && seg._z <= zMax &&
                seg._x2 >= xMin && seg._x2 <= xMax &&
                seg._y2 >= yMin && seg._y2 <= yMax &&
                seg._z2 >= zMin && seg._z2 <= zMax);
    }

    // Function to check if a line segment intersects with a box
    bool lineSegmentIntersectsBox(const GeometricEntity<T>& seg, T xMin, T yMin, T zMin, T xMax, T yMax, T zMax) {
        // Simplified check for demonstration purposes.
        // Real implementation should use more precise geometric calculations.
        return (seg._x <= xMax && seg._x2 >= xMin &&
                seg._y <= yMax && seg._y2 >= yMin &&
                seg._z <= zMax && seg._z2 >= zMin);
    }
};

template <typename T>
void createTree(Node<T>& node) {
    if (node.entities.size() > node.capacity) {
        node.divide();
        for (auto* child : node.children) {
            createTree(*child);
        }
    }
}

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

    cout << "\nOctree Structure:\n";
    root.print();

    cout << "\nRoot Node Point Size: " << root.pointSize() << endl;
    cout << "Root Node Total Segment Length: " << root.segmentLength() << endl;

    return 0;
}
