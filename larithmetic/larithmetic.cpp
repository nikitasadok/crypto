#include <iostream>
#include <cstdlib>
#include <vector>
#include <map>
#include <chrono>
#include <random>

#include "larithmetic.h"

using namespace std;

const long long BigInt::BASE_LEN = 9;
const long long BigInt::BASE = 1000000000;

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<double> dist(0, BigInt::BASE);

std::random_device rd_2;
std::mt19937 mt_2(rd_2());
std::uniform_real_distribution<double> dist_2(0, INT_MAX);

BigInt::BigInt() {
	isNegative = false;
	digits.clear();
	digits.resize(1, 0);
}

BigInt::BigInt(string s) {
	digits.clear();
	if (s[0] == '-') {
		isNegative = true;
		s.erase(s.begin());
	}
	else {
		isNegative = false;
	}

	for (long long i = 0; i < (long long)s.size(); i++) {
		if (!isDigit(s[i])) {
			throw "ValueError";
		}
	}

	for (long long i = (long long)s.length(); i > 0; i -= BASE_LEN) {
		if (i < BASE_LEN) {
			digits.push_back(atoi(s.substr(0, i).c_str()));
		}
		else {
			digits.push_back(atoi(s.substr(i - BASE_LEN, BASE_LEN
			).c_str()));
		}
	}

	removeLeadingZeros();
}

BigInt::BigInt(long long x) {
	digits.clear();
	if (x < 0) {
		isNegative = true;
		x = -x;
	}
	else {
		isNegative = false;
	}

	if (x == 0) {
		digits.push_back(0);
	}
	else {
		while (x != 0) {
			digits.push_back(x % BASE);
			x /= BASE;
		}
	}
}

BigInt::BigInt(vector <long long> digits_, bool isNegative_) {
	digits = digits_;
	isNegative = isNegative_;
	removeLeadingZeros();
}

BigInt::BigInt(const BigInt& bigInt) {
	digits = bigInt.digits;
	isNegative = bigInt.isNegative;
}



/*
 * helpers
 */
void BigInt::removeLeadingZeros() {
	while (digits.size() > 1 && digits.back() == 0) {
		digits.pop_back();
	}
}

// 12345678
// 87654321
// 0000 * 4321
// 0000 * 432
void BigInt::clearNumber() {
	removeLeadingZeros();
	if (isZero()) {
		isNegative = false;
	}
}

BigInt BigInt::abs() const {
	BigInt res = BigInt(digits, false);
	return res;
}

string BigInt::formatOutput(long long x) {
	string s;
	s.clear();
	if (x == 0) {
		s = "0";
	}
	else {
		while (x != 0) {
			s = (char)('0' + x % 10) + s;
			x /= 10;
		}
	}
	while (s.size() < BASE_LEN) {
		s = '0' + s;
	}
	return s;
}

bool BigInt::isDigit(char ch) {
	return ch >= '0' && ch <= '9';
}

bool BigInt::isZero() {
	return (digits.size() == 1 && digits[0] == 0);
}

bool BigInt::isPositiveOne() {
	return (!isNegative && digits.size() == 1 && digits[0] == 1);
}

bool BigInt::isNegativeOne() {
	return (isNegative && digits.size() == 1 && digits[0] == 1);
}

BigInt BigInt::sqrt() {
	if (*this < 0) {
		throw "ValueError";
	}
	BigInt l(0), r(*this);
	while (r - l > 1) {
		BigInt m = (l + r) / 2;
		if (m * m <= *this) {
			l = m;
		}
		else {
			r = m;
		}
	}
	if (r * r <= *this) {
		return r;
	}
	else {
		return l;
	}
}

BigInt BigInt::pow(BigInt n) {
	if (n < 0) {
		throw "ValueError";
	}
	if (n == 0) {
		return 1;
	}
	if (n == 1) {
		return *this;
	}
	BigInt tmp = this->pow(n / 2);
	tmp = tmp * tmp;
	if (n % 2 == 1) {
		tmp = tmp * *this;
	}
	return tmp;
}

BigInt BigInt::pow(BigInt* n, const BigInt& mod) {
	if (*n < 0) {
		throw "ValueError";
	}

	if (*n == 0) {
		return 1;
	}
	if (*n == 1) {
		return *this % mod;
	}
	if (*n == 2) {
		return (*this * *this) % mod;
	}

	BigInt tmp = this->pow(&(*n / 2), mod);
	tmp = karatsuba(tmp, tmp) % mod;
	if (*n % 2 == 1) {
		tmp = karatsuba(tmp, *this) % mod;
	}
	return tmp;
}

BigInt BigInt::reversedBySimpleMod(BigInt mod) {
	return this->powBySimpleMod(mod - 2, mod);
}

