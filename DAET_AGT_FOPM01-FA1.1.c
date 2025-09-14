#include <stdio.h>
#include <stdbool.h>

int main() {
    int quantity; //stores the amount of lollipops bought
    double amountDue, cash, change, vat, vatableAmount; //variables for calculating values involving money
    char inputBuffer[100]; // for input validation
    
    double price = 5.00; //price of lollipops
    bool flag = true; // Flags if cash is less than the price of purchase. Set to true by default

    printf("\t\tWelcome to Lollipops Factory!\n");

    //While loop runs until input is validated
    while (true) {
        printf("Sweet and minty Lollipops! How many would you want to buy? ");
        //Input Validation for quantity of lollipops
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
            if (sscanf(inputBuffer, "%d", &quantity) == 1 && quantity > 0) {
                break;
            } else {
                printf("Invalid input. Please enter a valid input.\n");
            }
        }
    }

    amountDue = quantity * price; //Calculates price of order

    //Do-while loop to prevent payments less than the amount due.
    do {
        //While loop runs until input is validated
        while (true) {
            printf("You have bought %d Sweet and minty Lollipops! Please pay, PhP %.2f. -> ",quantity, amountDue);
           //Input Validation for payment of amount due
            if (fgets(inputBuffer, sizeof(inputBuffer), stdin)) {
                if (sscanf(inputBuffer, "%lf", &cash) == 1 && cash > 0) {
                    break;
                } else {
                    printf("Invalid input. Please enter a valid input.\n");
                }
            }
        }
        flag = cash < amountDue;
        //Returns an error message if cash is less than amount due
        if (flag){
            printf("Not enough money.\n");
        }
    } while (flag);

    //Formulas for calculating vatable amount, Value-added Tax (VAT), and change respectively.
    vatableAmount = amountDue / (1 + 0.12);
    vat = amountDue - vatableAmount;
    change = cash - amountDue;

    printf("\nPlease examine the transaction details below:\n\n");
    printf("Amount Due (VAT inclusive):  PhP %.2f\n", amountDue);
    printf("VAT (12%%):                   PhP %.2f\n", vat);
    printf("VATable Amount:              PhP %.2f\n", vatableAmount);
    printf("Amount of Cash:              PhP %.2f\n", cash);
    printf("Change:                      PhP %.2f\n", change);

    printf("\n\"Thanks and enjoy our lollipops!\"\n");

    return 0;
}