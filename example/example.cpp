/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin example.cpp$$
$spell
	Cpp
$$

$section CppAD Examples and Tests$$

$head Running Tests$$
To build this program and run its correctness tests see $cref cmake_check$$.

$code
$srcfile%example/example.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// standard string
# include <string>

// memory utility
# include <cppad/utility/thread_alloc.hpp>

// prototype external compiled tests (this line expected by bin/new_test.sh)
extern bool abort_recording(void);
extern bool fabs(void);
extern bool acosh(void);
extern bool acos(void);
extern bool ad_assign(void);
extern bool ad_ctor(void);
extern bool AddEq(void);
extern bool Add(void);
extern bool ad_fun(void);
extern bool ad_in_c(void);
extern bool ad_input(void);
extern bool ad_output(void);
extern bool asinh(void);
extern bool asin(void);
extern bool atan2(void);
extern bool atanh(void);
extern bool atan(void);
extern bool azmul(void);
extern bool base_require(void);
extern bool BenderQuad(void);
extern bool BoolFun(void);
extern bool capacity_order(void);
extern bool change_param(void);
extern bool check_for_nan(void);
extern bool compare_change(void);
extern bool Compare(void);
extern bool complex_poly(void);
extern bool CondExp(void);
extern bool Cosh(void);
extern bool Cos(void);
extern bool DivEq(void);
extern bool Div(void);
extern bool eigen_array(void);
extern bool eigen_det(void);
extern bool EqualOpSeq(void);
extern bool Erf(void);
extern bool expm1(void);
extern bool exp(void);
extern bool ForOne(void);
extern bool ForTwo(void);
extern bool forward_dir(void);
extern bool forward_order(void);
extern bool Forward(void);
extern bool fun_assign(void);
extern bool FunCheck(void);
extern bool HesLagrangian(void);
extern bool HesLuDet(void);
extern bool HesMinorDet(void);
extern bool Hessian(void);
extern bool HesTimesDir(void);
extern bool Independent(void);
extern bool Integer(void);
extern bool Interface2C(void);
extern bool interp_onetape(void);
extern bool interp_retape(void);
extern bool JacLuDet(void);
extern bool JacMinorDet(void);
extern bool Jacobian(void);
extern bool log10(void);
extern bool log1p(void);
extern bool log(void);
extern bool LuRatio(void);
extern bool LuVecADOk(void);
extern bool MulEq(void);
extern bool mul_level_adolc_ode(void);
extern bool mul_level_adolc(void);
extern bool mul_level_ode(void);
extern bool mul_level(void);
extern bool Mul(void);
extern bool NearEqualExt(void);
extern bool number_skip(void);
extern bool NumericType(void);
extern bool num_limits(void);
extern bool OdeStiff(void);
extern bool ode_taylor(void);
extern bool opt_val_hes(void);
extern bool ParVar(void);
extern bool Poly(void);
extern bool pow_int(void);
extern bool pow(void);
extern bool print_for(void);
extern bool reverse_any(void);
extern bool reverse_one(void);
extern bool reverse_three(void);
extern bool reverse_two(void);
extern bool RevOne(void);
extern bool RevTwo(void);
extern bool Rosen34(void);
extern bool runge_45_2(void);
extern bool seq_property(void);
extern bool sign(void);
extern bool Sinh(void);
extern bool Sin(void);
extern bool Sqrt(void);
extern bool StackMachine(void);
extern bool SubEq(void);
extern bool Sub(void);
extern bool Tanh(void);
extern bool Tan(void);
extern bool TapeIndex(void);
extern bool UnaryMinus(void);
extern bool UnaryPlus(void);
extern bool Value(void);
extern bool Var2Par(void);
extern bool vec_ad(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), std::string name)
	{	bool ok      = true;
		std::streamsize width =  20;
		std::cout.width( width );
		std::cout.setf( std::ios_base::left );
		std::cout << name;
		//
		ok &= name.size() < size_t(width);
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK" << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error" << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;

	// This line is used by test_one.sh

	// run external compiled tests (this line expected by bin/new_test.sh)
	ok &= Run( abort_recording,   "abort_recording"  );
	ok &= Run( fabs,              "fabs"             );
	ok &= Run( acos,              "acos"             );
	ok &= Run( acosh,             "acosh"            );
	ok &= Run( ad_assign,         "ad_assign"        );
	ok &= Run( ad_ctor,           "ad_ctor"          );
	ok &= Run( Add,               "Add"              );
	ok &= Run( AddEq,             "AddEq"            );
	ok &= Run( ad_fun,            "ad_fun"           );
	ok &= Run( ad_in_c,           "ad_in_c"          );
	ok &= Run( ad_input,          "ad_input"         );
	ok &= Run( ad_output,         "ad_output"        );
	ok &= Run( asin,              "asin"             );
	ok &= Run( asinh,             "asinh"            );
	ok &= Run( atan2,             "atan2"            );
	ok &= Run( atan,              "atan"             );
	ok &= Run( atanh,             "atanh"            );
	ok &= Run( azmul,             "azmul"            );
	ok &= Run( BenderQuad,        "BenderQuad"       );
	ok &= Run( BoolFun,           "BoolFun"          );
	ok &= Run( capacity_order,    "capacity_order"   );
	ok &= Run( change_param,      "change_param"     );
	ok &= Run( check_for_nan,     "check_for_nan"    );
	ok &= Run( compare_change,    "compare_change"   );
	ok &= Run( Compare,           "Compare"          );
	ok &= Run( complex_poly,      "complex_poly"     );
	ok &= Run( CondExp,           "CondExp"          );
	ok &= Run( Cos,               "Cos"              );
	ok &= Run( Cosh,              "Cosh"             );
	ok &= Run( Div,               "Div"              );
	ok &= Run( DivEq,             "DivEq"            );
	ok &= Run( EqualOpSeq,        "EqualOpSeq"       );
	ok &= Run( Erf,               "Erf"              );
	ok &= Run( exp,               "exp"              );
	ok &= Run( expm1,             "expm1"            );
	ok &= Run( ForOne,            "ForOne"           );
	ok &= Run( ForTwo,            "ForTwo"           );
	ok &= Run( forward_dir,       "forward_dir"      );
	ok &= Run( Forward,           "Forward"          );
	ok &= Run( forward_order,     "forward_order"    );
	ok &= Run( fun_assign,        "fun_assign"       );
	ok &= Run( FunCheck,          "FunCheck"         );
	ok &= Run( HesLagrangian,     "HesLagrangian"    );
	ok &= Run( HesLuDet,          "HesLuDet"         );
	ok &= Run( HesMinorDet,       "HesMinorDet"      );
	ok &= Run( Hessian,           "Hessian"          );
	ok &= Run( HesTimesDir,       "HesTimesDir"      );
	ok &= Run( Independent,       "Independent"      );
	ok &= Run( Integer,           "Integer"          );
	ok &= Run( Interface2C,       "Interface2C"      );
	ok &= Run( interp_onetape,    "interp_onetape"   );
	ok &= Run( interp_retape,     "interp_retape"    );
	ok &= Run( JacLuDet,          "JacLuDet"         );
	ok &= Run( JacMinorDet,       "JacMinorDet"      );
	ok &= Run( Jacobian,          "Jacobian"         );
	ok &= Run( log10,             "log10"            );
	ok &= Run( log1p,             "log1p"            );
	ok &= Run( log,               "log"              );
	ok &= Run( LuRatio,           "LuRatio"          );
	ok &= Run( LuVecADOk,         "LuVecADOk"        );
	ok &= Run( MulEq,             "MulEq"            );
	ok &= Run( mul_level,         "mul_level"        );
	ok &= Run( mul_level_ode,     "mul_level_ode"    );
	ok &= Run( Mul,               "Mul"              );
	ok &= Run( NearEqualExt,      "NearEqualExt"     );
	ok &= Run( number_skip,       "number_skip"      );
	ok &= Run( NumericType,       "NumericType"      );
	ok &= Run( num_limits,        "num_limits"       );
	ok &= Run( OdeStiff,          "OdeStiff"         );
	ok &= Run( ode_taylor,        "ode_taylor"       );
	ok &= Run( opt_val_hes,       "opt_val_hes"      );
	ok &= Run( ParVar,            "ParVar"           );
	ok &= Run( Poly,              "Poly"             );
	ok &= Run( pow_int,           "pow_int"          );
	ok &= Run( pow,               "pow"              );
	ok &= Run( reverse_any,       "reverse_any"      );
	ok &= Run( reverse_one,       "reverse_one"      );
	ok &= Run( reverse_three,     "reverse_three"    );
	ok &= Run( reverse_two,       "reverse_two"      );
	ok &= Run( RevOne,            "RevOne"           );
	ok &= Run( RevTwo,            "RevTwo"           );
	ok &= Run( Rosen34,           "Rosen34"          );
	ok &= Run( runge_45_2,        "runge_45_2"       );
	ok &= Run( seq_property,      "seq_property"     );
	ok &= Run( sign,              "sign"             );
	ok &= Run( Sinh,              "Sinh"             );
	ok &= Run( Sin,               "Sin"              );
	ok &= Run( Sqrt,              "Sqrt"             );
	ok &= Run( StackMachine,      "StackMachine"     );
	ok &= Run( SubEq,             "SubEq"            );
	ok &= Run( Sub,               "Sub"              );
	ok &= Run( Tanh,              "Tanh"             );
	ok &= Run( Tan,               "Tan"              );
	ok &= Run( TapeIndex,         "TapeIndex"        );
	ok &= Run( UnaryMinus,        "UnaryMinus"       );
	ok &= Run( UnaryPlus,         "UnaryPlus"        );
	ok &= Run( Value,             "Value"            );
	ok &= Run( Var2Par,           "Var2Par"          );
	ok &= Run( vec_ad,            "vec_ad"           );
# ifdef CPPAD_HAS_ADOLC
	ok &= Run( mul_level_adolc,      "mul_level_adolc"     );
	ok &= Run( mul_level_adolc_ode,  "mul_level_adolc_ode" );
# endif
# ifdef CPPAD_HAS_EIGEN
	ok &= Run( eigen_array,       "eigen_array"      );
	ok &= Run( eigen_det,         "eigen_det"        );
# endif

	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CppAD::thread_alloc::free_all() )
	{	Run_ok_count++;
		cout << "OK:    " << "No memory leak detected" << endl;
	}
	else
	{	ok = false;
		Run_error_count++;
		cout << "Error: " << "memory leak detected" << endl;
	}
	// Run base_require after memory leak check because base_alloc.hpp uses
	// thread_alloc to allocate memory for static copies of nan.
	ok &= Run( base_require,      "base_require"     );
	// convert int(size_t) to avoid warning on _MSC_VER systems
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
// END C++
