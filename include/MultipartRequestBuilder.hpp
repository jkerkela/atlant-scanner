#ifndef MULTIPARTREQUESTBUILDER_HPP
#define MULTIPARTREQUESTBUILDER_HPP

#include<vector>
#include<string>

namespace requestBuilder {
	constexpr int BOUNDARY_BYTES = 32;
}

class MultiPartRequestBuilder {

public:
	MultiPartRequestBuilder();

	class Part {
	public:
		Part(const std::string& name, const std::string& content_type, const std::string& content);
		std::string getName() { return name; };
		std::string getContentType() { return content_type; };
		std::string getContent() { return content; };

		std::string encode();

	private:
		std::string name;
		std::string content_type;
		std::string content;
	};

	std::string getBoundary() { return boundary; };

	void addPart(const Part &part);
	std::string encode();

private:
	std::vector<Part> parts;
	std::string boundary;

	std::string makeBoundary();
};

#endif // MULTIPARTREQUESTBUILDER_HPP
