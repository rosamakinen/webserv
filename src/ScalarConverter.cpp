#include "../include/ScalarConverter.hpp"
#include "../include/WebServer.hpp"

ScalarConverter::~ScalarConverter()
{
}

ScalarConverter::TYPE ScalarConverter::getType(const std::string scalar)
{
	if (scalar.compare("nan") == 0 || scalar.compare("nanf") == 0)
		return ScalarConverter::NAN;
	if (scalar.compare("+inf") == 0 || scalar.compare("+inff") == 0
		|| scalar.compare("inf") == 0 || scalar.compare("inff") == 0)
		return ScalarConverter::INFPOS;
	if (scalar.compare("-inf") == 0 || scalar.compare("-inff") == 0)
		return ScalarConverter::INFNEG;
	if (scalar.find('.') != std::string::npos)
	{
		for (size_t i = 0; i < scalar.length(); i++)
		{
			if (!isdigit(scalar[i]) && scalar[i] != 'f')
				return ScalarConverter::NONE;
		}
		if (scalar.find('f') != std::string::npos)
			return ScalarConverter::FLOAT;
		return ScalarConverter::DOUBLE;
	}

	for (size_t i = 0; i < scalar.length(); i++)
	{
		if (!isdigit(scalar[i]))
			return ScalarConverter::NONE;
	}
	return ScalarConverter::INT;
}

static void	convertToNan(float *f, double *d)
{
	*f = std::numeric_limits<float>::quiet_NaN();
	*d = std::numeric_limits<double>::quiet_NaN();
}

static void	convertToInfPos(float *f, double *d)
{
	*f = std::numeric_limits<float>::infinity();
	*d = std::numeric_limits<double>::infinity();
}

static void	convertToInfNeg(float *f, double *d)
{
	*f = -std::numeric_limits<float>::infinity();
	*d = -std::numeric_limits<double>::infinity();
}

static void	convertToFloat(const std::string scalar, int *i, float *f, double *d)
{
	char* endptr;
	double temp = std::strtod(scalar.c_str(), &endptr);

	if ((*endptr != '\0' && *endptr != 'f')
		|| (temp >= FLT_MAX && scalar.compare("340282346638528859811704183484516925440.0f") != 0)
		|| (temp <= -FLT_MAX && scalar.compare("-340282346638528859811704183484516925440.0f") != 0))
		throw std::invalid_argument("Overflowing float input");

	*f = static_cast<float>(temp);
	*i = static_cast<int>(*f);
	*d = static_cast<double>(*f);
}

static void	convertToDouble(const std::string scalar, int *i, float *f, double *d)
{
	char *endptr;
	*d = std::strtod(scalar.c_str(), &endptr);

	if (*endptr != '\0'
		|| (*d >= DBL_MAX && scalar.compare("179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0") != 0)
		|| (*d <= -DBL_MAX && scalar.compare("-179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0") != 0))
		throw std::invalid_argument("Overflowing double input");

	*i = static_cast<int>(*d);
	*f = static_cast<float>(*d);
}

static void	convertToInt(const std::string scalar, int *i, float *f, double *d)
{
	char *endptr;
	long long longValue = std::strtol(scalar.c_str(), &endptr, 10);

	if (*endptr != '\0' || (longValue >= INT_MAX && scalar.compare("2147483647") != 0)
		|| (longValue <= INT_MIN && scalar.compare("-2147483648") != 0))
		throw std::invalid_argument("Overflowing int input");

	*i = static_cast<int>(longValue);
	*f = static_cast<float>(*i);
	*d = static_cast<double>(*i);
}


struct stringConversions ScalarConverter::convert(const std::string& scalar)
{
	struct stringConversions convs;
	convs.asInt = 0;
	convs.asFloat = 0.0f;
	convs.asDouble = 0.0;
	const char *input = scalar.c_str();

	ScalarConverter::TYPE type = getType(scalar);
	switch (type)
	{
		case NAN:
			convertToNan(&convs.asFloat, &convs.asDouble);
			break;
		case INFPOS:
			convertToInfPos(&convs.asFloat, &convs.asDouble);
			break;
		case INFNEG:
			convertToInfNeg(&convs.asFloat, &convs.asDouble);
			break;
		case DOUBLE:
			convertToDouble(input, &convs.asInt, &convs.asFloat, &convs.asDouble);
			break;
		case FLOAT:
			convertToFloat(input, &convs.asInt, &convs.asFloat, &convs.asDouble);
			break;
		case INT:
			convertToInt(input, &convs.asInt, &convs.asFloat, &convs.asDouble);
			break;
		default:
			throw std::invalid_argument("Invalid input");
	}

	return convs;
}

static int	getIntLength(long nb)
{
	int	len = 0;
	if (nb < 0)
	{
		nb = nb * -1;
		len++;
	}

	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

std::string ScalarConverter::intToString(int scalar)
{
	if (scalar == 0)
		return std::string("0");

	int i = getIntLength(scalar);
	char result[i + 2];
	result[i--] = '\0';
	if (scalar < 0)
	{
		result[0] = '-';
		scalar *= -1;
	}
	while (scalar > 0)
	{
		result[i--] = (scalar % 10) + '0';
		scalar /= 10;
	}
	return std::string(result);
}
