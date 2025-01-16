#ifndef _ELEMENTS_HPP
#define _ELEMENTS_HPP

#include <memory>
#include <vector>

namespace basis {

    class ProgramElement {

    };
    class Declaration : public ProgramElement {

    };
    struct CompilationUnit : public ProgramElement {
        std::vector<std::shared_ptr<Declaration>> declarations;
        CompilationUnit() = default;
    };

    class Import : public Declaration {

    };
    class Module : public Declaration {

    };
    class Export : public Declaration {

    };
    class Class : public Declaration {

    };
    class Record : public Declaration {

    };
    class Command : public Declaration {

    };
    class Intrinsic : public Declaration {

    };
    class Enumeration : public Declaration {

    };
    class TypeAlias : public Declaration {

    };
    class Subtype : public Declaration {

    };
    class Instance : public Declaration {

    };
    class Union : public Declaration {

    };
    class Variant : public Declaration {

    };
    class Object : public Declaration {

    };

}

#endif