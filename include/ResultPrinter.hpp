#ifndef RESULTPRINTER_HPP
#define RESULTPRINTER_HPP

#include <string>

#include "ScanResult.hpp"
#include "Detection.hpp"

class ResultPrinter
{
public:
	static void print(const ScanResult& scan_result);

private:
	static std::string resultToString(const ScanResult::Result& result);
	static std::string categoryToString(const Detection::Category& category);

};

#endif // RESULTPRINTER_HPP