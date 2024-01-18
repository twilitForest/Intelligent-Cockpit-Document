//
// CONFIDENTIAL - FORD MOTOR COMPANY
//
// This is an unpublished work, which is a trade secret, created in
// 2017.  Ford Motor Company owns all rights to this work and intends
// to maintain it in confidence to preserve its trade secret status.
// Ford Motor Company reserves the right to protect this work as an
// unpublished copyrighted work in the event of an inadvertent or
// deliberate unauthorized publication.  Ford Motor Company also
// reserves its rights under the copyright laws to protect this work
// as a published work.  Those having access to this work may not copy
// it, use it, or disclose the information contained in it without
// the written authorization of Ford Motor Company.
//

/*
 * soa_client_endpoint_compile_time.hpp
 *
 *  Created on: Jul 12, 2017
 *      Author: ASENGUP6
 */

#ifndef FNV_SOA_CLIENT_ENDPOINT_COMPILE_TIME_HPP_
#define FNV_SOA_CLIENT_ENDPOINT_COMPILE_TIME_HPP_

//Define the end point prefixes and their sizes including the null terminating character
#define REQUESTPREFIX "SERVICES/REQUEST/"
#define REQUESTPREFIXSIZE 18    // GCC doesn't like sizeof(string literal) in template

#define DATAPREFIX "SERVICES/DATA/"
#define DATAPREFIXSIZE 15       // GCC doesn't like sizeof(string literal) in template

#define RESPONSEPREFIX "SERVICES/RESPONSE/"
#define RESPONSEPREFIXSIZE 19   // GCC doesn't like sizeof(string literal) in template

#include <iostream>

namespace fnv {
namespace soa {
namespace framework {
namespace internal {

/**
 * This file contains code that allows compile time generation of endpoint names.
 *
 * The process of compile time endpoint name generation is done by concatenating
 * the predefined endpoint prefix (ex, SERVICES/REQUEST/) with a provided endpoint suffix.
 *
 * The code in this file can be broken down into 3 components.
 *
 * 1) MakeSequence - (makeIntegerSequence implements the c++14 integer_sequence)
 *    makeIntegerSequence is able to create integer packs of a specified size
 *
 *    Ex. makeIntegerSequence<5> becomes IntegerSequence<0, 1, 2, 3, 4>
 *
 * 2) StringLiteral - (StringLiteral takes a string literal and makes the characters accessible by
 *                        index and stores the size in the type)
 *
 *    Ex. auto test = asStringLiteral("test"); / /StringLiteral<4>("test")
 *        std::cout << test[1] << std::endl; / /prints e
 *
 * 3) StringArray - (is responsible for concatenating two StringLiteral instances)
 *    The public constructor for StringArray excepts two StringLiterals and creates two integer packs
 *    each the size of the two StringLiteral instances as shown below.
 *
 *    StringArray { firstStringLiteral, secondStringLiteral,
 *    				makeIntegerSequence<firstStringLength>{ },
 *                   makeIntegerSequence<stringArrayLength - firstStringLength>{ } } { }
 *
 *    The public constructor then calls the private constructor which unpacks the integer sequences
 *    and uses them to index the two StringLiteral instances. These indexes from the unpacked integer
 *    sequences are then used in a member initializer list to populate charArray with the concatenated string.
 *
 *    StringArray(const StringLiteral<firstStringLength>& firstString,             / /first StringLiteral instance
 *	  	const StringLiteral<stringArrayLength - firstStringLength>& secondString,  / /second StringLiteral instance
 *	  	IntegerSequence<firstStringLengthIntegerPack...>,  / /unpack the integer sequence for the first StringLiteral instance
 *	  	IntegerSequence<secondStringLengthIntegerPack...>) / /unpack the integer sequence for the second StringLiteral instance
 *	  	: charArray { / /member initializer list for charArray
 *	  	firstString[firstStringLengthIntegerPack]...,   / /use the integer sequence to access the first StringLiteral by character
 *	  	secondString[secondStringLengthIntegerPack]..., / /use the integer sequence to access the second StringLiteral by character
 *	  	'\0' } { } / /append null character
 *
 * The concatenated string with a null terminator is now stored in the charArray member within StringArray
 */

//Implementation of integer_sequence from c++14 with logarithmic complexity
//The implementation splits the sequence into two halves, and creates the sequence for
//both halves recursively and then combines the two halves
//Example of building a sequence of 25 elements
//Start -> 1 elem -> 2 elems -> 3 elems -> 6 elems -> 12 elems -> 24 elems -> 25 elems -> Finish

//the type which holds the sequence
template<int...> struct IntegerSequence {};

//DoubleSequence declaration
template<int numberOfElementsToDouble, class Sequence>
struct DoubleSequence;

//DoubleSequence is responsible for doubling the number of elements in Sequence
template<int numberOfElementsToDouble, int... Ints>
struct DoubleSequence<numberOfElementsToDouble, IntegerSequence<Ints...>> {
	using doubleSequenceType = IntegerSequence<Ints..., (numberOfElementsToDouble + Ints)...>;
};

//leave Sequence as is if needsIncrement is false
template<bool needsIncrement, class Sequence>
struct IncrementSequence {
	using incrementSequenceType = Sequence;
};

//add one element to Sequence if needsIncrement is true
template<int... integerPack>
struct IncrementSequence<true, IntegerSequence<integerPack...>> {
	using incrementSequenceType = IntegerSequence<integerPack..., sizeof...(integerPack)>;
};

template<int numberOfElementsInSequence>
struct MakeSequence {
	//example --> MakeSequence<10>
	//Number of recursive calls =  5
	//Sequence<0> <-- base case                   Elements = 1
	//Sequence<0, 1>                              Elements = 2   (DoubleSequence)
	//Sequence<0, 1, 2, 3>                        Elements = 4   (DoubleSequence)
	//Sequence<0, 1, 2, 3, 4>                     Elements = 5   (IncrementSequence)
	//Sequence<0, 1, 2, 3, 4, 5, 6, 7, 8, 9>      Elements = 10  (DoubleSequence)
	using makeSequenceType = typename IncrementSequence
			<(numberOfElementsInSequence % 2 != 0),	//if(numberOfElementsInSequence % 2 != 0) = true then increment the sequence
			 typename DoubleSequence<numberOfElementsInSequence / 2,
			 typename MakeSequence<numberOfElementsInSequence / 2>
			 ::makeSequenceType>::doubleSequenceType>::incrementSequenceType;
};

//base case to terminate recursion
template<> struct MakeSequence<0> {
	using makeSequenceType = IntegerSequence<>;
};

//alias template to simplify usage
template<int numberOfElementsInSequence>
using makeIntegerSequence = typename MakeSequence<numberOfElementsInSequence>::makeSequenceType;

//StringLiteral holds a reference to a C string literal with the size embedded in the type
template<int stringLengthWithoutNull>
class StringLiteral {
	//size of char array is stringLengthWithoutNull + 1 due to trailing null character
	const char (&literal)[stringLengthWithoutNull + 1];
public:
	//constructor takes string literal by reference so as to not make another copy of it
	constexpr StringLiteral(const char (&literal)[stringLengthWithoutNull + 1]) : literal(literal) {}

