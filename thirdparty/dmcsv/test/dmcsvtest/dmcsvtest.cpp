#include "gtest.h"

#include "dmcsv.hpp"
#include "dmdsvfilter.hpp"

std::string strFile = "slist.csv";
std::queue<std::vector<std::string>> q;

TEST(dmcsv, dmcsv_init) {
    std::ofstream out(strFile);
    q.push({ "name", "age", "ip" });

    uint64_t qwCount = 0;

    for (int i = 0; i < 1000000; ++i)
    {
        qwCount += i;
        q.push({ "Andy" + std::to_string(i), std::to_string(i), "172.30.10.21" });
    }
    std::cout << qwCount << std::endl;
    out.close();
}

TEST(dmcsv, dmcsv_write) {
    std::ofstream out(strFile);
    auto writer = csv::make_csv_writer(out);

    for (; !q.empty(); q.pop())
        writer.write_row(q.front());

    out.close();
}

TEST(dmcsv, dmcsv_read_name) {

    uint64_t qwCount = 0;

    csv::CSVReader reader(strFile, csv::DEFAULT_CSV);
    csv::CSVRow rows;
    for (size_t i = 0; reader.read_row(rows); i++) {
        qwCount += rows["age"].get<uint64_t>();
    }
    std::cout << qwCount << std::endl;
}

TEST(dmcsv, dmcsv_read_index) {

    uint64_t qwCount = 0;

    csv::CSVReader reader(strFile, csv::DEFAULT_CSV);
    csv::CSVRow rows;
    for (size_t i = 0; reader.read_row(rows); i++) {
        qwCount += rows[1].get<uint64_t>();
    }
    std::cout << qwCount << std::endl;
}

TEST(dsv_filter, dsv_filter_read) {

    dsv_filter oFilter;
    if (!oFilter.load(strFile))
    {
        return;
    }

    uint64_t qwCount = 0;

    for (int i = 1; i < static_cast<int>(oFilter.grid().row_count()); ++i)
    {
        strtk::token_grid::row_type row = oFilter.grid().row(i);
        qwCount += row.get<uint64_t>(1);
    }

    std::cout << qwCount << std::endl;
}

TEST(dmformat, dmformat) {

}