BigInt BigInt::mathMod(BigInt mod) {
	BigInt tmp = *this % mod;
	return tmp + (tmp < 0 ? mod : 0);
}

long long BigInt::getLength() const {
	return digits.size();
}

vector<long long> BigInt::getDigits() {
	return digits;
}




/*
 * input, output
 */
ostream& operator << (ostream& os, BigInt bigInt) {
	if (bigInt.isNegative) {
		os << "-";
	}
	os << bigInt.digits.back();
	for (long long i = (long long)bigInt.digits.size() - 2; i >= 0; i--) {
		os << BigInt::formatOutput(bigInt.digits[i]);
	}
	return os;
}

istream& operator >> (istream& in, BigInt& bigInt) {
	string s;
	in >> s;
	bigInt = BigInt(s);
	return in;
}



/*
 * comparing
 */
bool operator == (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isNegative != bigInt2.isNegative) {
		return false;
	}
	if (bigInt1.digits.size() != bigInt2.digits.size()) {
		return false;
	}
	for (long long i = 0; i < (long long)bigInt1.digits.size(); i++) {
		if (bigInt1.digits[i] != bigInt2.digits[i]) {
			return false;
		}
	}
	return true;
}

bool operator == (BigInt bigInt1, long long int2) {
	return bigInt1 == BigInt(int2);
}

bool operator == (long long int1, BigInt bigInt2) {
	return BigInt(int1) == bigInt2;
}


bool operator != (BigInt bigInt1, BigInt bigInt2) {
	return !(bigInt1 == bigInt2);
}

bool operator != (BigInt bigInt1, long long int2) {
	return bigInt1 != BigInt(int2);
}

bool operator != (long long int1, BigInt bigInt2) {
	return BigInt(int1) != bigInt2;
}


bool operator > (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isNegative && !bigInt2.isNegative) {
		return false;
	}
	if (!bigInt1.isNegative && bigInt2.isNegative) {
		return true;
	}

	if (!bigInt1.isNegative) {
		if (bigInt1.digits.size() > bigInt2.digits.size()) {
			return true;
		}
		if (bigInt1.digits.size() < bigInt2.digits.size()) {
			return false;
		}
		for (long long i = bigInt1.digits.size() - 1; i >= 0; i--) {
			if (bigInt1.digits[i] > bigInt2.digits[i]) {
				return true;
			}
			if (bigInt1.digits[i] < bigInt2.digits[i]) {
				return false;
			}
		}
		return false;
	}
	else {
		return bigInt2.abs() > bigInt1.abs();
	}
}

bool operator > (BigInt bigInt1, long long int2) {
	return bigInt1 > BigInt(int2);
}

bool operator > (long long int1, BigInt bigInt2) {
	return BigInt(int1) > bigInt2;
}


bool operator < (BigInt bigInt1, BigInt bigInt2) {
	return bigInt2 > bigInt1;
}

bool operator < (BigInt bigInt1, long long int2) {
	return bigInt1 < BigInt(int2);
}

bool operator < (long long int1, BigInt bigInt2) {
	return BigInt(int1) < bigInt2;
}


bool operator >= (BigInt bigInt1, BigInt bigInt2) {
	return bigInt1 > bigInt2 || bigInt1 == bigInt2;
}

bool operator >= (BigInt bigInt1, long long int2) {
	return bigInt1 >= BigInt(int2);
}

bool operator >= (long long int1, BigInt bigInt2) {
	return BigInt(int1) >= bigInt2;
}


bool operator <= (BigInt bigInt1, BigInt bigInt2) {
	return bigInt1 < bigInt2 || bigInt1 == bigInt2;
}

bool operator <= (BigInt bigInt1, long long int2) {
	return bigInt1 <= BigInt(int2);
}

bool operator <= (long long int1, BigInt bigInt2) {
	return BigInt(int1) <= bigInt2;
}



/*
 * +, -, *, /
 */
BigInt operator + (const BigInt& bigInt1, const BigInt& bigInt2) {
	if (bigInt1.isNegative != bigInt2.isNegative) {
		BigInt res;
		if (bigInt1.abs() > bigInt2.abs()) {
			res = BigInt((bigInt1.abs() - bigInt2.abs()).digits, bigInt1.isNegative);
		}
		else {
			res = BigInt((bigInt2.abs() - bigInt1.abs()).digits, bigInt2.isNegative);
		}
		res.clearNumber();
		return res;
	}
	else {
		vector <long long> resDigits(1, 0);
		long long carry = 0;
		for (long long i = 0; i < max((long long)bigInt1.digits.size(), (long long)bigInt2.digits.size()) || carry; i++) {
			if (i == (long long)resDigits.size()) {
				resDigits.push_back(0);
			}
			resDigits[i] = carry + (i < (long long)bigInt1.digits.size() ? bigInt1.digits[i] : 0) + (i < (long long)bigInt2.digits.size() ? bigInt2.digits[i] : 0);
			carry = resDigits[i] / BigInt::BASE;
			resDigits[i] %= BigInt::BASE;
		}
		BigInt res(resDigits, bigInt1.isNegative);
		res.clearNumber();
		return res;
	}
}

