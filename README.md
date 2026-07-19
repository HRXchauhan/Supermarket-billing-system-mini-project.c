# Supermarket Billing System

A console-based **Supermarket Billing System** built in **C**, developed as a
2nd Year Engineering Mini Project for the course *C Programming Fundamentals*.

The system manages product inventory, calculates customer bills (with tax
and discount rules), and generates itemized invoices — all backed by file
handling so data persists between runs.

## Team Members

- `<Add your name>`
- `<Add teammate name>`
- `<Add teammate name>`

**Guide:** `<Add guide/faculty name>`

## Features

- Add new products to inventory
- Display all products
- Search product by product code
- Update product price / stock quantity
- Delete product records
- Billing module — add multiple products to a cart, auto-calculates
  subtotal, discount, tax, and grand total
- Generates and saves an itemized invoice for every transaction
- Persistent storage using file handling (`Data/products.txt`)

## C Programming Concepts Used

Structures, Arrays, Functions, Loops, Switch Case, Conditional Statements,
File Handling.

## Repository Structure

```
Supermarket-Billing-System
│── SourceCode
│   ├── main.c
│   ├── billing.c
│   └── billing.h
│
│── Report
│   └── Supermarket_Billing_System_Report.docx
│
│── PPT
│   └── Supermarket_Billing_System_Presentation.pptx
│
│── Output
│   └── Screenshots
│
│── Data
│   └── products.txt
│
├── README.md
└── LICENSE
```

## How to Compile & Run

```bash
cd SourceCode
gcc -Wall -o billing_system main.c billing.c
./billing_system
```

On Windows (Code::Blocks / Dev-C++), create a project with `main.c`,
`billing.c`, and `billing.h`, then Build & Run.

## Sample Menu

```
1. Add Product
2. Display All Products
3. Search Product
4. Update Product
5. Delete Product
6. Billing (Generate Bill / Invoice)
7. Exit
```

## Data Files

- `Data/products.txt` — stores all product records (auto-created/updated)
- `Data/invoice_<n>.txt` — a saved copy of every invoice generated
- `Data/invoice_counter.txt` — tracks the next invoice number

## License

This project is licensed under the MIT License — see [LICENSE](LICENSE).
