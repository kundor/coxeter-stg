#ifndef TEXOUT_H
#define TEXOUT_H

#include <iosfwd>
//#include <string> // forward-declared in iosfwd, for gcc
#include <set>
#include <boost/dynamic_bitset_fwd.hpp>

class TeXout {
    std::string doc_class;
    std::set<std::string> classopts;
    std::set<std::string> packages;
    std::set<std::string> tikzlibraries;
    std::string preamble;
    std::string doc;

    public:
    TeXout(std::string docuclass = "standalone") : doc_class{docuclass} {}

    void documentclass(std::string doc_class);
    void classopt(std::string opt);
    void usepackage(std::string package);
    void usetikzlibrary(std::string lib);
    void addtopreamble(std::string pre);

    TeXout& operator<<(std::string s);
    TeXout& operator<<(double d); // fixed precision, 1 digit after decimal point
    TeXout& operator<<(int i);
    TeXout& operator<<(unsigned u);
    TeXout& operator<<(long unsigned u);
    TeXout& operator<<(boost::dynamic_bitset<> b); // wrong-endian hex

    std::ostream& tostream(std::ostream&);
};

inline std::ostream& operator<<(std::ostream& os, TeXout& t) {
    return t.tostream(os);
}

/* env_wrap manipulator, wraps the next input in a LaTeX environment */
/* Use: tex << "La la" << env_wrap{"displaymath"} << object << "whee"; */
struct EnvironmentWrapper {
    TeXout& t;
    std::string env;
};

struct env_wrap {
    std::string env;
};

inline EnvironmentWrapper operator<<(TeXout& t, const env_wrap& e) {
    return {t, e.env};
}

template<typename T>
TeXout& operator<<(EnvironmentWrapper&& ew, const T& data) {
    return ew.t << ("\\begin{" + ew.env + "}\n")
                << data
                << ("\\end{" + ew.env + "}\n");
}

#endif //TEXOUT_H
