/* ============================================================
   billing.c
   Supermarket Billing System - Function Definitions
   Concepts used: Structures, Arrays, Functions, Loops,
                  Conditional Statements, File Handling
   ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "billing.h"

/* ------------------------------------------------------------
   clearInputBuffer
   Clears leftover characters in stdin after scanf() calls so
   the next fgets()/scanf() is not corrupted.
   ------------------------------------------------------------ */
void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { /* discard */ }
}

/* ------------------------------------------------------------
   loadProducts
   Loads product records from the data file into the array.
   Returns the number of records loaded (File Handling).
   ------------------------------------------------------------ */
int loadProducts(Product products[]) {
    FILE *fp = fopen(FILENAME, "r");
    int count = 0;

    if (fp == NULL) {
        /* No existing data file yet - start with an empty inventory */
        return 0;
    }

    while (count < MAX_PRODUCTS &&
           fscanf(fp, "%d,%49[^,],%f,%d\n",
                  &products[count].code,
                  products[count].name,
                  &products[count].price,
                  &products[count].quantity) == 4) {
        count++;
    }

    fclose(fp);
    return count;
}

/* ------------------------------------------------------------
   saveProducts
   Saves all current product records back to the data file.
   ------------------------------------------------------------ */
void saveProducts(Product products[], int count) {
    FILE *fp = fopen(FILENAME, "w");
    int i;

    if (fp == NULL) {
        printf("\nError: Could not save product data to file.\n");
        return;
    }

    for (i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%.2f,%d\n",
                products[i].code, products[i].name,
                products[i].price, products[i].quantity);
    }

    fclose(fp);
}

/* ------------------------------------------------------------
   findProductIndex
   Searches the products array (Loop) for a given product code.
   Returns the array index, or -1 if not found.
   ------------------------------------------------------------ */
int findProductIndex(Product products[], int count, int code) {
    int i;
    for (i = 0; i < count; i++) {
        if (products[i].code == code) {
            return i;
        }
    }
    return -1;
}

/* ------------------------------------------------------------
   addProduct
   Adds a new product record. Rejects duplicate product codes
   (input validation / conditional statements).
   ------------------------------------------------------------ */
void addProduct(Product products[], int *count) {
    Product p;

    if (*count >= MAX_PRODUCTS) {
        printf("\nInventory is full. Cannot add more products.\n");
        return;
    }

    printf("\n--- Add New Product ---\n");

    printf("Enter Product Code   : ");
    scanf("%d", &p.code);
    clearInputBuffer();

    if (findProductIndex(products, *count, p.code) != -1) {
        printf("\nError: A product with code %d already exists.\n", p.code);
        return;
    }

    printf("Enter Product Name   : ");
    fgets(p.name, NAME_LEN, stdin);
    p.name[strcspn(p.name, "\n")] = '\0';   /* remove trailing newline */

    printf("Enter Price per Unit  : ");
    scanf("%f", &p.price);

    printf("Enter Stock Quantity  : ");
    scanf("%d", &p.quantity);
    clearInputBuffer();

    if (p.price < 0 || p.quantity < 0) {
        printf("\nError: Price and Quantity cannot be negative.\n");
        return;
    }

    products[*count] = p;
    (*count)++;

    saveProducts(products, *count);
    printf("\nProduct added successfully!\n");
}

/* ------------------------------------------------------------
   displayProducts
   Displays all product records in tabular form (Arrays, Loop).
   ------------------------------------------------------------ */
void displayProducts(Product products[], int count) {
    int i;

    if (count == 0) {
        printf("\nNo products found in inventory.\n");
        return;
    }

    printf("\n%-6s %-25s %-12s %-10s\n", "Code", "Name", "Price", "Stock");
    printf("--------------------------------------------------------\n");

    for (i = 0; i < count; i++) {
        printf("%-6d %-25s %-12.2f %-10d\n",
               products[i].code, products[i].name,
               products[i].price, products[i].quantity);
    }
}

/* ------------------------------------------------------------
   searchProduct
   Searches for a product by code and displays its details.
   ------------------------------------------------------------ */
