/*
 *========================================================================
 * See copyright in copyright.h and the accompanying file COPYING
 *========================================================================
 */

#include "dieharder.h"

void table_line(Dtest *dtest,Test **test);

void output(Dtest *dtest,Test **test)
{

/*
 * This is a one-stop-shop output routine for standard output in all
 * run_whatever_test() routines.  It manages all output format choices,
 * specifically the choice between report vs table output and the
 * control of just what fields or options are selected for output.
 */

 /*
  * This almost certainly belongs in the show_test_results section,
  * possibly with additional conditionals rejecting test results involving
  * rewinds, period.
  */
 if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
   if(!quiet && !table){
     fprintf(stdout,"# %u rands were used in this test\n",file_input_get_rtot(rng));
   }
   if(file_input_get_rewind_cnt(rng) != 0){
     fprintf(stderr,"# The file %s was rewound %u times\n",gsl_rng_name(rng),file_input_get_rewind_cnt(rng));
     fflush(stderr);
   }
 }

 /*
  * Show standard test results, for all statistics generated by a single run.
  * Output can either be a report (with histogram and all) or a table
  * line.
  */
 if(table) {
   table_line(dtest,test);
 } else {
   if(!quiet){
     show_test_header(dtest,test);
   }
   show_test_results(dtest,test);
 }

}

/*
 * Write a standard test header.
 */
void show_test_header(Dtest *dtest,Test **test)
{

 /*
  * Show standard test description
  */
 printf("%s",dtest->description);

 /*
  * This is the non-custom part of the header, which should be enough
  * for nearly all tests by design.
  */
 printf("#                        Run Details\n");

 /*
  * Show the generator being tested, handling file_input generators
  * separately.
  */
 if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
   printf("# Random number generator tested: %s\n",gsl_rng_name(rng));
   printf("# File %s contains %u rands of %c type.\n",filename,filecount,filetype);
 } else {
   printf("# Random number generator tested: %s\n",gsl_rng_name(rng));
 }

 /*
  * Now show both current and default values for tsamples and psamples.
  */
 printf("# Samples per test pvalue = %u (test default is %u)\n",test[0]->tsamples,dtest->tsamples_std);
 printf("# P-values in final KS test = %u (test default is %u)\n",test[0]->psamples,dtest->psamples_std);

 fflush(stdout);
}


/*
 * Write a standard test footer.  Arguments include the test
 * variables, the test pvalue, the ks_pvalues, and a test description.
 */
void show_test_results(Dtest *dtest,Test **test)
{

 unsigned int i,j,k;

 /*
  * There may be more than one statistic (final p-value) generated by
  * this test; we loop over all of them.
  */
 /* printf("In show_test_results for %s for %u statistics\n",dtest->name,dtest->nkps); */
 for(i=0;i<dtest->nkps;i++){

   /*
    * Display histogram of ks p-values (optional)
    */
   if(hist_flag && quiet == 0){
     /* Debugging
     for(j=0;j<test[i]->psamples;j++){
       printf("pvalue = %f\n",test[i]->pvalues[j]);
       fflush(stdout);
     }
     Debugging */
     histogram(test[i]->pvalues,test[i]->pvlabel,test[i]->psamples,0.0,1.0,10,"p-values");
     fflush(stdout);
   }

   /*
    * This isn't quiet-suppressible, as it is important information; we do
    * print it to stderr, though, so that one can fairly easily filter even
    * it away...
    */
   if(strncmp("file_input",gsl_rng_name(rng),10) == 0){
     fprintf(stderr,"# %u rands were used in this test\n",file_input_get_rtot(rng));
     fprintf(stderr,"# The file %s was rewound %u times\n",gsl_rng_name(rng),file_input_get_rewind_cnt(rng));
     fflush(stderr);
   }

   #if !defined(RDIEHARDER)
   if(quiet == 0){
     printf("#                          Results\n");
     fflush(stdout);
   }
   if(test[i]->psamples == 1){
     printf("Single test: p = %10.8f\n",test[i]->ks_pvalue);
     printf("Assessment: ");
     /* Work through some ranges here */
     if(test[i]->ks_pvalue < 0.0001 || test[i]->ks_pvalue > 0.9999){
       printf("FAILED at < 0.02%% for %s\n",dtest->name);
     } else if(test[i]->ks_pvalue < 0.01 || test[i] -> ks_pvalue > 0.99){
       printf("POOR at < 2%% for %s\n",dtest->name);
       printf("Recommendation:  Repeat test to verify failure.\n");
     } else {
       printf("PASSED at > 2%% for %s\n",dtest->name);
     }
     fflush(stdout);
   } else {
     printf("Kuiper KS: p = %10.8f\n",test[i]->ks_pvalue);
     printf("Assessment: ");
     /* Work through some ranges here */
     if(test[i]->ks_pvalue < 0.0001){
       printf("FAILED at < 0.01%% for %s\n",dtest->name);
     } else if(test[i]->ks_pvalue < 0.01){
       printf("POOR at < 1%% for %s\n",dtest->name);
       printf("Recommendation:  Repeat test to verify failure.\n");
     } else if(test[i]->ks_pvalue < 0.05){
       printf("POSSIBLY WEAK at < 5%% for %s\n",dtest->name);
       printf("Recommendation:  Repeat test to verify failure.\n");
     } else {
       printf("PASSED at > 5%% for %s\n",dtest->name);
     }
     fflush(stdout);
   }
   #endif  /* !defined(RDIEHARDER) */
 }

 #if defined(RDIEHARDER)
 save_values_for_R(dtest, test);
 #endif /* RDIEHARDER */
}

