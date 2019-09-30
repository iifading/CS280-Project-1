/**
 * Ricky Palaguachi
 * CS280 - 009
 * 
 * Program 1
*/

#include <iostream>
#include <fstream>
#include <ctime>
#include <cctype>
#include <map>
#include <unordered_set>

using namespace std;

void rot13(int argc, char * argv[]);      
void rotateLine(string line);          
void genCaesarDictionary(int argc, char* argv[]);   
void caesarCipher(int argc, char* argv[], char mode);   
void mapLine(string line, map<char, char> cipherMap);
bool isValidDictionaryFile(string dictFileName, map<char, char> * mapPtr, char mode);

/**
 * Interprets command line arguments and what encryption/decryption to run
 * and on what stream (std in or a file) to encrypt/decrypt
 * 
 * @param argc Count of command line arguments passed
 * @param argv An array of strings, where each element is a command line argument
 * @return 0 Succesful termination of program
 */
int main(int argc, char * argv[]) {
    if (argc == 1){
        cout << "MISSING COMMAND" << endl;
    }
    else {
        string arg = argv[1];
        
        if (arg == "-r"){
            rot13(argc, argv);
        }
        else if (arg == "-g"){
            genCaesarDictionary(argc, argv);
        }
        else if (arg == "-e"){
            caesarCipher(argc, argv, 'e');
        }
        else if (arg == "-d"){
            caesarCipher(argc, argv, 'd');
        }
        else{
            cout << argv[1] << " NOT A VALID COMMAND" << endl;
        }
    }
    
    return 0;
}

/**
 * Runs a rot13 cipher on either std in or the optionally passed in file and 
 * prints the encryption to std out.
 * 
 * @param argc Count of command line arguments passed
 * @param argv An array of strings, where each element is a command line argument
 */
void rot13(int argc, char* argv[]){
    string line;
    if (argc == 3){ //read from file
        string readFileName = argv[2];
        ifstream readStream;
        readStream.open(readFileName);
        
        if (!readStream.is_open()){
            cout << readFileName << " FILE COULD NOT BE OPENED" << endl;
            return;
        }
        
        //perform rot13 encryption on every line in the file
        while(getline(readStream, line)){
            rotateLine(line);
        }
        
        readStream.close();
    }
    else if (argc == 2){ //read from std in
        //perform rot13 encryption on every line from std in
        while(getline(cin, line)){
            rotateLine(line);
        }
    }
    else {
        cout << "TOO MANY ARGUMENTS" << endl;
    }
}//rot13()

/**
 * Prints rot13 encrypted text to std out based off of a passed in string parameter.
 * 
 * @param line A line of text to be encrypted
 */
void rotateLine(string line){
    //loop over every character in the line
    for (int i = 0; i < line.length(); i++){
        //prepare char to be rotated and determine case
        char c = line[i];
        bool upperCaseFlag = isupper(c);
        c = tolower(c);
        
        if ( 'a' > c || c > 'z'){
            cout << c;
            continue;
        }

        //wrap around alphabet first if needed
        if ('z' < c + 13){
            c -= 26;
        }
        
        //rotate 13
        c += 13;
    
        //output rotated char
        if (upperCaseFlag == true){
            cout << (char) toupper(c);
        }
        else {
            cout << c;
        }
    }//for

    cout << endl;
}

/**
 * Generates a dictionary of unique plainText character and cipherText character pairs.
 * Prints these pairs together and seperates each pair by whitespace to std out.
 * 
 * @param argc Count of command line arguments passed
 * @param argv An array of strings, where each element is a command line argument
 */
void genCaesarDictionary(int argc, char* argv[]){
    //populates two arrays of all lowercase
    //letters in the alphabet
    char abcList[26];
    char takeoutList[26];
    for (int i = 0; i < 26; i++){
        abcList[i] = i + 'a';
        takeoutList[i] = i + 'a';
    }
    
    //populate an array of unique characters from a-z in a random order
    char scrambledList[26];
    srand(time(NULL));
    int hitCount = 0;
    while (hitCount < 26){
        //generate random int from 0-25
        int randomIndex = rand() % 26;
        char c = takeoutList[randomIndex];
        
        //assign a new random unique character and remove it from the old list
        scrambledList[hitCount] = c;
        takeoutList[randomIndex] = '0';
        
        //increment count of successful mappings
        hitCount++;
    }
    
    //prints dictionary to std out
    for (int i = 0; i < 26; i++){
        cout << abcList[i] << scrambledList[i] << " " << endl;
    }
}

/**
 * Encrypts/Decrypts text according to a passed in dictionary file. Performs a caesar cipher 
 * encryption/decryption on an optional file, or std in by default, according to the dictionary and
 * the passed mode.
 * 
 * @param argc Argument Count
 * @param argv Argument Vector
 * @param mode Determines encryption or decryption
 */
