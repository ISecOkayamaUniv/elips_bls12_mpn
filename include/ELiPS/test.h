#ifndef TEST_H
#define TEST_H

#include <ELiPS/bls12_test.h>
#include <ELiPS/lazy_test.h>
#include <ELiPS/Jacobian_test.h>


extern void test_Field();
extern void test_compressed();
extern void test_Frobenius_map();
extern void test_skew_frobenius_map();
extern void test_twist();
extern int test_mod(int mulmod,int mod);
int test_BLS12_opt_ate_pairing_compress(int pairing);
extern void test_All();
#endif