BigInt operator + (BigInt bigInt1, long long int2) {
	return bigInt1 + BigInt(int2);
}

BigInt operator + (long long int1, BigInt bigInt2) {
	return BigInt(int1) + bigInt2;
}

const BigInt operator ++ (BigInt& a, int) {
	BigInt oldA = a;
	a = a + 1;
	return oldA;
}

BigInt operator - (const BigInt& bigInt1, const BigInt& bigInt2) {
	if (bigInt1.isNegative != bigInt2.isNegative) {
		BigInt res((bigInt1.abs() + bigInt2.abs()).digits, bigInt1.isNegative);
		res.clearNumber();
		return res;
	}
	else
		if (bigInt1.isNegative) {
			return bigInt2.abs() - bigInt1.abs();
		}
		else
			if (bigInt1 < bigInt2) {
				return BigInt((bigInt2 - bigInt1).digits, true);
			}
			else {
				vector <long long> resDigits = bigInt1.digits;
				long long carry = 0;
				for (long long i = 0; i < (long long)bigInt2.digits.size() || carry; i++) {
					resDigits[i] -= carry + (i < (long long)bigInt2.digits.size() ? bigInt2.digits[i] : 0);
					carry = resDigits[i] < 0;
					if (carry) {
						resDigits[i] += BigInt::BASE;
					}
				}
				BigInt res(resDigits, false);
				res.clearNumber();
				return res;
			}
}

BigInt operator - (BigInt bigInt1, long long int2) {
	return bigInt1 - BigInt(int2);
}

BigInt operator - (long long int1, BigInt bigInt2) {
	return BigInt(int1) - bigInt2;
}

BigInt BigInt::operator - () {
	return BigInt(digits, !isNegative);
}


BigInt operator * (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt1.isZero() || bigInt2.isZero()) {
		return BigInt(0);
	}
	else
		if (bigInt1.isPositiveOne()) {
			return bigInt2;
		}
		else
			if (bigInt1.isNegativeOne()) {
				return -bigInt2;
			}
			else
				if (bigInt2.isPositiveOne()) {
					return bigInt1;
				}
				else
					if (bigInt2.isNegativeOne()) {
						return -bigInt1;
					}
					else {
						vector <long long> resDigits(bigInt1.digits.size() + bigInt2.digits.size());
						for (long long i = 0; i < (long long)bigInt1.digits.size(); i++) {
							for (long long j = 0, carry = 0; j < (long long)bigInt2.digits.size() || carry; j++) {
								resDigits[i + j] += bigInt1.digits[i] * (j < (long long)bigInt2.digits.size() ? bigInt2.digits[j] : 0) + carry;
								carry = resDigits[i + j] / BigInt::BASE;
								resDigits[i + j] %= BigInt::BASE;
							}
						}
						BigInt res(resDigits, bigInt1.isNegative ^ bigInt2.isNegative);
						res.clearNumber();
						return res;
					}
}

BigInt operator * (BigInt bigInt1, long long int2) {
	return bigInt1 * BigInt(int2);
}

BigInt operator * (long long int1, BigInt bigInt2) {
	return BigInt(int1) * bigInt2;
}

BigInt karatsuba(const BigInt& x, const BigInt& y) {
	long long xLength = x.getLength();
	long long yLength = y.getLength();

	long long N = (long long)(fmax(xLength, yLength));
	if (N < 10)
		return x * y;

	N = (N / 2) + (N % 2);
		
	auto thousand = BigInt(BigInt::BASE);
	BigInt multiplier = thousand.pow(N);
	BigInt b = BigInt(vector<long long>(x.digits.begin() + N, x.digits.end()), x.isNegative);
	BigInt a = BigInt(vector<long long>(x.digits.begin(), x.digits.begin() + N), x.isNegative); 
	BigInt d = BigInt(vector<long long>(y.digits.begin() + N, y.digits.end()), y.isNegative);
	BigInt c = BigInt(vector<long long>(y.digits.begin(), y.digits.begin() + N), y.isNegative);

	BigInt z0 = karatsuba(a, c);
	BigInt z1 = karatsuba(a + b, c + d);
	BigInt z2 = karatsuba(b, d);


	return z0 + ((z1 - z0 - z2) * multiplier) + (z2 * (BigInt)(thousand.pow(2 * N)));
}


