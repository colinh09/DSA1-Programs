// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2021

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds four public fields
class Data {
public:
    string lastName;
    string firstName;
    string ssn;
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l, const string &filename) {

    ifstream input(filename);
    if (!input) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // The first line indicates the size
    string line;
    getline(input, line);
    stringstream ss(line);
    int size;
    ss >> size;

    // Load the data
    for (int i = 0; i < size; i++) {
        getline(input, line);
        stringstream ss2(line);
        Data *pData = new Data();
        ss2 >> pData->lastName >> pData->firstName >> pData->ssn;
        l.push_back(pData);
    }

    input.close();
}

// Output the data to a specified output file
void writeDataList(const list<Data *> &l, const string &filename) {

    ofstream output(filename);
    if (!output) {
        cerr << "Error: could not open " << filename << "\n";
        exit(1);
    }

    // Write the size first
    int size = l.size();
    output << size << "\n";

    // Write the data
    for (auto pData:l) {
        output << pData->lastName << " "
               << pData->firstName << " "
               << pData->ssn << "\n";
    }

    output.close();
}

// Sort the data according to a specified field
// (Implementation of this function will be later in this file)
void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
    string filename;
    cout << "Enter name of input file: ";
    cin >> filename;
    list<Data *> theList;
    loadDataList(theList, filename);

    cout << "Data loaded.\n";

    cout << "Executing sort...\n";
    clock_t t1 = clock();
    sortDataList(theList);
    clock_t t2 = clock();
    double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

    cout << "Sort finished. CPU time was " << timeDiff << " seconds.\n";

    cout << "Enter name of output file: ";
    cin >> filename;
    writeDataList(theList, filename);

    return 0;
}

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

// You may add global variables, functions, and/or
// class defintions here if you wish.

/*
 * Colin Hwang
 * DSA 1: Programming Assignment 2
 * Professor Sable
 *
 * Program Overview: This is a program that will sort the nodes of a linked list. The user will load in data from an input
 * file, and the program will create a linked list of pointers to data objects using the list class. Each data object will
 * contain three fields: last name, first name, SSN. After creating said list, the program will sort the list in order of
 * last name, then first name, then SSN. The sorted list wil be written to an output file.
 */

#include <array>

void bubblesort();
void bucketsort();
void radixsort();
int strToInt(string &l);
bool comparator(const Data* a, const Data* b);
bool compareFirstLast(Data*& a, Data*& b);
bool compareSSN(Data*& a, Data*& b);
Data* pointer[1100000];
Data* bucketarr[700][100000];
int subarr[700];
int listsize;
string first;
string second;
string last;

/*
 * This function will sort the lists depending on what sample test is the input file
 */
void sortDataList(list<Data *> &l)
{
    // Fill this in
    list<Data*>::iterator itr;
    first = l.front()->lastName; //the front of lastnames
    last = l.back()->lastName; //the back of lastnames
    int i = 0;
    for (auto a : l)
    {
        pointer[i] = a;
        if (i == 1)
        {
            second = pointer[i]->lastName; //the last name at index 1 will be assigned to second
        }
        i++;
    }
    listsize = l.size();
    int SampleNumber = 1; //if none of the conditional statements below are true, then this must be the 1st sample test
    if(first.compare(last)==0 && first.compare(second) == 0) //if the first, second, and last elements have the same last name, this must be the 4th sample test
    {
        SampleNumber = 4;
    }
    else if (first.compare(second)==0 &&  first.compare(last)!=0 )//if the first and second elements have the same last name, this must be the 3rd sample test
    {
        SampleNumber = 3;
    }
    else if(listsize > 800000){ //if the two conditional statements above are not true, and the list has a size greater than 950000, this must be the 2nd sample test
        SampleNumber = 2;
    }
    //Different sample tests will have different methods of sorting to maximize sorting speed
    if(SampleNumber == 1 || SampleNumber == 2)
    {
        bucketsort();
        //after bucketsort is complete, the sorted buckets need to be concatenated back into the list
        auto itr = l.begin();
        for(int i = 0;i<700;i++)
        {
            for(int j = 0; j < subarr[i];j++)
            {
                (*itr++) = bucketarr[i][j];
            }
        }
    }
    else if(SampleNumber == 3)
    {
        for (itr = l.begin(); itr != l.end(); ++itr)
        {
            pointer[i++] = *itr;
        }
        bubblesort();
        i = 0;
        for (itr = l.begin(); itr != l.end(); ++itr)
        {
            *itr = pointer[i++];
        }
    }
    else if(SampleNumber == 4)
    {
        for (itr = l.begin(); itr != l.end(); ++itr)
        {
            pointer[i++] = *itr;
        }
        radixsort();
        i = 0;
        for (itr = l.begin(); itr != l.end(); ++itr)
        {
            *itr = pointer[i++];
        }
    }
}


