# ifndef CPPAD_LOCAL_SWEEP_CALL_ATOMIC_HPP
# define CPPAD_LOCAL_SWEEP_CALL_ATOMIC_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/local/atomic_index.hpp>
# include <cppad/core/atomic/atomic_two.hpp>
# include <cppad/core/atomic/atomic_three.hpp>

// BEGIN_CPAPD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file call_atomic.hpp
Callbacks to atomic functions corresponding to atomic_index.
*/
// ----------------------------------------------------------------------------
/*!
Forward mode callback to atomic functions.

\tparam Base
Is the type corresponding to the Taylor coefficients.

\tparam RecBase
Is the type corresponding to this atomic function.

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param type_x [in]
if is not size zero, which components of x are
constants, dynamics, and variables.

\param type_y [out]
if size not zero, which components of y are variables

\param taylor_x [in]
Taylor coefficients corresponding to x.

\param taylor_y [out]
Taylor coefficient corresponding to y.
*/
template <class Base, class RecBase>
void call_atomic_forward(
    size_t                       order_low  ,
    size_t                       order_up   ,
    size_t                       atom_index ,
    size_t                       atom_old   ,
    const vector<ad_type_enum>&  type_x     ,
    vector<ad_type_enum>&        type_y     ,
    const vector<Base>&          taylor_x   ,
    vector<Base>&                taylor_y   )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type     = 0;          // set to avoid warning
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr    = CPPAD_NULL; // set to avoid warning
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
# ifndef NDEBUG
    bool ok = v_ptr != CPPAD_NULL;
    if( ok )
    {
        if( type == 2 )
        {   atomic_base<RecBase>* afun =
                reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
            afun->set_old(atom_old);
            ok = afun->forward(
                order_low, order_up, type_x, type_y, taylor_x, taylor_y
            );
        }
        else
        {   CPPAD_ASSERT_UNKNOWN( type == 3 );
            atomic_three<RecBase>* afun =
                reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
            ok = afun->forward(
                order_low, order_up, type_x, type_y, taylor_x, taylor_y
            );
        }
    }
    if( ! ok )
    {   // now take the extra time to copy the name
        std::string name;
        local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
        std::string msg = name;
        if( v_ptr == CPPAD_NULL )
            msg += ": this atomic_three function has been deleted";
        else
            msg += ": atomic forward returned false";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# else
    if( type == 2 )
    {   atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
        afun->set_old(atom_old);
        afun->forward(
            order_low, order_up, type_x, type_y, taylor_x, taylor_y
        );
    }
    else
    {   atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
        afun->forward(
            order_low, order_up, type_x, type_y, taylor_x, taylor_y
        );
    }
# endif
}
// ----------------------------------------------------------------------------
/*!
Reverse mode callback to atomic functions.

\tparam Base
Is the type corresponding to the Taylor coefficients.

\tparam RecBase
Is the type corresponding to this atomic function.

\param order_up [in]
highest order for this reverse mode calculation.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param taylor_x [in]
Taylor coefficients corresponding to x.

\param taylor_y [in]
Taylor coefficient corresponding to y.

\param partial_x [out]
Partials w.r.t the x Taylor coefficients.

\param partial_y [in]
Partials w.r.t the y Taylor coefficients.
*/
template <class Base, class RecBase>
void call_atomic_reverse(
    size_t                       order_up   ,
    size_t                       atom_index ,
    size_t                       atom_old   ,
    const vector<Base>&          taylor_x   ,
    const vector<Base>&          taylor_y   ,
    vector<Base>&                partial_x  ,
    const vector<Base>&          partial_y  )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type     = 0;          // set to avoid warning
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr    = CPPAD_NULL; // set to avoid warning
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
# ifndef NDEBUG
    bool ok = v_ptr != CPPAD_NULL;
    if( ok )
    {
        if( type == 2 )
        {   atomic_base<RecBase>* afun =
                reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
            afun->set_old(atom_old);
            ok = afun->reverse(
                order_up, taylor_x, taylor_y, partial_x, partial_y
            );
        }
        else
        {   CPPAD_ASSERT_UNKNOWN( type == 3 );
            atomic_three<RecBase>* afun =
                reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
            ok = afun->reverse(
                order_up, taylor_x, taylor_y, partial_x, partial_y
            );
        }
    }
    if( ! ok )
    {   // now take the extra time to copy the name
        std::string name;
        local::atomic_index<RecBase>(set_null, atom_index, type, &name, v_ptr);
        std::string msg = name;
        if( v_ptr == CPPAD_NULL )
            msg += ": this atomic_three function has been deleted";
        else
            msg += ": atomic reverse returned false";
        CPPAD_ASSERT_KNOWN(false, msg.c_str() );
    }