void searchProduct(Product products[], int count) {
    int code, index;

    printf("\nEnter Product Code to search: ");
    scanf("%d", &code);
    clearInputBuffer();

    index = findProductIndex(products, count, code);

    if (index == -1) {
        printf("\nProduct Not Found.\n");
        return;
    }

    printf("\n--- Product Found ---\n");
    printf("Code     : %d\n", products[index].code);
    printf("Name     : %s\n", products[index].name);
    printf("Price    : %.2f\n", products[index].price);
    printf("Stock    : %d\n", products[index].quantity);
}

/* ------------------------------------------------------------
   updateProduct
   Updates the price and/or stock quantity of an existing product.
   ------------------------------------------------------------ */
void updateProduct(Product products[], int count) {
    int code, index;

    printf("\nEnter Product Code to update: ");
    scanf("%d", &code);
    clearInputBuffer();

    index = findProductIndex(products, count, code);

    if (index == -1) {
        printf("\nProduct Not Found.\n");
        return;
    }

    printf("Current Price : %.2f\n", products[index].price);
    printf("Enter New Price (0 = keep current): ");
    float newPrice;
    scanf("%f", &newPrice);
    if (newPrice > 0) {
        products[index].price = newPrice;
    }

    printf("Current Stock : %d\n", products[index].quantity);
    printf("Enter New Stock Quantity (-1 = keep current): ");
    int newQty;
    scanf("%d", &newQty);
    clearInputBuffer();
    if (newQty >= 0) {
        products[index].quantity = newQty;
    }

    saveProducts(products, count);
    printf("\nRecord updated successfully!\n");
}

/* ------------------------------------------------------------
   deleteProduct
   Deletes a product record by shifting subsequent array
   elements left by one position (Arrays).
   ------------------------------------------------------------ */
void deleteProduct(Product products[], int *count) {
    int code, index, i;

    printf("\nEnter Product Code to delete: ");
    scanf("%d", &code);
    clearInputBuffer();

    index = findProductIndex(products, *count, code);

    if (index == -1) {
        printf("\nProduct Not Found.\n");
        return;
    }

    for (i = index; i < (*count) - 1; i++) {
        products[i] = products[i + 1];
    }
    (*count)--;

    saveProducts(products, *count);
    printf("\nProduct deleted successfully!\n");
}

/* ------------------------------------------------------------
   calculateBill
   Calculates subtotal, discount, tax, and grand total for the
   items currently in the cart.
   ------------------------------------------------------------ */
void calculateBill(CartItem cart[], int cartCount, float *subtotal,
                    float *discount, float *tax, float *grandTotal) {
    int i;
    *subtotal = 0;

    for (i = 0; i < cartCount; i++) {
        *subtotal += cart[i].total;
    }

    /* Discount rule: bills above DISCOUNT_LIMIT get DISCOUNT_RATE off */
    if (*subtotal > DISCOUNT_LIMIT) {
        *discount = *subtotal * DISCOUNT_RATE;
    } else {
        *discount = 0;
    }

    *tax = (*subtotal - *discount) * TAX_RATE;
    *grandTotal = (*subtotal - *discount) + *tax;
}

/* ------------------------------------------------------------
   getNextInvoiceNumber
   Reads/updates a small counter file so every invoice gets a
   unique, increasing invoice number across program runs.
   ------------------------------------------------------------ */
int getNextInvoiceNumber(void) {
    int number = 1;
    FILE *fp = fopen(INVOICE_DIR "invoice_counter.txt", "r");

    if (fp != NULL) {
        fscanf(fp, "%d", &number);
        fclose(fp);
        number++;
    }

    fp = fopen(INVOICE_DIR "invoice_counter.txt", "w");
    if (fp != NULL) {
        fprintf(fp, "%d", number);
        fclose(fp);
    }

    return number;
}

/* ------------------------------------------------------------
   generateInvoice
   Prints an itemized invoice to the console AND saves a copy
   to a text file inside the Data folder (File Handling).
   ------------------------------------------------------------ */
