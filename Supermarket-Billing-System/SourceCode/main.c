/* ============================================================
   main.c
   Supermarket Billing System
   C Programming Mini Project - Console Based Application

   Manages products, calculates bills, and generates invoices.
   Concepts used: Structures, Arrays, Functions, Loops,
                  Switch Case, Conditional Statements,
                  File Handling
   ============================================================ */

#include <stdio.h>
#include "billing.h"

int main(void)
{
    Product products[MAX_PRODUCTS];
    int count;
    int choice;

    /* Start with an empty in-memory inventory */
    count = loadProducts(products);

    printf("=====================================================\n");
    printf("        SUPERMARKET BILLING SYSTEM\n");
    printf("=====================================================\n");

    do
    {
        printf("\n--------------------- MAIN MENU ---------------------\n");
        printf("1. Add Product\n");
        printf("2. Display All Products\n");
        printf("3. Search Product\n");
        printf("4. Update Product\n");
        printf("5. Delete Product\n");
        printf("6. Billing (Generate Bill / Invoice)\n");
        printf("7. Exit\n");
        printf("-------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
        case 1:
            addProduct(products, &count);
            break;
        case 2:
            displayProducts(products, count);
            break;
        case 3:
            searchProduct(products, count);
            break;
        case 4:
            updateProduct(products, count);
            break;
        case 5:
            deleteProduct(products, &count);
            break;
        case 6:
            billingModule(products, &count);
            break;
        case 7:
            saveProducts(products, count);
            printf("\nThank you for using the Supermarket Billing "
                   "System. Goodbye!\n");
            break;
        default:
            printf("\nInvalid choice. Please select an option between "
                   "1 and 7.\n");
        }

    } while (choice != 7);

    return 0;
}
