/* ============================================================
   billing.h
   Supermarket Billing System - Header File
   Contains structure definitions, macros, and function prototypes
   ============================================================ */

#ifndef BILLING_H
#define BILLING_H

#define MAX_PRODUCTS  100      /* Maximum products the system can store   */
#define MAX_CART      50       /* Maximum distinct items in one bill      */
#define NAME_LEN      50
#define FILENAME      "../Data/products.txt"
#define INVOICE_DIR   "../Data/"
#define TAX_RATE      0.05f    /* 5% tax on subtotal                      */
#define DISCOUNT_LIMIT 1000.0f /* Bills above this amount get a discount  */
#define DISCOUNT_RATE  0.10f   /* 10% discount                            */

/* ---------- Structures (as per project template) ---------- */

/* Structure to store a single product record */
typedef struct {
    int   code;                /* Unique product code            */
    char  name[NAME_LEN];      /* Product name                    */
    float price;               /* Price per unit                  */
    int   quantity;            /* Stock quantity available        */
} Product;

/* Structure to store one line item inside a customer's cart/bill */
typedef struct {
    int   code;
    char  name[NAME_LEN];
    float price;
    int   qty;                 /* Quantity purchased by customer  */
    float total;                /* price * qty for this item       */
} CartItem;

/* ---------- Function Prototypes ---------- */

/* File handling */
int  loadProducts(Product products[]);
void saveProducts(Product products[], int count);

/* Product management (Structures, Arrays, Functions) */
void addProduct(Product products[], int *count);
void displayProducts(Product products[], int count);
int  findProductIndex(Product products[], int count, int code);
void searchProduct(Product products[], int count);
void updateProduct(Product products[], int count);
void deleteProduct(Product products[], int *count);

/* Billing / invoice modules */
void billingModule(Product products[], int *count);
void calculateBill(CartItem cart[], int cartCount, float *subtotal,
                    float *discount, float *tax, float *grandTotal);
void generateInvoice(CartItem cart[], int cartCount, float subtotal,
                      float discount, float tax, float grandTotal);

/* Utility */
void clearInputBuffer(void);
int  getNextInvoiceNumber(void);

#endif /* BILLING_H */
