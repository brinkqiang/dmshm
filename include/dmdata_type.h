
#ifndef __DMDATA_TYPE_H_INCLUDE__
#define __DMDATA_TYPE_H_INCLUDE__

#include <math.h>
#include <cctype>
#include <string>

namespace csv {
    /** Enumerates the different CSV field types that are
    *  recognized by this library
    *
    *  - 0. CSV_NULL (empty string)
    *  - 1. CSV_STRING
    *  - 2. CSV_INT
    *  - 3. CSV_LONG_INT
    *  - 4. CSV_LONG_LONG_INT
    *  - 5. CSV_DOUBLE
    *
    *  **Note**: Overflowing integers will be stored and classified as doubles.
    *  Furthermore, the same number may either be a CSV_LONG_INT or CSV_INT depending on
    *  compiler and platform.
    */
    enum DataType {
        CSV_NULL,
        CSV_STRING,
        CSV_INT,
        CSV_LONG_INT,
        CSV_LONG_LONG_INT,
        CSV_DOUBLE
    };

    namespace internals {
        template<typename T>
        DataType type_num();
        template<> inline DataType type_num<char>() { return CSV_INT; }
        template<> inline DataType type_num<unsigned char>() { return CSV_INT; }
        template<> inline DataType type_num<short>() { return CSV_INT; }
        template<> inline DataType type_num<unsigned short>() { return CSV_INT; }
        template<> inline DataType type_num<int>() { return CSV_INT; }
        template<> inline DataType type_num<unsigned int>() { return CSV_INT; }
        template<> inline DataType type_num<long>() { return CSV_INT; }
        template<> inline DataType type_num<unsigned long>() { return CSV_INT; }
        template<> inline DataType type_num<long long>() { return CSV_LONG_LONG_INT; }
        template<> inline DataType type_num<unsigned long long>() { return CSV_LONG_LONG_INT; }
        template<> inline DataType type_num<float>() { return CSV_DOUBLE; }
        template<> inline DataType type_num<double>() { return CSV_DOUBLE; }
        template<> inline DataType type_num<long double>() { return CSV_DOUBLE; }
        template<> inline DataType type_num<std::nullptr_t>() { return CSV_NULL; }
        template<> inline DataType type_num<std::string>() { return CSV_STRING; }

        std::string type_name(const DataType&);
        DataType data_type(const std::string& in, long double* const out = nullptr);
    }
}

#endif // __DMDATA_TYPE_H_INCLUDE__
