/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
# include <limits>
# include <cppad/cppad.hpp>
# include <cppad/core/llvm/ir.hpp>
# include <cppad/core/llvm/link.hpp>
namespace { // BEGIN_EMPTY_NAMESPACE
// -----------------------------------------------------------------------------
bool tst_initialize(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::llvm_ir;
    using CppAD::AD;
    //
    size_t nx = 1, ny = 1;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    ax[0]  = 0.0;
    CppAD::Independent(ax);
    ay[0] = ax[0] * ax[0];
    CppAD::ADFun<double> f(ax, ay);
    std::string function_name = "llvm_tst";
    f.function_name_set(function_name);
    //
    // create a cpp_graph from this function
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // llvm_ir object
    llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // link_obj
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // load object file
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    CppAD::vector<double> input(nx);
    input[0] = -2.0;
    //
    // vector to hold return value
    CppAD::vector<double> output(ny);
    output[0] = 0.0;
    //
    // vector to hold message
    size_t nm = 1;
    CppAD::vector<char> message(nm);
    //
    // call function
    int32_t len_input   = static_cast<int32_t>(nx);
    int32_t len_output  = static_cast<int32_t>(ny);
    int32_t len_message = static_cast<int32_t>(nm);
    int32_t error_no;
    error_no    = fun_ptr(
        len_input,   input.data(),
        len_output,  output.data(),
        len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    //
    // check output
    ok &= input[0] == -2.0;
    ok &= output[0] == input[0] * input[0];
    //
    // re-initialize link_obj
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // Searching for this function should fail
    msg = link_obj.function_ptr(function_name, fun_ptr);
    ok &= msg != "";
    //
    // re-load object file
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // get fun_ptr
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // call function
    output[0] = 0.0;
    input[0]  = -3.0;
    error_no    = fun_ptr(
        len_input,   input.data(),
        len_output,  output.data(),
        len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    //
    // check output
    ok &= input[0] == -3.0;
    ok &= output[0] == input[0] * input[0];
    //
    return ok;
}
// -----------------------------------------------------------------------------
bool tst_adfun_print(void)
{   bool ok = true;
    using CppAD::AD;

    size_t nx = 1, ny = 1;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    ax[0] = 1.0;
    CppAD::Independent(ax);
    //
    PrintFor(ax[0], "x[0] = ", ax[0], "");
    ay[0] = ax[0] * ax[0];
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    std::string function_name = "llvm_tst";
    f.function_name_set(function_name);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // optimize ir_obj
    ir_obj.optimize();
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // link_obj
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // load object file
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    CppAD::vector<double> input(nx);
    input[0] = -2.0; // x[0] < 0.0;
    //
    // vector to hold return value
    CppAD::vector<double> output(ny);
    output[0] = 0.0;
    //
    // vector to hold message
    size_t nm = 200;
    CppAD::vector<char> message(nm);
    //
    // incorrect call to function
    int32_t len_input   = int32_t(nx);
    int32_t len_output  = int32_t(ny);
    int32_t len_message = int32_t(nm);
    int32_t error_no    = fun_ptr(
        len_input,   input.data(),
        len_output,  output.data(),
        len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= input[0]  == -2.0;
    ok &= output[0] == input[0] * input[0];
    ok &= std::string(message.data()) == "x[0] = -2.0";
    //
    // convert from IR object back to cpp_graph object
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // convert from cpp_graph object back to ADFun object
    f.from_graph(graph_obj);
    //
    // Test PrintFor operation
    std::stringstream os;
    CPPAD_TESTVECTOR(double) x(nx), y(ny);
    x[0] = -2.0;
    y = f.Forward(0, x, os);
    //
    ok &= y[0] == x[0] * x[0];
    ok &= os.str() == "x[0] = -2";
    //
    return ok;
}
// -----------------------------------------------------------------------------
bool tst_cppad_link_print(void)
{   bool ok = true;
    //
    // link_obj
    std::string msg;
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    std::string          fun_name = "cppad_link_print";
    int (*fun_ptr)(int, int, char*, double, const char*, double, const char*);
    msg = link_obj.function_ptr(fun_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    int n_in           = 0;
    int len_message    = 200;
    double notpos      = -1.0;
    std::string before = "before ";
    std::string after  = " after\n";
    char message[201];
    int n_out;
    double value;
    //
    // +4.5678
    value = 45.678;
    n_out = fun_ptr(
        n_in, len_message, message, notpos, before.data(), value, after.data()
    );
    n_in  = n_out;
    //
    // +inf
    value = 1.0 / 0.0;
    n_out = fun_ptr(
        n_in, len_message, message, notpos, before.data(), value, after.data()
    );
    n_in  = n_out;
    //
    // nan
    value = 0.0 / 0.0;
    n_out = fun_ptr(
        n_in, len_message, message, notpos, before.data(), value, after.data()
    );
    n_in  = n_out;
    //
    // +0.0
    value = 0.0;
    n_out = fun_ptr(
        n_in, len_message, message, notpos, before.data(), value, after.data()
    );
    n_in  = n_out;
    //
    // -1.0e-100
    value = -1e-100;
    n_out = fun_ptr(
        n_in, len_message, message, notpos, before.data(), value, after.data()
    );
    n_in  = n_out;
    //
    // terminate string
    message[n_out] = '\0';
    //
    std::string str(message);
    std::string line1 = before + "+4.5678e+1" + after;
    std::string line2 = before + "+inf" + after;
    std::string line3 = before + "nan" + after;
    std::string line4 = before + "+0.0" + after;
    std::string line5 = before + "-1.0e-100" + after;
    //
    ok &= str == line1 + line2 + line3 + line4 + line5;
    //
    return ok;
}
// -----------------------------------------------------------------------------
bool tst_link_lib(void)
{   bool ok = true;
    //
    // link_obj
    std::string msg;
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    std::string          fun_name = "cos";
    double (*fun_ptr)(double);
    msg = link_obj.function_ptr(fun_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    double cos2 = fun_ptr(2.0);
    ok         &= cos2 == std::cos(2.0);
    //
    return ok;
}
// -----------------------------------------------------------------------------
// algo
template <class VectorFloat>
VectorFloat algo(const VectorFloat& p, const VectorFloat& x)
// END_PROTOTYPE
{   size_t nx = x.size();
    size_t np = p.size();
    size_t ny = nx + np;
    typedef typename VectorFloat::value_type value_type;
    //
    // number of operator: add, sub, mul, div, neg
    size_t n_operator = 5;
    //
    // operation counter mod n_operator
    size_t operation_count = 0;
    //
    // define the actual function
    VectorFloat y(ny);
    for(size_t i = 0; i < ny; ++i)
    {   value_type arg;
        if( i < np )
            arg = p[i];
        else
            arg = x[i-np];
        operation_count = (operation_count + 1) % n_operator;
        switch( operation_count )
        {   case 0:
            y[i] = arg + double(i);
            break;

            case 1:
            y[i] = double(i) - arg;
            break;

            case 2:
            y[i] = arg * double(i);
            break;

            case 3:
            y[i] = double(i) / arg;
            break;

            case 4:
            y[i] = - arg;
            break;
        }
    }
    //
    // some operations that optimizer should get removed by optimizer
    typename VectorFloat::value_type sum = 0.0;
    for(size_t i = 0; i < ny; i++)
        sum += y[i];
    //
    return y;
}
// -----------------------------------------------------------------------------
// algo2adfun
void algo2adfun(size_t np, size_t nx, CppAD::ADFun<double>& adfun)
{   using CppAD::AD;
    using CppAD::vector;
    //
    // ap, ax
    vector< AD<double> > ap(np), ax(nx);
    for(size_t i = 0; i < np; ++i)
        ap[i] = AD<double>( i + 1 );
    for(size_t i = 0; i < nx; ++i)
        ax[i] = AD<double>( i + np );
    //
    // ax independent variables,  ap dynamic parameters
    CppAD::Independent(ax, ap);
    //
    // ay
    vector< AD<double> > ay = algo(ap, ax);
    //
    // f : x -> y
    adfun.Dependent(ax, ay);
    //
    // function_name
    adfun.function_name_set("llvm_tst");
    return;
}
// -----------------------------------------------------------------------------
bool tst_llvm_ir(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // np, nx
    size_t np = 2;
    size_t nx = 5;
    //
    // f
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimized ir_obj
    ir_obj.optimize();
    //
    // optimized graph_obj
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // g is an optimized verison of f
    CppAD::ADFun<double> g;
    g.from_graph(graph_obj);
    // ---------------------------------------------------------------------
    // Check zero order forward mode for g
    //
    // rand_max
    double rand_max = double(RAND_MAX);
    //
    // x, p
    vector<double> p(np), x(nx);
    for(size_t i = 0; i < np; ++i)
        p[i] = std::rand() / rand_max;
    for(size_t i = 0; i < nx; ++i)
        x[i] = std::rand() / rand_max;
    //
    // y = g(p; x)
    g.new_dynamic(p);
    vector<double> y = g.Forward(0, x);
    //
    // check
    double         eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    vector<double> check = algo(p, x);
    size_t         ny    = g.Range();
    ok &= check.size() == ny;
    ok &= f.Range()    == ny;
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(y[i], check[i], eps99, eps99);
    //
    return ok;
}
// -----------------------------------------------------------------------------
bool tst_load(void)
{   bool ok = true;
    using CppAD::vector;
    using CppAD::llvm_ir;
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    size_t np = 1;
    size_t nx = 3;
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
    //
    // create a cpp_graph from this function
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    std::string function_name = graph_obj.function_name_get();
    //
    // llvm_ir object
    llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // link_obj
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // load object file
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // input
    CppAD::vector<double> input(np + nx);
    for(size_t i = 0; i < np + nx; ++i)
        input[i] = double(i) + 4.0;
    //
    // vector to hold return value
    size_t ny = f.Range();
    CppAD::vector<double> output(ny);
    for(size_t i = 0; i < ny; ++i)
        output[i] = 0.0;
    //
    // vector to hold message
    size_t nm = 1;
    CppAD::vector<char> message(nm);
    //
    // incorrect call to function
    int32_t len_input   = 0;
    int32_t len_output  = 0;
    int32_t len_message = 0;
    int32_t error_no    = fun_ptr(
        len_input,   input.data(),
        len_output,  output.data(),
        len_message, message.data()
    );
    ok &= error_no != 0;
    for(size_t i = 0; i < ny; ++i)
        ok &= output[i] == 0.0;
    //
    // correct call function
    len_input   = static_cast<int32_t>(np + nx);
    len_output  = static_cast<int32_t>(ny);
    len_message = static_cast<int32_t>(nm);
    error_no    = fun_ptr(
        len_input,   input.data(),
        len_output,  output.data(),
        len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    //
    // check output
    CppAD::vector<double> p(np), x(nx), check(ny);
    for(size_t i = 0; i < np; ++i)
        p[i] = input[i];
    for(size_t i = 0; i < nx; ++i)
        x[i] = input[np + i];
    check = algo(p, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(output[i], check[i], eps99, eps99);
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_azmul
bool tst_azmul(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    // ax
    size_t nx = 4;
    vector< AD<double> > ax(nx);
    CppAD::Independent(ax);
    //
    // ay
    size_t ny = 2;
    vector< AD<double> > ay(ny);
    ay[0] = azmul(ax[0],  ax[1]);
    ay[1] = azmul(ax[2], ax[3]);
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    f.function_name_set("llvm_tst");
    f.check_for_nan(false);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // optimize it
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    f.from_graph(graph_obj);
    //
    // test both forms of 0 * nan
    vector<double> x(nx), y(ny);
    x[0] = 0.0;
    x[1] = std::numeric_limits<double>::quiet_NaN();
    x[2] = 2.0;
    x[3] = 3.0;
    y    = f.Forward(0, x);
    //
    ok &= y[0] == 0.0;
    ok &= y[1] == x[2] * x[3];
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_unary
bool tst_unary(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    // nx, x
    size_t nx = 20;
    vector<double> x(nx);
    x[0]  = 0.2;
    x[1]  = std::cos(x[0]);
    x[2]  = 0.3;
    x[3]  = std::sin(x[2]);
    x[4]  = 0.4;
    x[5]  = std::tan(x[4]);
    //
    x[6]  = 0.5;
    x[7]  = std::cosh(x[0]);
    x[8]  = 0.6;
    x[9]  = std::sinh(x[2]);
    x[10] = 0.7;
    x[11] = std::tanh(x[4]);
    //
    x[12] = 0.8;
    x[13] = std::exp(x[12]);
    x[14] = 0.8;
    x[15] = std::expm1(x[14]);
    //
    x[16] = 0.9;
    x[17] = 1.0;
    x[18] = -1.0;
    x[19] = 1.1;
    //
    // ax
    vector< AD<double> > ax(nx);
    for(size_t i = 0; i < nx; ++i)
        ax[i] = x[i];
    CppAD::Independent(ax);
    //
    // ny, ay
    size_t ny = nx;
    vector< AD<double> > ay(ny);
    ay[0]  =  cos(ax[0]);
    ay[1]  = acos(ax[1]);
    ay[2]  =  sin(ax[2]);
    ay[3]  = asin(ax[3]);
    ay[4]  =  tan(ax[4]);
    ay[5]  = atan(ax[5]);
    //
    ay[6]  =  cosh(ax[6]);
    ay[7]  = acosh(ax[7]);
    ay[8]  =  sinh(ax[8]);
    ay[9]  = asinh(ax[9]);
    ay[10] =  tanh(ax[10]);
    ay[11] = atanh(ax[11]);
    //
    ay[12] = exp(ax[12]);
    ay[13] = log(ax[13]);
    ay[14] = expm1(ax[14]);
    ay[15] = log1p(ax[15]);
    //
    ay[16] = erf(ax[16]);
    ay[17] = erfc(ax[17]);
    ay[18] = fabs(ax[18]);
    ay[19] = sign(ax[19]);
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    std::string function_name = "llvm_tst";
    f.function_name_set(function_name);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize it
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    f.from_graph(graph_obj);
    //
    // check
    vector<double> y(nx);
    y = f.Forward(0, x);
    ok &= y[0]  == std::cos(  x[0] );
    ok &= y[1]  == std::acos( x[1] );
    ok &= y[2]  == std::sin(  x[2] );
    ok &= y[3]  == std::asin( x[3] );
    ok &= y[4]  == std::tan(  x[4] );
    ok &= y[5]  == std::atan( x[5] );
    //
    ok &= y[6]  == std::cosh(  x[6] );
    ok &= y[7]  == std::acosh( x[7] );
    ok &= y[8]  == std::sinh(  x[8] );
    ok &= y[9]  == std::asinh( x[9] );
    ok &= y[10] == std::tanh(  x[10] );
    ok &= y[11] == std::atanh( x[11] );
    //
    ok &= y[12] == std::exp(   x[12] );
    ok &= y[13] == std::log(   x[13] );
    ok &= y[14] == std::expm1( x[14] );
    ok &= y[15] == std::log1p( x[15] );
    //
    ok &= y[16] == std::erf(  x[16] );
    ok &= y[17] == std::erfc( x[17] );
    ok &= y[18] == std::fabs( x[18] );
    ok &= y[19] == CppAD::sign( x[19] );
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load the object file
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // clean out old value for y
    for(size_t i = 0; i < ny; ++i)
        y[i] = std::numeric_limits<double>::quiet_NaN();
    //
    // vector where message is placed
    size_t nm = 1;
    vector<char> message(nm);
    //
    // call compiled version of function
    int32_t len_x    = int32_t (nx);
    int32_t len_y    = int32_t (ny);
    int32_t len_message = int32_t (nm);
    int32_t error_no = fun_ptr(
        len_x, x.data(), len_y, y.data(), len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    //
    // check result
    ok &= y[0] == std::cos(  x[0] );
    ok &= y[1] == std::acos( x[1] );
    ok &= y[2] == std::sin(  x[2] );
    ok &= y[3] == std::asin( x[3] );
    ok &= y[4] == std::tan(  x[4] );
    ok &= y[5] == std::atan( x[5] );
    //
    ok &= y[6]  == std::cosh(  x[6] );
    ok &= y[7]  == std::acosh( x[7] );
    ok &= y[8]  == std::sinh(  x[8] );
    ok &= y[9]  == std::asinh( x[9] );
    ok &= y[10] == std::tanh(  x[10] );
    ok &= y[11] == std::atanh( x[11] );
    //
    ok &= y[12] == std::exp(   x[12] );
    ok &= y[13] == std::log(   x[13] );
    ok &= y[14] == std::expm1( x[14] );
    ok &= y[15] == std::log1p( x[15] );
    //
    ok &= y[16] == std::erf(  x[16] );
    ok &= y[17] == std::erfc( x[17] );
    ok &= y[18] == std::fabs( x[18] );
    ok &= y[19] == CppAD::sign( x[19] );
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_binary
bool tst_binary(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    // nx, x
    size_t nx = 2;
    vector<double> x(nx);
    x[0]  = 0.2;
    x[1]  = 0.3;
    //
    // ax
    vector< AD<double> > ax(nx);
    for(size_t i = 0; i < nx; ++i)
        ax[i] = x[i];
    CppAD::Independent(ax);
    //
    // ny, ay
    size_t ny = nx / 2;
    vector< AD<double> > ay(ny);
    ay[0]  =  pow(ax[0], ax[1] );
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    std::string function_name = "llvm_tst";
    f.function_name_set(function_name);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize it
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    f.from_graph(graph_obj);
    //
    // check
    vector<double> y(ny);
    y = f.Forward(0, x);
    ok &= y[0]  == std::pow(  x[0], x[1] );
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load the object file
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // clean out old value for y
    for(size_t i = 0; i < ny; ++i)
        y[i] = std::numeric_limits<double>::quiet_NaN();
    //
    // vector where message is placed
    size_t nm = 1;
    vector<char> message(nm);
    //
    // call compiled version of function
    int32_t len_x    = int32_t (nx);
    int32_t len_y    = int32_t (ny);
    int32_t len_message = int32_t (nm);
    int32_t error_no = fun_ptr(
        len_x, x.data(), len_y, y.data(), len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    //
    // check result
    ok &= y[0] == std::pow(  x[0], x[1] );
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_cexp
bool tst_cexp(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    // nx, x
    size_t nx = 4;
    vector<double> x(nx);
    x[0]  = 0.2;
    x[1]  = 0.3;
    x[2]  = 0.4;
    x[3]  = 0.5;
    //
    // ax
    vector< AD<double> > ax(nx);
    for(size_t i = 0; i < nx; ++i)
        ax[i] = x[i];
    CppAD::Independent(ax);
    //
    AD<double> left     = ax[0];
    AD<double> right    = ax[1];
    AD<double> if_true  = ax[2];
    AD<double> if_false = ax[3];

    //
    // ny, ay
    size_t ny = 3;
    vector< AD<double> > ay(ny);
    ay[0]  =  CondExpEq(left, right, if_true, if_false);
    ay[1]  =  CondExpLe(left, right, if_true, if_false);
    ay[2]  =  CondExpLt(left, right, if_true, if_false);
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    std::string function_name = "llvm_tst";
    f.function_name_set(function_name);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize it
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    f.from_graph(graph_obj);
    //
    // check
    vector<double> y(ny);
    x[1] = x[0] + .1;
    y = f.Forward(0, x);
    ok &= y[0] == x[3];
    ok &= y[1] == x[2];
    ok &= y[2] == x[2];
    //
    x[1] = x[0];
    y = f.Forward(0, x);
    ok &= y[0] == x[2];
    ok &= y[1] == x[2];
    ok &= y[2] == x[3];
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load the object file
    CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // clean out old value for y
    for(size_t i = 0; i < ny; ++i)
        y[i] = std::numeric_limits<double>::quiet_NaN();
    //
    // vector where message is placed
    size_t nm = 1;
    vector<char> message(nm);
    //
    // call compiled version of function
    int32_t len_x    = int32_t (nx);
    int32_t len_y    = int32_t (ny);
    int32_t len_message = int32_t (nm);
    int32_t error_no;
    //
    // check result
    x[1] = x[0] + .1;
    error_no = fun_ptr(
        len_x, x.data(), len_y, y.data(), len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    ok &= y[0] == x[3];
    ok &= y[1] == x[2];
    ok &= y[2] == x[2];
    //
    x[1] = x[0];
    error_no = fun_ptr(
        len_x, x.data(), len_y, y.data(), len_message, message.data()
    );
    ok &= error_no == 0;
    ok &= message[0] == '\0';
    ok &= y[0] == x[2];
    ok &= y[1] == x[2];
    ok &= y[2] == x[3];
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_compare_1
bool tst_compare_1(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    for(size_t comp_case = 0; comp_case < 4; ++comp_case)
    {
        //
        // nx, x
        size_t nx = 4;
        vector<double> x(nx);
        //
        // ax
        vector< AD<double> > ax(nx);
        for(size_t i = 0; i < nx; ++i)
            ax[i] = x[i] = double(i);
        CppAD::Independent(ax);
        //
        // ny, ay
        size_t ny = 1;
        vector< AD<double> > ay(ny);
        std::string comp_op = "";
        switch(comp_case )
        {   //
            // comp_eq_graph_op
            case 0:
            comp_op = "eq";
            if( ax[0] == ax[1] )
                ay[0]  =  ax[2];
            else
                ay[0]  =  ax[3];
            break;
            //
            // comp_le_graph_op
            case 1:
            comp_op = "le";
            if( ax[0] <= ax[1] )
                ay[0]  =  ax[2];
            else
                ay[0]  =  ax[3];
            break;
            //
            // comp_le_graph_op
            case 2:
            comp_op = "lt";
            if( ax[0]  <  ax[1] )
                ay[0]  =  ax[2];
            else
                ay[0]  =  ax[3];
            break;
            //
            // comp_ne_graph_op
            case 3:
            comp_op = "ne";
            if( ax[0] !=  ax[1] )
                ay[0]  =  ax[2];
            else
                ay[0]  =  ax[3];
            break;
        }
        //
        // f
        CppAD::ADFun<double> f(ax, ay);
        std::string function_name = "llvm_tst";
        f.function_name_set(function_name);
        //
        // graph_obj
        CppAD::cpp_graph graph_obj;
        f.to_graph(graph_obj);
        //
        // ir_obj
        CppAD::llvm_ir ir_obj;
        std::string msg = ir_obj.from_graph(graph_obj);
        if( msg != "" )
        {   std::cout << "\n" << msg << "\n";
            return false;
        }
        // optimize it
        ir_obj.optimize();
        //
        // back to graph
        msg = ir_obj.to_graph(graph_obj);
        if( msg != "" )
        {   std::cout << "\n" << msg << "\n";
            return false;
        }
        //
        // back to function
        f.from_graph(graph_obj);
        //
        // x[0] < x[1] (same as during recording)
        vector<double> y(ny);
        size_t number;
        x[0] = 0.2;
        x[1] = 0.3;
        y       = f.Forward(0, x);
        number = f.compare_change_number();
        if( comp_op == "eq" )
        {   ok &= y[0] == x[3];
            ok &= number  == 0;
        }
        else
        {   ok &= y[0] == x[2];
            ok &= number  == 0;
        }
        //
        // x[0] ==  x[1]
        x[0] = 0.3;
        x[1] = 0.3;
        y       = f.Forward(0, x);
        number = f.compare_change_number();
        if( comp_op == "eq" )
        {   ok &= y[0] == x[3];
            ok &= number  == 1;
        }
        else
        {   ok &= y[0] == x[2];
            if( comp_op != "le" )
                ok &= number == 1;
            else
                ok &= number  == 0;
        }
        //
        // create object file
        std::string file_name = function_name + ".o";
        msg = ir_obj.to_object_file(file_name);
        if( msg != "" )
        {   std::cout << "\n" << msg << "\n";
            return false;
        }
        //
        // load the object file
         CppAD::llvm_link link_obj;
        msg = link_obj.initialize();
        if( msg != "" )
        {   std::cout << "\n" << msg << "\n";
            return false;
        }
        msg = link_obj.object_file(file_name);
        if( msg != "" )
        {   std::cout << "\n" << msg << "\n";
            return false;
        }
        //
        // fun_ptr
        CppAD::compiled_ir_t fun_ptr;
        msg = link_obj.function_ptr(function_name, fun_ptr);
        if( msg != "" )
        {   std::cerr << "\n" << msg << "\n";
            return false;
        }
        //
        // clean out old value for y
        for(size_t i = 0; i < ny; ++i)
            y[i] = std::numeric_limits<double>::quiet_NaN();
        //
        // vector where message is placed
        size_t nm = 1;
        vector<char> message(nm);
        //
        // call compiled version of function
        int32_t len_x    = int32_t (nx);
        int32_t len_y    = int32_t (ny);
        int32_t len_message = int32_t (nm);
        int32_t error_no;
        //
        // x[0] < x[1] (same as during recording)
        x[0] = 0.2;
        x[1] = 0.3;
        error_no = fun_ptr(
            len_x, x.data(), len_y, y.data(), len_message, message.data()
        );
        ok &= message[0] == '\0';
        if( comp_op == "eq" )
        {   ok &= y[0] == x[3];
            ok &= error_no  == 0;
        }
        else
        {   ok &= y[0] == x[2];
            ok &= error_no  == 0;
        }
        //
        // x[0] ==  x[1]
        x[0] = 0.3;
        x[1] = 0.3;
        error_no = fun_ptr(
            len_x, x.data(), len_y, y.data(), len_message, message.data()
        );
        ok &= message[0] == '\0';
        if( comp_op == "eq" )
        {   ok &= y[0] == x[3];
            ok &= error_no  == 1;
        }
        else
        {   ok &= y[0] == x[2];
            if( comp_op != "le" )
                ok &= error_no == 1;
            else
                ok &= error_no  == 0;
        }
    }
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_compare_2
bool tst_compare_2(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    // nx, x
    size_t nx = 4;
    vector<double> x(nx);
    //
    // ax
    vector< AD<double> > ax(nx);
    for(size_t i = 0; i < nx; ++i)
        ax[i] = x[i] = double(i);
    CppAD::Independent(ax);
    //
    // ny, ay
    size_t ny = 4;
    vector< AD<double> > ay(ny);
    std::string comp_op = "";
    //
    // ==
    if( ax[0] == ax[1] )
        ay[0]  =  ax[2];
    else
        ay[0]  =  ax[3];
    //
    // <=
    if( ax[0] <= ax[1] )
        ay[1]  =  ax[2];
    else
        ay[1]  =  ax[3];
    //
    // <
    if( ax[0]  <  ax[1] )
        ay[2]  =  ax[2];
    else
        ay[2]  =  ax[3];
    //
    // !=
    if( ax[0] !=  ax[1] )
        ay[3]  =  ax[2];
    else
        ay[3]  =  ax[3];
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    std::string function_name = "llvm_tst";
    f.function_name_set(function_name);
    //
    // graph_obj
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize it
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    f.from_graph(graph_obj);
    //
    // x[0] < x[1] (same as during recording)
    vector<double> y(ny);
    size_t number;
    x[0] = 0.2;
    x[1] = 0.3;
    y       = f.Forward(0, x);
    number = f.compare_change_number();
    ok    &= number == 0;
    ok    &= y[0] == x[3];
    ok    &= y[1] == x[2];
    ok    &= y[2] == x[2];
    ok    &= y[3] == x[2];
    //
    // x[0] ==  x[1]
    // ==, <=, and != change
    x[0] = 0.3;
    x[1] = 0.3;
    y       = f.Forward(0, x);
    number = f.compare_change_number();
    ok    &= number != 0;
    ok    &= y[0] == x[3];
    ok    &= y[1] == x[2];
    ok    &= y[2] == x[2];
    ok    &= y[3] == x[2];
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load the object file
     CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // clean out old value for y
    for(size_t i = 0; i < ny; ++i)
        y[i] = std::numeric_limits<double>::quiet_NaN();
    //
    // vector where message is placed
    size_t nm = 1;
    vector<char> message(nm);
    //
    // call compiled version of function
    int32_t len_x    = int32_t (nx);
    int32_t len_y    = int32_t (ny);
    int32_t len_message = int32_t (nm);
    int32_t error_no;
    //
    // x[0] < x[1] (same as during recording)
    x[0] = 0.2;
    x[1] = 0.3;
    error_no = fun_ptr(
        len_x, x.data(), len_y, y.data(), len_message, message.data()
    );
    ok    &= error_no == 0;
    ok    &= message[0] == '\0';
    ok    &= y[0] == x[3];
    ok    &= y[1] == x[2];
    ok    &= y[2] == x[2];
    ok    &= y[3] == x[2];
    //
    // x[0] ==  x[1]
    // ==, <=, and != change
    x[0] = 0.3;
    x[1] = 0.3;
    error_no = fun_ptr(
        len_x, x.data(), len_y, y.data(), len_message, message.data()
    );
    ok    &= error_no == 1;
    ok    &= message[0] == '\0';
    ok    &= y[0] == x[3];
    ok    &= y[1] == x[2];
    ok    &= y[2] == x[2];
    ok    &= y[3] == x[2];
    //
    return ok;
}
// -----------------------------------------------------------------------------
// tst_sum
bool tst_sum(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    std::string function_name = "llvm_tst";
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
    CppAD::cpp_graph graph_obj;
    //
    // operator being used
    CppAD::graph::graph_op_enum op_enum;
    //
    // set scalars
    graph_obj.function_name_set(function_name);
    size_t n_dynamic_ind = 3;
    graph_obj.n_dynamic_ind_set(n_dynamic_ind);
    size_t n_variable_ind = 1;
    graph_obj.n_variable_ind_set(n_variable_ind);
    //
    // node_5 : p[0] + p[1] + p[2]
    //
    op_enum = CppAD::graph::sum_graph_op;
    graph_obj.operator_vec_push_back(op_enum);
    graph_obj.operator_arg_push_back(3);  // n_node_arg
    graph_obj.operator_arg_push_back(1);  // first node arg
    graph_obj.operator_arg_push_back(2);  // second node arg
    graph_obj.operator_arg_push_back(3);  // third node are
    //
    // node_6 : x[0] + p[0] + p[1] + p[2]
    //
    // n_arg comes before first_node
    graph_obj.operator_arg_push_back(2);
    graph_obj.operator_vec_push_back(op_enum);
    graph_obj.operator_arg_push_back(4);
    graph_obj.operator_arg_push_back(5);
    //
    // y[0]   = x[0] + p[0] + p[1] + p[2]
    graph_obj.dependent_vec_push_back(6);
    //
    // ir_obj
    CppAD::llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize it
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    CppAD::ADFun<double> f;
    f.from_graph(graph_obj);
    //
    size_t np = 3, nx = 1, ny = 1;
    vector<double> p(np), x(nx), y(ny);
    p[0] = 1.0;
    p[1] = 2.0;
    p[2] = 3.0;
    x[0] = 4.0;
    f.new_dynamic(p);
    y   = f.Forward(0, x);
    ok &= y[0] == x[0] + p[0] + p[1] + p[2];
    //
    // create object file
    std::string file_name = function_name + ".o";
    msg = ir_obj.to_object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // load the object file
     CppAD::llvm_link link_obj;
    msg = link_obj.initialize();
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    msg = link_obj.object_file(file_name);
    if( msg != "" )
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // fun_ptr
    CppAD::compiled_ir_t fun_ptr;
    msg = link_obj.function_ptr(function_name, fun_ptr);
    if( msg != "" )
    {   std::cerr << "\n" << msg << "\n";
        return false;
    }
    //
    // clean out old value for y
    for(size_t i = 0; i < ny; ++i)
        y[i] = std::numeric_limits<double>::quiet_NaN();
    //
    // vector where message is placed
    size_t nm = 1;
    vector<char> message(nm);
    //
    // call compiled version of function
    int32_t len_px   = int32_t (np + nx);
    int32_t len_y    = int32_t (ny);
    int32_t len_message = int32_t (nm);
    int32_t error_no;
    //
    vector<double>  px(np + nx);
    px[0] = p[0];
    px[1] = p[1];
    px[2] = p[2];
    px[3] = x[0];
    error_no = fun_ptr(
        len_px, px.data(), len_y, y.data(), len_message, message.data()
    );
    ok      &= error_no == 0;
    ok      &= message[0] == '\0';
    ok      &= y[0] == x[0] + p[0] + p[1] + p[2];
    //
    return ok;
}
// -----------------------------------------------------------------------------
namespace {
    // atomic_reciprocal
    class atomic_reciprocal : public CppAD::atomic_three<double> {
    public:
        atomic_reciprocal(const std::string& name) :
        CppAD::atomic_three<double>(name)
        { }
    private:
        virtual bool for_type(
            const CppAD::vector<double>&               parameter_x ,
            const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
            CppAD::vector<CppAD::ad_type_enum>&        type_y      )
        {   assert( parameter_x.size() == type_x.size() );
            bool ok = type_x.size() == 1; // n
            ok     &= type_y.size() == 1; // m
            if( ! ok )
                return false;
            type_y[0] = type_x[0];
            return true;
        }
        virtual bool forward(
            const CppAD::vector<double>&               parameter_x  ,
            const CppAD::vector<CppAD::ad_type_enum>&  type_x       ,
            size_t                                     need_y       ,
            size_t                                     order_low    ,
            size_t                                     order_up     ,
            const CppAD::vector<double>&               taylor_x     ,
            CppAD::vector<double>&                     taylor_y     )
        {
    # ifndef NDEBUG
            size_t n = taylor_x.size() / (order_up + 1);
            size_t m = taylor_y.size() / (order_up + 1);
            assert( n == 1 );
            assert( m == 1 );
            assert( order_low <= order_up );
    # endif

            // return flag
            bool ok = order_up == 0;
            if( ! ok )
                return ok;

            // reciprocal
            taylor_y[0] = 1. / taylor_x[0];
            //
            return ok;
        }
    };
    // atomic_square
    class atomic_square : public CppAD::atomic_three<double> {
    public:
        atomic_square(const std::string& name) :
        CppAD::atomic_three<double>(name)
        { }
    private:
        virtual bool for_type(
            const CppAD::vector<double>&               parameter_x ,
            const CppAD::vector<CppAD::ad_type_enum>&  type_x      ,
            CppAD::vector<CppAD::ad_type_enum>&        type_y      )
        {   assert( parameter_x.size() == type_x.size() );
            bool ok = type_x.size() == 1; // n
            ok     &= type_y.size() == 1; // m
            if( ! ok )
                return false;
            type_y[0] = type_x[0];
            return true;
        }
        virtual bool forward(
            const CppAD::vector<double>&               parameter_x  ,
            const CppAD::vector<CppAD::ad_type_enum>&  type_x       ,
            size_t                                     need_y       ,
            size_t                                     order_low    ,
            size_t                                     order_up     ,
            const CppAD::vector<double>&               taylor_x     ,
            CppAD::vector<double>&                     taylor_y     )
        {
    # ifndef NDEBUG
            size_t n = taylor_x.size() / (order_up + 1);
            size_t m = taylor_y.size() / (order_up + 1);
            assert( n == 1 );
            assert( m == 1 );
            assert( order_low <= order_up );
    # endif

            // return flag
            bool ok = order_up == 0;
            if( ! ok )
                return ok;

            // square
            taylor_y[0] = taylor_x[0] * taylor_x[0];
            //
            return ok;
        }
    };
}
// ---------------------------------------------------------------------------
bool tst_atomic(void)
{   bool ok = true;
    using CppAD::llvm_ir;
    using CppAD::AD;
    //
    atomic_reciprocal recirpocal("reciprocal");
    atomic_square     square("square");
    //
    size_t nx = 2, ny = 2;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
    ax[0] = 1.0;
    ax[1] = 2.0;
    CppAD::Independent(ax);
    CPPAD_TESTVECTOR( AD<double> ) au(1), av(1);
    // y_0 = 1 / x_0
    au[0] = ax[0];
    recirpocal(au, av);
    ay[0] = av[0];
    // y_1 = x_1^2
    au[0] = ax[1];
    square(au, av);
    ay[1] = av[0];
    CppAD::ADFun<double> f(ax, ay);
    f.function_name_set("llvm_tst");
    //
    // create a cpp_graph from this function
    CppAD::cpp_graph graph_obj;
    f.to_graph(graph_obj);
    std::string function_name = graph_obj.function_name_get();
    //
    // llvm_ir object
    llvm_ir ir_obj;
    std::string msg = ir_obj.from_graph(graph_obj);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    // optimize
    ir_obj.optimize();
    //
    // back to graph
    msg = ir_obj.to_graph(graph_obj);
    if( msg != "")
    {   std::cout << "\n" << msg << "\n";
        return false;
    }
    //
    // back to function
    CppAD::ADFun<double> g;
    g.from_graph(graph_obj);
    //
    CPPAD_TESTVECTOR(double) x(nx), y(ny);
    x[0] = 4.0;
    x[1] = 5.0;
    y    = g.Forward(0, x);
    //
    ok  &= y[0] == 1.0 / x[0];
    ok  &= y[1] == x[1] * x[1];
    //
    return ok;
}

} // END_EMPTY_NAMESPACE

bool llvm_tst(void)
{   bool ok = true;
    ok     &= tst_initialize();
    ok     &= tst_adfun_print();
    ok     &= tst_cppad_link_print();
    ok     &= tst_link_lib();
    ok     &= tst_llvm_ir();
    ok     &= tst_load();
    ok     &= tst_azmul();
    ok     &= tst_unary();
    ok     &= tst_binary();
    ok     &= tst_cexp();
    ok     &= tst_compare_1();
    ok     &= tst_compare_2();
    ok     &= tst_sum();
    ok     &= tst_atomic();
    return ok;
}
