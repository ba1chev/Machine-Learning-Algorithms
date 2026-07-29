#pragma once

class CSVReader;
template <class T> class BINReader;

class ReaderVisitor {
public:
    virtual void visit(const CSVReader& csv_reader) = 0;
    virtual void visit(const BINReader<float>& bin_reader) = 0;
    virtual ~ReaderVisitor() = default;
};