	//used to access the characters in StringLiteral to build StringArray
	constexpr char operator[](int i) const {
		return literal[i];
	}
};

//takes a string literal by reference and converts it to a StringLiteral
template<int stringLengthWithNull>
constexpr auto asStringLiteral(const char (&lit)[stringLengthWithNull])
-> StringLiteral<stringLengthWithNull - 1> {
	return StringLiteral<stringLengthWithNull - 1>(lit);
}

//StringArray is the type that holds the concatenated string
template<int stringArrayLength>
class StringArray {
	//charArray holds the concatenated string
	char charArray[stringArrayLength + 1];

	/* C++11 requires constexpr constructor bodies to be empty,
	 * due to this constraint variadic templates and template parameter packs are used
	 * to recursively build the concatenated string held in charArray at compile time
	 */
	template<int firstStringLength, int... firstStringLengthIntegerPack, int... secondStringLengthIntegerPack>
	constexpr StringArray(const StringLiteral<firstStringLength>& firstString,
			const StringLiteral<stringArrayLength - firstStringLength>& secondString,
			IntegerSequence<firstStringLengthIntegerPack...>,
			IntegerSequence<secondStringLengthIntegerPack...>) :
			charArray { firstString[firstStringLengthIntegerPack]..., secondString[secondStringLengthIntegerPack]..., '\0' } { }

public:
	template<int firstStringLength>
	constexpr StringArray(const StringLiteral<firstStringLength>& firstStringLiteral, const StringLiteral<stringArrayLength - firstStringLength>& secondStringLiteral) :
		//delegate to private constructor
		StringArray { firstStringLiteral, secondStringLiteral, makeIntegerSequence<firstStringLength>{ }, makeIntegerSequence<stringArrayLength - firstStringLength>{ } } { }

	//converting StringArray to char array
	constexpr operator const char * () const {
		return charArray;
	}

	//array index operator overload
	const char operator[](int i) const {
		if(i >= 0 && i < stringArrayLength){
			//within bounds return correct index
			return charArray[i];
		} else {
			//out of bounds return null character
			return charArray[stringArrayLength];
		}
	}

	//delete copy assignment operator to disallow assigning StringArray to a different value after creation
	StringArray& operator=(const StringArray<stringArrayLength>&) = delete;
};

//+ operator overload for concatenation
template<int firstStringLength, int secondStringLength>
constexpr auto operator+(const StringLiteral<firstStringLength>& s1, const StringLiteral<secondStringLength>& s2)
-> StringArray<firstStringLength + secondStringLength> {
	return StringArray<firstStringLength + secondStringLength>(s1, s2);
}

//<< operator overload for printing
template<int stringLength>
std::ostream& operator<<(std::ostream& os, const StringArray<stringLength>& stringArray)
{
    os << static_cast<const char*>(stringArray);
    return os;
}

//== operator overload for comparison
template<int firstStringLength, int secondStringLength>
bool operator==(const StringArray<firstStringLength>& firstString, const StringArray<secondStringLength>& secondString)
{
	if(&firstString == &secondString){
		return true;
	}
	return strcmp(firstString, secondString) == 0;
}

} /* namespace internal */
} /* namespace framework */
} /* namespace soa */
} /* namespace fvn */
#endif /* FNV_SOA_CLIENT_ENDPOINT_COMPILE_TIME_HPP_ */
