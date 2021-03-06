#ifndef SEARCH_ENGINE_DATA_HPP
#define SEARCH_ENGINE_DATA_HPP

#include <boost/thread/tss.hpp>

#include "engine/algorithm.hpp"
#include "util/binary_heap.hpp"
#include "util/typedefs.hpp"

namespace osrm
{
namespace engine
{

struct HeapData
{
    NodeID parent;
    /* explicit */ HeapData(NodeID p) : parent(p) {}
};

struct ManyToManyHeapData : HeapData
{
    EdgeWeight duration;
    ManyToManyHeapData(NodeID p, EdgeWeight duration) : HeapData(p), duration(duration) {}
};

template <typename Algorithm> struct SearchEngineData
{
    using QueryHeap = util::
        BinaryHeap<NodeID, NodeID, EdgeWeight, HeapData, util::UnorderedMapStorage<NodeID, int>>;
    using SearchEngineHeapPtr = boost::thread_specific_ptr<QueryHeap>;

    using ManyToManyQueryHeap = util::BinaryHeap<NodeID,
                                                 NodeID,
                                                 EdgeWeight,
                                                 ManyToManyHeapData,
                                                 util::UnorderedMapStorage<NodeID, int>>;

    using ManyToManyHeapPtr = boost::thread_specific_ptr<ManyToManyQueryHeap>;

    static SearchEngineHeapPtr forward_heap_1;
    static SearchEngineHeapPtr reverse_heap_1;
    static SearchEngineHeapPtr forward_heap_2;
    static SearchEngineHeapPtr reverse_heap_2;
    static SearchEngineHeapPtr forward_heap_3;
    static SearchEngineHeapPtr reverse_heap_3;
    static ManyToManyHeapPtr many_to_many_heap;

    void InitializeOrClearFirstThreadLocalStorage(unsigned number_of_nodes);

    void InitializeOrClearSecondThreadLocalStorage(unsigned number_of_nodes);

    void InitializeOrClearThirdThreadLocalStorage(unsigned number_of_nodes);

    void InitializeOrClearManyToManyThreadLocalStorage(unsigned number_of_nodes);
};

struct MultiLayerDijkstraHeapData
{
    NodeID parent;
    bool from_clique_arc;
    MultiLayerDijkstraHeapData(NodeID p) : parent(p), from_clique_arc(false) {}
    MultiLayerDijkstraHeapData(NodeID p, bool from) : parent(p), from_clique_arc(from) {}
};

template <> struct SearchEngineData<routing_algorithms::mld::Algorithm>
{
    using QueryHeap = util::BinaryHeap<NodeID,
                                       NodeID,
                                       EdgeWeight,
                                       MultiLayerDijkstraHeapData,
                                       util::UnorderedMapStorage<NodeID, int>>;

    using SearchEngineHeapPtr = boost::thread_specific_ptr<QueryHeap>;

    static SearchEngineHeapPtr forward_heap_1;
    static SearchEngineHeapPtr reverse_heap_1;
    static SearchEngineHeapPtr forward_heap_2;
    static SearchEngineHeapPtr reverse_heap_2;

    void InitializeOrClearFirstThreadLocalStorage(unsigned number_of_nodes);

    void InitializeOrClearSecondThreadLocalStorage(unsigned number_of_nodes);
};
}
}

#endif // SEARCH_ENGINE_DATA_HPP
