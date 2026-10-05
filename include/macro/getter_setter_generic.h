#pragma once

/// GENERIC getter/setter layer. No domain knowledge - the member type is
/// always passed in, so the same macros serve any member of any type.
///
/// These generate full DEFINITIONS with a body, so they are used inside a
/// class body (where the body becomes implicitly inline) - nothing goes into a
/// .cpp.
///
/// Naming: ret_type is what the method returns, variable_name the member being
/// written, par_list the already parenthesised parameter list, par_var_name
/// the parameter name the body reads from.

/// setter definition
///   MACRO_SETTER_DECL (Validate &, mainTemp, _mainTemp, (int v), v)
///   ->  Validate &setmainTemp (int v) { this->_mainTemp = v; return *this; }
/// ret_type carries the & itself, so a chaining setter and a plain value
/// setter both work.
#define MACRO_SETTER_DECL(ret_type, method_name, variable_name, par_list,     \
                          par_var_name)                                       \
  ret_type set##method_name par_list                                          \
  {                                                                           \
    this->variable_name = par_var_name;                                       \
    return *this;                                                             \
  }

/// getter definition
///   MACRO_GETTER (int, mainTemp, _mainTemp)
///   ->  int getmainTemp () { return this->_mainTemp; }
#define MACRO_GETTER(ret_type, method_name, variable_name)                    \
  ret_type get##method_name () { return this->variable_name; }

/// setter + getter definition
/// The two return types differ on purpose: the setter hands back the class for
/// chaining, the getter hands back the member type. So setter_ret and
/// getter_ret are separate.
#define MACRO_GETTER_SETTER_DECL(setter_ret, getter_ret, method_name,         \
                                 variable_name, par_list, par_var_name)       \
  MACRO_SETTER_DECL (setter_ret, method_name, variable_name, par_list,        \
                     par_var_name);                                           \
  MACRO_GETTER (getter_ret, method_name, variable_name);

/// ---- variants for a class that overrides a virtual from a base interface
/// ---- Same definitions, plus the override keyword. The keyword is only legal
/// on the in-class declaration, and an in-class definition counts as that, so
/// it goes in the same place.

#define MACRO_SETTER_DECL_OVERRIDE(ret_type, method_name, variable_name,      \
                                   par_list, par_var_name)                    \
  ret_type set##method_name par_list override                                 \
  {                                                                           \
    this->variable_name = par_var_name;                                       \
    return *this;                                                             \
  }

#define MACRO_GETTER_OVERRIDE(ret_type, method_name, variable_name)           \
  ret_type get##method_name () override { return this->variable_name; }

#define MACRO_GETTER_SETTER_DECL_OVERRIDE(setter_ret, getter_ret,             \
                                          method_name, variable_name,         \
                                          par_list, par_var_name)             \
  MACRO_SETTER_DECL_OVERRIDE (setter_ret, method_name, variable_name,         \
                              par_list, par_var_name);                        \
  MACRO_GETTER_OVERRIDE (getter_ret, method_name, variable_name);

/// ---- pointer member variants ----
/// A member declared as "Type *Member" needs the opposite treatment: the
/// setter stores the ADDRESS of its argument, and the getter hands back what
/// it points at rather than the pointer. Using the value variants on a pointer
/// member produces "invalid conversion from 'Sensor*' to 'Sensor&'".

#define MACRO_SETTER_PTR_DECL_OVERRIDE(ret_type, method_name, variable_name,  \
                                       par_list, par_var_name)                \
  ret_type set##method_name par_list override                                 \
  {                                                                           \
    this->variable_name = &par_var_name;                                      \
    return *this;                                                             \
  }

#define MACRO_GETTER_PTR_DECL_OVERRIDE(ret_type, method_name, variable_name)  \
  ret_type get##method_name () override { return *this->variable_name; }

#define MACRO_GETTER_SETTER_PTR_DECL_OVERRIDE(setter_ret, getter_ret,         \
                                              method_name, variable_name,     \
                                              par_list, par_var_name)         \
  MACRO_SETTER_PTR_DECL_OVERRIDE (setter_ret, method_name, variable_name,     \
                                  par_list, par_var_name);                    \
  MACRO_GETTER_PTR_DECL_OVERRIDE (getter_ret, method_name, variable_name);