BigInt BigInt::div2() {
	long long carry = 0;
	vector <long long> resDigits = digits;
	for (long long i = (long long)resDigits.size() - 1; i >= 0; i--) {
		resDigits[i] = resDigits[i] + carry * BASE;
		carry = resDigits[i] % 2;
		resDigits[i] /= 2;
	}
	BigInt res(resDigits, isNegative);
	res.clearNumber();
	return res;
}

BigInt BigInt::mod2() {
	return (digits[0] % 2) * (isNegative ? -1 : 1);
}


BigInt operator / (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt2 == 0) {
		throw "DivisionByZero";
	}
	else
		if (bigInt1 == 0) {
			return 0;
		}
		else
			if (bigInt2.abs() == 1) {
				return bigInt1 * bigInt2;
			}
			else
				if (bigInt2 == 2) {
					return bigInt1.div2();
				}
				else
					if (bigInt1 < 0 || bigInt2 < 0) {
						return (bigInt1.abs() / bigInt2.abs()) * (bigInt1.isNegative ^ bigInt2.isNegative ? -1 : 1);
					}
					else {
						BigInt bigInt2Power2 = bigInt2;
						BigInt power2(1);
						while (bigInt2Power2 < bigInt1) {
							bigInt2Power2 = bigInt2Power2 * 2;
							power2 = power2 * 2;
						}
						BigInt tmp;
						BigInt res;
						while (power2 > 0) {
							if (tmp + bigInt2Power2 <= bigInt1) {
								tmp = tmp + bigInt2Power2;
								res = res + power2;
							}
							bigInt2Power2 = bigInt2Power2 / 2;
							power2 = power2 / 2;
						}
						return res;
					}


}

BigInt operator / (BigInt bigInt1, long long int2) {
	return bigInt1 / BigInt(int2);
}

BigInt operator / (long long int1, BigInt bigInt2) {
	return BigInt(int1) / bigInt2;
}


BigInt operator % (BigInt bigInt1, BigInt bigInt2) {
	if (bigInt2 == 0) {
		throw "DivisionByZero";
	}
	else
		if (bigInt1 == 0 || bigInt2.abs() == 1) {
			return 0;
		}
		else
			if (bigInt2 == 2) {
				return bigInt1.mod2();
			}
			else
				if (bigInt1 < 0 || bigInt2 < 0) {
					return (bigInt1.abs() % bigInt2.abs()) * (bigInt1.isNegative ? -1 : 1);
				}
				else {
					BigInt bigInt2Power2 = bigInt2;
					BigInt power2(1);
					while (bigInt2Power2 < bigInt1) {
						bigInt2Power2 = bigInt2Power2 * 2;
						power2 = power2 * 2;
					}
					BigInt tmp;
					BigInt res;
					while (power2 > 0) {
						if (tmp + bigInt2Power2 <= bigInt1) {
							tmp = tmp + bigInt2Power2;
							res = res + power2;
						}
						bigInt2Power2 = bigInt2Power2 / 2;
						power2 = power2 / 2;
					}
					return bigInt1 - tmp;
				}


}

BigInt operator % (BigInt bigInt1, long long int2) {
	return bigInt1 % BigInt(int2);
}

BigInt operator % (long long int1, BigInt bigInt2) {
	return BigInt(int1) % bigInt2;
}



/*
 * by simple mod
 */
BigInt BigInt::addBySimpleMod(BigInt other, BigInt mod) {
	return (*this + other).mathMod(mod);
}

BigInt BigInt::subBySimpleMod(BigInt other, BigInt mod) {
	return (*this - other).mathMod(mod);
}

BigInt BigInt::mulBySimpleMod(BigInt other, BigInt mod) {
	// return (karatsuba(*this, other).mathMod(mod));
	return (karatsuba(this->mathMod(mod), other.mathMod(mod))).mathMod(mod);
}

BigInt BigInt::divBySimpleMod(BigInt other, BigInt mod) {
	return this->mulBySimpleMod(other.reversedBySimpleMod(mod), mod);
}

BigInt BigInt::powBySimpleMod(BigInt n, BigInt mod) {
	if (n < 0) {
		throw "ValueError";
	}
	if (n == 0) {
		return 1;
	}
	if (n == 1) {
		return this->mathMod(mod);
	}
	BigInt tmp = this->powBySimpleMod(n / 2, mod);
	tmp = tmp * tmp;
	if (n % 2 == 1) {
		tmp = tmp * *this;
	}
	tmp = tmp.mathMod(mod);
	return tmp;
}



