#include "Jwt.hpp"

Jwt::Jwt(const std::string& secret, int expiry, const std::string& issuer)
    : secret_(secret), expiry_(expiry), issuer_(issuer) {}

std::string Jwt::encode(const std::string& key, int value) const {
    // Dummy implementation for example
    return "encoded_token";
}

Jwt Jwt::decode(const std::string& token) const {
    // Dummy implementation for example
    return Jwt(secret_, expiry_, issuer_);
}

std::string Jwt::get_issuer() const {
    return issuer_;
}

Jwt::PayloadClaim Jwt::get_payload_claim(const std::string& key) const {
    return PayloadClaim();
}

std::string Jwt::PayloadClaim::as_string() const {
    return "42";
}