/*
 * Print out all results in table format only
 */
void table_line(Dtest *dtest,Test **test)
{

 unsigned int i,j,k;

#if !defined(RDIEHARDER)

 /*
  * There may be more than one statistic (final p-value) generated by
  * this test; we loop over all of them.
  */
 for(i=0;i<dtest->nkps;i++){

   if(tflag & TNAME){
     fprintf(stdout,"|");
     if(fromfile){
       fprintf(stdout,"%-30s",filename);
     } else {
       fprintf(stdout,"%-30s",dtest->name);
     }
   }
   if(tflag & TSNAME){
     fprintf(stdout,"|");
     if(fromfile){
       fprintf(stdout,"%-20.20s",filename);
     } else {
       fprintf(stdout,"%-20.20s",dtest->sname);
     }
   }
   if(tflag & TNTUPLE){
     fprintf(stdout,"|");
     fprintf(stdout,"%6d",test[i]->ntuple);
   }
   if(tflag & TTSAMPLES){
     fprintf(stdout,"|");
     fprintf(stdout,"%12u",test[0]->tsamples);
   }
   if(tflag & TPSAMPLES){
     fprintf(stdout,"|");
     fprintf(stdout,"%8u",test[0]->psamples);
   }
   if(tflag & TPVALUE){
     fprintf(stdout,"|");
     fprintf(stdout,"%10.8f",test[i]->ks_pvalue);
   }
   if(tflag & TASSESSMENT){
     fprintf(stdout,"|");
     if(test[i]->ks_pvalue < 0.0001 || test[i]->ks_pvalue > 0.9999){
       fprintf(stdout,"%16s","FAILED(0.02%) ");
     } else if(test[i]->ks_pvalue < 0.01 || test[i]->ks_pvalue > 0.99){
       fprintf(stdout,"%16s","MAY BE WEAK(2%)");
     } else {
       fprintf(stdout,"%16s","PASSED     ");
     }
   }

   fprintf(stdout,"|\n");

 }

#endif  /* !defined(RDIEHARDER) */

}

void table_header()
{

 unsigned int i,j,k;

 #if !defined(RDIEHARDER)
 /*
  * We assemble the table header according to what tflag's value is.
  */
 if(tflag & THEADER == 0) return;   /* No header! */

 fprintf(stdout,"#==============================================================================\n");
 if(tflag & TGEN){
   fprintf(stdout,"#| Testing generator %s",gsl_rng_name(rng));
   if(fromfile){
     fprintf(stdout," (from file %s)",filename);
   } else {
     fprintf(stdout," -- Rands per second = %e.\n",rng_rands_per_second);
   }
 }
 
 fprintf(stdout,"#");
 if(tflag & TNAME){
   fprintf(stdout,"%30s","Test (short) Name");
 }
 if(tflag & TSNAME){
   fprintf(stdout,"%20s","Test (short) Name  ");
 }
 if(tflag & TNTUPLE){
   fprintf(stdout,"|");
   fprintf(stdout,"%6s","ntuple");
 }
 if(tflag & TTSAMPLES){
   fprintf(stdout,"|");
   fprintf(stdout,"%12s","  tsamples  ");
 }
 if(tflag & TPSAMPLES){
   fprintf(stdout,"|");
   fprintf(stdout,"%8s","psamples");
 }
 if(tflag & TPVALUE){
   fprintf(stdout,"|");
   fprintf(stdout,"%10s","p-value ");
 }
 if(tflag & TASSESSMENT){
   fprintf(stdout,"|");
   fprintf(stdout,"%16s","Assessment   ");
 }
 fprintf(stdout,"|\n");
 fprintf(stdout,"#==============================================================================\n");

#endif  /* !defined(RDIEHARDER) */

}

