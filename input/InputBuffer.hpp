#ifndef INPUT_BUFFER_HPP_
#define INPUT_BUFFER_HPP_

namespace reprize
{
namespace inp
{
class InputBuffer
{
public:
    InputBuffer(void) {}
    virtual ~InputBuffer(void) {}

    inline const bool init(void) { reset(); return true; }
    inline void reset(void) { buffer.clear(); itr = buffer.end(); }

    virtual const InputCode Current(void) const
        { return static_cast<InputCode>(*itr); }
    virtual void First(void) { itr = buffer.begin(); }
    virtual void Next(void) { ++itr; }
    virtual const bool IsDone(void) const { return (itr == buffer.end()); }
    virtual const bool IsEmpty(void) const { return buffer.empty(); }

    inline void Add(InputCode keycode_) { buffer.push_back(keycode_); }

protected:
    std::vector<InputCode> buffer;
    std::vector<InputCode>::iterator itr;
};
}
}
#endif
