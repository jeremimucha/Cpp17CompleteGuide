#include <iostream>
#include <memory_resource>
#include <vector>

#include <pmr_tracker.hpp>

#include "pmr_type.hpp"


int main()
{
    TrackingResource tracker{};
    std::pmr::vector<PmrCustomer> coll(&tracker);   // uses tracker as the allocator
    coll.reserve(100);                              // allocates with the tracker

    PmrCustomer cust1{"Peter, Paul & Mary"};        // allocates with `get_default_resource()`
    // Inserting elements into the vector now depends on the supported allocator,
    // elements can be inserted by move only if the allocators compare equal

    // copy - allocates with vector allocator (tracker)
    coll.push_back(cust1);
    // also copy! - allocators are not equivalent
    coll.push_back(std::move(cust1));

    // if we initialize a customer with the TrackingResource allocator the move will work
    PmrCustomer cust2{"Movable Customer"};
    coll.push_back(std::move(cust2));   // actually moved

    for (auto const& cust : coll) {
        std::cerr << cust.getName() << "\n";
    }
}
