#include "Lang.h"

std::string bookslibrary::ToString(bookslibrary::ELang lang)
{
	if (lang == ELang::ru)
		return "ru";

	if (lang == ELang::en)
		return "en";

	if (lang == ELang::cs)
		return "cs";

	throw std::exception("Unknown language");
}

bookslibrary::ELang bookslibrary::FromString(const std::string& lang)
{
	if (lang == "RU" || lang == "ru" || lang == "ru-RU")
		return ELang::ru;

	if (lang == "en")
		return ELang::en;

	if (lang == "cs")
		return ELang::cs;

	throw std::exception("Unknown language");
}
