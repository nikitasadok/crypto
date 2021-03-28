#include "larithmetic.h"
#include "larithmetic.cpp"

using namespace std;

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