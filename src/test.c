#include <ELiPS/test.h>
/*----------------------------------------------------------------------------*/
//time
float timedifference_msec(struct timeval tv_start, struct timeval tv_end){
    return (tv_end.tv_sec - tv_start.tv_sec) * 1000.0f + (tv_end.tv_usec - tv_start.tv_usec) / 1000.0f;
}

float timedifference_usec(struct timeval tv_start, struct timeval tv_end){
    return (tv_end.tv_sec - tv_start.tv_sec) + (tv_end.tv_usec - tv_start.tv_usec);
}

/*----------------------------------------------------------------------------*/
//test
void test_Field(){
    printf("====================================================================================\n");
    Fp12 A_Fp12,test1,test2;
    Fp12_init(&A_Fp12);
    Fp12_init(&test1);
    Fp12_init(&test2);
    mpz_t exp;
    mpz_init(exp);
    
    gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned long)time(NULL));
    Fp12_set_random(&A_Fp12,state);
    Fp12_printf(&A_Fp12,"");
    printf("\n\n");
    
    printf("mul/sqr\n");
    Fp12_mul(&test1,&A_Fp12,&A_Fp12);
    Fp12_printf(&test1,"");
    printf("\n");
    
    Fp12_sqr(&test2,&A_Fp12);
    Fp12_printf(&test2,"");
    printf("\n");
    
    if(Fp12_cmp(&test1,&test2)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
    
    printf("pow/inv\n");
    mpz_pow_ui(exp,prime_z,12);
    mpz_sub_ui(exp,exp,2);
    Fp12_pow(&test1,&A_Fp12,exp);
    Fp12_printf(&test1,"");
    printf("\n");
    
    Fp12_inv(&test2,&A_Fp12);
    Fp12_printf(&test2,"");
    printf("\n");
    
    if(Fp12_cmp(&test1,&test2)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
}

void test_compressed(int n){
    printf("====================================================================================\n");
    int i;
    struct timeval tv_A,tv_B;
    float cyc_time=0,com_time=0,rec_time=0,gs_time=0;
    Fp12 f,test1,test2,test3;
    EFp12 P,Q;
    Fp12 tmp,t0,t1;
    
    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));
    
    
    for(i=0;i<n;i++){
    BLS12_EFp12_generate_G1(&P);
    EFp12_generate_G2(&Q);
    BLS12_Miller_algo_for_opt_ate(&f,&P,&Q);
    
    //EASY PART
    Fp12_frobenius_map_p6(&t0,&f);//f^(p^6)
    Fp12_inv(&t1,&f);//f^-1
    Fp12_mul(&tmp,&t0,&t1);//f^(p^6)*f^-1
    
    Fp12_frobenius_map_p2(&t0,&tmp);//f^(p^2)
    Fp12_mul(&tmp,&t0,&tmp);//f^(p^2)*f
    
    
    gettimeofday(&tv_A,NULL);
    Fp12_sqr_cyclotomic(&test1,&tmp);
    gettimeofday(&tv_B,NULL);
    cyc_time+=timedifference_msec(tv_A,tv_B);
    
    gettimeofday(&tv_A,NULL);
    Fp12_sqr_compressed(&test2,&tmp);
    gettimeofday(&tv_B,NULL);
    com_time+=timedifference_msec(tv_A,tv_B);
    
    gettimeofday(&tv_A,NULL);
    Fp12_sqr_recover_g1(&test2,&test2);
    Fp12_sqr_recover_g0(&test2,&test2);
    gettimeofday(&tv_B,NULL);
    rec_time+=timedifference_msec(tv_A,tv_B);
    
    gettimeofday(&tv_A,NULL);
    Fp12_sqr_GS(&test3,&tmp);
    gettimeofday(&tv_B,NULL);
    gs_time+=timedifference_msec(tv_A,tv_B);

    }
    
    printf("Sqr cyclotomic.     : %.4f[ms]\n",cyc_time/n);
    printf("Sqr compressed.     : %.4f[ms]\n",com_time/n);
    printf("Sqr co_recover.     : %.4f[ms]\n",rec_time/n);
    printf("Sqr Karabina.       : %.4f[ms]\n",(com_time+rec_time)/n);
    printf("Sqr GS.             : %.4f[ms]\n",gs_time/n);
    
    if(Fp12_cmp(&test1,&test2)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
        /*
        Fp2_printf(&test1.x1.x0,"test1(g0)=");
        printf("\n");
        Fp2_printf(&test2.x1.x0,"test2(g0)=");
        printf("\n\n");
    
        Fp2_printf(&test1.x1.x1,"test1(g1)=");
        printf("\n");
        Fp2_printf(&test2.x1.x1,"test2(g1)=");
        printf("\n\n");
        
        Fp2_printf(&test1.x1.x0,"test1(g2)=");
        printf("\n");
        Fp2_printf(&test2.x1.x0,"test2(g2)=");
        printf("\n\n");
    
        Fp2_printf(&test1.x0.x2,"test1(g3)=");
        printf("\n");
        Fp2_printf(&test2.x0.x2,"test2(g3)=");
        printf("\n\n");
    
        Fp2_printf(&test1.x0.x1,"test1(g4)=");
        printf("\n");
        Fp2_printf(&test2.x0.x1,"test2(g4)=");
        printf("\n\n");
    
        Fp2_printf(&test1.x1.x2,"test1(g5)=");
        printf("\n");
        Fp2_printf(&test2.x1.x2,"test2(g5)=");
        printf("\n");
        */
    }
    
}
void test_Frobenius_map(){
    printf("====================================================================================\n");
    Fp12 A_Fp12,test1,test2;
    Fp12_init(&A_Fp12);
    Fp12_init(&test1);
    Fp12_init(&test2);
    mpz_t exp;
    mpz_init(exp);
    
    gmp_randinit_default (state);
	gmp_randseed_ui(state,(unsigned long)time(NULL));
    Fp12_set_random(&A_Fp12,state);
    Fp12_printf(&A_Fp12,"");
    printf("\n\n");
    
    printf("frobenius\n");
    Fp12_frobenius_map_p10(&test1,&A_Fp12);
    Fp12_printf(&test1,"");
    printf("\n\n");
    
    mpz_pow_ui(exp,prime_z,10);
    Fp12_pow(&test2,&A_Fp12,exp);
    Fp12_printf(&test2,"");
    printf("\n");
    
    if(Fp12_cmp(&test1,&test2)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
    
    mpz_clear(exp);
}

void test_skew_frobenius_map(){
    printf("====================================================================================\n");
    EFp12 Q,test1,test2;
    EFp12_init(&Q);
    EFp12_init(&test1);
    EFp12_init(&test2);
    EFp2 twisted_Q;
    EFp2_init(&twisted_Q);
    
    EFp12_generate_G2(&Q);
    EFp12_to_EFp2(&twisted_Q,&Q);
    
    Fp12_frobenius_map_p10(&test1.x,&Q.x);
    Fp12_frobenius_map_p10(&test1.y,&Q.y);
    EFp12_printf(&test1,"");
    printf("\n\n");
    
    EFp2_skew_frobenius_map_p10(&twisted_Q,&twisted_Q);
    EFp2_to_EFp12(&test2,&twisted_Q);
    EFp12_printf(&test2,"");
    printf("\n\n");
    
    if(Fp12_cmp(&test1.x,&test2.x)==0 && Fp12_cmp(&test1.y,&test2.y)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
}


void test_twist(){
    printf("====================================================================================\n");
    EFp12 Q,test1,test2;
    EFp12_init(&Q);
    EFp12_init(&test1);
    EFp12_init(&test2);
    EFp2 twist_Q;
    EFp2_init(&twist_Q);
    
    
    EFp12_generate_G2(&Q);
    EFp12_printf(&Q,"Q\n");
    printf("\n\n");
    
    EFp12_to_EFp2(&twist_Q,&Q);
    EFp2_ECD(&twist_Q,&twist_Q);
    EFp2_to_EFp12(&test1,&twist_Q);
    EFp12_printf(&test1,"");
    printf("\n\n");
    
    EFp12_ECD(&test2,&Q);
    EFp12_printf(&test2,"");
    printf("\n\n");
    
    if(Fp12_cmp(&test1.x,&test2.x)==0 && Fp12_cmp(&test1.y,&test2.y)==0){
        printf("success\n\n");
    }else{
        printf("failed\n\n");
    }
    
}
int test_mod(int mulmod,int mod){
    int i,n=0;
    float add_time=0,dbl_time=0,mul_time=0,sqr_time=0,mod_time=0,mod_mont_time=0;
    struct timeval tv_A,tv_B;
    printf("====================================================================================\n");
    printf("Field Mod test\n");
    mp_limb_t A[FPLIMB],B[FPLIMB],C[FPLIMB],C1[FPLIMB2];
    mp_limb_t At[FPLIMB2],Bt[FPLIMB2],Ct[FPLIMB2],test_mul[FPLIMB2];
    Fp test1,test2;

    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));

for(i=0;i<mulmod;i++){
    mpn_random(A,FPLIMB);
    mpn_random(B,FPLIMB);

    Lazy_mod(A,A,FPLIMB);
    Lazy_mod(B,B,FPLIMB);

    mpn_mul_n(At,A,A,FPLIMB);
    mpn_mul_n(Bt,B,B,FPLIMB);

    gettimeofday(&tv_A,NULL);
    mpn_add_n(test_mul,At,At,FPLIMB2);
    gettimeofday(&tv_B,NULL);
    add_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    mpn_lshift(test_mul,At,FPLIMB2,1);
    gettimeofday(&tv_B,NULL);
    dbl_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    mpn_mul_n(test_mul,A,A,FPLIMB);
    gettimeofday(&tv_B,NULL);
    mul_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    mpn_sqr(test_mul,A,FPLIMB);
    gettimeofday(&tv_B,NULL);
    sqr_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    mpn_mod(&test1,test_mul,FPLIMB2);
    gettimeofday(&tv_B,NULL);
    mod_time+=timedifference_msec(tv_A,tv_B);

}
    printf("Fp add.            : %.6f[ms]\n",add_time/mulmod);
    printf("Fp dbl.            : %.6f[ms]\n",dbl_time/mulmod);
    printf("Fp mod.            : %.6f[ms]\n",mod_time/mulmod);
    printf("Fp sqr.            : %.6f[ms]\n",sqr_time/mulmod);
    printf("Fp mul.            : %.6f[ms]\n",mul_time/mulmod);
    printf("Fp add + mul * 2.  : %.6f[ms]\n",(add_time+(mul_time*2))/mulmod);
mod_time=0;
for(i=0;i<mod;i++){
    mpn_random(A,FPLIMB);
    mpn_random(B,FPLIMB);

    Lazy_mod(A,A,FPLIMB);
    Lazy_mod(B,B,FPLIMB);

    mpn_mul_n(C1,A,B,FPLIMB);

    Fp_mul_montgomery(At,FPLIMB2,A,FPLIMB);
    Fp_MR(A,At,FPLIMB2);

    Fp_mul_montgomery(Bt,FPLIMB2,B,FPLIMB);
    Fp_MR(B,Bt,FPLIMB2);

    mpn_mul_n(Ct,A,B,FPLIMB);
    Fp_MR(C,Ct,FPLIMB2);

    gettimeofday(&tv_A,NULL);
    mpn_mod(&test1,C1,FPLIMB2);
    gettimeofday(&tv_B,NULL);
    mod_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    Fp_MR(C,Ct,FPLIMB2);
    gettimeofday(&tv_B,NULL);
    mod_mont_time+=timedifference_msec(tv_A,tv_B);

    Fp_MR(test2.x0,C,FPLIMB);

    if(Fp_cmp(&test1,&test2)!=0){
        printf("failed!\n\n");
	Fp_printf(&test1,"");
	Fp_printf(&test2,"\n");
	printf("\n\n");
	return 1;
    }
}
    printf("Fp mod.            : %.6f[ms]\n",mod_time/mod);
    printf("Fp mod montgomery. : %.6f[ms]\n",mod_mont_time/mod);
    
}