void caesarCipher(int argc, char* argv[], char mode){
    string dictFileName;
    ifstream dictStream;
    string readFileName = "";
    ifstream readStream;
    
    //Validate dictionary to be read in and opened while also attempting to generate a map
    //of plainText -> cipherText key-value pairs
    if (argc == 2){
        cout << "NO DICTIONARY GIVEN" << endl;
        return;
    }
    if (argc >= 3){
        dictFileName = argv[2];
    }
    
    //validates dictFileName and assigns a pointer to the address
    //of a map that is built while validating the file for either encryption or decryption
    map<char, char> cipherMap;
    map<char, char> * mapPtr = &cipherMap;
    if (!isValidDictionaryFile(dictFileName, mapPtr, mode)){
        return;
    }

    //Check for optional readFileName
    if (argc == 4){
        readFileName = argv[3];
        readStream.open(readFileName);
        
        if (!readStream.is_open()){
            cout << readFileName << " FILE COULD NOT BE OPENED" << endl;
            return;
        }
    }
    
    if (argc > 4){
        cout << "TOO MANY ARGUMENTS" << endl;
        return;
    }
    
    //Begin encrpytion/decryption
    string line;
    if (readFileName != ""){ //read from file
        while(getline(readStream, line)){
            mapLine(line, cipherMap);
        }
    }
    else { //read from std in
        while(getline(cin, line)){
            mapLine(line, cipherMap);
        }
    }
}

/**
 * Maps every character from line according to the value
 * that is associated to it in cipherMap
 * 
 * @param line A passed in string to encrypt according to cipherMap
 * @param cipherMap A map of plainText -> cipherText for encryption or the opposite for decryption
 */
void mapLine(string line, map<char, char> cipherMap){
    //loop over every character in the input
    for (int i = 0; i < line.length(); i++){
        //prepare char to be ciphered
        char c = line[i];
        //skip non-alphabetic characters in encryption
        if (!isalpha(c)){
            cout << c;
            continue;
        }

        bool upperCaseFlag = isupper(c);
        c = tolower(c);

        //output encrypted char to std out
        if (upperCaseFlag == true){
            cout << (char) toupper(cipherMap[c]);
        }
        else {
            cout << cipherMap[c];
        }
    }//for

    cout << endl;
}

/**
 * Validates a cipher dictionary while also building a map
 * for plaintext -> cipherText key-value pairs for encryption
 * or cipherText -> plainText key-value pairs for decryption
 * 
 * @param dictFileName File name of a dictionary of plainText and cipherText values to be validated
 * @param mapPtr A pointer that holds the address of the map that decodes the cipher
 * @param mode Encryption or Decryption flag char
 * @return valid True if valid dictionary, False if invalid dictionary
 */
bool isValidDictionaryFile(string dictFileName, map<char, char> * mapPtr, char mode){
    /*
     Dictionary Error Checking
     "FORMATTING ERROR " + entry:   non two-letter pair entry found     NEEDS TESTING
     "DUPLICATE CIPHERTEXT " + L:   duplicate ciphertext found          NEEDS TESTING
     "MAPPING ERROR " + L:          plaintext == ciphertext             NEEDS TESTING
     "MISSING LETTER " + L:         abc order broken at some L          NEEDS TESTING
     */

    ifstream dictStream;
    dictStream.open(dictFileName);

    //Checking if dictionary could be opened
    if (!dictStream.is_open()){
        cout << dictFileName << " DICTIONARY COULD NOT BE OPENED" << endl;
        return false;
    }

    unordered_set<char> cipherCharSet;

    string line;
    char expectedChar = 'a';

    //Builds a validated map of plainText -> cipherText
    while(getline(dictStream, line)){
        //Validates proper entry type (plainText + cipherText)
        if (line.size() != 2){
            cout << "FORMATTING ERROR " << line << endl;
            return false;
        }
        char plainText = line[0];
        char cipherText = line[1];
        
        //Validates abc order
        if (plainText != expectedChar){
            cout << "MISSING LETTER " << expectedChar << endl;
            return false;
        }
        
        //Validates no plainText maps to itself
        if (plainText == cipherText){
            cout << "MAPPING ERROR " << plainText << endl;
            return false;
        }
        
        //Validates no duplicate cipherText characters using a set
        if (cipherCharSet.find(cipherText) == cipherCharSet.end()){
            cipherCharSet.insert(cipherText);
        }
        else {
            cout << "DUPLICATE CIPHERTEXT " << cipherText << endl;
            return false;
        }
        
        //validate next plainText is in abc order
        expectedChar += 1;
        
        //Finally, map plainText -> cipherText for Encryption
        //     or, map cipherText -> plaintext for Decryption
        if (mode == 'e'){
            (*mapPtr)[plainText] = cipherText;
        } 
        else if (mode == 'd'){
            (*mapPtr)[cipherText] = plainText;
        }
    }//while
    
    dictStream.close();
    return true;
}