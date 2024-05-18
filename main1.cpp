#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Product Class
class Product {
private:
    string name;
    string category;
    double price;
    double taxRate;
    double shippingCost;

public:
    Product(string n, string c, double p, double t, double s)
        : name(n), category(c), price(p), taxRate(t), shippingCost(s) {}

    string getName() const { return name; }
    string getCategory() const { return category; }
    double getPrice() const { return price; }
    double getTaxRate() const { return taxRate; }
    double getShippingCost() const { return shippingCost; }
    
    double getTotalCost() const {
        return price + (price * taxRate) + shippingCost;
    }
    
    void setPrice(double newPrice) {
        price = newPrice;
    }
};

// DiscountStrategy Class (Abstract Class)
class DiscountStrategy {
public:
    virtual void applyDiscount(Product &product) = 0; // pure virtual function
    virtual ~DiscountStrategy() = default;
};

// CategoryDiscountStrategy Class
class CategoryDiscountStrategy : public DiscountStrategy {
private:
    string category;
    double discountRate;

public:
    CategoryDiscountStrategy(string c, double d) : category(c), discountRate(d) {}

    void applyDiscount(Product &product) override {
        if (product.getCategory() == category) {
            double newPrice = product.getPrice() * (1 - discountRate);
            product.setPrice(newPrice);
            cout << product.getName() << " has category discount applied: new price " << newPrice << "\n";
        }
    }
};

// PromotionDiscountStrategy Class
class PromotionDiscountStrategy : public DiscountStrategy {
private:
    string promotionCode;
    double discountRate;

public:
    PromotionDiscountStrategy(string p, double d) : promotionCode(p), discountRate(d) {}

    void applyDiscount(Product &product) override {
        double newPrice = product.getPrice() * (1 - discountRate);
        product.setPrice(newPrice);
        cout << product.getName() << " has promotion discount applied: new price " << newPrice << "\n";
    }
};

// ShoppingCart Class
class ShoppingCart {
private:
    vector<Product> products;
    vector<shared_ptr<DiscountStrategy>> discountStrategies;

public:
    void addProduct(const Product &product) {
        products.push_back(product);
        cout << product.getName() << " added to cart.\n";
    }

    void addDiscountStrategy(shared_ptr<DiscountStrategy> strategy) {
        discountStrategies.push_back(strategy);
    }

    void applyDiscounts() {
        for (auto &product : products) {
            for (auto &strategy : discountStrategies) {
                strategy->applyDiscount(product);
            }
        }
    }

    double calculateTotalCost() const {
        double total = 0.0;
        for (const auto &product : products) {
            total += product.getTotalCost();
        }
        return total;
    }

    void listProducts() const {
        for (const auto &product : products) {
            cout << "Product: " << product.getName() << ", Category: " << product.getCategory()
                 << ", Price: " << product.getPrice() << ", Tax Rate: " << product.getTaxRate()
                 << ", Shipping Cost: " << product.getShippingCost() << ", Total Cost: " << product.getTotalCost() << "\n";
        }
    }
};

// Function to get product information from user
Product getProductFromUser() {
    string name, category;
    double price, taxRate, shippingCost;

    cout << "Enter product name: ";
    cin >> name;
    cout << "Enter product category: ";
    cin >> category;
    cout << "Enter product price: ";
    cin >> price;
    cout << "Enter tax rate (decimal): ";
    cin >> taxRate;
    cout << "Enter shipping cost: ";
    cin >> shippingCost;

    return Product(name, category, price, taxRate, shippingCost);
}

int main() {
    ShoppingCart cart;
    char choice;

    do {
        cout << "1. Add product\n2. Apply discounts\n3. Calculate total cost\n4. List products\n5. Exit\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case '1': {
                Product product = getProductFromUser();
                cart.addProduct(product);
                break;
            }
            case '2': {
                cout << "Choose discount type:\n1. Category Discount\n2. Promotion Discount\nEnter your choice: ";
                char discountChoice;
                cin >> discountChoice;

                if (discountChoice == '1') {
                    string category;
                    double discountRate;
                    cout << "Enter category for discount: ";
                    cin >> category;
                    cout << "Enter discount rate (decimal): ";
                    cin >> discountRate;

                    auto discount = make_shared<CategoryDiscountStrategy>(category, discountRate);
                    cart.addDiscountStrategy(discount);
                } else if (discountChoice == '2') {
                    string promotionCode;
                    double discountRate;
                    cout << "Enter promotion code: ";
                    cin >> promotionCode;
                    cout << "Enter discount rate (decimal): ";
                    cin >> discountRate;

                    auto discount = make_shared<PromotionDiscountStrategy>(promotionCode, discountRate);
                    cart.addDiscountStrategy(discount);
                }

                cart.applyDiscounts();
                break;
            }
            case '3': {
                double total = cart.calculateTotalCost();
                cout << "Total cost: " << total << "\n";
                break;
            }
            case '4': {
                cart.listProducts();
                break;
            }
            case '5': {
                cout << "Exiting...\n";
                break;
            }
            default:
                cout << "Invalid choice, please try again.\n";
        }
    } while (choice != '5');

    return 0;
}
