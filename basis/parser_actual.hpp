#ifndef _INCLUDE_PARSER_ACTUAL_HPP
#define _INCLUDE_PARSER_ACTUAL_HPP

#include "Elements.hpp"

namespace basis {

    // whole compilation unit
    std::shared_ptr<Parser> compilation_unit(Sink<std::shared_ptr<CompilationUnit>> d);

    // declaration parsers
    std::shared_ptr<Parser> decl_module(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_import(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_export(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_class(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_record(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_command(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_intrinsic(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_enumeration(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_typeAlias(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_subtype(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_instance(Sink<std::shared_ptr<Declaration>> d);
    std::shared_ptr<Parser> decl_instance(Sink<std::shared_ptr<Declaration>> d);


}
#endif