#include "currency_format.hpp"

Eagle::String CurrencyFormat(const char* format, int value)
{
	Eagle::String currency = Eagle::Format(format, value);
	if (value < 1000)
	{
		return currency;
	}
	std::reverse(currency.begin(), currency.end());
	Eagle::String result = "";
	Eagle::String tmp = "";
	size_t b = 0, n = 3;
	while ((b + n) < currency.length())
	{
		tmp = currency.substr(b, n);
		if (tmp.front() == ' ' && result.back() == ',')
		{
			result.pop_back();
		}
		result += tmp;
		if (result.back() != ' ')
		{
			result += ",";
		}
		b += n;
	}
	n = currency.length() - b;
	if (n > 0)
	{
		tmp = currency.substr(b, n);
		if (!Eagle::Math::IsBetween(tmp.front(), '0', '9'))
		{
			result.pop_back();
		}
		result += currency.substr(b, n);
	}
	else
	{
		if (result.back() == ',')
		{
			result.pop_back();
		}
	}
	std::reverse(result.begin(), result.end());
	return result;
}