pair<BigInt, BigInt> solveSimpleSystem(vector< vector<BigInt> > sys) {
	long long n = sys.size();
	BigInt c = sys[0][0].reversedBySimpleMod(sys[0][2]) * sys[0][1];
	c = c.mathMod(sys[0][2]);
	BigInt d = sys[0][2];
	for (long long i = 1; i < n; i++) {
		BigInt c_ = c + d * (sys[i][1] * sys[i][0].reversedBySimpleMod(sys[i][2]) - c) * d.reversedBySimpleMod(sys[i][2]);
		BigInt d_ = d * sys[i][2];
		c_ = c_.mathMod(d_);
		c = c_;
		d = d_;
	}
	return { c, d };
}





BigInt gcd(BigInt a, BigInt b) {
	if (a == 0) {
		return b;
	}
	else {
		return gcd(b % a, a);
	}
}


BigInt randBigInt(BigInt p) {
	long long len = p.getLength();
	vector<long long> digits(len);
	for (long long i = 0; i + 1 < len; i++) {
		digits[i] = (long long)dist(mt) % BigInt::BASE;
	}
	auto pDigits = p.getDigits();
	digits[len - 1] = (long long)dist(mt) % pDigits[len - 1];
	return BigInt(digits, false);
}


bool testPrimeMillerRabin(BigInt a, long long countRetry = 10) {
	if (a == 2 || a == 3) {
		return true;
	}
	if (a == 1 || a % 2 == 0 || a % 3 == 0) {
		return false;
	}

	BigInt q = a - 1;
	BigInt t = q;
	int n = 0;
	while (q % 2 == 0) {
		q = q / 2;
		n++;
	}
	for (long long i = 0; i < countRetry; i++) {
		BigInt k = randBigInt(a - 2) + 2;
		BigInt b = k.pow(&q, a);
		if (b == 1 || b == t) {
			continue;
		}
		BigInt j = 0;
		while (j < n) {
			auto two = BigInt(2);
			b = b.pow(&two, a);
			if (b == t) {
				break;
			}
			j++;
		}
		if (j < n) {
			continue;
		}
		else {
			return false;
		}
	}
	return true;
}

BigInt rhoPollardDivisor(BigInt n, BigInt iterationsCount) {
	BigInt x = randBigInt(n);
	BigInt y = fPollard(x, n);
	BigInt g = gcd((x - y).abs(), n);

	for (BigInt i = 0; i < iterationsCount && (g == 1 || g == n); i++) {
		x = fPollard(x, n);
		y = fPollard(fPollard(y, n), n);
		g = gcd((x - y).abs(), n);
	}
	return g;
}


BigInt fPollard(BigInt a, BigInt mod) {
	return (a * a + 1) % mod;
}

BigInt getDivisor(BigInt n, long long countRetry = 10) {
	if (testPrimeMillerRabin(n)) {
		return n;
	}
	BigInt iterationsCount = (n.sqrt()).sqrt() + 2;
	BigInt d;
	for (long long i = 0; i < countRetry; i++) {
		d = rhoPollardDivisor(n, iterationsCount);
		if (d > 1 && d < n) {
			return getDivisor(n / d);
		}
	}
	return n;
}


vector<pair<BigInt, BigInt>> rhoPollardFactorization(BigInt n) {
	vector <pair<BigInt, BigInt>> factorization;// <d, pw>

	while (n % 2 == 0) {
		n = n / 2;
		if ((long long)factorization.size() == 0) {
			factorization.push_back({ 2, 1 });
		}
		else {
			factorization[0].second++;
		}
	}

	BigInt d = 1;
	while (n != 1) {
		d = getDivisor(n);
		factorization.push_back({ d, 0 });
		while (n % d == 0) {
			n = n / d;
			factorization.back().second++;
		}
	}

	if (n > 1 || (long long)factorization.size() == 0) {
		factorization.push_back({ n, 1 });
	}

	return factorization;
}


BigInt babyStepGiantStepLog(BigInt a, BigInt b, BigInt p) {
	BigInt m = p.sqrt() + 1;
	BigInt am = a.powBySimpleMod(m, p);

	map<BigInt, BigInt> mp;
	for (BigInt i = 1, tmp = am; i <= m; i++) {
		cout << i << " "  << m << endl;
		if (!mp.count(tmp)) {
			mp[tmp] = i;
		}
		tmp = tmp.mulBySimpleMod(am, p);
	}

	for (BigInt i = 0, tmp = b; i <= m; i++) {
		cout << i << " " << m << endl;
		if (mp.count(tmp)) {
			BigInt ans = mp[tmp] * m - i;
			if (ans < p) {
				return ans;
			}
		}
		tmp = tmp.mulBySimpleMod(a, p);
	}

	return -1;
}


