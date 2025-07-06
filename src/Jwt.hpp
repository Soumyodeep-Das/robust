#ifndef JWT_HPP
#define JWT_HPP

#include <string>

class Jwt {
public:
    Jwt(const std::string& secret, int expiry, const std::string& issuer);
    std::string encode(const std::string& key, int value) const;
    Jwt decode(const std::string& token) const;

    std::string get_issuer() const;
    class PayloadClaim {
    public:
        std::string as_string() const;
    };
    PayloadClaim get_payload_claim(const std::string& key) const;

private:
    std::string secret_;
    int expiry_;
    std::string issuer_;
};

#endif // JWT_HPP