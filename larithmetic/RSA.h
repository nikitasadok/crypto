#pragma once
#include "crypto.h"

struct PublicKey{
	BigInt e;
	BigInt n;
};

struct PrivateKey {
	BigInt d;
	BigInt n;
};

class RSA {
private:
	int bits;
	BigInt p, q, n;
	BigInt q_p_mod_p;
	BigInt p_q_mod_q;
	BigInt d_mod_p;
	BigInt d_mod_q;
	int hash_a, hash_b;
	BigInt get_p();
	BigInt get_q();
	BigInt get_n(BigInt p, BigInt q);
	BigInt carmichael(BigInt p, BigInt q);
	int get_exponent(BigInt phi_n);
	string vec_to_str(vector<BigInt> dec);
public:
	BigInt convert_to_int(string s);
	string int_to_string(BigInt n);
	vector<BigInt> cypher(string message, BigInt e, const BigInt& n);
	BigInt cypher_int(BigInt i, BigInt e, const BigInt& n);
	string decypher(vector<BigInt> m);
	vector<BigInt> decypher_to_vec(vector<BigInt> m);
	BigInt decypher_int(BigInt c);
	pair<vector<BigInt>, BigInt> send_message(string msg, BigInt d, PublicKey pub);
	string accept_message(pair<vector<BigInt>, BigInt> m, BigInt d, PublicKey pub);
	RSA(int bits, int hash_a, int hash_b);
	std::pair<PrivateKey, PublicKey>generate_keys();
	BigInt sign(BigInt c, BigInt d);
	bool is_sign_verified(BigInt sign, PublicKey key, BigInt hash);
};
