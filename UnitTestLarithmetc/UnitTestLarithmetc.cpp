#include "pch.h"
#include "CppUnitTest.h"
#include "../larithmetic/crypto.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTestLarithmetc
{
	TEST_CLASS(UnitTestLarithmetc)
	{
	public:
		
		TEST_METHOD(TestPSWPrime_smallMustBePrime)
		{
			BigInt a = 13;
			Assert::AreEqual(true, psw_prime(a));
		}

		TEST_METHOD(TestPSWPrime_smallMustNotBePrime)
		{
			BigInt a = 27;
			Assert::AreEqual(false, psw_prime(a));
		}

		TEST_METHOD(TestPSWPrime_bigMustBePrime)
		{
			BigInt a = BigInt("123426017006182806728593424683999798008235734137469123231828679");
			Assert::AreEqual(true, psw_prime(a));
		}

		TEST_METHOD(TestPSWPrime_bigMustNotBePrime)
		{
			BigInt a = BigInt("1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139");
			Assert::AreEqual(false, psw_prime(a));
		}

		TEST_METHOD(TestPresentAsBase_2)
		{
			BigInt a = 31;
			Assert::AreEqual(string("2base : 11111"), a.present_as_base(2));
		}

		TEST_METHOD(TestPresentAsBase_8)
		{
			BigInt a = 31;
			Assert::AreEqual(string("8base : 37"), a.present_as_base(8));
		}

		TEST_METHOD(TestPresentAsBase_16)
		{
			BigInt a = 31;
			Assert::AreEqual(string("16base : 1 | 15"), a.present_as_base(16));
		}

		TEST_METHOD(TestPresentAsBase_64)
		{
			BigInt a = 31;
			Assert::AreEqual(string("64base : 31"), a.present_as_base(64));
		}

		TEST_METHOD(TestPrimeMillerRabin_smallMustBePrime) {
			BigInt a = 17;
			Assert::AreEqual(true, testPrimeMillerRabin(a, 1));
		}

		TEST_METHOD(TestPrimeMillerRabin_smallMustNotBePrime) {
			BigInt a = 20;
			Assert::AreEqual(false, testPrimeMillerRabin(a, 1));
		}

		TEST_METHOD(TestPrimeMillerRabin_bigMustBePrime) {
			BigInt a = BigInt("123426017006182806728593424683999798008235734137469123231828679");
			Assert::AreEqual(true, testPrimeMillerRabin(a,1));
		}

		TEST_METHOD(TestPrimeMillerRabin_bigMustNotBePrime) {
			BigInt a = BigInt("1522605027922533360535618378132637429718068114961380688657908494580122963258952897654000350692006139");
			Assert::AreEqual(false, testPrimeMillerRabin(a,1));
		}
	};
}