void generateInvoice(CartItem cart[], int cartCount, float subtotal,
                      float discount, float tax, float grandTotal) {
    int i, invoiceNo;
    char filename[100];
    FILE *fp;

    invoiceNo = getNextInvoiceNumber();
    sprintf(filename, "%sinvoice_%d.txt", INVOICE_DIR, invoiceNo);
    fp = fopen(filename, "w");

    printf("\n==================== INVOICE ====================\n");
    printf("Invoice No : %d\n", invoiceNo);
    printf("---------------------------------------------------\n");
    printf("%-6s %-20s %-6s %-8s %-10s\n", "Code", "Name", "Qty", "Price", "Total");

    if (fp != NULL) {
        fprintf(fp, "==================== INVOICE ====================\n");
        fprintf(fp, "Invoice No : %d\n", invoiceNo);
        fprintf(fp, "---------------------------------------------------\n");
        fprintf(fp, "%-6s %-20s %-6s %-8s %-10s\n", "Code", "Name", "Qty", "Price", "Total");
    }

    for (i = 0; i < cartCount; i++) {
        printf("%-6d %-20s %-6d %-8.2f %-10.2f\n",
               cart[i].code, cart[i].name, cart[i].qty,
               cart[i].price, cart[i].total);
        if (fp != NULL) {
            fprintf(fp, "%-6d %-20s %-6d %-8.2f %-10.2f\n",
                    cart[i].code, cart[i].name, cart[i].qty,
                    cart[i].price, cart[i].total);
        }
    }

    printf("---------------------------------------------------\n");
    printf("Subtotal   : %.2f\n", subtotal);
    printf("Discount   : %.2f\n", discount);
    printf("Tax (%.0f%%)  : %.2f\n", TAX_RATE * 100, tax);
    printf("Grand Total: %.2f\n", grandTotal);
    printf("===================================================\n");
    printf("Thank you for shopping with us!\n");

    if (fp != NULL) {
        fprintf(fp, "---------------------------------------------------\n");
        fprintf(fp, "Subtotal   : %.2f\n", subtotal);
        fprintf(fp, "Discount   : %.2f\n", discount);
        fprintf(fp, "Tax (%.0f%%)  : %.2f\n", TAX_RATE * 100, tax);
        fprintf(fp, "Grand Total: %.2f\n", grandTotal);
        fprintf(fp, "===================================================\n");
        fprintf(fp, "Thank you for shopping with us!\n");
        fclose(fp);
        printf("\n(Invoice saved to Data/invoice_%d.txt)\n", invoiceNo);
    }
}

/* ------------------------------------------------------------
   billingModule
   Runs the customer checkout / cart process: lets the cashier
   add products to a cart by code + quantity, reduces stock,
   then calculates and generates the invoice.
   ------------------------------------------------------------ */
void billingModule(Product products[], int *count) {
    CartItem cart[MAX_CART];
    int cartCount = 0;
    int code, qty, index;
    char choice;
    float subtotal, discount, tax, grandTotal;

    printf("\n========== NEW BILL / CHECKOUT ==========\n");

    do {
        if (cartCount >= MAX_CART) {
            printf("\nCart is full. Proceeding to billing.\n");
            break;
        }

        printf("\nEnter Product Code (0 to finish): ");
        scanf("%d", &code);

        if (code == 0) {
            break;
        }

        index = findProductIndex(products, *count, code);
        if (index == -1) {
            printf("Product Not Found. Try again.\n");
            clearInputBuffer();
            continue;
        }

        printf("Product: %s | Price: %.2f | Available Stock: %d\n",
               products[index].name, products[index].price,
               products[index].quantity);

        printf("Enter Quantity: ");
        scanf("%d", &qty);
        clearInputBuffer();

        if (qty <= 0) {
            printf("Invalid quantity.\n");
            continue;
        }

        if (qty > products[index].quantity) {
            printf("Error: Insufficient stock. Only %d units available.\n",
                   products[index].quantity);
            continue;
        }

        /* Add item to cart */
        cart[cartCount].code  = products[index].code;
        strcpy(cart[cartCount].name, products[index].name);
        cart[cartCount].price = products[index].price;
        cart[cartCount].qty   = qty;
        cart[cartCount].total = products[index].price * qty;
        cartCount++;

        /* Reduce stock */
        products[index].quantity -= qty;

        printf("Added to cart. Add another product? (y/n): ");
        scanf(" %c", &choice);
        clearInputBuffer();

    } while (choice == 'y' || choice == 'Y');

    if (cartCount == 0) {
        printf("\nNo items added. Billing cancelled.\n");
        return;
    }

    calculateBill(cart, cartCount, &subtotal, &discount, &tax, &grandTotal);
    generateInvoice(cart, cartCount, subtotal, discount, tax, grandTotal);

    /* Persist the updated stock quantities */
    saveProducts(products, *count);
}
