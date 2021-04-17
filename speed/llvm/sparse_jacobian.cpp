/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <cstring>
# include <cppad/utility/vector.hpp>

/*
$begin llvm_sparse_jacobian.cpp$$
$spell
    llvm
    jacobian
$$

$section Llvm Speed: sparse_jacobian$$

$head Specifications$$
$cref link_sparse_jacobian$$

$srccode%cpp% */
// A llvm version of this test is not yet available
bool link_sparse_jacobian(
    const std::string&               job      ,
    size_t                           size     ,
    size_t                           repeat   ,
    size_t                           m        ,
    const CppAD::vector<size_t>&     row      ,
    const CppAD::vector<size_t>&     col      ,
    CppAD::vector<double>&           x        ,
    CppAD::vector<double>&           jacobian ,
    size_t&                          n_color  )
{
    return false;
}
/* %$$
$end
*/
