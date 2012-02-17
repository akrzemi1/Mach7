#include "match.hpp"
#include <complex>
#include <iostream>

struct gcd_functor
{
    template <class A> 
    A operator()(const A& m, const A& n) const
    { 
        return m; 
    }
};

template <typename E1, typename E2> auto gcd(E1&& e1, E2&& e2) -> XTL_RETURN_ENABLE_IF
(
    is_expression<E1>::value && is_expression<E2>::value, 
    make_expr<gcd_functor>(
        filter(std::forward<E1>(e1)),
        filter(std::forward<E2>(e2))
    )
)

#include <cmath>
struct sqrt_functor
{
    template <class A> 
    A operator()(const A& a) const { return std::sqrt(a); }
};

template <typename E> auto sqrt(E&& e) -> XTL_RETURN_ENABLE_IF
(
    is_expression<E>::value, 
    make_expr<sqrt_functor>(filter(std::forward<E>(e)))
)

template <typename E1, typename S>
inline bool solve(const expr<sqrt_functor,E1>& e, const S& r)
{
    // sqrt(e) = r  => e = r^2;
    return solve(e.m_e1,r*r);
}

template <>
inline bool solve(const expr<division,var_ref<variable<double>>,var_ref<variable<double>>>& e, const double& r)
{
    // n/m = 3.1415926 => n=22,m=7
    long m[2][2];
    double x, startx;
    long maxden;
    long ai;

    startx = x = r;
    maxden = 100;

    /* initialize matrix */
    m[0][0] = m[1][1] = 1;
    m[0][1] = m[1][0] = 0;

    /* loop finding terms until denom gets too big */
    while (m[1][0] *  ( ai = (long)x ) + m[1][1] <= maxden) {
        long t;
        t = m[0][0] * ai + m[0][1];
        m[0][1] = m[0][0];
        m[0][0] = t;
        t = m[1][0] * ai + m[1][1];
        m[1][1] = m[1][0];
        m[1][0] = t;
        if(x==(double)ai) break;     // AF: division by zero
        x = 1/(x - (double) ai);
        if(x>(double)0x7FFFFFFF) break;  // AF: representation failure
    } 

    ///* now remaining x is between 0 and 1/ai */
    ///* approx as either 0 or 1/m where m is max that will fit in maxden */
    ///* first try zero */
    //printf("%ld/%ld, error = %e\n", m[0][0], m[1][0], startx - ((double) m[0][0] / (double) m[1][0]));

    ///* now try other possibility */
    //ai = (maxden - m[1][1]) / m[1][0];
    //m[0][0] = m[0][0] * ai + m[0][1];
    //m[1][0] = m[1][0] * ai + m[1][1];
    //printf("%ld/%ld, error = %e\n", m[0][0], m[1][0], startx - ((double) m[0][0] / (double) m[1][0]));
    e.m_e1(m[0][0]);
    e.m_e2(m[1][0]);
    return true;
}


int main()
{
    const    std::complex<double>  i(0.0,1.0);
             std::complex<double>  c(1,1);
    variable<std::complex<double>> d;
    variable<double> a,b,r,f;
    wildcard _;

    std::cout << a << "+"    << b << "*i" << std::endl;

    //if (match<std::complex<double>>(b*i)(i))   std::cout << b << "*i" << std::endl;
    if (match<std::complex<double>>(b*i+a)(i)) std::cout << a << "+"    << b << "*i" << std::endl;
    if (match<double>(sqrt(a))(4.0))   std::cout << "sqrt("  << a << ")=4" << std::endl;
    if (match<double>(a/b)(3.1415926)) std::cout << "3.1415926=" << a << "/" << b << std::endl;
    if (match<double>(-a)(3.1415926))  std::cout << "-3.1415926=" << a << std::endl;

//    if (match<std::complex<double>>(i*a)(c)) std::cout << "(2,2) + " << a << " * i" << std::endl;
    if (match<std::complex<double>>(d*i)(c)) std::cout << d << " * i" << std::endl;
	if (match<std::complex<double>>(i)(c))   std::cout << "i" << std::endl;
    if (match<std::complex<double>>(d)(c))   std::cout << d << std::endl;

    int n = 3;
    variable<size_t> m;

    Match(n)
    {
        When(m)   std::cout << m << std::endl;
        When(m+0) std::cout << m << "+0" << std::endl;
        When(m+1) std::cout << m << "+1" << std::endl;
        When(m+2) std::cout << m << "+2" << std::endl;
        When(m+3) std::cout << m << "+3" << std::endl;
        When(m+4) std::cout << m << "+4" << std::endl;
        When(m+5) std::cout << m << "+5" << std::endl;
    }
    EndMatch
}
