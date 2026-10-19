#include <stdio.h>
#include <string.h>

struct Account {
    int accountNo;
    char name[50];
    char password[20];
    float balance;
};

struct Account accounts[100];
int totalAccounts = 0;

void saveToFile() {
    FILE *fp = fopen("bank_data.txt", "w");
    for(int i=0; i<totalAccounts; i++) {
        fprintf(fp, "%d %s %s %.2f\n", accounts[i].accountNo, accounts[i].name, accounts[i].password, accounts[i].balance);
    }
    fclose(fp);
}

void loadFromFile() {
    FILE *fp = fopen("bank_data.txt", "r");
    if(fp == NULL) return;
    while(fscanf(fp, "%d %s %s %f", &accounts[totalAccounts].accountNo, accounts[totalAccounts].name, accounts[totalAccounts].password, &accounts[totalAccounts].balance) == 4) {
        totalAccounts++;
    }
    fclose(fp);
}

int findAccount(int accNo) {
    for(int i=0; i<totalAccounts; i++) {
        if(accounts[i].accountNo == accNo) return i;
    }
    return -1;
}

void createAccount() {
    printf("\n--- নতুন একাউন্ট খুলুন ---\n");
    printf("Account Number: ");
    scanf("%d", &accounts[totalAccounts].accountNo);

    if(findAccount(accounts[totalAccounts].accountNo)!= -1) {
        printf("এই Number এ Account আছেই!\n");
        return;
    }

    printf("নাম (space ছাড়া): ");
    scanf("%s", accounts[totalAccounts].name);
    printf("Password সেট করুন: ");
    scanf("%s", accounts[totalAccounts].password);
    printf("প্রথম জমা: ");
    scanf("%f", &accounts[totalAccounts].balance);

    totalAccounts++;
    saveToFile();
    printf("Account তৈরি হয়ে গেছে!\n");
}

void checkBalance() {
    int accNo; char pass[20];
    printf("\nAccount No: "); scanf("%d", &accNo);
    int idx = findAccount(accNo);
    if(idx == -1) { printf("Account নেই!\n"); return; }

    printf("Password: "); scanf("%s", pass);
    if(strcmp(accounts[idx].password, pass)!= 0) {
        printf("ভুল Password!\n"); return;
    }
    printf("\n--- Info ---\nName: %s\nBalance: %.2f Taka\n", accounts[idx].name, accounts[idx].balance);
}

void deposit() {
    int accNo; float amount;
    printf("\nAccount No: "); scanf("%d", &accNo);
    int idx = findAccount(accNo);
    if(idx == -1) { printf("Account নেই!\n"); return; }
    printf("জমার পরিমাণ: "); scanf("%f", &amount);
    accounts[idx].balance += amount;
    saveToFile();
    printf("জমা সফল! Balance: %.2f\n", accounts[idx].balance);
}

void withdraw() {
    int accNo; float amount; char pass[20];
    printf("\nAccount No: "); scanf("%d", &accNo);
    int idx = findAccount(accNo);
    if(idx == -1) { printf("Account নেই!\n"); return; }

    printf("Password: "); scanf("%s", pass);
    if(strcmp(accounts[idx].password, pass)!= 0) {
        printf("ভুল Password!\n"); return;
    }
    printf("তোলার পরিমাণ: "); scanf("%f", &amount);
    if(amount > accounts[idx].balance) printf("Balance কম!\n");
    else {
        accounts[idx].balance -= amount;
        saveToFile();
        printf("সফল! Balance: %.2f\n", accounts[idx].balance);
    }
}

void transfer() {
    int fromAcc, toAcc; float amount; char pass[20];
    printf("\n--- টাকা Transfer ---\n");
    printf("তোমার Account No: "); scanf("%d", &fromAcc);
    int fromIdx = findAccount(fromAcc);
    if(fromIdx == -1) { printf("তোমার Account নেই!\n"); return; }

    printf("Password: "); scanf("%s", pass);
    if(strcmp(accounts[fromIdx].password, pass)!= 0) {
        printf("ভুল Password!\n"); return;
    }

    printf("যাকে পাঠাবে তার Account No: "); scanf("%d", &toAcc);
    int toIdx = findAccount(toAcc);
    if(toIdx == -1) { printf("ওই Account নেই!\n"); return; }

    printf("কত টাকা পাঠাবে: "); scanf("%f", &amount);
    if(amount > accounts[fromIdx].balance) {
        printf("তোমার Balance কম!\n"); return;
    }

    accounts[fromIdx].balance -= amount;
    accounts[toIdx].balance += amount;
    saveToFile();
    printf("Transfer সফল! %.2f টাকা %s কে পাঠানো হয়েছে।\n", amount, accounts[toIdx].name);
    printf("তোমার নতুন Balance: %.2f\n", accounts[fromIdx].balance);
}

int main() {
    loadFromFile();
    int choice;
    while(1) {
        printf("\n===== BANK PRO v2 =====\n");
        printf("1. Account খুলুন\n2. জমা\n3. তুলুন\n4. Balance দেখুন\n5. Transfer করুন\n6. বের হোন\nপছন্দ: ");
        scanf("%d", &choice);
        if(choice==1) createAccount();
        else if(choice==2) deposit();
        else if(choice==3) withdraw();
        else if(choice==4) checkBalance();
        else if(choice==5) transfer();
        else if(choice==6) break;
    }
    return 0;
}
