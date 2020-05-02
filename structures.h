#ifndef STUCTURES_H
#define STUCTURES_H

#include <QString>

#define DEFAULT_BASIC_WAGE 500
#define DEFAULT_BASIC_WAGE_KOEF_SALARY 0.3

#define CASH_STRING "нал."
#define NONCASH_STRING "б/нал."

struct DefinedCategory {
    QString categoryTitle;
    QString taxTitle;
    bool selfcoast;
    float koefSalary;
    float koefProfit;
    bool isSelling;
    bool isContracted;
};

struct DefCategoryRegisterRecord {
    int id;
    QString date;
    int stafferId;
    int categoryId;
    int taxId;
    int cashId;
    float amount;
    float selfcoast;
};

//struct DataBaseTables {
//    const QString staffers = "staffers";
//    const QString categories = "categories";
//    const QString taxes = "taxes";
//    const QString defcategories = "defcategories_view";
//};

struct StaffersTable {
    const QString table = "staffers";
    const QString id = "id";
    const QString name = "name";
};
extern StaffersTable staffersTable;

struct CategoriesTable {
    const QString table = "categories";
    const QString id = "id";
    const QString title = "title";
};
extern CategoriesTable categoriesTable;

struct TaxesTable {
    const QString table = "taxes";
    const QString id = "id";
    const QString title = "title";
};
extern TaxesTable taxesTable;

struct CashTable {
    const QString table = "cash";
    const QString id = "id";
    const QString title = "title";
};
extern CashTable cashTable;

struct DefCategoriesTable {
    const QString table = "defcategories";
    const QString id = "id";
    const QString title = "title_id";
    const QString tax = "tax_id";
    const QString selfcoast = "self_coast";
    const QString koefSalary = "koef_salary";
    const QString koefProfit = "koef_profit";
    const QString isSelling = "selling";
    const QString isContracted = "contracted";
};
extern DefCategoriesTable defCategoriesTable;

struct DefCategoriesView {
    const QString table = "defcategories_view";
    const QString id = "id";
    const QString title = "title";
    const QString tax = "tax";
    const QString selfcoast = "self_coast";
    const QString koefSalary = "koef_salary";
    const QString koefProfit = "koef_profit";
    const QString isSelling = "selling";
    const QString isContracted = "contracted";
};
extern DefCategoriesView defCategoriesView;

struct DefCategoryRegisterTable {
    const QString table = "defcategory_register";
    const QString id = "id";
    const QString date = "date";
    const QString staffer = "staffer_id";
    const QString category = "category_id";
    const QString tax = "tax_id";
    const QString cash = "cash_id";
    const QString amount = "amount";
    const QString selfcoast = "self_coast";
};
extern DefCategoryRegisterTable defCategoryRegisterTable;

struct DefCategoryRegisterView {
    const QString table = "defcategory_register_view";
    const QString id = "id";
    const QString date = "date";
    const QString staffer = "staffer";
    const QString category= "category";
    const QString tax = "tax";
    const QString cash = "cash";
    const QString amount = "amount";
    const QString selfcoast = "self_coast";
};
extern DefCategoryRegisterView defCategoryRegisterView;

struct CostsRegisterTable {
    const QString table = "costs_register";
    const QString id = "id";
    const QString date = "date";
    const QString cash = "cash_id";
    const QString amount = "amount";
    const QString description = "description";
};
extern CostsRegisterTable costsRegisterTable;
#endif // STUCTURES_H
