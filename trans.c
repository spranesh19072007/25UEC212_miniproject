/*
    BANK MANAGEMENT SYSTEM
    Random Access File Version
*/

#include <stdio.h>
#include <stdlib.h>

// structure
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);

void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

void searchRecord(FILE *fPtr);
void displayAllRecords(FILE *fPtr);

void deposit(FILE *fPtr);
void withdraw(FILE *fPtr);

void transferMoney(FILE *fPtr);
void totalBalance(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // open binary file
    cfPtr = fopen("credit.dat", "rb+");

    if (cfPtr == NULL)
    {
        printf("File could not be opened.\n");
        return 1;
    }

    while ((choice = enterChoice()) != 11)
    {
        switch (choice)
        {
            case 1:
                textFile(cfPtr);
                break;

            case 2:
                updateRecord(cfPtr);
                break;

            case 3:
                newRecord(cfPtr);
                break;

            case 4:
                deleteRecord(cfPtr);
                break;

            case 5:
                searchRecord(cfPtr);
                break;

            case 6:
                displayAllRecords(cfPtr);
                break;

            case 7:
                deposit(cfPtr);
                break;

            case 8:
                withdraw(cfPtr);
                break;

            case 9:
                transferMoney(cfPtr);
                break;

            case 10:
                totalBalance(cfPtr);
                break;

            default:
                printf("Invalid choice.\n");
        }
    }

    fclose(cfPtr);

    return 0;
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n========== BANK MENU ==========\n");
    printf("1  - Export accounts to text file\n");
    printf("2  - Update account\n");
    printf("3  - Add new account\n");
    printf("4  - Delete account\n");
    printf("5  - Search account\n");
    printf("6  - Display all accounts\n");
    printf("7  - Deposit money\n");
    printf("8  - Withdraw money\n");
    printf("9  - Transfer money\n");
    printf("10 - Total bank balance\n");
    printf("11 - Exit\n");

    printf("Enter choice: ");
    scanf("%u", &choice);

    return choice;
}

// export accounts to text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client;

    writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL)
    {
        printf("Unable to create text file.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "%-6s%-16s%-11s%10s\n",
            "Acct",
            "Last Name",
            "First Name",
            "Balance");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-6u%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);

    printf("accounts.txt created successfully.\n");
}

// update account
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double amount;

    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (long)((account - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);

    printf("Enter amount (+ deposit / - withdraw): ");
    scanf("%lf", &amount);

    client.balance += amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Updated Balance: %.2f\n", client.balance);
}

// create new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (long)((accountNum - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter lastname firstname balance:\n");

    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = accountNum;

    fseek(fPtr,
          (long)((accountNum - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Account created successfully.\n");
}

// delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (long)((accountNum - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr,
          (long)((accountNum - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fwrite(&blankClient,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Account deleted successfully.\n");
}

// search account
void searchRecord(FILE *fPtr)
{
    unsigned int account;

    struct clientData client;

    printf("Enter account number to search: ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (long)((account - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    printf("\n===== ACCOUNT DETAILS =====\n");

    printf("Account Number : %u\n", client.acctNum);
    printf("Last Name      : %s\n", client.lastName);
    printf("First Name     : %s\n", client.firstName);
    printf("Balance        : %.2f\n", client.balance);
}

// display all accounts
void displayAllRecords(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%10s\n",
           "Acct",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            printf("%-6u%-16s%-11s%10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// deposit
void deposit(FILE *fPtr)
{
    unsigned int account;
    double amount;

    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    printf("Enter deposit amount: ");
    scanf("%lf", &amount);

    fseek(fPtr,
          (long)((account - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    client.balance += amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Deposit successful.\n");
}

// withdraw
void withdraw(FILE *fPtr)
{
    unsigned int account;
    double amount;

    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &account);

    printf("Enter withdrawal amount: ");
    scanf("%lf", &amount);

    fseek(fPtr,
          (long)((account - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
        return;
    }

    if (amount > client.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    client.balance -= amount;

    fseek(fPtr,
          -(long)sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Withdrawal successful.\n");
}

// transfer money
void transferMoney(FILE *fPtr)
{
    unsigned int sender;
    unsigned int receiver;

    double amount;

    struct clientData senderClient;
    struct clientData receiverClient;

    printf("Enter sender account number: ");
    scanf("%u", &sender);

    printf("Enter receiver account number: ");
    scanf("%u", &receiver);

    printf("Enter amount to transfer: ");
    scanf("%lf", &amount);

    // sender
    fseek(fPtr,
          (long)((sender - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&senderClient,
          sizeof(struct clientData),
          1,
          fPtr);

    // receiver
    fseek(fPtr,
          (long)((receiver - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fread(&receiverClient,
          sizeof(struct clientData),
          1,
          fPtr);

    if (senderClient.acctNum == 0 ||
        receiverClient.acctNum == 0)
    {
        printf("Invalid account.\n");
        return;
    }

    if (amount > senderClient.balance)
    {
        printf("Insufficient balance.\n");
        return;
    }

    senderClient.balance -= amount;
    receiverClient.balance += amount;

    // write sender
    fseek(fPtr,
          (long)((sender - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fwrite(&senderClient,
           sizeof(struct clientData),
           1,
           fPtr);

    // write receiver
    fseek(fPtr,
          (long)((receiver - 1) * sizeof(struct clientData)),
          SEEK_SET);

    fwrite(&receiverClient,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Transfer successful.\n");
}

// total bank balance
void totalBalance(FILE *fPtr)
{
    struct clientData client;

    double total = 0;

    rewind(fPtr);

    while (fread(&client,
                  sizeof(struct clientData),
                  1,
                  fPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            total += client.balance;
        }
    }

    printf("Total Bank Balance = %.2f\n", total);
}