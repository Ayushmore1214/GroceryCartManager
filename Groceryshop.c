#include <stdio.h>
#include <string.h>

// Define maximum items and customers
#define MAX_ITEMS 10
#define GST_RATE 0.18 // 18% GST

// Structure to store item details
typedef struct {
    char name[30];
    float price;
    int stock;
} Item;

// Function declarations
void displayWelcomeMessage();
void initializeItems(Item items[], int *numItems);
void displayItems(Item items[], int numItems);
float calculateBill(Item items[], int selectedItem, int quantity, float *gst);
void saveBillToFile(float totalBill, float gst, Item items[], int numItems);
void displayThankYouMessage();

int main() {
    Item items[MAX_ITEMS];
    int numItems = 0;
    int selectedItem, quantity;
    float totalBill = 0.0, gst = 0.0;
    char anotherPurchase;

    // Display welcome message
    displayWelcomeMessage();

    // Initialize items
    initializeItems(items, &numItems);

    // Shopping process
    do {
        // Display items
        displayItems(items, numItems);

        // Take user input for item selection
        printf("\nEnter the item number to purchase: ");
        scanf("%d", &selectedItem);
        while (getchar() != '\n'); // Clear the input buffer
        
        if (selectedItem < 1 || selectedItem > numItems) {
            printf("Invalid item number. Please try again.\n");
            continue;
        }

        // Take user input for quantity
        printf("Enter the quantity: ");
        scanf("%d", &quantity);
        while (getchar() != '\n'); // Clear the input buffer

        // Check stock availability
        if (quantity > items[selectedItem - 1].stock) {
            printf("Sorry, not enough stock for %s. Available: %d\n", items[selectedItem - 1].name, items[selectedItem - 1].stock);
            continue;
        }

        // Update stock and calculate bill
        items[selectedItem - 1].stock -= quantity;
        totalBill += calculateBill(items, selectedItem, quantity, &gst);

        // Ask for another purchase
        printf("\nDo you want to purchase another item? (y/n): ");
        scanf(" %c", &anotherPurchase);
        while (getchar() != '\n'); // Clear the input buffer

    } while (anotherPurchase == 'y' || anotherPurchase == 'Y');

    // Display total bill and GST
    printf("\nYour total bill (including GST) is: %.2f\n", totalBill);
    printf("GST included: %.2f\n", gst);

    // Save the bill to a file
    saveBillToFile(totalBill, gst, items, numItems);

    // Thank-you message
    displayThankYouMessage();

    return 0;
}

void displayWelcomeMessage() {
    printf("\n==============================\n");
    printf("   Welcome to the Grocery Shop\n");
    printf("==============================\n");
}

void initializeItems(Item items[], int *numItems) {
    *numItems = MAX_ITEMS;

    // Sample items initialization
    Item sampleItems[] = {
        {"Apple", 50.0, 20},
        {"Milk", 30.0, 15},
        {"Bread", 40.0, 10},
        {"Rice", 80.0, 25},
        {"Sugar", 60.0, 30},
        {"Butter", 100.0, 10},
        {"Eggs", 5.0, 50},
        {"Cheese", 120.0, 15},
        {"Flour", 45.0, 20},
        {"Oil", 150.0, 18}
    };

    for (int i = 0; i < *numItems; i++) {
        items[i] = sampleItems[i];
    }
}

void displayItems(Item items[], int numItems) {
    printf("\nAvailable Items:\n");
    printf("---------------------------------------------\n");
    printf("%4s %-20s %8s %8s\n", "No.", "Name", "Price", "Stock");
    printf("---------------------------------------------\n");

    for (int i = 0; i < numItems; i++) {
        printf("%4d %-20s %8.2f %8d\n", i + 1, items[i].name, items[i].price, items[i].stock);
    }
}

float calculateBill(Item items[], int selectedItem, int quantity, float *gst) {
    float itemCost = items[selectedItem - 1].price * quantity;
    float gstAmount = itemCost * GST_RATE;
    *gst += gstAmount;
    return itemCost + gstAmount;
}

void saveBillToFile(float totalBill, float gst, Item items[], int numItems) {
    FILE *file = fopen("bill.txt", "w");
    if (file == NULL) {
        printf("Error saving the bill to a file.\n");
        return;
    }

    fprintf(file, "==============================\n");
    fprintf(file, "          Grocery Shop Bill    \n");
    fprintf(file, "==============================\n");

    fprintf(file, "%-20s %8s %8s\n", "Item", "Qty", "Price");
    fprintf(file, "---------------------------------------------\n");

    for (int i = 0; i < numItems; i++) {
        int sold = MAX_ITEMS - items[i].stock; // Calculate sold quantity
        if (sold > 0) {
            fprintf(file, "%-20s %8d %8.2f\n", items[i].name, sold, sold * items[i].price);
        }
    }

    fprintf(file, "---------------------------------------------\n");
    fprintf(file, "Total Bill (including GST): %.2f\n", totalBill);
    fprintf(file, "GST Amount: %.2f\n", gst);
    fprintf(file, "==============================\n");

    fclose(file);
    printf("\nThe bill has been saved to 'bill.txt'.\n");
}

void displayThankYouMessage() {
    printf("\n==============================\n");
    printf("   Thank you for shopping with us!\n");
    printf("==============================\n");
}
