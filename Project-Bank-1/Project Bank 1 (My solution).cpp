#include <vector> 
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;
const string FileName = "Clients.txt";
enum enMainMenue { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Exit = 6 };
struct stBankClient {
    string AccountNumber;
    string Pin;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

vector<string> SplitString(string S1, string Del = " ") {
    short pos = 0;
    string sWord;
    vector <string>vsWord;

    // use find() function to get the position of the delimiters
    while ((pos = S1.find(Del)) != std::string::npos) {
        sWord = S1.substr(0, pos); // store the word
        if (sWord != "") {
            vsWord.push_back(sWord);
        }
        S1.erase(0, pos + Del.length());
        /* erase() until position and move to next word. */
    }
    if (S1 != "") {
        vsWord.push_back(S1);
    }
    return vsWord;
}
stBankClient ConvertLineToRecord(string S1, string Del = " ")
{
    stBankClient stClient;
    vector<string>vClient = SplitString(S1, Del);
    if (vClient.size() >= 5)
    {
        stClient.AccountNumber = vClient[0];
        stClient.Pin = vClient[1];
        stClient.Name = vClient[2];
        stClient.Phone = vClient[3];
        stClient.AccountBalance = stod(vClient[4]);
    }
    return stClient;
}
string ConvertRecordToLine(stBankClient Client, string Seperator = "#//#") {
    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.Pin + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}
vector<stBankClient> LoadDataFromFileToVector(string FileName)
{
    vector<stBankClient>vClient;
    fstream BankClientData;
    stBankClient stClient;
    BankClientData.open(FileName, ios::in);
    if (BankClientData.is_open())
    {
        string Line;
        while (getline(BankClientData, Line))
        {
            stClient = ConvertLineToRecord(Line, "#//#");
            vClient.push_back(stClient);
        }
        BankClientData.close();
    }
    return vClient;
}
void PrintClientCard(stBankClient stClient)
{
    cout << "the client data :\n";
    cout << "Account Number : " << stClient.AccountNumber;
    cout << "\nPin Code : " << stClient.Pin;
    cout << "\nClient Name : " << stClient.Name;
    cout << "\nPhone Number : " << stClient.Phone;
    cout << "\nAccount Balance : " << stClient.AccountBalance;
}
void MarkClientForDeleteByAccountNumber(string AccountNumber, vector<stBankClient>& vClient)
{
    for (stBankClient& stClient : vClient)
    {
        if (stClient.AccountNumber == AccountNumber)
        {
            stClient.MarkForDelete = true;
        }
    }
}
bool FindClientByAccountNumber(string AccountNumber, vector<stBankClient> vClients, stBankClient& stClient) {
    for (stBankClient C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            stClient = C;
            return true;
        }
    }
    return false;
}
vector <stBankClient> SaveClientsDataToFile(string FileName, vector<stBankClient> vClients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out); //overwrite
    string DataLine;
    if (MyFile.is_open()) {
        for (stBankClient C : vClients) {
            if (C.MarkForDelete == false) {
                //we only write records that are not marked for delete.
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}
string ReadClientAccountNumber()
{
    string AccountNumber = "";
    cout << "enter client account number : ";
    cin >> AccountNumber;
    return AccountNumber;
}
void PrintClientRecord(stBankClient stClient)
{
    cout << "| " << setw(20) << left << stClient.AccountNumber;
    cout << "| " << setw(15) << left << stClient.Pin;
    cout << "| " << setw(15) << left << stClient.Name;
    cout << "| " << setw(16) << left << stClient.Phone;
    cout << "| " << setw(10) << left << stClient.AccountBalance;
}
void ShowAllClientRecord(vector<stBankClient> vClient)
{
    cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << setw(20) << left << "Account Number ";
    cout << "| " << setw(15) << left << "Pin Code ";
    cout << "| " << setw(15) << left << "client name ";
    cout << "| " << setw(16) << left << "Phone Number ";
    cout << "| " << setw(10) << left << "Account Balance ";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stBankClient stClient : vClient)
    {
        PrintClientRecord(stClient);
        cout << endl;
    }
    cout << "_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}
stBankClient AddClient(string AccountNumber)
{
    stBankClient stClient;
    stClient.AccountNumber = AccountNumber;

    cout << "enter new client data : \n";
    string Input;

    cout << "Pin Code : ";
    getline(cin >> ws, Input);
    if (!Input.empty()) stClient.Pin = Input;

    cout << "Name : ";
    getline(cin, Input);
    if (!Input.empty()) stClient.Name = Input;

    cout << "Phone : ";
    getline(cin, Input);
    if (!Input.empty()) stClient.Phone = Input;

    cout << "Balance Number : ";
    getline(cin, Input);
    if (!Input.empty()) stClient.AccountBalance = stod(Input);


    stClient.MarkForDelete = false;

    return stClient;
}
void AddClientByAccountNumber(vector<stBankClient>& vClient)
{
    stBankClient stClient;
    char Answer = 'y';
    do {
        string AccountNumber = ReadClientAccountNumber();
        if (!FindClientByAccountNumber(AccountNumber, vClient, stClient))
        {
            stClient = AddClient(AccountNumber);

            vClient.push_back(stClient);
            SaveClientsDataToFile(FileName, vClient);
            cout << "Client Added Succesfully,do you want to add another client?(y/n): ";
            cin >> Answer;
            cin.ignore();
        }
        else
        {
            cout << "Client already exist, do you want to enter another account number?(y/n): ";
            cin >> Answer;
            cin.ignore();
        }
    } while (Answer == 'y' || Answer == 'Y');

}
void DeleteClientByAccountNumber(vector<stBankClient>& vClient)
{
    stBankClient stClient;
    string AccountNumber;
    char Answer = 'n';
    do {
        AccountNumber = ReadClientAccountNumber();
        if (FindClientByAccountNumber(AccountNumber, vClient, stClient))
        {
            PrintClientCard(stClient);
            cout << "\nare you sure you want delete this client info (y/n): ";
            cin >> Answer;
            if (Answer == 'y' || Answer == 'Y')
            {
                MarkClientForDeleteByAccountNumber(AccountNumber, vClient);
                SaveClientsDataToFile(FileName, vClient);
                cout << "\n\n Client Deleted Succesfully,do you want to delete another client(y/n)(default = y): ";
                cin >> Answer;
                cin.ignore();
            }
        }
        else
        {
            cout << "Client with account number (" << AccountNumber << ") is not found, do you want to look for another account number?(y/n) (deafault=no) : ";
            cin >> Answer;
            cin.ignore();
        }
    } while (Answer == 'y' || Answer == 'Y');
}
stBankClient UpdateClientData(stBankClient stClient)
{
    cout << "enter new data (leave empty to keep current value):\n";

    string Input;

    cout << "Pin Code : ";
    getline(cin >> ws, Input);
    if (!Input.empty()) stClient.Pin = Input;

    cout << "Name : ";
    getline(cin, Input);
    if (!Input.empty()) stClient.Name = Input;

    cout << "Phone : ";
    getline(cin, Input);
    if (!Input.empty()) stClient.Phone = Input;

    cout << "Balance Number : ";
    getline(cin, Input);
    if (!Input.empty()) stClient.AccountBalance = stod(Input);

    stClient.MarkForDelete = false;

    return stClient;
}
void UpdateClientInVecotr(string AccountNumber, vector<stBankClient>& vClient)
{
    for (stBankClient& client : vClient) {
        if (client.AccountNumber == AccountNumber) {
            client = UpdateClientData(client);
        }
    }
}

void UpdateClientByAccountNumber(vector<stBankClient>& vClient) {
    stBankClient Client;
    char Answer = 'n';
    do {
        string AccountNumber = ReadClientAccountNumber();
        if (FindClientByAccountNumber(AccountNumber, vClient, Client)) {
            PrintClientCard(Client);
            cout << "\n\nAre you sure you want update this client? y/n ? ";
            cin >> Answer;
            cin.ignore();
            if (Answer == 'y' || Answer == 'Y') {
                UpdateClientInVecotr(AccountNumber, vClient);
                SaveClientsDataToFile(FileName, vClient);
                cout << "Client Updated succesfully, do you want to update another client?(y/n) (default = y): ";
                cin >> Answer;
                cin.ignore();
            }
        }
        else {
            cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!,do you want to search for another account number(y/n) (default = n): ";
            cin >> Answer;
            cin.ignore();
        }
    } while (Answer == 'y' || Answer == 'Y');
}

bool FindClient(string AccountNumber, vector<stBankClient>vClient)
{
    stBankClient stClient;
    if (FindClientByAccountNumber(AccountNumber, vClient, stClient))
    {
        PrintClientCard(stClient);
        return true;
    }
    else
    {
        cout << "Client with account number (" << AccountNumber << ") not found";
        return false;
    }
}
void ShowEndScreen()
{
    system("cls");
    cout << "---------------------\n";
    cout << "Program ends";
    cout << "\n---------------------\n";
}
void GoBackToMainMenu() {
    cout << "\n\nPress any key to go back to Main Menu...";
    system("pause>0");
    system("cls");
}
enMainMenue ReadMainMenueChoice()
{
    int Choice;
    cout << "Choose what do you want to do [1 to 6]? ";
    cin >> Choice;
    switch (Choice) {
    case 1:
        return enMainMenue::Show;
    case 2:
        return enMainMenue::Add;
    case 3:
        return enMainMenue::Delete;
    case 4:
        return enMainMenue::Update;
    case 5:
        return enMainMenue::Find;
    case 6:
        return enMainMenue::Exit;
    }
}
void UserChoice(enMainMenue enWhatChoice, vector<stBankClient>vClient, stBankClient stClient)
{
    switch (enWhatChoice)
    {
    case enMainMenue::Show:
        ShowAllClientRecord(vClient);
        GoBackToMainMenu();
        break;
    case enMainMenue::Add:
        AddClientByAccountNumber(vClient);
        GoBackToMainMenu();
        break;
    case enMainMenue::Delete:
        DeleteClientByAccountNumber(vClient);
        GoBackToMainMenu();
        break;
    case enMainMenue::Update:
        UpdateClientByAccountNumber(vClient);
        GoBackToMainMenu();
        break;
    case enMainMenue::Find:
        FindClient(ReadClientAccountNumber(), vClient);
        GoBackToMainMenu();
        break;
    case enMainMenue::Exit:
        ShowEndScreen();
        break;
    }
}
void ShowMainMenue()
{
    system("cls");
    cout << "===================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Exit.\n";
    cout << "===================================\n";
}
void BankProgram()
{
    stBankClient stClient;
    enMainMenue enWhatChoice;
    vector<stBankClient> vClient = LoadDataFromFileToVector(FileName);
    do {
        ShowMainMenue();
        enWhatChoice = ReadMainMenueChoice();

        system("cls");
        UserChoice(enWhatChoice, vClient, stClient);
        vClient = LoadDataFromFileToVector(FileName);
        if (enWhatChoice != enMainMenue::Exit)
            ShowMainMenue();
    } while (enWhatChoice != enMainMenue::Exit);
}
int main() {
    BankProgram();
    system("pause>0");
    return 0;
}