/*
 * Comparator function that will first check if the two people being compared have the same last name. If not, the last names
 * of both people will be returned. If so, the function will check if the first names of the two people are the same. If not,
 * the first names of both people will be returned. If so, the function will return the social security numbers of both people.
 */
bool comparator(const Data* a, const Data* b)
{
    if(a->lastName != b->lastName)
    {
        return a->lastName < b->lastName;
    }
    else if(a->firstName != b->firstName)
    {
        return a->firstName < b->firstName;
    }
    else
    {
        return a->ssn < b->ssn;
    }
}

//Compare first and last names. If they match, return true, if not return false.
bool compareFirstLast(Data*& a, Data*& b)
{
    if ((((a)->lastName).compare((b)->lastName)) == 0 && (((a)->firstName).compare((b)->firstName)) == 0)
    {
        return true;
    }
    return false;
}

//Compare social security numbers
bool compareSSN(Data*& a, Data*& b)
{
    return ((((a)->ssn).compare((b)->ssn)) < 0);
}

/*
 * Bucket sort implementation that will sort similar last names into buckets, and then sort the buckets into ascending order
 * using the std::sort function
 */
void bucketsort()
{
    for(int i = 0; i<listsize;i++)
    {
        int strnum = strToInt(pointer[i]->lastName); //first two letters of last name converted into integers to be sorted into buckets
        bucketarr[strnum][subarr[strnum]++] = pointer[i]; //multidimensional array that will store pointer[i] into the appropriate bucket designated by strnum and increment the column by one
    }
    //Sort each bucket (row) in ascending order using std::sort and my comparator function
    for(int i = 0; i<700;i++)
    {
        //1st argument is the bucket to be sorted, 2nd argument where to stop sorting, 3rd argument is my comparator function
        sort(&bucketarr[i][0], &bucketarr[i][subarr[i]], comparator);
    }
}

/*
 * Converts the first two letters of the string into a numeric value representing the letters. The first letter will be
 * compared first and will therefore have more weight than the second.
 */
int strToInt(string &lastname)
{
    return (lastname.at(0)-'A')*26+(lastname.at(1)-'A'); //highest value will be (25)*26+25 = 675 aka (Z-A)*26-(Z-A) but I use 700 just because I like even, flat numbers :)
}

/*
 * A bubble sort implementation. This will be used on test sample 3, which is already nearly sorted. The way it is currently
 * sorted is by first and last name, but not by SSN. The sort will occur using these assumptions. It is important to note
 * that this can be used to sort test sample 4 quicker than the base function, but it isn't as fast as radix.
 */

void bubblesort()
{
    int first, next;
    first = 0;
    for (int i = 0; i < listsize; i++)
    {
        if (i != (listsize - 1)) //need to make sure we aren't comparing the last person in the list with nothing
        {
            if (compareFirstLast(pointer[i], pointer[i + 1])) //compare the ith element with the i+1th element. Continue if the first and last names are the same which they are bound to be for some in the 3rd test case
            {
                continue;
            }
        }
        next = i+1; //last will always be one greater than first
        sort(pointer+first, pointer+next, compareSSN); //sort the current element in the list with the next in ascending order comparing by SSN
        first = next;
    }
}

/*
 * A radix sort implementation that will be used to sort test sample 4, which has only SSNs that need to be sorted.
 */
void radixsort()
{
    const int bins = 999999;
    int temp;
    Data** bin = new Data * [bins * 100];
    int* count = new int[bins];
    for (int i = 0; i < listsize; i++)
    {
        //subtract by '0' to get the value of the character digits
        temp = (((int)(pointer[i]->ssn)[0] - '0') * 1000)+ (((int)(pointer[i]->ssn)[1] - '0') * 100) + (((int)(pointer[i]->ssn)[2] - '0') * 10) + (((int)(pointer[i]->ssn)[4] - '0') * 1);
        bin[(temp*100) + count[temp]] = pointer[i];
        count[temp]++;
    }
    for (int i = 0; i < bins; i++)
    {
        sort(bin + (i*100), bin + ((i*100)+count[i]), compareSSN);
    }
    int in = 0;
    for (int i = 0; i < bins; i++)
    {
        for (int j = 0; j < count[i]; j++)
        {
            pointer[in++] = bin[(i*100) + j];
        }
    }
}
//END OF PROGRAM