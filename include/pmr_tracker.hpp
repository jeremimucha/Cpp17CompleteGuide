#if !defined(CPP17_PMR_TRACKER_INCLUDE_HEADER_GUARD_)
#define CPP17_PMR_TRACKER_INCLUDE_HEADER_GUARD_

#include <iostream>
#include <string>
#include <memory_resource>

// std::pmr::memory_resource is the base class for all pmr resources
class TrackingResource : public std::pmr::memory_resource
{
private:
// pmr::memory_resources normally support passing another memory resource,
// to wrap it (add functionality), or use as fallback
    std::pmr::memory_resource* upstream_{std::pmr::get_default_resource()};
    std::string prefix_{};

public:
    TrackingResource() = default;
    explicit TrackingResource(std::string p) noexcept
        : prefix_{std::move(p)} { }
    explicit TrackingResource(std::string p, std::pmr::memory_resource* us)
        : upstream_{us}, prefix_{std::move(p)} { }

private:
    // implementing a memory_resource requires us to implement the following three, private virtual
    // member functions:
    void* do_allocate(std::size_t bytes, std::size_t align) override
    {
        std::cerr << prefix_ << "allocate " << bytes << " Bytes\n";
        return upstream_->allocate(bytes, align);
    }

    void do_deallocate(void* ptr, std::size_t bytes, std::size_t align) override
    {
        std::cerr << prefix_ << "deallocate " << bytes << " Bytes\n";
        upstream_->deallocate(ptr, bytes, align);
    }

    bool do_is_equal(std::pmr::memory_resource const& other) const noexcept override
    {
        // is same object?
        if (this == &other) { return true; }
        // same type and equal prefix and equal upstream?
        auto const op = dynamic_cast<const TrackingResource* const>(&other);
        return op != nullptr && op->prefix_ == prefix_ && upstream_->is_equal(other);
    }
};

#endif // CPP17_PMR_TRACKER_INCLUDE_HEADER_GUARD_