BigInt euler(BigInt n) {
	if (n == 1) {
		return 1;
	}
	auto factorization = rhoPollardFactorization(n);
	BigInt ans = 1;
	for (auto el : factorization) {
		ans = ans * el.first.pow(el.second - 1) * (el.first - 1);
	}
	return ans;
}


BigInt mobius(BigInt n) {
	if (n == 1) {
		return 1;
	}
	auto factorization = rhoPollardFactorization(n);
	for (auto el : factorization) {
		if (el.second > 1) {
			return 0;
		}
	}
	return (long long)factorization.size() % 2 == 0 ? 1 : -1;
}


BigInt legendre(BigInt a, BigInt p) {
	BigInt ans = a.powBySimpleMod((p - 1) / 2, p);
	if (ans > 1) {
		ans = ans - p;
	}
	return ans;
}


BigInt jacobi(BigInt n, BigInt m) {
	cout << "n: " << n << endl;
	cout << "m: " << m << endl;
	if (gcd(n,m) != 1) {
		return 0;
	}

	int r = 1;
	BigInt c;

	if (n < 0) {
		n = -n;
		if (m.mathMod(4) == 3) {
			r = -r;
		}
	}

	while (n != 0) {
		int t = 0;

		while (n % 2 == 0) {
			t++;
			n = n / 2;
		}

		if (t % 2) {
			if (m.mathMod(8) == 3 || m.mathMod(8) == 5) {
				r = -r;
			}
		}

		if (n.mathMod(4) == 3 && m.mathMod(4) == 3) {
			r = -r;
		}
		c = n;
		n = m.mathMod(c);
		m = c;
	}
	return r;

}


pair<BigInt, BigInt> factorization2Primes(BigInt n, long long countRetry = 10) {
	BigInt iterationsCount = (n.sqrt()).sqrt() + 2;
	BigInt d;
	for (long long i = 0; i < countRetry; i++) {
		d = rhoPollardDivisor(n, iterationsCount);
		if (d > 1 && d < n) {
			return { d, n / d };
		}
	}
	return { -1, -1 };
}


//pair<BigInt, BigInt> cipolla(BigInt a, BigInt p, long long countRetry = 10) {
//	if (countRetry == 0) {
//		return { -1, -1 };
//	}
//
//	BigInt b = randBigInt(p);
//	BigInt w = (b * b - a).mathMod(p);
//
//	if (w == 0) {
//		BigInt c = p - b;
//		if (c < b) {
//			swap(c, b);
//		}
//		return { b, c };
//	}
//	else {
//		if (legendre(w, p) == 1) {
//			return cipolla(a, p, countRetry - 1);
//		}
//		else {
//			SqrtPolynomial2 tmp(b, 1, w);
//			SqrtPolynomial2 ans1_ = tmp.pow((p + 1) / 2, p);
//			BigInt ans1 = ans1_.getX().mathMod(p);
//			BigInt ans2 = p - ans1;
//			if (ans2 < ans1) {
//				swap(ans1, ans2);
//			}
//			return { ans1, ans2 };
//		}
//	}
//}


SqrtPolynomial2::SqrtPolynomial2(BigInt x_, BigInt y_, BigInt w_) {
	this->x = x_;
	this->y = y_;
	this->w = w_;
}

SqrtPolynomial2 SqrtPolynomial2::pow(BigInt n, BigInt mod) {
	if (n == 0) {
		return SqrtPolynomial2(1, 0, w);
	}
	if (n == 1) {
		return *this;
	}
	SqrtPolynomial2 tmp = this->pow(n / 2, mod);
	SqrtPolynomial2 tmp1((tmp.x * tmp.x % mod + tmp.y * tmp.y * w % mod) % mod, 2 * tmp.x * tmp.y % mod, w);
	if (n % 2 == 0) {
		return tmp1;
	}
	else {
		SqrtPolynomial2 tmp2((tmp1.x * x % mod + tmp1.y * y * w % mod) % mod, (tmp1.x * y % mod + tmp1.y * x % mod) % mod, w);
		return tmp2;
	}
}

BigInt SqrtPolynomial2::getX() {
	return x;
}




BigInt hexToBigInt(string s) {
	BigInt tmp = 1, ans = 0;
	for (long long i = (long long)s.size() - 1; i >= 0; i--) {
		char ch = s[i];
		long long v = 0;
		if (ch >= '0' && ch <= '9') {
			v = ch - '0';
		}
		else {
			v = ch - 'A' + 10;
		}
		ans = ans + (tmp * v);
		tmp = tmp * 16;
	}
	return ans;
}


Point::Point() {
	x = 0;
	y = 0;
	isZero = false;
}

Point::Point(BigInt x_, BigInt y_) {
	x = x_;
	y = y_;
	isZero = false;
}

