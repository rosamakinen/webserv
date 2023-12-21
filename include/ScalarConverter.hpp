/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ScalarConverter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: meskelin <meskelin@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 20:35:11 by meskelin          #+#    #+#             */
/*   Updated: 2023/12/19 18:06:53 by meskelin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

# include <iostream>
# include <iomanip>
# include <limits>
# include <cstdlib>
# include <cfloat>
# include <string>
# include <float.h>

class ScalarConverter
{
	private:
		enum TYPE
		{
			NONE,
			NAN,
			INFPOS,
			INFNEG,
			DOUBLE,
			FLOAT,
			INT,
			CHAR
		};

		static ScalarConverter::TYPE getType(const std::string scalar);
	public:
		~ScalarConverter(void);
		static struct stringConversions	convert(const std::string& scalar);
		static std::string				intToString(const int scalar);
};

struct stringConversions
{
	int asInt;
	float asFloat;
	double asDouble;
};
