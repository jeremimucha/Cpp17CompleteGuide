#if !defined(PMR_TYPE_INCLUDE_HEADER_GUARD_)
#define PMR_TYPE_INCLUDE_HEADER_GUARD_

#include <string>
#include <memory_resource>

// a polymorphic-allocator-aware type Customer
// - the allocator is stored in the string member
class PmrCustomer
{
private:
    std::pmr::string name_;

public:
    using allocator_type = std::pmr::polymorphic_allocator<char>;   // could use std::byte here

    // initializing constructor(s)
    PmrCustomer(std::pmr::string s, allocator_type alloc = {})
        : name_{std::move(s), alloc} { }

    // copy/move constructors also need to be provided with allocators:
    PmrCustomer(PmrCustomer const& other, allocator_type alloc)
        : name_{other.name_, alloc} { }
    PmrCustomer(PmrCustomer&& other, allocator_type alloc)
        : name_{std::move(other.name_), alloc} { }

    // setters/getters
    void setName(std::pmr::string s)
    {
        name_ = std::move(s);
    }

    std::pmr::string getName() const
    {
        return name_;
    }

    // provide an explicit way to get a non-pmr representation
    std::string getNameAsString() const
    {
        return std::string{name_};
    }
};

#endif // PMR_TYPE_INCLUDE_HEADER_GUARD_
