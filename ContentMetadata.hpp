#ifndef CONTENTMETADATA_HPP
#define CONTENTMETADATA_HPP

#include<string>

class ContentMetadata {

public:
	ContentMetadata(
		const std::string &SHA1 = "",
		const std::string &URI = "",
		const int content_length = 0,
		const std::string &content_type = "",
		const std::string &charset = ""
	);

	std::string getSHA1() { return SHA1 };
	std::string getURI() { return URI };
	int getContentLength() { return content_length };
	std::string getContentType() { return content_type};
	std::string getCharset() { return charset };


private:
	std::string SHA1;
	std::string URI;
	int content_length;
	std::string content_type;
	std::string charset;
};
#endif // CONTENTMETADATA_HPP
