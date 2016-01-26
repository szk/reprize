#ifndef PARAGRAPH_HPP_
#define PARAGRAPH_HPP_

namespace reprize
{
namespace res
{
class Paragraph : public Node
{
public:
    Paragraph(const Str name_, Size32 max_ln_ = 512)
        : Node(name_), max_ln(max_ln_), updated_ln(0), idx_gap(0), overall_ln(0)
        {
            for (Size32 i = 0; max_ln_ > i; ++i) { history.push_back(""); }
        }
    virtual ~Paragraph(void) {}

    Size32 printf(const Char* msg_, ...)
        {
            va_list ap;
            Char buffer[MAX_CONSOLE_CHAR];
            va_start(ap, msg_);
            vsnprintf(buffer, MAX_CONSOLE_CHAR, msg_, ap);
            va_end(ap);

#ifdef WIN32
            OutputDebugStringA((LPCSTR)buffer);
            OutputDebugStringA("\r\n");
#else
            std::cerr << buffer << std::endl;
#endif
            if (buffer == NULL) { return 1; }
            if (++idx_gap >= max_ln) { idx_gap = 0; }

            history[idx_gap].assign(buffer);

            // Overflow XXX
            ++updated_ln;
            ++overall_ln;

            return 0;
        }
    Size32 get_overall_ln_n(void) { return overall_ln; }
    const Str& get_line(Size32 n_) { return history.at(n_); }
    const Str& tail(Size32 history_idx_)
        { return history[(idx_gap - history_idx_) % max_ln]; }
    void refresh(void) { updated_ln = 0; }

protected:
    std::vector<Str> history;
    const Size32 max_ln;
    Size32 updated_ln, idx_gap, overall_ln;
    std::ofstream of;
};
}
}

#endif
