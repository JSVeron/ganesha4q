
#ifndef QS_STRING_UTILS_H
#define QS_STRING_UTILS_H

//#include  "QsSdkConfig.h"

#include  <vector>
#include  <string>


namespace QingStor
{
    namespace Utils
    {
        /**
        * All the things the c++ stdlib is missing for string operations that I needed.
        */
        class StringUtils
        {
        public:

            static bool HasPrefix(std::string& s, const char* prefix);

            static void Replace(std::string& s, const char* search, const char* replace);

            /**
            * Converts a string to lower case.
            */
            static std::string ToLower(const char* source);


            /**
            * Converts a string to upper case.
            */
            static std::string ToUpper(const char* source);


            /**
            * Does a caseless comparison of two strings.
            */
            static bool CaselessCompare(const char* value1, const char* value2);


            /**
            * URL encodes a string (uses %20 not + for spaces).
            */
            static std::string URLEncode(const char* unsafe);

            /**
            * Http Clients tend to escape some characters but not all. Escaping all of them causes problems, because the client
            * will also try to escape them.
            * So this only escapes non-ascii characters and the + character
            */
            static std::string UTF8Escape(const char* unicodeString, const char* delimiter);

            /**
            * URL encodes a double (if it ends up going to scientific notation) otherwise it just returns it as a string.
            */
            static std::string URLEncode(double unsafe);


            /**
            * Decodes a URL encoded string (will handle both encoding schemes for spaces).
            */
            static std::string URLDecode(const char* safe);


            /**
            * Splits a string on a delimiter (empty items are excluded).
            */
            static std::vector<std::string> Split(const std::string& toSplit, char splitOn);


            /**
            * Splits a string on new line characters.
            */
            static std::vector<std::string> SplitOnLine(const std::string& toSplit);


            /** static Aws::vector<std::string> SplitOnRegex(std::string regex);
             *  trim from start
             */
            static std::string LTrim(const char* source);


            /**
             * trim from end
             */
            static std::string RTrim(const char* source);

            /**
             * trim from both ends
             */
            static std::string Trim(const char* source);


            /**
             * convert to int 64
             */
            static long long ConvertToInt64(const char* source);


            /**
             * convert to int 32
             */
            static long ConvertToInt32(const char* source);


            /** 
             * convert to bool
             */
            static bool ConvertToBool(const char* source);


            /**
             * convert to double
             */
            static double ConvertToDouble(const char* source);


#ifdef _WIN32
            /**
            * Converts a string to wstring.
            */
            static std::wstring ToWString(const char* source);

            /**
            * Converts a wstring to string.
            */
            static std::string FromWString(const wchar_t* source);
#endif

        };


    } // namespace Utils
} // namespace Aws


#endif // QS_STRING_UTILS_H