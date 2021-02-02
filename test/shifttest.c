#include "ELiPS/bls12.h"
#include "ELiPS/fr.h"

/*============================================================================*/
/* main                                                                       */
/*============================================================================*/
int main(void){
    bls12_init();
    bls12_print_parameters();
    fr_order_init();
    //mpz_println("X=",X_z);
    //mpn_println("X=",X,FXLIMB);
    //getchar();

    //g1_test(100);
    //g2_test(100);
    //test_field(0,1000,0,0,0);
    //test_fp(1000);
    //test_fp2(10000);
    //test_fp6(100);
    //test_mod(1000000);
    //test_Frobenius_map();
    //test_skew_frobenius_map();
    //test_twist();
    //test_mod(10000000,100000);

    //test_efp(100,100);
    //test_efp2(1000,1000);
    //test_efp12(10,10,10);

    //bls12_test_rational_point();
    //bls12_test_opt_ate_pairing(100000);
    //g1_test(10);
    //g2_test(10);
    //g3_test(10);
    //debug_pairing(100);
    //bls12_test_g1_scm(100);
    //bls12_test_g2_scm(1);
    //bls12_test_g3_exp(100);
    test_fp_montgomery(100000);

    //test_All();
    return 0;
}
