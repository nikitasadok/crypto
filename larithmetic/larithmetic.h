#include <iostream>
#include <vector>

using namespace std;

class BigInt {
private:
	bool isNegative;
	vector <long long> digits;

public:
	static const long long BASE_LEN;
	static const long long BASE;


	BigInt();
	BigInt(string s);
	BigInt(long long x);
	BigInt(vector<long long> digits_, bool isNegative_);
	BigInt(const BigInt& bigInt);

	void removeLeadingZeros();
	void clearNumber();
	BigInt abs() const;
	static string formatOutput(long long x);
	static bool isDigit(char ch);
	bool isZero();
	bool isPositiveOne();
	bool isNegativeOne();
	BigInt sqrt();
	BigInt pow(BigInt n);
	BigInt pow(BigInt* n, const BigInt& mod);
	BigInt reversedBySimpleMod(BigInt mod);
	BigInt mathMod(BigInt mod);
	long long getLength() const;
	vector<long long> getDigits();

	friend ostream& operator << (ostream& os, BigInt bigInt);
	friend istream& operator >> (istream& is, BigInt& bigInt);

	friend bool operator == (BigInt bigInt1, BigInt bigInt2);
	friend bool operator == (BigInt bigInt1, long long int2);
	friend bool operator == (long long int1, BigInt bigInt2);

	friend bool operator != (BigInt bigInt1, BigInt bigInt2);
	friend bool operator != (BigInt bigInt1, long long int2);
	friend bool operator != (long long int1, BigInt bigInt2);

	friend bool operator > (BigInt bigInt1, BigInt bigInt2);
	friend bool operator > (BigInt bigInt1, long long int2);
	friend bool operator > (long long int1, BigInt bigInt2);

	friend bool operator < (BigInt bigInt1, BigInt bigInt2);
	friend bool operator < (BigInt bigInt1, long long int2);
	friend bool operator < (long long int1, BigInt bigInt2);

	friend bool operator >= (BigInt bigInt1, BigInt bigInt2);
	friend bool operator >= (BigInt bigInt1, long long int2);
	friend bool operator >= (long long int1, BigInt bigInt2);

	friend bool operator <= (BigInt bigInt1, BigInt bigInt2);
	friend bool operator <= (BigInt bigInt1, long long int2);
	friend bool operator <= (long long int1, BigInt bigInt2);

	friend BigInt operator + (const BigInt& bigInt1, const BigInt& bigInt2);
	friend BigInt operator + (BigInt bigInt1, long long int2);
	friend BigInt operator + (long long int1, BigInt bigInt2);
	BigInt& operator += (BigInt other); // not implemented
	BigInt& operator += (long long other); // not implemented
	friend const BigInt operator ++ (BigInt& a, int);

	friend BigInt operator - (const BigInt& bigInt1, const BigInt& bigInt2);
	friend BigInt operator - (BigInt bigInt1, long long int2);
	friend BigInt operator - (long long int1, BigInt bigInt2);
	BigInt operator - ();
	BigInt& operator -= (BigInt other); // not implemented
	BigInt& operator -= (long long other); // not implemented

	friend BigInt operator * (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator * (BigInt bigInt1, long long int2);
	friend BigInt operator * (long long int1, BigInt bigInt2);
	friend BigInt karatsuba(const BigInt& bigInt1, const BigInt& bigInt2);
	BigInt& operator *= (BigInt other); // not implemented
	BigInt& operator *= (long long other); // not implemented

	BigInt div2();
	BigInt mod2();

	friend BigInt operator / (BigInt bigInt1, BigInt bigInt2); // not implemented
	friend BigInt operator / (BigInt bigInt1, long long int2);
	friend BigInt operator / (long long int1, BigInt bigInt2);
	BigInt& operator /= (BigInt other); // not implemented
	BigInt& operator /= (long long other); // not implemented

	friend BigInt operator % (BigInt bigInt1, BigInt bigInt2);
	friend BigInt operator % (BigInt bigInt1, long long int2);
	friend BigInt operator % (long long int1, BigInt bigInt2);


	BigInt addBySimpleMod(BigInt other, BigInt mod);
	BigInt subBySimpleMod(BigInt other, BigInt mod);
	BigInt mulBySimpleMod(BigInt other, BigInt mod);
	BigInt divBySimpleMod(BigInt other, BigInt mod);
	BigInt powBySimpleMod(BigInt n, BigInt mod);
	string present_as_base(int n);

	friend BigInt reverse(BigInt a);
};


BigInt gcd(BigInt a, BigInt b);

BigInt randBigInt(BigInt p);

BigInt rhoPollard(BigInt n, BigInt iterationsCount);
BigInt fPollard(BigInt a, BigInt mod);
BigInt getDivisor(BigInt n, long long countRetry);
vector<pair<BigInt, BigInt>> rhoPollardFactorization(BigInt n);
pair<BigInt, BigInt> factorization2Primes(BigInt n, long long countRetry);
bool psw_prime(BigInt n);
BigInt get_lucas_seq_element(BigInt n, int Q, int D);

BigInt babyStepGiantStepLog(BigInt a, BigInt b, BigInt p);

BigInt euler(BigInt n);
BigInt mobius(BigInt n);

BigInt legendre(BigInt n, BigInt p);
BigInt jacobi(BigInt n, BigInt m);

pair<BigInt, BigInt> cipolla(BigInt a, BigInt p, long long countRetry);

BigInt get_lucas_seq_element(BigInt n, int Q, int D);
vector<bool> get_element_numbers(BigInt n);
bool low_divisible(const BigInt& a);

class SqrtPolynomial2 {
private:
	BigInt x, y;
	BigInt w;

public:
	SqrtPolynomial2(BigInt x_, BigInt y_, BigInt w_);
	SqrtPolynomial2 pow(BigInt n, BigInt mod);
	BigInt getX();
};

bool testPrimeMillerRabin(BigInt a, long long countRetry);

BigInt hexToBigInt(string s);

class Point {
private:
	BigInt x, y;
	bool isZero;
public:
	Point();
	Point(BigInt x_, BigInt y_);
	Point(bool isZero_);
	Point operator - ();

	BigInt getX();
	BigInt getY();

	friend bool operator == (Point p1, Point p2);
	friend bool operator != (Point p1, Point p2);

	friend ostream& operator << (ostream& os, Point bigInt);

	friend class ElGamal;
};


class ElGamal {
private:
	BigInt p;
	BigInt a, b;
	BigInt n;
	Point P;
public:
	ElGamal();
	ElGamal(BigInt p_, BigInt a_, BigInt b_, BigInt n_, Point P_);
	pair<Point, Point> encrypt(Point M, Point Y);
	Point decrypt(pair<Point, Point> c, BigInt k);
	Point addPoints(Point p1, Point p2);
	Point mulPoint(BigInt k, Point p1);
	Point getP();
	void testPoint(Point p1);
	Point getRandomPointOnCurve(long long countRetry);
	BigInt getRandomSecretKey();
};

