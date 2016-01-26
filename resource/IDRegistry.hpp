#ifndef ID_REGISTRY_HPP_
#define ID_REGISTRY_HPP_

namespace reprize
{
namespace res
{
class IDRegistry
{
public:
    IDRegistry(void) : size(0)
        {
            get_new(); // get first 0 (NID_NIL)
        }
    virtual ~IDRegistry(void)
        {
        }

    NID get_new(void)
        {
            if (vacant.empty())
            {
                occupied.insert(size);
                return size++;
            }
            NID min = (*vacant.begin());
            vacant.erase(min);
            occupied.insert(min);
            return min;
        }
    void back(const NID id_)
        {
            occupied.erase(id_);
            vacant.insert(id_);
        }
protected:
    std::set<NID> occupied, vacant;
    NID size;
};
}
}

#endif
