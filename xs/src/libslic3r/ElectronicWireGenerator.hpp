#ifndef slic3r_ElectronicWireGenerator_hpp_
#define slic3r_ElectronicWireGenerator_hpp_

#include "libslic3r.h"
#include "ElectronicRoutingGraph.hpp"
#include "RubberBand.hpp"
#include "Layer.hpp"
#include "Print.hpp"
#include "Polyline.hpp"
#include "Polygon.hpp"
#include "ExPolygonCollection.hpp"
#include  "SVG.hpp"


namespace Slic3r {

class ElectronicWireGenerator;
class ElectronicWireRouter;
typedef std::vector<ElectronicWireGenerator> ElectronicWireGenerators;
typedef std::vector<ElectronicWireGenerator*> ElectronicWireGeneratorPtrs;

struct WireSegment {
    Line line;
    std::vector<std::pair<Line, double> > connecting_lines;
    std::set<routing_edge_t> edges;
};
typedef std::vector<WireSegment> WireSegments;

class ElectronicWireGenerator {
public:
    ElectronicWireGenerator(
        /// Input:
        /// pointer to layer object on which wires will be refined
        Layer*                     layer,
        ElectronicWireGenerator*   previous_ewg,
        double                     extrusion_width,
        double                     extrusion_overlap,
        double                     first_extrusion_overlap,
        double                     overlap_min_extrusion_length,
        double                     conductive_wire_channel_width);

    const coordf_t get_print_z() const;
    const coord_t get_scaled_print_z() const;
    const coordf_t get_bottom_z() const;
    const coordf_t get_scaled_bottom_z() const;
    ExPolygonCollections* get_contour_set();
    WireSegments get_wire_segments(Lines& wire, const double routing_perimeter_factor, const double routing_hole_factor);
    void add_routed_wire(Polyline &routed_wire);

    void generate_wires();

private:
    void sort_unrouted_wires();
    void channel_from_wire(Polyline &wire);
    void apply_overlap(Polylines input, Polylines *output);
    coord_t offset_width(const LayerRegion* region, int perimeters) const;
    void _align_to_prev_perimeters();
    //void polygon_to_graph(const Polygon& p, routing_graph_t* g, point_index_t* index, const double& weight_factor = 1.0) const;

    // Inputs:
    Layer* layer;             ///< pointer to layer object
    ElectronicWireGenerator* previous_ewg;  ///< pointer to previous wire generator to align polygons
    Polylines unrouted_wires; ///< copy of original path collection,
                              /// will be deleted during process()
    Polylines routed_wires;   ///< intermediate state after contour following
    double extrusion_width;
    double extrusion_overlap;
    double first_extrusion_overlap;
    double overlap_min_extrusion_length;
    double conductive_wire_channel_width;
    int max_perimeters;
    const ExPolygonCollection* slices;
    ExPolygonCollections deflated_slices;

    friend class ElectronicWireRouter;
};


class ElectronicWireRouter {
public:
    ElectronicWireRouter(
            const double layer_overlap,
            const double routing_astar_factor,
            const double routing_perimeter_factor,
            const double routing_hole_factor,
            const double routing_interlayer_factor,
            const int layer_count);
    void append_wire_generator(ElectronicWireGenerator& ewg);
    ElectronicWireGenerator* last_ewg();
    void route(const RubberBand* rb, const Point3 offset);
    void generate_wires();

private:
    coord_t _map_z_to_layer(coord_t z) const;

    ElectronicWireGenerators ewgs;
    std::map<coord_t, ElectronicWireGenerator*> z_map;
    const double layer_overlap;
    const double routing_astar_factor;
    const double routing_perimeter_factor;
    const double routing_hole_factor;
    const double routing_interlayer_factor;
};

}

#endif
