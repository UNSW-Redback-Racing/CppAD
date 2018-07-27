# ifndef CPPAD_LOCAL_SWEEP_DYNAMIC_HPP
# define CPPAD_LOCAL_SWEEP_DYNAMIC_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/play/user_op_info.hpp>

// BEGIN_CPPAD_LOCAL_SWEEP_NAMESPACE
namespace CppAD { namespace local { namespace sweep {
/*!
\file sweep/dynamic.hpp
Under Consruction
*/

/*
\def CPPAD_ATOMIC_CALL
This avoids warnings when NDEBUG is defined and user_ok is not used.
If NDEBUG is defined, this resolves to
\code
	user_atom->forward
\endcode
otherwise, it respolves to
\code
	user_ok = user_atom->forward
\endcode
This maco is undefined at the end of this file to facillitate is
use with a different definition in other files.
*/
# ifdef NDEBUG
# define CPPAD_ATOMIC_CALL user_atom->forward
# else
# define CPPAD_ATOMIC_CALL user_ok = user_atom->forward
# endif

/*!
\def CPPAD_DYNAMIC_TRACE
This value is either zero or one.
Zero is the normal operational value.
If it is one, a trace of every forward0sweep computation is printed.
(Note that forward0sweep is not used if CPPAD_USE_DYNAMIC is zero).
*/
# define CPPAD_DYNAMIC_TRACE 0

/*!
Compute dynamic parameters.

\tparam Base
The type of the parameters.

\param num_ind_dynamic
number of independent dynamic parameters

\param par_vec
the vector of all the parameters are inputs.
The other dynamic parameters are outputs.
All of the constant parmaeters are inputs.

\param dyn_par_is
is a vector with the same length as par_vec.
The i-th parameter is dynamic if and only if dyn_par_is[i] is true.

\param dyn_par_op
is a vector with length equal to the number of dynamic parameters.
The element dyn_par_op_[j] is the operator for the j-th dynamic parameter.
Note that if dyn_par_is[i] is false, the i-th parameter does not
have a parameter in this list.

\param dyn_par_arg
is a vector containing the arguments for the dynamic parameters.
The first argument for the j-th dynamic parameter is dyn_par_arg[k]
where
\code
	k = NumArg( dyn_par_op[0] ) + ... + NumArg( dyn_par_op[j-1] )
\endcode
The arguments for each dynamic parameter have index value
lower than the index value for the parameter.
*/
template <class Base>
void dynamic(
	size_t                        num_ind_dynamic ,
	pod_vector_maybe<Base>&       par_vec         ,
	const pod_vector<bool>&       dyn_par_is      ,
	const pod_vector<opcode_t>&   dyn_par_op      ,
	const pod_vector<addr_t>&     dyn_par_arg     )
{
# ifndef NDEBUG
	for(size_t j = 0; j < num_ind_dynamic; ++j)
		CPPAD_ASSERT_UNKNOWN(
			dyn_par_is[j] && OpCode( dyn_par_op[j] ) == InvOp
	);
# endif
	size_t i_op  = num_ind_dynamic;
	size_t i_arg = 0;
	for(size_t i = num_ind_dynamic; i < par_vec.size(); ++i)
	if( dyn_par_is[i] )
	{	OpCode op = OpCode( dyn_par_op[i_op++];
# ifndef NDEBUG
		for(size_t j = 0; j < NumArg(op); ++j)
			CPPAD_ASSERT_UNKNOWN( size_t( dyn_par_arg[i_arg + j] ) < i );
# endif
		switch(op)
		{	case AbsOp:
			par_vec[i] = abs( dyn_par_arg[i_arg+0] );
			break;

			default:
			CPPAD_ASSERT_KNOWN(false, OpName(op) );
			break;
		}
		i_arg += NumArg(op);
	}
	return;
}

} } } // END_CPPAD_LOCAL_SWEEP_NAMESPACE