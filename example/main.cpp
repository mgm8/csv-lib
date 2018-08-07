#include <csv.hpp>
#include <vector>
#include <iostream>

using namespace std;

int main()
{
    CSV<int> csv;

    vector<int> row;

    for(int i=0; i<5; i++)
    {
        row.push_back(i);
    }

    csv.AppendRow(row);
    csv.AppendRow(row);
    csv.AppendRow(row);
    csv.AppendRow(row);

    cout << "Table of ints:" << endl;
    cout << csv << endl;

    csv.Write("example.csv");

    CSV<float> csv_in("example.csv");

    cout << "Table of floats from a CSV file:" << endl;
    cout << csv_in << endl;

    cout << "Table of floats modified:" << endl;
    csv_in.WriteCell(1, 0, csv_in.ReadCell(1, 0) + 0.25);

    cout << csv_in << endl;

    return 0;
}
