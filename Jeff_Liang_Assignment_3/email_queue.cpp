/*
 * Program: EECS 348 Assignment 3
 *
 * Description:
 *   C++ program that prioritizes unread emails for a company CEO.
 *   Emails are stored in a MaxHeap implemented with a list.
 *   Sender category is the main priority. Within the same category,
 *   the newer email has higher priority.
 *
 * Inputs:
 *   A test-file path given on the command line.
 *   The file contains EMAIL, NEXT, READ, and COUNT commands.
 *
 * Output:
 *   Terminal output for COUNT and NEXT.
 *   COUNT prints the number of unread emails in a sentence.
 *   NEXT prints the highest-priority email without removing it.
 *   READ removes that email and prints a message only when none remain.
 *   File and command errors are written to standard error.
 *
 * Collaborators:
 *   None
 *
 * Other sources:
 *   ChatGPT. The generated program was revised for this assignment.
 *
 * Author: Jeffrey Liang
 * Creation date: Septemer 30, 2026
 * Revision date: October 1, 2026
 *
 * Revisions:
 *   Updated COUNT and NEXT so their output matches the assignment sample.
 */
#include <iostream>   //Importing the library for terminal input and output
#include <fstream>    //Importing the library for reading a file
#include <sstream>    //Importing the library for reading pieces of a date
#include <string>     //Importing the library for text
#include <list>       //Importing the list used to store the heap
#include <stdexcept>  //Importing the library for reporting an empty heap
using namespace std;  //Use standard library names without writing std::
//This block was obtained from ChatGPT (listed in Other sources in the prologue)
//Email represents one unread email
//Higher senderPriority means the email has higher priority
//If two emails have the same sender priority, the newer date has higher priority
class Email  //This class stores one email
{
private:  //These members can only be used inside Email
    string sender;          //This stores the sender category
    string subject;         //This stores the subject line
    string date;            //This stores the date text
    int senderPriority;     //This stores the sender's priority number
    int dateValue;          //This stores the date as a number for comparison
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Convert MM-DD-YYYY into YYYYMMDD so dates can be compared as numbers
    int convertDate(const string& dateString) const  //This turns a date string into a number
    {
        int month, day, year;             //These store the month, day, and year
        char dash1, dash2;                //These store the two dashes in the date
        stringstream ss(dateString);      //This reads the date string one piece at a time
        ss >> month >> dash1 >> day >> dash2 >> year;  //Read the month, day, and year
        return year * 10000 + month * 100 + day;       //Return the date as YYYYMMDD
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Assign numeric priorities to sender categories
    int getSenderPriority(const string& senderName) const  //This finds the sender's priority
    {
        if (senderName == "Boss")              //Check if the sender is Boss
            return 5;                          //Boss is read first
        else if (senderName == "Subordinate")  //Check if the sender is Subordinate
            return 4;                          //Subordinate is read next
        else if (senderName == "Peer")         //Check if the sender is Peer
            return 3;                          //Peer is read next
        else if (senderName == "ImportantPerson")  //Check if the sender is ImportantPerson
            return 2;                          //ImportantPerson is read next
        else                                   //The sender is OtherPerson
            return 1;                          //OtherPerson is read last
    }
public:  //These members can be used outside Email
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    Email()  //This creates an empty email
    {
        senderPriority = 0;  //Start with no sender priority
        dateValue = 0;       //Start with no date value
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Save the sender, subject, and date, then compute their priority numbers
    Email(const string& senderName,    //This is the sender category
          const string& subjectLine,   //This is the subject line
          const string& emailDate)     //This is the date
    {
        sender = senderName;                         //Save the sender category
        subject = subjectLine;                       //Save the subject line
        date = emailDate;                            //Save the date text
        senderPriority = getSenderPriority(sender);  //Save the sender's priority number
        dateValue = convertDate(date);               //Save the date as a number
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Return true if this email should have higher priority than another email
    bool operator>(const Email& other) const  //This compares two emails
    {
        //Sender category is the primary priority
        if (senderPriority != other.senderPriority)          //Check if the senders differ
            return senderPriority > other.senderPriority;    //The higher sender priority wins
        //Within the same category, the newest email comes first
        return dateValue > other.dateValue;                  //The newer date wins
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    void display() const  //This prints one email
    {
        cout << "Sender: " << sender << endl;    //Print the sender
        cout << "Subject: " << subject << endl;  //Print the subject
        cout << "Date: " << date << endl;        //Print the date
    }
};
//This block was obtained from ChatGPT (listed in Other sources in the prologue)
//MaxHeap stores emails so the highest-priority email stays at the front
//A list is used as the storage. No library heap or priority_queue is used
//getIterator lets the heap reach an email by its position in the list
class MaxHeap  //This class is the list-based max heap
{
private:  //These members can only be used inside MaxHeap
    list<Email> heap;  //This list stores the emails in heap order
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Return an iterator to the email at the given position
    list<Email>::iterator getIterator(int index)  //This finds the email at index
    {
        list<Email>::iterator it = heap.begin();  //Start at the first email
        for (int i = 0; i < index; i++)           //Move forward index times
            ++it;                                 //Step to the next email
        return it;                                //Return the email at that position
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Move a new email up until the heap is ordered again
    void heapifyUp(int index)  //This restores order after an insert
    {
        while (index > 0)  //Keep going until this email is the root
        {
            int parentIndex = (index - 1) / 2;                 //Find the parent position
            list<Email>::iterator current = getIterator(index);       //Find this email
            list<Email>::iterator parent = getIterator(parentIndex);  //Find the parent email
            if (*current > *parent)  //Check if this email outranks its parent
            {
                //Swap the two emails
                Email temp = *current;  //Save this email
                *current = *parent;     //Move the parent into this spot
                *parent = temp;         //Move this email into the parent spot
                index = parentIndex;    //Continue from the parent position
            }
            else  //This email does not outrank its parent
            {
                break;  //Stop because the heap is ordered
            }
        }
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Move the root email down until the heap is ordered again
    void heapifyDown(int index)  //This restores order after a removal
    {
        int heapSize = static_cast<int>(heap.size());  //This is how many emails are stored
        while (true)  //Keep going until the heap is ordered
        {
            int leftChild = 2 * index + 1;   //Find the left child position
            int rightChild = 2 * index + 2;  //Find the right child position
            int largest = index;             //Start by assuming this email is largest
            //Compare this email with its left child
            if (leftChild < heapSize)  //Check if a left child exists
            {
                list<Email>::iterator left = getIterator(leftChild);    //Find the left child
                list<Email>::iterator largestIt = getIterator(largest); //Find the current largest email
                if (*left > *largestIt)  //Check if the left child is larger
                    largest = leftChild; //Remember the left child as largest
            }
            //Compare the current largest email with the right child
            if (rightChild < heapSize)  //Check if a right child exists
            {
                list<Email>::iterator right = getIterator(rightChild);   //Find the right child
                list<Email>::iterator largestIt = getIterator(largest);  //Find the current largest email
                if (*right > *largestIt)  //Check if the right child is larger
                    largest = rightChild; //Remember the right child as largest
            }
            //Stop when this email is already the largest of the three
            if (largest == index)  //Check if no child outranks this email
                break;             //Stop because the heap is ordered
            list<Email>::iterator current = getIterator(index);      //Find this email
            list<Email>::iterator largestIt = getIterator(largest);  //Find the larger child
            Email temp = *current;   //Save this email
            *current = *largestIt;   //Move the larger child into this spot
            *largestIt = temp;       //Move this email into the child's spot
            index = largest;         //Continue from the child's position
        }
    }
public:  //These members can be used outside MaxHeap
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    MaxHeap()  //This creates an empty heap
    {
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Add a new email to the heap
    void insert(const Email& email)  //This adds one email
    {
        heap.push_back(email);                              //Place the email at the end
        int newIndex = static_cast<int>(heap.size()) - 1;   //This is the new email's position
        heapifyUp(newIndex);                                //Move it up until the heap is ordered
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Return the highest-priority email without removing it
    const Email& getMax() const  //This looks at the front email
    {
        if (heap.empty())                          //Check if there are no emails
            throw runtime_error("Heap is empty."); //Stop because there is no email to return
        return heap.front();                       //Return the highest-priority email
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Remove the highest-priority email
    void removeMax()  //This deletes the front email
    {
        if (heap.empty())  //Check if there are no emails
            return;        //Do nothing because the heap is already empty
        //Only one email is stored
        if (heap.size() == 1)  //Check if this is the only email
        {
            heap.pop_front();  //Remove that email
            return;            //Stop because the heap is now empty
        }
        //Move the last email into the front spot and delete the old last email
        heap.front() = heap.back();  //Copy the last email into the front spot
        heap.pop_back();             //Delete the old last email
        heapifyDown(0);              //Move the front email down until the heap is ordered
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    bool empty() const  //This reports whether the heap has no emails
    {
        return heap.empty();  //Return true when no emails are stored
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    int size() const  //This reports how many emails are stored
    {
        return static_cast<int>(heap.size());  //Return the number of emails
    }
};
//This block was obtained from ChatGPT (listed in Other sources in the prologue)
//EmailPriorityQueue provides the operations the CEO uses on the inbox
class EmailPriorityQueue  //This class is the email priority queue
{
private:  //These members can only be used inside EmailPriorityQueue
    MaxHeap emailHeap;  //This heap stores the unread emails
public:  //These members can be used outside EmailPriorityQueue
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    void addEmail(const Email& email)  //This adds one email to the inbox
    {
        emailHeap.insert(email);  //Put the email into the heap
    }
    //This block was obtained from ChatGPT and revised for the sample output
    //NEXT displays the highest-priority email but does not remove it
    //Repeated NEXT commands display the same email until READ is executed
    void next()  //This handles the NEXT command
    {
        if (emailHeap.empty())  //Check if there are no unread emails
        {
            cout << "No unread emails." << endl;  //Print this if the inbox is empty
            return;                                //Stop because there is nothing to show
        }
        cout << "Next email:" << endl;  //Print the label before the email
        emailHeap.getMax().display();   //Print the highest-priority email without removing it
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //READ removes the current highest-priority email
    //It does not depend on NEXT being called first
    void read()  //This handles the READ command
    {
        if (emailHeap.empty())  //Check if there are no unread emails
        {
            cout << "No unread emails." << endl;  //Print this if the inbox is empty
            return;                                //Stop because there is nothing to remove
        }
        emailHeap.removeMax();  //Remove the highest-priority email without displaying it
    }
    //This line was revised for the sample output
    void count() const  //This handles the COUNT command
    {
        cout << "There are " << emailHeap.size()  //Print how many emails are unread
             << " emails to read." << endl;       //Finish the count sentence
    }
};
//This block was obtained from ChatGPT (listed in Other sources in the prologue)
//EmailProgram reads and processes commands from the test file
//Expected EMAIL format: EMAIL sender,subject,date
//Example: EMAIL Boss,Quarterly budget meeting,09-25-2026
class EmailProgram  //This class runs the commands in the test file
{
private:  //These members can only be used inside EmailProgram
    EmailPriorityQueue emailQueue;  //This is the CEO's inbox
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Remove whitespace from the beginning and end of a string
    string trim(const string& text) const  //This cleans extra spaces off a string
    {
        size_t start = text.find_first_not_of(" \t\r\n");  //Find the first real character
        if (start == string::npos)                         //Check if the string is only whitespace
            return "";                                     //Return an empty string
        size_t end = text.find_last_not_of(" \t\r\n");     //Find the last real character
        return text.substr(start, end - start + 1);        //Return the text without extra spaces
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Process an EMAIL command
    //Subject lines may contain spaces, so commas separate the three fields
    void processEmailCommand(const string& line)  //This handles one EMAIL command
    {
        string emailData = trim(line.substr(5));                  //Keep everything after the word EMAIL
        size_t firstComma = emailData.find(',');                  //Find the comma after the sender
        size_t secondComma = emailData.find(',', firstComma + 1); //Find the comma after the subject
        if (firstComma == string::npos ||   //Check if the first comma is missing
            secondComma == string::npos)    //Check if the second comma is missing
        {
            cerr << "Invalid EMAIL command: " << line << endl;  //Print this if the command is invalid
            return;                                              //Stop because the email cannot be read
        }
        string sender =                                          //This will store the sender category
            trim(emailData.substr(0, firstComma));               //Take the text before the first comma
        string subject =                                         //This will store the subject line
            trim(emailData.substr(firstComma + 1,                //Take the text after the first comma
                                  secondComma - firstComma - 1)); //Stop before the second comma
        string date =                                            //This will store the date
            trim(emailData.substr(secondComma + 1));             //Take the text after the second comma
        Email newEmail(sender, subject, date);                   //Create the email
        emailQueue.addEmail(newEmail);                           //Add the email to the inbox
    }
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    void processCommand(const string& line)  //This handles one line from the test file
    {
        string command = trim(line);          //Remove extra spaces from the line
        if (command.empty())                  //Check if the line is blank
            return;                           //Skip a blank line
        if (command.rfind("EMAIL", 0) == 0)   //Check if the line is an EMAIL command
        {
            processEmailCommand(command);     //Add that email to the inbox
        }
        else if (command == "NEXT")           //Check if the line is NEXT
        {
            emailQueue.next();                //Display the next email
        }
        else if (command == "READ")           //Check if the line is READ
        {
            emailQueue.read();                //Remove the next email
        }
        else if (command == "COUNT")          //Check if the line is COUNT
        {
            emailQueue.count();               //Print how many emails are unread
        }
        else                                  //The line is not a known command
        {
            cerr << "Unknown command: " << command << endl;  //Print this if the command is unknown
        }
    }
public:  //These members can be used outside EmailProgram
    //This block was obtained from ChatGPT (listed in Other sources in the prologue)
    //Read all commands from the test file
    void run(const string& fileName)  //This runs the commands in one file
    {
        ifstream inputFile(fileName);  //Open the test file
        if (!inputFile)                //Check if the file did not open
        {
            cerr << "Unable to open file: "  //Print that the file could not be opened
                 << fileName << endl;        //Print the file name
            return;                          //Stop because there is no file to read
        }
        string line;                         //This stores one line from the file
        while (getline(inputFile, line))     //Read the file one line at a time
        {
            processCommand(line);            //Handle that line
        }
        inputFile.close();                   //Close the test file
    }
};
//This block was obtained from ChatGPT (listed in Other sources in the prologue)
//The test file path is supplied on the command line
int main(int argc, char* argv[])  //This is the main method
{
    if (argc != 2)  //Check if the test file was not given
    {
        cerr << "Usage: " << argv[0]   //Print how to run the program
             << " <test-file>" << endl; //Print the expected file argument
        return 1;                       //End the program because the file is missing
    }
    EmailProgram program;    //Create the program object
    program.run(argv[1]);    //Run the commands in the test file
    return 0;                //End the program
}