Point::Point(bool isZero_) {
	x = 0;
	y = 0;
	isZero = isZero_;
}

Point Point::operator - () {
	if (isZero) {
		return *this;
	}
	else {
		return Point(x, -y);
	}
}

BigInt Point::getX() {
	return x;
}

BigInt Point::getY() {
	return y;
}

bool operator == (Point p1, Point p2) {
	return p1.x == p2.x && p1.y == p2.y;
}

bool operator != (Point p1, Point p2) {
	return !(p1 == p2);
}

ostream& operator << (ostream& os, Point p1) {
	os << "(" << p1.x << ", " << p1.y << ")";
	return os;
}


ElGamal::ElGamal() {
	string s_a = "DB7C2ABF62E35E668076BEAD2088";
	string s_b = "659EF8BA043916EEDE8911702B22";
	string s_g_x = "09487239995A5EE76B55F9C2F098";
	string s_g_y = "A89CE5AF8724C0A23E0E0FF77500";
	string s_n = "DB7C2ABF62E35E7628DFAC6561C5";

	p = (BigInt(2).pow(128) - 3) / 76439;
	a = hexToBigInt(s_a);
	b = hexToBigInt(s_b);
	P = Point(hexToBigInt(s_g_x), hexToBigInt(s_g_y));
	n = hexToBigInt(s_n);
}

ElGamal::ElGamal(BigInt p_, BigInt a_, BigInt b_, BigInt n_, Point P_) {
	p = p_;
	a = a_;
	b = b_;
	n = n_;
	P = P_;
}

pair<Point, Point> ElGamal::encrypt(Point M, Point Y) {
	BigInt r = 1 + randBigInt(n - 1);
	Point d = mulPoint(r, Y);
	Point g = mulPoint(r, P);
	Point h = addPoints(M, d);
	return { g, h };
}

Point ElGamal::decrypt(pair<Point, Point> c, BigInt k) {
	Point s = mulPoint(k, c.first);
	Point s1 = -s;
	Point M = addPoints(s1, c.second);
	return M;
}
// -k*g = -k*r*P + M+r*k*P

Point ElGamal::addPoints(Point p1, Point p2) {
	if (p1.isZero) {
		return p2;
	}
	if (p2.isZero) {
		return p1;
	}
	if (p1 != p2 && p1.x == p2.x) {
		return Point(true);
	}
	if (p1 == p2 && p1.y == 0) {
		return Point(true);
	}

	BigInt lambda;
	if (p1 != p2 && p1.x != p2.x) {
		BigInt x2x1 = p2.x.subBySimpleMod(p1.x, p);
		lambda = (p2.y.subBySimpleMod(p1.y, p)).divBySimpleMod(x2x1, p);
	}
	else {
		lambda = (((3 * p1.x) % p * p1.x) % p + a).divBySimpleMod(2 * p1.y, p);
	}

	BigInt xNew = (lambda * lambda - (p1.x + p2.x)).mathMod(p);
	BigInt yNew = (lambda * (p1.x - xNew) - p1.y).mathMod(p);
	return Point(xNew, yNew);
}


Point ElGamal::mulPoint(BigInt k, Point p1) {
	if (p1.isZero || k == 0) {
		return Point(true);
	}
	if (k == 1) {
		return p1;
	}
	Point tmp = mulPoint(k / 2, p1);

	Point ans = addPoints(tmp, tmp);

	if (k.mod2() == 1) {
		ans = addPoints(ans, p1);
	}
	return ans;
}


Point ElGamal::getP() {
	return P;
}

void ElGamal::testPoint(Point p1) {
	cout << "testing of point" << endl;
	cout << "x                = " << p1.x << endl;
	cout << "y                = " << p1.y << endl;
	cout << "y^2              = " << p1.y * p1.y % p << endl;
	// cout << "x^3 + a*x + b    = " << (p1.x.pow(3, p) + a * p1.x % p + b) % p << endl;
}

bool psw_prime(BigInt n) {
	if (low_divisible(n))
		return false;

	if (!testPrimeMillerRabin(n, 1)) {
		return false;
	}

	int D = 5;
	int P = 1;

	// how to save
	int k = 1;
	while (jacobi(D, n) != -1) {
		cout << D << endl;
		D *= -1;
		D += 2 * pow(-1, k);
		k++;
	}
	 
	int Q = (1 - D) / 4;

	BigInt element = get_lucas_seq_element(n + 1, Q, D);

	if (element % n != 0)
		return false;
	return true;
}

