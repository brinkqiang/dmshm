
#ifndef __DMDSVFILTER_H_INCLUDE__
#define __DMDSVFILTER_H_INCLUDE__

#include <string>
#include <deque>
#include <vector>

#pragma warning(push)          // Saves the current warning state.
#pragma warning(disable:4996)  // Temporarily disables warning 4996.
#include "dmstrtk.hpp"
#pragma warning(pop)           // Restores the warning state.

class dsv_filter
{
public:
    dsv_filter()
        : file_name_(""),
        input_delimiter_(","),
        output_delimiter_("|")
    {
    }

    inline std::string file_name() const
    {
        return file_name_;
    }

    inline void set_input_delimiter(const std::string& input_delimiter)
    {
        input_delimiter_ = input_delimiter;
    }

    inline void set_output_delimiter(const std::string& output_delimiter)
    {
        output_delimiter_ = output_delimiter;
    }

    inline std::string input_delimiter() const
    {
        return input_delimiter_;
    }

    inline std::string output_delimiter() const
    {
        return output_delimiter_;
    }

    inline std::size_t row_count() const
    {
        return grid_.row_count();
    }

    inline bool load(const std::string& file_name)
    {
        if (!strtk::fileio::file_exists(file_name))
          return false;
        file_name_ = file_name;
        strtk::token_grid::options options;
        options.column_delimiters = input_delimiter_;
        options.set_column_split_option(strtk::split_options::default_mode);

        if (!grid_.load(file_name,options))
            return false;

        if (0 == grid_.row_count())
            return false;
        return true;
    }

    inline bool loadbuff(const std::string& buff)
    {
        strtk::token_grid::options options;
        options.column_delimiters = input_delimiter_;
        options.set_column_split_option(strtk::split_options::default_mode);

        if (!grid_.load(reinterpret_cast<unsigned char*>(const_cast<char*>(buff.data())), buff.size(),options))
            return false;

        if (0 == grid_.row_count())
            return false;
        return true;
    }

    inline std::string error()
    {
        return error_;
    }

    enum filter_result
    {
        e_error,
        e_match,
        e_mismatch
    };

    const strtk::token_grid& grid() const
    {
        return grid_;
    }

private:
    std::string file_name_;
    std::string input_delimiter_;
    std::string output_delimiter_;
    std::string error_;

    strtk::token_grid grid_;
    strtk::token_grid::row_type row_;
};

#endif // __DMDSVFILTER_H_INCLUDE__