int test_BLS12_opt_ate_pairing_compress(int pairing){
    int i,n=0;
    float opt_time=0,opt_compress_time=0,opt_compress_lazy_time=0,opt_lazy_time=0;
    struct timeval tv_A,tv_B;
    printf("====================================================================================\n");
    printf("BLS12_Opt-ate pairing\n\n");

    EFp12 P,Q;
    EFp12_init(&P);
    EFp12_init(&Q);

    Fp12 Z,test1,test2,test3,test4;
    Fp12_init(&Z);
    Fp12_init(&test1);
    Fp12_init(&test2);
    Fp12_init(&test3);
    Fp12_init(&test4);


    gmp_randinit_default (state);
    gmp_randseed_ui(state,(unsigned long)time(NULL));

for(i=0;i<pairing;i++){

    BLS12_EFp12_generate_G1(&P);
    EFp12_generate_G2(&Q);

    gettimeofday(&tv_A,NULL);
    BLS12_Opt_ate_pairing(&test1,&P,&Q);
    gettimeofday(&tv_B,NULL);
    opt_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BLS12_Opt_ate_pairing_compress(&test2,&P,&Q);
    gettimeofday(&tv_B,NULL);
    opt_compress_time+=timedifference_msec(tv_A,tv_B);

    gettimeofday(&tv_A,NULL);
    BLS12_Opt_ate_pairing_compress_lazy(&test3,&P,&Q);
    gettimeofday(&tv_B,NULL);
    opt_compress_lazy_time+=timedifference_msec(tv_A,tv_B);
    
    gettimeofday(&tv_A,NULL);
    BLS12_Opt_ate_pairing_lazy(&test4,&P,&Q);
    gettimeofday(&tv_B,NULL);
    opt_lazy_time+=timedifference_msec(tv_A,tv_B);
    
    if(Fp12_cmp(&test1,&test2)!=0 || Fp12_cmp(&test1,&test3)!=0 || Fp12_cmp(&test1,&test4)!=0){
        printf("failed!\n\n");
	Fp12_printf(&test1,"");
	Fp12_printf(&test2,"\n");
	printf("\n\n");
	return 1;
    }
}

    printf("BLS12 opt ate.                : %.4f[ms]\n",opt_time/pairing);
    printf("BLS12 opt ate compress.       : %.4f[ms]\n",opt_compress_time/pairing);
    printf("BLS12 opt ate compress lazy.  : %.4f[ms]\n",opt_compress_lazy_time/pairing);
    printf("BLS12 opt ate lazy.           : %.4f[ms]\n",opt_lazy_time/pairing);

	return 0;
}
void test_All(){
	int test_point,test_G1,test_G2,test_G3,test_pairing;
	int Lazy_Field,Lazy_EFp,Lazy_EFp2,Lazy_EFp12,Lazy_G1,Lazy_G2,Lazy_G3,Lazy_pairing;
	int Jacobian_EFp,Jacobian_EFp2,Jacobian_G1;
	
printf("====================================================================================\n");
    printf("Normal test\n");
	
	test_point = BLS12_test_rational_point();
	test_G1 = BLS12_test_G1_SCM();
	test_G2 = BLS12_test_G2_SCM();
	test_G3 = BLS12_test_G3_EXP();
	test_pairing = BLS12_test_opt_ate_pairing();

printf("====================================================================================\n");
    printf("Lazy test\n");
	
	Lazy_Field = test_Field_Lazy(100,100,100);
	Lazy_EFp = test_EFp_Lazy(100,100,10);
	Lazy_EFp2 = test_EFp2_Lazy(10,10,10);
	Lazy_EFp12 = test_EFp12_Lazy(10,10,1);
	Lazy_G1 = test_BLS12_G1_SCM_Lazy(10);
	Lazy_G2 = test_BLS12_G2_SCM_Lazy(1);
	Lazy_G3 = test_BLS12_G3_exp_Lazy(1);
	Lazy_pairing = test_BLS12_opt_ate_pairing_Lazy(1);

printf("====================================================================================\n");
    printf("Jacobian test\n");

	
	Jacobian_EFp = test_EFp_Jacobian(100,100,10);
	Jacobian_EFp2 = test_EFp2_Jacobian(100,100,10);
	Jacobian_G1 = test_BLS12_G1_SCM_Jacobian(10);


printf("====================================================================================\n");
    printf("Test Result\n\n");

printf("------------------------------------------------------------------------------------\n");
    printf("Normal test\n");

	printf("Test Rational Point  :");
	if(test_point==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G1 test         :");
	if(test_G1==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G2 test         :");
	if(test_G2==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G3 test         :");
	if(test_G3==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test pairing test    :");
	if(test_pairing==0) printf("Success\n");
	else printf("Failed\n");

printf("------------------------------------------------------------------------------------\n");
    printf("Lazy Reduction test\n");

	printf("Test Field Lazy      :");
	if(Lazy_Field==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test EFp Lazy        :");
	if(Lazy_EFp==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test EFp2 Lazy       :");
	if(Lazy_EFp2==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test EFp12 Lazy      :");
	if(Lazy_EFp12==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G1 Lazy         :");
	if(Lazy_G1==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G2 Lazy         :");
	if(Lazy_G2==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G3 Lazy         :");
	if(Lazy_G3==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test pairing Lazy    :");
	if(Lazy_pairing==0) printf("Success\n");
	else printf("Failed\n");

printf("------------------------------------------------------------------------------------\n");
    printf("Jacobian test\n");

	printf("Test EFp Jacobian    :");
	if(Jacobian_EFp==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test EFp2 Jacobian   :");
	if(Jacobian_EFp2==0) printf("Success\n");
	else printf("Failed\n");

	printf("Test G1 Jacobian     :");
	if(Jacobian_G1==0) printf("Success\n");
	else printf("Failed\n");
}