vector<bool> get_element_numbers(BigInt n) {
	int k = 1;
	vector<bool> need_to_double;
	int i = 1;

	// 12345678
	// 87654321
	while (n != 0) {
		if (n % 2 == 0) {
			need_to_double.push_back(true);
		}
		else {
			need_to_double.push_back(false);
			n = n - 1;
			continue;
		}
		n = n / 2;
	}
	// 1010

	std::reverse(need_to_double.begin(), need_to_double.end() - 1);
	return need_to_double;
}

BigInt reverse(BigInt a) {
	std::reverse(a.digits.begin(), a.digits.end());
	return a;
}

BigInt get_lucas_seq_element(BigInt n, int Q, int D) {
	BigInt Uk = 1;
	BigInt Vk = 1;
	BigInt P = 1;
	BigInt U2k;
	BigInt V2k;
	BigInt k;
	BigInt u2k_add;
	BigInt v2k_add;

	BigInt q = Q;
	
	BigInt m = n - 1;
	BigInt two = 2;
	auto need_to_double = get_element_numbers(n);
	

	for (int k = 0; k < need_to_double.size() - 1; k++) {
		if (!need_to_double[k]) {
			u2k_add = P * Uk + Vk;
			v2k_add = D * Uk + P * Vk;
			if (u2k_add % 2 == 1) {
				u2k_add = u2k_add + m;
			}
			if (v2k_add % 2 == 1) {
				v2k_add = v2k_add + m;
			}
			Uk = ((u2k_add) / 2) % m;
			Vk = ((v2k_add) / 2) % m;
			continue;
		}
		U2k = Uk * Vk;

		v2k_add = Vk.pow(&two, m) + D * Uk.pow(&two, m);
		if (v2k_add % 2 == 1) {
			v2k_add = v2k_add + m;
		}
		V2k = (v2k_add)  / 2 ; 
		
		Uk = U2k % m;
		Vk = V2k % m;
	}

	return Uk;
}

BigInt rand_num_fixed_bits(int n) {
	if (n == 2) {
		return 2;
	}
	int *bits=new int[n];
	bits[0] = 1;
	bits[n - 1] = 1;
	for (int i = 1; i + 1 < n; i++) {
		bits[i] = rand() % 2;
	}

	BigInt rnd = 0;
	BigInt pw2 = 1;
	for (int i = 0; i < n; i++) {
		rnd = rnd + pw2 * bits[i];
		pw2 = pw2 * 2;
	}
	return rnd;
}


BigInt rand_prime_fixed_bits(int n) {
	BigInt rnd = rand_num_fixed_bits(n);
	while (!psw_prime(rnd)) {
		rnd = rnd + 2;
		// rnd = rand_num_fixed_bits(n);
	}
	return rnd;
}

string BigInt::present_as_base(int base) {
	BigInt cp = *this;
	if (cp == 0) {
		return to_string(base) + "base:0";
	}
	string ans = "";
	while (cp != 0) {
		BigInt rest = cp % base;
		int ost = rest.digits[0];
		ans = to_string(ost) + ans;
		cp = cp / base;
		if (base > 10 && cp != 0) {
			ans = " | " + ans;
		}
	}
	return to_string(base) + "base : " + ans;
}

//BigInt generate_prime(BigInt seed) {
//
//}

string random_number(int bits) {
	if (bits <= 64) {
		return to_string(int(dist_2(mt_2)));
	}

	return random_number(bits / 2) + random_number(bits / 2);
}

bool low_divisible(const BigInt& a) {
	auto small_primes = vector<int>{ 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
		73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
		191, 193, 197, 199 };
	for (auto prime : small_primes) {
		if (a == prime || a % prime == 0)
			return true;
	}
	return false;
}
//Point ElGamal::getRandomPointOnCurve(long long countRetry = 5) {
//	if (countRetry == 0) {
//		return Point(-1, -1);
//	}
//	BigInt x = randBigInt(p);
//	cout << "countRetry = " << countRetry << endl;
//	cout << "check x = " << x << endl;
//	BigInt v = (x.pow(3, p) + a * x % p + b) % p;
//	if (jacobi(v, p) != 1) {
//		return getRandomPointOnCurve(countRetry - 1);
//	}
//	auto y2 = cipolla(v, p);
//	if (y2.first != -1) {
//		return Point(x, y2.first);
//	}
//	else {
//		return getRandomPointOnCurve(countRetry - 1);
//	}
//}

BigInt ElGamal::getRandomSecretKey() {
	return randBigInt(p);
}

int main() {
	ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
	// srand(time(0));
	
	while (true) {
		BigInt a,b;
		int c;
		cin >> c;
		auto t1 = chrono::high_resolution_clock::now();
		cout << rand_prime_fixed_bits(c) << endl;
		auto t2 = chrono::high_resolution_clock::now();
		auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);

		cout << ms_int.count() << " ms" <<  endl;
	}

	return 0;
}

