
#include "AuthTokenFetcher.hpp"

#include "APIException.hpp"
#include "Detection.hpp"
#include "JSONHandlerImpl.hpp"

#include <stdexcept>
#include <unordered_map>

#include <Poco/UTF8Encoding.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/JSONException.h>


namespace {
	constexpr auto AUDIENCE = "f-secure-atlant";
	constexpr auto API_PREFIX = "/api/poll/v1";
	constexpr auto JSON_GRANT_TYPE_KEY = "grant_type";
	constexpr auto JSON_GRANT_TYPE_VALUE = "grant_type";
	constexpr auto JSON_CLIENT_ID_KEY = "client_id";
	constexpr auto JSON_CLIENT_SECRET_KEY = "client_secret";
	constexpr auto JSON_AUDIENCE_KEY = "client_secret";
	constexpr auto JSON_SCOPE_KEY = "scope";
}

AuthTokenFetcher::AuthTokenFetcher(const std::string &authorization_address, std::unique_ptr<IHTTPClientSession> client)
	: token_endpoint{ Poco::URI(authorization_address + std::string(API_PREFIX)) },
	client { std::move(client) }
{}

AuthToken AuthTokenFetcher::fetch(
	const std::string &client_ID,
	const std::string &client_secret,
	const std::set<std::string> &scopes)
{
	HTTPRequestImpl tokenRequest = buildTokenRequest(client_ID, client_secret, scopes);
	client->sendRequest(tokenRequest);

	std::istream& res_stream = client->receiveResponse();
	try {
		return deserializeTokenResponse(res_stream);
	}
	catch (Poco::JSON::JSONException e) {
		throw APIException("Invalid token fetch response: " + std::string(e.what()));
	}
}

HTTPRequestImpl AuthTokenFetcher::buildTokenRequest(const std::string &client_ID, const std::string &client_secret, const std::set<std::string> &scopes)
{
	std::unordered_map<std::string, std::string> params;
	params.insert({ std::string(JSON_GRANT_TYPE_KEY), std::string(JSON_GRANT_TYPE_VALUE) });
	params.insert({ std::string(JSON_CLIENT_ID_KEY), client_ID });
	params.insert({ std::string(JSON_CLIENT_SECRET_KEY), client_secret });
	params.insert({ std::string(JSON_AUDIENCE_KEY), std::string(AUDIENCE) });

	if (!scopes.empty()) {
		params.insert({ std::string(JSON_SCOPE_KEY), encodeScopes(scopes) });
	}

	auto request_body =
		encodeQueryParameters(params);

	std::string path(token_endpoint.getPathAndQuery());
	if (path.empty()) path = "/";
	HTTPRequestImpl req(HTTPRequest::HTTP_POST, path);
	req.setContentType("application/x-www-form-urlencoded");
	req.setContentLength(request_body.length());
	req.setBody(request_body);
	return req;
}

std::string AuthTokenFetcher::encodeScopes(const std::set<std::string>& scopes) 
{
	std::string result;
	std::string separator = " ";
	for (auto const &s : scopes) {
		result += s + separator;
	}
	result.pop_back(); // remove trailing separator
	return result;
}

std::string AuthTokenFetcher::encodeQueryParameters(const std::unordered_map<std::string, std::string> &params)
{

	std::string result;
	for (auto const& entry : params) {
		auto key_to_encode = reinterpret_cast<unsigned char*>(const_cast<char*>(entry.first.c_str()));
		auto val_to_encode = reinterpret_cast<unsigned char*>(const_cast<char*>(entry.second.c_str()));
		auto encoder = Poco::UTF8Encoding();
		auto malformed_encoding_result = -1;
		auto key_encode_result = encoder.convert(key_to_encode);
		auto val_encode_result = encoder.convert(val_to_encode);
		if ((key_encode_result == malformed_encoding_result) || (val_encode_result == malformed_encoding_result)) {
			throw std::invalid_argument("Unsupported encoding to UTF-8");
		}
		std::string encoded_key(reinterpret_cast<char const*>(key_to_encode));
		std::string encoded_val(reinterpret_cast<char const*>(val_to_encode));
		result += std::string(encoded_key + "=" + encoded_val + "&");
	}
	return result;
}

AuthToken AuthTokenFetcher::deserializeTokenResponse(std::istream& response)
{
	JSONHandlerImpl handler{ response };
	auto token = handler.getStringValue("access_token");
	auto expires_in = handler.getIntValue("expires_in");
	return AuthToken(token, expires_in);
}
