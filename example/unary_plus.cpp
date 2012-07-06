/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin UnaryPlus.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Unary Plus Operator: Example and Test$$

$index unary plus, example$$
$index example, unary plus$$
$index test, unary plus$$

$code
$verbatim%example/unary_plus.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool UnaryPlus(void)
{	bool ok = true;
	using CppAD::AD;


	// domain space vector
	size_t n = 1;
	CPPAD_TESTVECTOR(AD<double>) x(n);
	x[0]      = 3.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) y(m);
	y[0] = + x[0];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check values
	ok &= ( y[0] == 3. );

	// forward computation of partials w.r.t. x[0]
	CPPAD_TESTVECTOR(double) dx(n);
	CPPAD_TESTVECTOR(double) dy(m);
	size_t p = 1;
	dx[0]    = 1.;
	dy       = f.Forward(p, dx);
	ok      &= ( dy[0] == 1. );   // dy[0] / dx[0]

	// reverse computation of dertivative of y[0]
	CPPAD_TESTVECTOR(double)  w(m);
	CPPAD_TESTVECTOR(double) dw(n);
	w[0] = 1.;
	dw   = f.Reverse(p, w);
	ok &= ( dw[0] == 1. );       // dy[0] / dx[0]

	// use a VecAD<Base>::reference object with unary plus
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = x[0];
	AD<double> result = + v[zero];
	ok     &= (result == y[0]);
	 
	return ok;
}
// END C++