# else
    if( type == 2 )
    {   atomic_base<RecBase>* afun =
            reinterpret_cast< atomic_base<RecBase>* >(v_ptr);
        afun->set_old(atom_old);
        afun->reverse(
            order_up, taylor_x, taylor_y, partial_x, partial_y
        );
    }
    else
    {   atomic_three<RecBase>* afun =
            reinterpret_cast< atomic_three<RecBase>* >(v_ptr);
        afun->reverse(
            order_up, taylor_x, taylor_y, partial_x, partial_y
        );
    }
# endif
}
// ----------------------------------------------------------------------------
/*!
Forward Jacobian sparsity callback to atomic functions.

\tparam Base
is the type corresponding to parameter_x
and to this atomic function.

\tparam InternalSparsity
is the internal type used to represent sparsity; i.e.,
sparse_pack or sparse_list.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param parameter_x [in]
value of the parameter arguments to the atomic function
(other arguments have the value nan).

\param x_index [in]
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

\param y_index [in]
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

\param var_sparsity [in/out]
On input, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the sparsity for the j-th argument to this atomic function.
On output, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the j-th result for this atomic function.
*/
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_for_jac_sparsity(
    size_t                       atom_index    ,
    size_t                       atom_old      ,
    const vector<Base>&          parameter_x   ,
    const pod_vector<size_t>&    x_index       ,
    const pod_vector<size_t>&    y_index       ,
    InternalSparsity&            var_sparsity  )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type     = 0;          // set to avoid warning
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr    = CPPAD_NULL; // set to avoid warning
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
    CPPAD_ASSERT_UNKNOWN( type == 2 );
    //
    atomic_base<Base>* afun = reinterpret_cast< atomic_base<Base>* >(v_ptr);
    afun->set_old(atom_old);
    afun->for_sparse_jac(
        parameter_x, x_index, y_index, var_sparsity
    );
}
// ----------------------------------------------------------------------------
/*!
Reverse Jacobian sparsity callback to atomic functions.

\tparam Base
is the type corresponding to parameter_x
and to this atomic function.

\tparam InternalSparsity
is the internal type used to represent sparsity; i.e.,
sparse_pack or sparse_list.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param parameter_x [in]
value of the parameter arguments to the atomic function
(other arguments have the value nan).

\param x_index [in]
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

\param y_index [in]
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

\param var_sparsity [in/out]
On input, for i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the sparsity for the i-th argument to this atomic function.
On output, for j = 0, ... , n-1, the sparsity pattern with index x_index[j],
the sparsity has been updated to remove y as a function of x.
*/
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_rev_jac_sparsity(
    size_t                       atom_index    ,
    size_t                       atom_old      ,
    const vector<Base>&          parameter_x   ,
    const pod_vector<size_t>&    x_index       ,
    const pod_vector<size_t>&    y_index       ,
    InternalSparsity&            var_sparsity  )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type     = 0;          // set to avoid warning
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr    = CPPAD_NULL; // set to avoid warning
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
    CPPAD_ASSERT_UNKNOWN( type == 2 );
    //
    atomic_base<Base>* afun = reinterpret_cast< atomic_base<Base>* >(v_ptr);
    afun->set_old(atom_old);
    afun->rev_sparse_jac(
        parameter_x, x_index, y_index, var_sparsity
    );
}
// ----------------------------------------------------------------------------
/*!
Forward Hessian sparsity callback to atomic functions.

\tparam Base
is the type corresponding to parameter_x
and to this atomic function.

\tparam InternalSparsity
is the internal type used to represent sparsity; i.e.,
sparse_pack or sparse_list.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param parameter_x [in]
value of the parameter arguments to the atomic function
(other arguments have the value nan).

\param x_index [in]
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

\param y_index [in]
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

\param for_jac_sparsity
For j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the forward Jacobian sparsity for the j-th argument to this atomic function.

\param rev_jac_sparsity
For i = 0, ... , m-1, the sparsity pattern with index y_index[i],
is the reverse Jacobian sparsity for the i-th result to this atomic function.
This shows which components of the result affect the function we are
computing the Hessian of.

\param for_hes_sparsity
This is the sparsity pattern for the Hessian. On input, the non-linear
terms in the atomic fuction have not been included. Upon return, they
have been included.
*/
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_for_hes_sparsity(
    size_t                       atom_index        ,
    size_t                       atom_old          ,
    const vector<Base>&          parameter_x       ,
    const pod_vector<size_t>&    x_index           ,
    const pod_vector<size_t>&    y_index           ,
    const InternalSparsity&      for_jac_sparsity  ,
    const InternalSparsity&      rev_jac_sparsity  ,
    InternalSparsity&            for_hes_sparsity  )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type     = 0;          // set to avoid warning
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr    = CPPAD_NULL; // set to avoid warning
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
    CPPAD_ASSERT_UNKNOWN( type == 2 );
    //
    atomic_base<Base>* afun = reinterpret_cast< atomic_base<Base>* >(v_ptr);
    afun->set_old(atom_old);
    afun->for_sparse_hes(
        parameter_x,
        x_index,
        y_index,
        for_jac_sparsity,
        rev_jac_sparsity,
        for_hes_sparsity
    );
}
// ----------------------------------------------------------------------------
/*!
Reverse Hessian sparsity callback to atomic functions.

\tparam Base
is the type corresponding to parameter_x
and to this atomic function.

\tparam InternalSparsity
is the internal type used to represent sparsity; i.e.,
sparse_pack or sparse_list.

\param atom_index [in]
is the index, in local::atomic_index, corresponding to this atomic function.

\param atom_old [in]
is the extra id information for this atomic function in the atomic_one case.

\param parameter_x [in]
value of the parameter arguments to the atomic function
(other arguments have the value nan).

\param x_index [in]
is a mapping from the index of an atomic function argument
to the corresponding variable on the tape.

\param y_index [in]
is a mapping from the index of an atomic function result
to the corresponding variable on the tape.

\param for_jac_sparsity
For j = 0, ... , n-1, the sparsity pattern with index x_index[j],
is the forward Jacobian sparsity for the j-th argument to this atomic function.

\param rev_jac_flag
On input, for i = 0, ... , m-1, rev_jac_flag[ y_index[i] ] is true
if the fuction (we are computing the sparsity for)
depends on the variable y_index[i].
Upon return, for j = 0, ..., n-1, rev_jac_flag[ x_index[j] ] has been set to
true any of the y_index variables are flagged depnend on x_index[j].
Otherwise, rev_jac_flag[ x_index[j] ] is not modified.

\param rev_hes_sparsity
This is the sparsity pattern for the Hessian.
On input, for i = 0, ... , m-1, row y_index[i] is the reverse Hessian sparsity
with one of the partials with respect to to y_index[i].
Upon return, for j = 0, ..., n-1, the row x_index[j] has been
modified to include components that have a non-zero hessian through
the atomic fucntion with one of the partials w.r.t. x_index[j].
*/
template <class Base, class RecBase, class InternalSparsity>
void call_atomic_rev_hes_sparsity(
    size_t                       atom_index        ,
    size_t                       atom_old          ,
    const vector<Base>&          parameter_x       ,
    const pod_vector<size_t>&    x_index           ,
    const pod_vector<size_t>&    y_index           ,
    const InternalSparsity&      for_jac_sparsity  ,
    bool*                        rev_jac_flag      ,
    InternalSparsity&            rev_hes_sparsity  )
{   CPPAD_ASSERT_UNKNOWN( 0 < atom_index );
    bool         set_null = false;
    size_t       type     = 0;          // set to avoid warning
    std::string* name_ptr = CPPAD_NULL;
    void*        v_ptr    = CPPAD_NULL; // set to avoid warning
    local::atomic_index<RecBase>(set_null, atom_index, type, name_ptr, v_ptr);
    CPPAD_ASSERT_UNKNOWN( type == 2 );
    //
    atomic_base<Base>* afun = reinterpret_cast< atomic_base<Base>* >(v_ptr);
    afun->set_old(atom_old);
    afun->rev_sparse_hes(
        parameter_x,
        x_index,
        y_index,
        for_jac_sparsity,
        rev_jac_flag,
        rev_hes_sparsity
    );
}


} } } // END_CPAPD_LOCAL_SWEEP_NAMESPACE
# endif