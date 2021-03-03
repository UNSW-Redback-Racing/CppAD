/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
------------------------------------------------------------------------------
$begin llvm_ir_ctor$$
$spell
    llvm_ir
    obj
$$

$section LLVM Intermediate Representation Object Constructor$$

$head Syntax$$
$codei%llvm_ir %ir_obj%;%$$

$head Purpose$$
The creates the empty $code llvm_ir$$ object $icode ir_obj$$.

$end
*/
# include <cppad/core/llvm_ir.hpp>
//
namespace CppAD { // BEGIN_CPPAD_NAMESPACE

llvm_ir::llvm_ir(void)
:
function_name_(""),
n_dynamic_ind_(0),
n_variable_ind_(0),
n_variable_dep_(0),
context_ir_(nullptr),
module_ir_(nullptr)
{ }

} // END_CPPAD_NAMESPACE
