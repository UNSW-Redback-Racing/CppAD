/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin graph_sum_op.cpp$$
$spell
    sum
$$

$section C++ AD Graph sum Operator: Example and Test$$

$head Source Code$$
$srcfile%test_more/graph/sum_op.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool sum_op(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::AD;
    using std::string;
    typedef CppAD::local::graph::graph_op_struct graph_op_struct;
    typedef CppAD::local::graph::cpp_graph       cpp_graph;
    //
    // AD graph example
    // node_1 : p[0]
    // node_2 : p[1]
    // node_3 : p[2]
    // node_4 : x[0]
    // node_5 : p[0] + p[1] + p[2]
    // node_6 : x[0] + p[0] + p[1] + p[2]
    // y[0]   = x[0] + p[0] + p[1] + p[2]
    //
    //
    // C++ graph object
    cpp_graph graph_obj;
    //
    //
    // structure corresponding to one operator
    graph_op_struct         op_usage;
    //
    // size_t value that is not used
    //
    // set scalars
    graph_obj.function_name_set("sum_op example");
    size_t n_dynamic_ind = 3;
    graph_obj.n_dynamic_ind_set(n_dynamic_ind);
    size_t n_independent = 1;
    graph_obj.n_independent_set(n_independent);
    //
    // node_5 : p[0] + p[1] + p[2]
    //
    // n_arg comes before first_node
    graph_obj.operator_arg_push_back(3);
    op_usage = CppAD::local::graph::sum_graph_op;
    graph_obj.operator_vec_push_back(op_usage);
    graph_obj.operator_arg_push_back(1);
    graph_obj.operator_arg_push_back(2);
    graph_obj.operator_arg_push_back(3);
    //
    // node_6 : x[0] + p[0] + p[1] + p[2]
    //
    // n_arg comes before first_node
    graph_obj.operator_arg_push_back(2);
    graph_obj.operator_vec_push_back(op_usage);
    graph_obj.operator_arg_push_back(4);
    graph_obj.operator_arg_push_back(5);
    //
    // y[0]   = x[0] + p[0] + p[1] + p[2]
    graph_obj.dependent_vec_push_back(6);
    //
    // f(x, p) = x_0 + p_0 + p_1 + p_2
    CppAD::ADFun<double> f;
    f.from_graph(graph_obj);
    //
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 3;
    //
    // set independent variables and parameters
    vector<double> p(3), x(1);
    for(size_t j = 0; j < 3; ++j)
        p[j] = double(j + 1);
    x[0] = 5.0;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    vector<double> y = f.Forward(0, x);
    //
    // check result
    ok &= y[0] == x[0] + p[0] + p[1] + p[2];
    // -----------------------------------------------------------------------
    // Convert to Graph graph and back again
    f.to_graph(graph_obj);
    // std::cout << "json = " << json;
    f.from_graph(graph_obj);
    // -----------------------------------------------------------------------
    ok &= f.Domain() == 1;
    ok &= f.Range() == 1;
    ok &= f.size_dyn_ind() == 3;
    //
    // set independent variables and parameters
    for(size_t j = 0; j < 3; ++j)
        p[j] = double(j + 1);
    x[0] = 5.0;
    //
    // compute y = f(x, p)
    f.new_dynamic(p);
    y = f.Forward(0, x);
    //
    // check result
    ok &= y[0] == x[0] + p[0] + p[1] + p[2];
    //
    return ok;
}
// END C++
