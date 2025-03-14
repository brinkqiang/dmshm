#include "dmcsv_parser.hpp"

namespace csv {
    /** @file
      * Calculates statistics from CSV files
      */

    CSVStat::CSVStat(std::string filename, CSVFormat format) :
        CSVReader(filename, format) {
        /** Lazily calculate statistics for a potentially large file. Once this constructor
         *  is called, CSVStat will process the entire file iteratively. Once finished,
         *  methods like get_mean(), get_counts(), etc... can be used to retrieve statistics.
         */
        while (!this->eof()) {
            this->read_csv("", ITERATION_CHUNK_SIZE, false);
            this->calc();
        }

        if (!this->records.empty())
            this->calc();
    }

    void CSVStat::end_feed() {
        CSVReader::end_feed();
        this->calc();
    }

    /** @brief Return current means */
    std::vector<long double> CSVStat::get_mean() const {
        std::vector<long double> ret;        
        for (size_t i = 0; i < this->col_names->size(); i++) {
            ret.push_back(this->rolling_means[i]);
        }
        return ret;
    }

    /** @brief Return current variances */
    std::vector<long double> CSVStat::get_variance() const {
        std::vector<long double> ret;        
        for (size_t i = 0; i < this->col_names->size(); i++) {
            ret.push_back(this->rolling_vars[i]/(this->n[i] - 1));
        }
        return ret;
    }

    /** @brief Return current mins */
    std::vector<long double> CSVStat::get_mins() const {
        std::vector<long double> ret;        
        for (size_t i = 0; i < this->col_names->size(); i++) {
            ret.push_back(this->mins[i]);
        }
        return ret;
    }

    /** @brief Return current maxes */
    std::vector<long double> CSVStat::get_maxes() const {
        std::vector<long double> ret;        
        for (size_t i = 0; i < this->col_names->size(); i++) {
            ret.push_back(this->maxes[i]);
        }
        return ret;
    }

    /** @brief Get counts for each column */
    std::vector<CSVStat::FreqCount> CSVStat::get_counts() const {
        std::vector<FreqCount> ret;
        for (size_t i = 0; i < this->col_names->size(); i++) {
            ret.push_back(this->counts[i]);
        }
        return ret;
    }

    /** @brief Get data type counts for each column */
    std::vector<CSVStat::TypeCount> CSVStat::get_dtypes() const {
        std::vector<TypeCount> ret;        
        for (size_t i = 0; i < this->col_names->size(); i++) {
            ret.push_back(this->dtypes[i]);
        }
        return ret;
    }

    void CSVStat::calc() {
        /** Go through all records and calculate specified statistics */
        for (size_t i = 0; i < this->col_names->size(); i++) {
            dtypes.push_back(TypeCount());
            counts.push_back(FreqCount());
            rolling_means.push_back(0);
            rolling_vars.push_back(0);
            mins.push_back(NAN);
            maxes.push_back(NAN);
            n.push_back(0);
        }

        std::vector<std::thread> pool;

        // Start threads
        for (size_t i = 0; i < this->col_names->size(); i++)
            pool.push_back(std::thread(&CSVStat::calc_worker, this, i));

        // Block until done
        for (auto& th: pool)
            th.join();

        this->records.clear();
    }

    void CSVStat::calc_worker(const size_t &i) {
        /** Worker thread for CSVStat::calc() which calculates statistics for one column.
         * 
         *  @param[in] i Column index
         */

        auto current_record = this->records.begin();
        for (size_t processed = 0; current_record != this->records.end(); processed++) {
            auto current_field = (*current_record)[i];

            // Optimization: Don't count() if there's too many distinct values in the first 1000 rows
            if (processed < 1000 || this->counts[i].size() <= 500)
                this->count(current_field, i);

            this->dtype(current_field, i);

            // Numeric Stuff
            if (current_field.type() >= CSV_INT) {
                long double x_n = current_field.get<long double>();

                // This actually calculates mean AND variance
                this->variance(x_n, i);
                this->min_max(x_n, i);
            }

            ++current_record;
        }
    }

    void CSVStat::dtype(CSVField& data, const size_t &i) {
        /** Given a record update the type counter
         *  @param[in]  record Data observation
         *  @param[out] i      The column index that should be updated
         */
        
        auto type = data.type();
        if (this->dtypes[i].find(type) !=
            this->dtypes[i].end()) {
            // Increment count
            this->dtypes[i][type]++;
        } else {
            // Initialize count
            this->dtypes[i].insert(std::make_pair(type, 1));
        }
    }

    void CSVStat::count(CSVField& data, const size_t &i) {
        /** Given a record update the frequency counter
         *  @param[in]  record Data observation
         *  @param[out] i      The column index that should be updated
         */

        auto item = data.get<std::string>();

        if (this->counts[i].find(item) !=
            this->counts[i].end()) {
            // Increment count
            this->counts[i][item]++;
        } else {
            // Initialize count
            this->counts[i].insert(std::make_pair(item, 1));
        }
    }

    void CSVStat::min_max(const long double &x_n, const size_t &i) {
        /** Update current minimum and maximum
         *  @param[in]  x_n Data observation
         *  @param[out] i   The column index that should be updated
         */
        if (isnan(this->mins[i]))
            this->mins[i] = x_n;
        if (isnan(this->maxes[i]))
            this->maxes[i] = x_n;
        
        if (x_n < this->mins[i])
            this->mins[i] = x_n;
        else if (x_n > this->maxes[i])
            this->maxes[i] = x_n;
    }

    void CSVStat::variance(const long double &x_n, const size_t &i) {
        /** Given a record update rolling mean and variance for all columns
         *  using Welford's Algorithm
         *  @param[in]  x_n Data observation
         *  @param[out] i   The column index that should be updated
         */
        long double& current_rolling_mean = this->rolling_means[i];
        long double& current_rolling_var = this->rolling_vars[i];
        long double& current_n = this->n[i];
        long double delta;
        long double delta2;

        current_n++;
        
        if (current_n == 1) {
            current_rolling_mean = x_n;
        } else {
            delta = x_n - current_rolling_mean;
            current_rolling_mean += delta/current_n;
            delta2 = x_n - current_rolling_mean;
            current_rolling_var += delta*delta2;
        }
    }

    /** @brief Useful for uploading CSV files to SQL databases.
     *
     *  Return a data type for each column such that every value in a column can be
     *  converted to the corresponding data type without data loss.
     *  @param[in]  filename The CSV file
     *
     *  \return A mapping of column names to csv::DataType enums
     */
    std::unordered_map<std::string, DataType> csv_data_types(const std::string& filename) {
        CSVStat stat(filename);
        std::unordered_map<std::string, DataType> csv_dtypes;

        auto col_names = stat.get_col_names();
        auto temp = stat.get_dtypes();

        for (size_t i = 0; i < stat.get_col_names().size(); i++) {
            auto& col = temp[i];
            auto& col_name = col_names[i];

            if (col[CSV_STRING])
                csv_dtypes[col_name] = CSV_STRING;
            else if (col[CSV_LONG_LONG_INT])
                csv_dtypes[col_name] = CSV_LONG_LONG_INT;
            else if (col[CSV_LONG_INT])
                csv_dtypes[col_name] = CSV_LONG_INT;
            else if (col[CSV_INT])
                csv_dtypes[col_name] = CSV_INT;
            else
                csv_dtypes[col_name] = CSV_DOUBLE;
        }

        return csv_dtypes;
    }
}