#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstddef>

/// Model to represent a map file
class Model
{
public:
    // Each waypoint, "node" cell in the OSM XML model
    struct Node {
        double x = 0.f;
        double y = 0.f;
    };
    
    // Each collection of nodes is a "way" in OSM terms
    struct Way {
        std::vector<int> nodes;
    };
    
    // a way of type "road"
    struct Road {
        enum Type { Invalid, Unclassified, Service, Residential,
            Tertiary, Secondary, Primary, Trunk, Motorway, Footway };
        int way;
        Type type;
    };
    
    // a way of type "railway"
    struct Railway {
        int way;
    };    
    
    // a shape that may have holes
    struct Multipolygon {
        std::vector<int> outer;
        std::vector<int> inner;
    };
    
    // a way of type "building"
    struct Building : Multipolygon {};
    // a way of type "leisure"
    struct Leisure : Multipolygon {};
    // a way of type "water"
    struct Water : Multipolygon {};
    // a way of type "landuse"
    struct Landuse : Multipolygon {
        enum Type { Invalid, Commercial, Construction, Grass, Forest, Industrial, Railway, Residential };
        Type type;
    };
    
    // constructor signature
    Model( const std::vector<std::byte> &xml );
    
    // accessors
    auto MetricScale() const noexcept { return m_MetricScale; }
    auto &Nodes() const noexcept { return m_Nodes; }
    auto &Ways() const noexcept { return m_Ways; }
    auto &Roads() const noexcept { return m_Roads; }
    auto &Buildings() const noexcept { return m_Buildings; }
    auto &Leisures() const noexcept { return m_Leisures; }
    auto &Waters() const noexcept { return m_Waters; }
    auto &Landuses() const noexcept { return m_Landuses; }
    auto &Railways() const noexcept { return m_Railways; }
    
private:
    void AdjustCoordinates();
    void BuildRings( Multipolygon &mp );
    void LoadData(const std::vector<std::byte> &xml);
    
    // object attributes
    std::vector<Node> m_Nodes;
    std::vector<Way> m_Ways;
    std::vector<Road> m_Roads;
    std::vector<Railway> m_Railways;
    std::vector<Building> m_Buildings;
    std::vector<Leisure> m_Leisures;
    std::vector<Water> m_Waters;
    std::vector<Landuse> m_Landuses;
    // map bounds
    double m_MinLat = 0.;
    double m_MaxLat = 0.;
    double m_MinLon = 0.;
    double m_MaxLon = 0.;
    double m_MetricScale = 1.f;
};
