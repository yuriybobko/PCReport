#ifndef STUCTURES_H
#define STUCTURES_H

#include <QString>

#define DEFAULT_BASIC_WAGE 500
#define DEFAULT_BASIC_WAGE_KOEF_SALARY 0.3

#define CASH_STRING "нал."
#define NONCASH_STRING "б/нал."
#define SELFCOAST_STRING "себест-ть"
#define TOTAL_CASH_STRING "всего"

enum RegisterType {
    Register_defCategory,
    Register_Salary,
    Register_Costs,
};

struct DefinedCategory {
    QString categoryTitle;
    QString taxTitle;
    bool selfcoast;
    float koefSalary;
    float koefProfit;
    bool isSelling;
    bool isContracted;
};

struct DefinedCategoryRecord {
    int id;
    int categoryId;
    int taxId;
    int selfcoast;
    float koefSalary;
    float koefProfit;
    int isSelling;
    int isContracted;
};

struct DefCategoryRegisterRecord {
    int id;
    QString date = "0";
    int stafferId = 0;
    int categoryId = 0;
    int taxId;
    int cashId;
    float amount;
    float selfcoast;
};

struct DefCategoryRegisterRecordView {
    int id;
    QString date = "0";
    QString staffer;
    QString category;
    QString tax;
    QString cash;
    float amount;
    float selfcoast;
};

struct CostsRegisterRecord {
    int id;
    QString date;
    int cashId;
    float amount;
    QString description;
};

struct CostsRegisterRecordView {
    int id;
    QString date;
    QString cash;
    float amount;
    QString description;
};

struct SalaryRegisterRecord {
    int id;
    QString date;
    int stafferId;
    float amount;
    float basicWage;
    float koefBasicWage;
};

struct SalaryRegisterRecordView {
    int id;
    QString date;
    QString staffer;
    float amount;
    float basicWage;
    float koefBasicWage;
};

struct StaffersTable {
    const QString table = "staffers";
    const QString id = "id";
    const QString name = "name";
};

struct CategoriesTable {
    const QString table = "categories";
    const QString id = "id";
    const QString title = "title";
};

struct TaxesTable {
    const QString table = "taxes";
    const QString id = "id";
    const QString title = "title";
};

struct CashTable {
    const QString table = "cash";
    const QString id = "id";
    const QString title = "title";
};

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

struct DefCategoryRegisterTable {
    const QString table = "defcategory_register";
    const QString id = "id";
    const QString date = "date_value";
    const QString staffer = "staffer_id";
    const QString category = "category_id";
    const QString tax = "tax_id";
    const QString cash = "cash_id";
    const QString amount = "amount";
    const QString selfcoast = "self_coast";
};

struct DefCategoryRegisterView {
    const QString table = "defcategory_register_view";
    const QString id = "id";
    const QString date = "date_value";
    const QString staffer = "staffer";
    const QString category= "category";
    const QString tax = "tax";
    const QString cash = "cash";
    const QString amount = "amount";
    const QString selfcoast = "self_coast";
};

struct CostsRegisterTable {
    const QString table = "costs_register";
    const QString id = "id";
    const QString date = "date_value";
    const QString cash = "cash_id";
    const QString amount = "amount";
    const QString description = "description";
};

struct CostsRegisterView {
    const QString table = "costs_register_view";
    const QString id = "id";
    const QString date = "date_value";
    const QString cash = "cash";
    const QString amount = "amount";
    const QString description = "description";
};

struct SalaryRegisterTable {
    const QString table = "salary_register";
    const QString id = "id";
    const QString date = "date_value";
    const QString staffer = "staffer_id";
    const QString amount = "amount";
    const QString basicWage = "basic_wage";
    const QString koefBasicWage = "koef_basic_wage";
};

struct SalaryRegisterView {
    const QString table = "salary_register_view";
    const QString id = "id";
    const QString date = "date_value";
    const QString staffer = "staffer";
    const QString amount = "amount";
    const QString basicWage = "basic_wage";
    const QString koefBasicWage = "koef_basic_wage";
};

#endif // STUCTURES_H
