// Modified Bank Management System using Random Access File

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[15];
    char mobile[15];
    char accountType[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void searchRecord(FILE *fPtr);

int main()
{
    FILE *cfPtr;
    unsigned int choice;

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        // create file if not exists
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL)
        {
            printf("File could not be opened.\n");
            exit(1);
        }

        // create 100 blank records
        struct clientData blankClient = {0, "", "", "", "", 0.0};

        for (int i = 0; i < 100; i++)
        {
            fwrite(&blankClient, sizeof(struct clientData), 1, cfPtr);
        }

        rewind(cfPtr);
    }

    // password protection
    char password[20];

    printf("Enter Password: ");
    scanf("%19s", password);

    if (strcmp(password, "admin123") != 0)
    {
        printf("Wrong Password!\n");
        fclose(cfPtr);
        return 0;
    }

    // menu loop
    while ((choice = enterChoice()) != 6)
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

        default:
            printf("Invalid Choice\n");
        }
    }

    fclose(cfPtr);

    printf("Program Ended.\n");

    return 0;
}

// menu function
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\n=================================\n");
    printf("      BANK MANAGEMENT SYSTEM\n");
    printf("=================================\n");
    printf("1. Store accounts to text file\n");
    printf("2. Update account\n");
    printf("3. Add new account\n");
    printf("4. Delete account\n");
    printf("5. Search account\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");

    scanf("%u", &choice);

    return choice;
}

// create text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;

    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be created.\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr,
            "%-6s %-15s %-15s %-15s %-12s %10s\n",
            "Acct",
            "Last Name",
            "First Name",
            "Mobile",
            "Type",
            "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1)
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr,
                    "%-6u %-15s %-15s %-15s %-12s %10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.mobile,
                    client.accountType,
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
    int option;
    double amount;

    struct clientData client;

    printf("Enter account number to update (1-100): ");
    scanf("%u", &account);

    if (account < 1 || account > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    printf("\nAccount Found\n");

    printf("Account No : %u\n", client.acctNum);
    printf("Name       : %s %s\n",
           client.firstName,
           client.lastName);

    printf("Balance    : %.2f\n", client.balance);

    printf("\n1. Deposit\n");
    printf("2. Withdraw\n");
    printf("Choose option: ");
    scanf("%d", &option);

    printf("Enter amount: ");
    scanf("%lf", &amount);

    if (option == 1)
    {
        client.balance += amount;
    }
    else if (option == 2)
    {
        if (amount > client.balance)
        {
            printf("Insufficient Balance.\n");
            return;
        }

        client.balance -= amount;
    }
    else
    {
        printf("Invalid option.\n");
        return;
    }

    fseek(fPtr,
          -sizeof(struct clientData),
          SEEK_CUR);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account Updated Successfully.\n");
    printf("New Balance : %.2f\n", client.balance);
}

// add new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1-100): ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists.\n");
        return;
    }

    client.acctNum = accountNum;

    printf("Enter Last Name: ");
    scanf("%14s", client.lastName);

    printf("Enter First Name: ");
    scanf("%14s", client.firstName);

    printf("Enter Mobile Number: ");
    scanf("%14s", client.mobile);

    printf("Enter Account Type (Savings/Current): ");
    scanf("%9s", client.accountType);

    printf("Enter Balance: ");
    scanf("%lf", &client.balance);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("New Account Added Successfully.\n");
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blankClient = {0, "", "", "", "", 0.0};

    unsigned int accountNum;

    printf("Enter account number to delete: ");
    scanf("%u", &accountNum);

    if (accountNum < 1 || accountNum > 100)
    {
        printf("Invalid account number.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fwrite(&blankClient,
           sizeof(struct clientData),
           1,
           fPtr);

    printf("Account Deleted Successfully.\n");
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
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\n========== ACCOUNT DETAILS ==========\n");

        printf("Account Number : %u\n", client.acctNum);
        printf("Last Name      : %s\n", client.lastName);
        printf("First Name     : %s\n", client.firstName);
        printf("Mobile Number  : %s\n", client.mobile);
        printf("Account Type   : %s\n", client.accountType);
        printf("Balance        : %.2f\n", client.balance);
    }
}