#include "RSA.h"
#include <chrono>
#include <random>

std::random_device rd_3;
std::mt19937 mt_3(rd_3());
std::uniform_real_distribution<double> dist_3(27, INT32_MAX);

RSA::RSA(int bits, int hash_a, int hash_b) {
	this->bits = bits;
	this->hash_a = hash_a;
	this->hash_b = hash_b;
}

BigInt RSA::get_n(BigInt p, BigInt q) {
	// maybe dont work
	auto p_t = p - 1;
	auto q_t = q - 1;

	this->q_p_mod_p = q.pow(&p_t, p);
	this->p_q_mod_q = p.pow(&q_t, q);
	this->p = p;
	this->q = q;
	return karatsuba(p, q);
}

BigInt RSA::carmichael(BigInt p, BigInt q) {
	BigInt x, y;
	return ((p - 1) * (q - 1)) / gcd_extended(p - 1, q - 1, &x, &y);
}

// todo improve later
int RSA::get_exponent(BigInt phi_n) {
	auto popular_values = vector<int>{ 3, 5, 17, 257, 65537 };
	for (auto i : popular_values) {
		if (gcd(phi_n, i) == 1) {
			return i;
		}
	}
	return 0;
}

pair<PrivateKey, PublicKey> RSA::generate_keys() {
	srand(time(0));
	auto p = rand_prime_fixed_bits(this->bits);
	auto q = rand_prime_fixed_bits(this->bits);
	cout << "WE ARE DONE GENERATING NUMBERS" << endl;

	this->n = get_n(p, q);

	auto phi_n = carmichael(p, q);
	auto e = get_exponent(phi_n);
	auto d = reverse_modulo(e, phi_n);

	this->d_mod_p = d % (p - 1);
	this->d_mod_q = d % (q - 1);

	auto keys = pair<PrivateKey, PublicKey>();
	keys.first = PrivateKey{ d,n };
	keys.second = PublicKey{ e,n };

	return keys;
}

vector<BigInt> RSA::cypher(string message, BigInt e, const BigInt& n) {
	auto ciphered = vector<BigInt>();
	for (int i = 0; i < message.size(); i++) {
		cout << "message[i]: " << int(message[i]) << endl;
		BigInt tmp = message[i];
		ciphered.push_back(tmp.pow(&e, n));
	}
	return ciphered;
}

BigInt cypher_int(BigInt i, BigInt e, const BigInt& n) {
	return i.pow(&e, n);
}

string RSA::decypher(vector<BigInt> c_vec) {
	string res;
	for (auto c : c_vec) {
		auto m = (c.pow(&d_mod_p, p) * q_p_mod_p + c.pow(&d_mod_q, q) * p_q_mod_q) % n;
		cout << "stuff: " << m.getDigits()[0] << endl;
		res += char(m.getDigits()[0] / 2);
	}
	
	return res;
}

vector<BigInt> RSA::decypher_to_vec(vector<BigInt> c_vec) {
	vector<BigInt> res;
	for (auto c : c_vec) {
		res.push_back((c.pow(&d_mod_p, p) * q_p_mod_p + c.pow(&d_mod_q, q) * p_q_mod_q) % n);
	}

	return res;
}

string RSA::vec_to_str(vector<BigInt> dec) {
	string res;
	for (auto c : dec) {
		res += char(c.getDigits()[0] / 2);
	}

	return res;
}

BigInt RSA::decypher_int(BigInt c) {
	return (c.pow(&d_mod_p, p) * q_p_mod_p + c.pow(&d_mod_q, q) * p_q_mod_q) % n;
}

BigInt polynomial_hash(std::string str, int m, const BigInt& n) {
	int p = 31;
	long long power_of_p = 1;
	long long hash_val = 0;

	for (int i = 0; i < str.length(); i++) {
		hash_val
			= (hash_val
				+ ((long long)str[i] - 'a' + 1) * power_of_p)
			% m;
		power_of_p
			= (power_of_p * p) % m;
	}
	return hash_val % n;
}

BigInt universal_hash(vector<BigInt> c, int a, int b, const BigInt& n) {
	BigInt hash;
	for (auto c_i : c) {
		hash = hash + (a * c_i + b) % n; 
	}
	return hash % n;
}

BigInt RSA::convert_to_int(string s) {
	int num = 0;
	for (int i = 0; i < s.length(); i++) {
		num += int((int(num) << 8) | s[i]);
	}

	return num;
}

BigInt RSA::sign(BigInt c, BigInt d) {
	return c.pow(&d, this->n);
}

bool RSA::is_sign_verified(BigInt sign, PublicKey key, BigInt hash) {
	auto sign_pow = sign.pow(&key.e, key.n);
	return sign_pow == hash;
}

pair<vector<BigInt>, BigInt> RSA::send_message(string message, BigInt d, PublicKey pub) {
	auto c = cypher(message, pub.e, pub.n);
	for (auto c_i : c) {
		cout << "c_i send: " << c_i << endl;
	}
	auto hash = universal_hash(c, this->hash_a, this->hash_b, 100000000);
	cout << "hash send: " << hash << endl;
	auto sign_c = sign(hash, d);

	return pair<vector<BigInt>, BigInt>{c, sign_c};
}

string RSA::accept_message(pair<vector<BigInt>, BigInt> m, BigInt d, PublicKey pub) {
	auto msg = decypher_to_vec(m.first);
	for (auto c_i : msg) {
		cout << "c_i acc: " << c_i << endl;
	}
	// auto sign_dec = rsa.decypher_int(m.second); // need to be orig hash
	auto hash = universal_hash(m.first, this->hash_a, this->hash_b, 100000000);
	cout << "hash: " << hash << endl;
	// cout << "dec_hash: " << sign_dec << endl;
	if (is_sign_verified(m.second, pub, hash)) {
		cout << "VALID" << endl;
	}
	// cout << "sign_dec: " << sign_dec << endl;
	return vec_to_str(msg);
}

PublicKey Bob(pair<vector<BigInt>, BigInt> msg, RSA rsa, pair<PrivateKey, PublicKey> keys) {
	cout << "MESSAGE: " << rsa.accept_message(msg, keys.first.d, keys.second) << endl;
}

void Alice(int hash_a, int hash_b, int bits) {
	cout << "ENTER ALICE" << endl;
	auto rsa_A = RSA(bits, hash_a, hash_b);
	auto keys_A = rsa_A.generate_keys();
	auto rsa_B = RSA(bits, hash_a, hash_b);
	auto keys_B = rsa_B.generate_keys();
	auto msg = rsa_A.send_message("hello world", keys_A.first.d, keys_B.second);
	Bob(msg, rsa_B, keys_A);
}

int main() {
	auto hash_a = dist_3(mt_3);
	auto hash_b = dist_3(mt_3);
	// auto rsa = RSA(128, hash_a, hash_b);

	auto t1 = chrono::high_resolution_clock::now();
	Alice(hash_a, hash_b, 64);
	auto t2 = chrono::high_resolution_clock::now();

	auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
	cout << ms_int.count() << " ms (for 64)" <<  endl;

	t1 = chrono::high_resolution_clock::now();
	Alice(hash_a, hash_b, 128);
	t2 = chrono::high_resolution_clock::now();

	ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);
	cout << ms_int.count() << " ms (for 128)" << endl;


	return 0;
}