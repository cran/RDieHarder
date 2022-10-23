/*
 * This is an example header file for a test.  For existing tests these
 * headers are in the library includes already, but this one is an example
 * suitable for use as a template.
 */

#pragma once

/*
 * The function needs a prototype.  In order to use the standard test
 * creation/destruction/execution routines, the prototype should have
 * precisely this form.
 */
int rgb_lagged_sums(Test **test,int irun);

/*
 * This is default data for the test at hand.  The first field is
 * the test name.  The second is the test description.  The third
 * is the default number of p-values generated by a run for display
 * in a histogram and to generate a cumulative test p-value using
 * Kuiper-Kolmogorov-Smirnov.  The fourth is the number of "samples"
 * accumulated per test, if relevant (some tests do not permit this
 * to be varied).  The fifth and final Dtest parameter is the number
 * of statistics generated by the test (per test invocation) -- usually
 * this will be one but for several it is two and could be more.
 */
static Dtest rgb_lagged_sums_dtest __attribute__((unused)) = {
  "RGB Lagged Sum Test",
  "rgb_lagged_sum",
  "\
#==================================================================\n\
#                     RGB Lagged Sums Test\n\
# This package contains many very lovely tests.  Very few of them,\n\
# however, test for lagged correlations -- the possibility that\n\
# the random number generator has a bitlevel correlation after\n\
# some fixed number of intervening bits.\n\
#\n\
# The lagged sums test is therefore very simple.   One simply adds up\n\
# uniform deviates sampled from the rng, skipping lag samples in between\n\
# each rand used.  The mean of tsamples samples thus summed should be\n\
# 0.5*tsamples.  The standard deviation should be sqrt(tsamples/12).\n\
# The experimental values of the sum are thus converted into a\n\
# p-value (using the erf()) and a ks-test applied to psamples of them.\n\
#==================================================================\n",
  100,
  1000000,
  1,
  rgb_lagged_sums,
  0
};

